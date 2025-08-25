import subprocess
import time
import os
import signal
from pathlib import Path
from typing import Optional, Dict, List
import tempfile
import shutil


class ApacheServer:
    """Manages Apache server lifecycle for testing"""
    
    def __init__(self, httpd_path: str, server_root: Path, conf_dir: Path, 
                 logs_dir: Path, modules_dir: str, port: int = 8088, 
                 server_name: str = "localhost"):
        self.httpd_path = httpd_path
        self.server_root = server_root
        self.conf_dir = conf_dir
        self.logs_dir = logs_dir
        self.modules_dir = modules_dir
        self.port = port
        self.server_name = server_name
        self.pid_file = logs_dir / "httpd.pid"
        self.httpd_conf = conf_dir / "httpd.conf"
        self._process = None
        self._base_config = None
        
        # Ensure directories exist
        for directory in [server_root, conf_dir, logs_dir]:
            directory.mkdir(parents=True, exist_ok=True)
        
        # Generate base configuration
        self._generate_base_config()
    
    def _generate_base_config(self):
        """Generate base Apache configuration"""
        self._base_config = f"""
# Basic Apache Configuration for ModSecurity Testing
ServerRoot "{self.server_root}"
PidFile "{self.pid_file}"
Timeout 300
KeepAlive On
MaxKeepAliveRequests 100
KeepAliveTimeout 15

# Multi-processing module
LoadModule mpm_prefork_module {self.modules_dir}/mod_mpm_prefork.so

# Core modules
LoadModule authz_core_module {self.modules_dir}/mod_authz_core.so
LoadModule dir_module {self.modules_dir}/mod_dir.so
LoadModule mime_module {self.modules_dir}/mod_mime.so
LoadModule rewrite_module {self.modules_dir}/mod_rewrite.so
LoadModule headers_module {self.modules_dir}/mod_headers.so

# ModSecurity module
LoadModule security2_module {self.modules_dir}/mod_security2.so

# Basic server settings
ServerName {self.server_name}:{self.port}
Listen {self.port}

# Document root
DocumentRoot "{self.server_root}/htdocs"

# Directory permissions
<Directory />
    AllowOverride None
    Require all denied
</Directory>

<Directory "{self.server_root}/htdocs">
    AllowOverride None
    Require all granted
    DirectoryIndex index.html
</Directory>

# MIME types
TypesConfig /etc/mime.types

# Logging
LogLevel warn
ErrorLog "{self.logs_dir}/error.log"
LogFormat "%h %l %u %t \\"%r\\" %>s %O \\"%{{Referer}}i\\" \\"%{{User-Agent}}i\\"" combined
CustomLog "{self.logs_dir}/access.log" combined

# ModSecurity basic configuration
SecRuleEngine DetectionOnly
SecRequestBodyAccess On
SecResponseBodyAccess On
SecResponseBodyMimeType null
SecDataDir "{self.server_root}/tmp"
SecTmpDir "{self.server_root}/tmp"
SecUploadDir "{self.server_root}/upload"

# Default ModSecurity audit log configuration
SecAuditEngine RelevantOnly
SecAuditLogRelevantStatus "^(?:5|4(?!04))"
SecAuditLogParts ABDEFHIJZ
SecAuditLogType Serial
SecAuditLog "{self.logs_dir}/modsec_audit.log"

# Debug log configuration (will be enabled in tests as needed)
SecDebugLog "{self.logs_dir}/modsec_debug.log"
SecDebugLogLevel 0

"""
    
    def create_config(self, additional_config: str = "", include_files: List[str] = None) -> Path:
        """Create Apache configuration with optional additional configuration"""
        config_content = self._base_config
        
        # Add include files
        if include_files:
            for include_file in include_files:
                config_content += f"\nInclude {include_file}\n"
        
        # Add additional configuration
        if additional_config:
            config_content += f"\n# Test-specific configuration\n{additional_config}\n"
        
        # Write configuration file
        with open(self.httpd_conf, 'w') as f:
            f.write(config_content)
        
        return self.httpd_conf
    
    def start(self, config: str = "", timeout: int = 60) -> bool:
        """Start Apache server with optional additional configuration"""
        # Stop any existing instance
        if self.is_running():
            self.stop()
        
        # Create configuration
        self.create_config(config)
        
        # Create test files if they don't exist
        self._create_test_files()
        
        # Start Apache
        cmd = [
            self.httpd_path,
            '-D', 'FOREGROUND',
            '-f', str(self.httpd_conf)
        ]
        
        try:
            self._process = subprocess.Popen(
                cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                preexec_fn=os.setsid
            )
            
            # Wait for server to start
            start_time = time.time()
            while time.time() - start_time < timeout:
                if self._check_server_ready():
                    return True
                time.sleep(0.1)
            
            # If we get here, server didn't start properly
            self.stop()
            return False
            
        except Exception as e:
            print(f"Failed to start Apache: {e}")
            return False
    
    def stop(self, timeout: int = 30) -> bool:
        """Stop Apache server"""
        if not self._process:
            return True
        
        try:
            # Send SIGTERM to the process group
            os.killpg(os.getpgid(self._process.pid), signal.SIGTERM)
            
            # Wait for graceful shutdown
            start_time = time.time()
            while time.time() - start_time < timeout:
                if self._process.poll() is not None:
                    self._process = None
                    return True
                time.sleep(0.1)
            
            # Force kill if graceful shutdown failed
            os.killpg(os.getpgid(self._process.pid), signal.SIGKILL)
            self._process.wait()
            self._process = None
            return True
            
        except Exception as e:
            print(f"Error stopping Apache: {e}")
            return False
    
    def restart(self, config: str = "", timeout: int = 60) -> bool:
        """Restart Apache server"""
        self.stop()
        return self.start(config, timeout)
    
    def is_running(self) -> bool:
        """Check if Apache server is running"""
        if not self._process:
            return False
        return self._process.poll() is None
    
    def _check_server_ready(self) -> bool:
        """Check if server is ready to accept connections"""
        try:
            import socket
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.settimeout(1)
            result = sock.connect_ex((self.server_name, self.port))
            sock.close()
            return result == 0
        except Exception:
            return False
    
    def _create_test_files(self):
        """Create basic test files needed for testing"""
        htdocs = self.server_root / "htdocs"
        htdocs.mkdir(exist_ok=True)
        
        # Create basic test files
        test_files = {
            'index.html': '<html><body>ModSecurity Test Server</body></html>',
            'test.txt': 'TEST',
            '8k.txt': 'A' * 8192,  # 8KB file for testing
        }
        
        for filename, content in test_files.items():
            file_path = htdocs / filename
            if not file_path.exists():
                with open(file_path, 'w') as f:
                    f.write(content)
    
    def get_log_content(self, log_type: str = "error") -> str:
        """Get content from specified log file"""
        log_files = {
            'error': self.logs_dir / "error.log",
            'access': self.logs_dir / "access.log", 
            'audit': self.logs_dir / "modsec_audit.log",
            'debug': self.logs_dir / "modsec_debug.log"
        }
        
        log_file = log_files.get(log_type)
        if not log_file or not log_file.exists():
            return ""
        
        try:
            with open(log_file, 'r') as f:
                return f.read()
        except Exception:
            return ""
    
    def clear_logs(self):
        """Clear all log files"""
        log_files = [
            self.logs_dir / "error.log",
            self.logs_dir / "access.log",
            self.logs_dir / "modsec_audit.log", 
            self.logs_dir / "modsec_debug.log"
        ]
        
        for log_file in log_files:
            if log_file.exists():
                try:
                    log_file.unlink()
                except Exception:
                    pass
    
    def wait_for_log_entry(self, pattern: str, log_type: str = "error", 
                          timeout: int = 10) -> Optional[str]:
        """Wait for a specific pattern to appear in a log file"""
        import re
        
        log_files = {
            'error': self.logs_dir / "error.log",
            'access': self.logs_dir / "access.log",
            'audit': self.logs_dir / "modsec_audit.log", 
            'debug': self.logs_dir / "modsec_debug.log"
        }
        
        log_file = log_files.get(log_type)
        if not log_file:
            return None
        
        regex = re.compile(pattern)
        start_time = time.time()
        last_position = 0
        
        while time.time() - start_time < timeout:
            if log_file.exists():
                try:
                    with open(log_file, 'r') as f:
                        f.seek(last_position)
                        new_content = f.read()
                        last_position = f.tell()
                        
                        for line in new_content.split('\n'):
                            match = regex.search(line)
                            if match:
                                return match.group()
                except Exception:
                    pass
            
            time.sleep(0.1)
        
        return None 