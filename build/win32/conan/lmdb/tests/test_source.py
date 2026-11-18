"""Exercise LMDB source selection with the real Conan CLI and loopback downloads."""

import gzip
import hashlib
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from io import BytesIO
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tarfile
import tempfile
import threading
import unittest


RECIPE = Path(__file__).resolve().parents[1] / "conanfile.py"
CONAN_CLI = "import sys; from conan.cli.cli import main; sys.exit(main(sys.argv[1:]))"


def _archive(marker):
    """Create a reproducible source archive with one recognizable LMDB header."""
    content = marker.encode("utf-8")
    output = BytesIO()
    with tarfile.open(fileobj=output, mode="w") as archive:
        header = tarfile.TarInfo("lmdb-source/libraries/liblmdb/lmdb.h")
        header.size = len(content)
        archive.addfile(header, BytesIO(content))
    return gzip.compress(output.getvalue(), mtime=0)


class _DownloadHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        """Record the request and serve its configured status and response body."""
        self.server.requests.append(self.path)
        status, body = self.server.responses.get(self.path, (404, b"not found"))
        self.send_response(status)
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, format_string, *args):
        """Keep expected error responses out of unittest output."""


class SourceDownloadTests(unittest.TestCase):
    def run_source(self, primary_status=200, primary_body=None, primary_hash=None,
                   mirror_status=200, mirror_body=None, mirror_hash=None):
        """Run an isolated source command against two local archive endpoints."""
        primary_body = _archive("primary") if primary_body is None else primary_body
        mirror_body = _archive("mirror") if mirror_body is None else mirror_body
        responses = {
            "/primary.tar.gz": (primary_status, primary_body),
            "/mirror.tar.gz": (mirror_status, mirror_body),
        }
        with tempfile.TemporaryDirectory() as directory, ThreadingHTTPServer(
            ("127.0.0.1", 0), _DownloadHandler
        ) as server:
            server.responses = responses
            server.requests = []
            worker = threading.Thread(target=server.serve_forever, kwargs={"poll_interval": 0.01}, daemon=True)
            worker.start()
            try:
                root = Path(directory)
                recipe = root / "recipe"
                recipe.mkdir()
                shutil.copyfile(RECIPE, recipe / "conanfile.py")
                base_url = f"http://127.0.0.1:{server.server_port}"
                data = {
                    "sources": {"0.9.32": {
                        "url": base_url + "/primary.tar.gz",
                        "sha256": primary_hash or hashlib.sha256(primary_body).hexdigest(),
                    }},
                    "fallback_sources": {"0.9.32": {
                        "url": base_url + "/mirror.tar.gz",
                        "sha256": mirror_hash or hashlib.sha256(mirror_body).hexdigest(),
                    }},
                }
                # JSON is valid YAML and avoids a test-only YAML dependency.
                (recipe / "conandata.yml").write_text(json.dumps(data), encoding="utf-8")
                conan_home = root / "conan-home"
                conan_home.mkdir()
                (conan_home / "global.conf").write_text(
                    "tools.files.download:retry=2\ntools.files.download:retry_wait=0\n",
                    encoding="utf-8",
                )
                environment = os.environ.copy()
                environment.update(CONAN_HOME=str(conan_home), NO_PROXY="127.0.0.1", no_proxy="127.0.0.1")
                result = subprocess.run(
                    [sys.executable, "-c", CONAN_CLI, "source", str(recipe), "--version", "0.9.32"],
                    cwd=root, env=environment, capture_output=True, text=True, timeout=30,
                    check=False,
                )
                headers = [path.read_text(encoding="utf-8") for path in recipe.rglob("lmdb.h")]
                return result, list(server.requests), headers
            finally:
                server.shutdown()
                worker.join(timeout=5)

    def test_primary_download_is_preferred(self):
        """A healthy original source wins even when a valid mirror is available."""
        result, requests, headers = self.run_source()
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual(requests, ["/primary.tar.gz"])
        self.assertEqual(headers, ["primary"])

    def test_rate_limit_retries_before_using_mirror(self):
        """Exhausted original HTTP 429 retries permit a successful mirror download."""
        result, requests, headers = self.run_source(primary_status=429)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        first_mirror = requests.index("/mirror.tar.gz")
        self.assertGreaterEqual(first_mirror, 2)
        self.assertEqual(requests[:first_mirror], ["/primary.tar.gz"] * first_mirror)
        self.assertEqual(requests[first_mirror:], ["/mirror.tar.gz"])
        self.assertEqual(headers, ["mirror"])

    def test_other_http_errors_do_not_use_mirror(self):
        """Authentication, missing files and server errors must remain failures."""
        for status in (401, 403, 404, 500):
            with self.subTest(status=status):
                result, requests, headers = self.run_source(primary_status=status)
                self.assertNotEqual(result.returncode, 0, result.stdout + result.stderr)
                self.assertTrue(requests)
                self.assertEqual(set(requests), {"/primary.tar.gz"})
                self.assertEqual(headers, [])

    def test_primary_checksum_failure_does_not_use_mirror(self):
        """A downloaded original with the wrong digest cannot trigger source switching."""
        result, requests, headers = self.run_source(primary_hash="0" * 64)
        self.assertNotEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual(requests, ["/primary.tar.gz"])
        self.assertEqual(headers, [])

    def test_primary_extraction_failure_does_not_use_mirror(self):
        """A correctly hashed invalid archive remains an extraction failure."""
        result, requests, headers = self.run_source(primary_body=b"not a tar archive")
        self.assertNotEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual(requests, ["/primary.tar.gz"])
        self.assertEqual(headers, [])

    def test_mirror_download_and_checksum_failures_remain_errors(self):
        """An eligible fallback still must download successfully and match its digest."""
        for mirror in ({"mirror_status": 500}, {"mirror_hash": "0" * 64}):
            with self.subTest(mirror=mirror):
                result, requests, headers = self.run_source(primary_status=429, **mirror)
                self.assertNotEqual(result.returncode, 0, result.stdout + result.stderr)
                first_mirror = requests.index("/mirror.tar.gz")
                self.assertGreaterEqual(first_mirror, 2)
                self.assertEqual(requests[:first_mirror], ["/primary.tar.gz"] * first_mirror)
                self.assertEqual(set(requests[first_mirror:]), {"/mirror.tar.gz"})
                self.assertEqual(headers, [])


if __name__ == "__main__":
    unittest.main()
