Please note that installing ModSecurity for IIS requires IIS to be installed and enabled.


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
