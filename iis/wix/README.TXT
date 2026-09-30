Please note that installing ModSecurity for IIS requires IIS to be installed and enabled.

PREREQUISITES:

ModSecurity for IIS requires the Microsoft Visual C++ 2019 Redistributable to be installed:
- For 64-bit systems: Install both x64 and x86 versions (for 32-bit application pool support)
  - x64: https://aka.ms/vs/17/release/vc_redist.x64.exe
  - x86: https://aka.ms/vs/17/release/vc_redist.x86.exe
- For 32-bit systems: Install x86 version only
  - x86: https://aka.ms/vs/17/release/vc_redist.x86.exe

The installer will check for these prerequisites and provide download links if they are missing.


After installing ModSecurity for IIS, the module will be running in all websites by default. To remove from a website add to web.config:

<modules>
<remove name="ModSecurityIIS" />
</modules>

To configure module in a website add to web.config:

<?xml version="1.0" encoding="UTF-8"?>
<configuration>
    <system.webServer>
        <ModSecurity enabled="true" configFile="c:\inetpub\wwwroot\xss.conf" />
    </system.webServer>
</configuration>

where configFile is standard ModSecurity config file.

Events from the module will show up in "Application" Windows log.
