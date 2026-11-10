"""Apache server lifecycle for regression tests.

Ports httpd_start/httpd_stop from run-regression-tests.pl: reuse the real
autoconf-generated base config (tests/regression/server_root/conf/httpd.conf,
built from httpd.conf.in - it already resolves the right LoadModule paths,
MPM, ServerRoot etc. for this platform) and layer the per-test config on top
via `-c "Include <path>"`, exactly like the Perl harness does. `httpd -k
start` daemonizes; actual readiness is confirmed by polling the error log
for Apache's own startup/shutdown log lines, not by waiting on the control
subprocess (which returns almost immediately).
"""

import os
import re
import subprocess
import time
from pathlib import Path
from typing import Optional

STARTUP_PATTERN = re.compile(rb"resuming normal operations")
SHUTDOWN_PATTERN = re.compile(rb"caught SIG[A-Z]+, shutting down")
STARTUP_TIMEOUT = 60
SHUTDOWN_TIMEOUT = 60


def _resolve_httpd_and_modules_dir(script_dir: Path) -> tuple:
    """run-regression-tests.pl (generated from the .pl.in by ./configure)
    already has these resolved for this platform; read them from there
    rather than re-deriving configure logic."""
    runner = script_dir / "run-regression-tests.pl"
    httpd = modules_dir = None
    if runner.exists():
        content = runner.read_text()
        m = re.search(r'^\s*my\s+\$HTTPD\s*=\s*q\(([^)]*)\)', content, re.MULTILINE)
        if m:
            httpd = m.group(1)
        m = re.search(r'^\s*my\s+\$MODULES_DIR\s*=\s*q\(([^)]*)\)', content, re.MULTILINE)
        if m:
            modules_dir = m.group(1)
    return httpd or "/usr/local/apache2/bin/httpd", modules_dir or "/usr/local/apache2/modules"


def _resolve_server_root(httpd_path: str) -> str:
    try:
        out = subprocess.run([httpd_path, "-V"], capture_output=True, text=True, check=True).stdout
    except (subprocess.SubprocessError, OSError):
        return ""
    m = re.search(r'-D HTTPD_ROOT="([^"]*)"', out)
    return m.group(1) if m else ""


class ApacheServer:
    """Manages one httpd instance's lifecycle for a single regression test."""

    def __init__(self, log_matcher, port: int = 8088, server_name: str = "localhost"):
        self.script_dir = Path(__file__).resolve().parent
        self.sroot_dir = self.script_dir / "regression" / "server_root"
        self.conf_dir = self.sroot_dir / "conf"
        self.base_conf = self.conf_dir / "httpd.conf"
        self.port = port
        self.server_name = server_name
        self.log_matcher = log_matcher

        self.httpd_path, self.modules_dir = _resolve_httpd_and_modules_dir(self.script_dir)
        self.apache_server_root = _resolve_server_root(self.httpd_path)

        if not self.base_conf.exists():
            raise RuntimeError(
                f"{self.base_conf} not found. Run ./configure at the repo root first "
                "(it generates this from httpd.conf.in)."
            )

        self._per_test_conf: Optional[Path] = None

    def _run_control(self, extra_conf: Optional[str], action: str) -> subprocess.CompletedProcess:
        cmd = [self.httpd_path]
        if self.apache_server_root:
            cmd += ["-d", self.apache_server_root]
        cmd += [
            "-f", str(self.base_conf),
            "-c", f"Listen {self.server_name}:{self.port}",
        ]
        if extra_conf:
            cmd += ["-c", extra_conf]
        cmd += ["-k", action]
        return subprocess.run(cmd, capture_output=True, text=True)

    def start(self, conf_text: str = "", timeout: int = STARTUP_TIMEOUT) -> bool:
        if self.is_running():
            self.stop()

        self.log_matcher.reset_positions()

        self.conf_dir.mkdir(parents=True, exist_ok=True)
        self._per_test_conf = self.conf_dir / f"test_{id(self)}.conf"
        self._per_test_conf.write_text(conf_text)

        result = self._run_control(f"Include {self._per_test_conf}", "start")
        out = "\n".join(
            line for line in (result.stdout + result.stderr).splitlines() if "POOL DEBUG" not in line
        ).strip()
        if out:
            self.stop()
            return False

        return self.log_matcher.wait_for_pattern("error", STARTUP_PATTERN, timeout=timeout) is not None

    def stop(self, timeout: int = SHUTDOWN_TIMEOUT) -> bool:
        result = self._run_control(None, "graceful-stop")
        out = "\n".join(
            line for line in (result.stdout + result.stderr).splitlines() if "POOL DEBUG" not in line
        ).strip()
        if out:
            return False

        ok = self.log_matcher.wait_for_pattern("error", SHUTDOWN_PATTERN, timeout=timeout) is not None
        time.sleep(0.5)  # matches httpd_stop()'s own settle delay
        if self._per_test_conf and self._per_test_conf.exists():
            self._per_test_conf.unlink()
        return ok

    def is_running(self) -> bool:
        pid_file = self.sroot_dir / "logs" / "httpd.pid"
        if not pid_file.exists():
            return False
        try:
            pid = int(pid_file.read_text().strip())
        except (ValueError, OSError):
            return False
        try:
            os.kill(pid, 0)
        except ProcessLookupError:
            return False
        except PermissionError:
            pass  # process exists, just owned by someone else
        return True
