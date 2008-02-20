<?xml version='1.0'?> 
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

<xsl:import href="/opt/docbook/xsl/html/onechunk.xsl"/>

<xsl:template name="user.header.navigation">
    <div style="background:#F5F5F5;width:100%;border-top:1px solid #DDDDDD;border-bottom:1px solid #DDDDDD"><table cellpadding="0" cellspacing="0" width="100%"><tr><td><a href="http://www.modsecurity.org"><img border="0" alt="ModSecurity" height="36" width="120" src="modsecurity.gif" style="margin:4px"/></a></td><td align="right"><a href="http://www.breach.com"><img border="0" width="100" height="36" src="breach-logo-small.gif" style="margin:6px"/></a></td></tr></table></div>	
</xsl:template>

<xsl:template name="user.footer.navigation">
    <div class="copyright" align="center">Copyright (C) 2004-2008 <a href="http://www.breach.com">Breach Security</a></div>
</xsl:template>

<xsl:template name="article.titlepage.separator">
<hr size="1"/>
</xsl:template>

<xsl:param name="html.stylesheet">modsecurity-reference.css</xsl:param>

</xsl:stylesheet>
