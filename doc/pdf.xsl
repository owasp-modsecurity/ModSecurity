<?xml version='1.0'?> 
<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:fo="http://www.w3.org/1999/XSL/Format"
    version="1.0"
>

<xsl:import href="/opt/docbook/xsl/fo/docbook.xsl"/>

<xsl:param name="paper.type" select="A4"/>

<!--
<xsl:param name="section.autolabel" select="1"/>
<xsl:param name="appendix.autolabel" select="1"/>
<xsl:param name="section.label.include.component.label" select="1"/>
-->

<xsl:param name="fop.extensions" select="1"/>
<xsl:param name="title.margin.left">0pt</xsl:param>
<xsl:param name="body.font.master">11</xsl:param>
<xsl:param name="line-height">1.5</xsl:param>
<xsl:param name="header.column.widths">1 20 1</xsl:param>

<xsl:attribute-set name="normal.para.spacing">
  <xsl:attribute name="space-before.optimum">0.0em</xsl:attribute>
  <xsl:attribute name="space-before.minimum">0.0em</xsl:attribute>
  <xsl:attribute name="space-before.maximum">0.0em</xsl:attribute>

  <xsl:attribute name="space-after.optimum">0.2em</xsl:attribute>
  <xsl:attribute name="space-after.minimum">0.2em</xsl:attribute>
  <xsl:attribute name="space-after.maximum">0.2em</xsl:attribute>
</xsl:attribute-set>

<xsl:attribute-set name="section.title.properties">
  <xsl:attribute name="font-family">
    <xsl:value-of select="$title.font.family"/>
  </xsl:attribute>

  <xsl:attribute name="font-weight">bold</xsl:attribute>
  <xsl:attribute name="keep-with-next.within-column">always</xsl:attribute>
  <xsl:attribute name="text-align">left</xsl:attribute>
  <xsl:attribute name="space-before.minimum">1.0em</xsl:attribute>
  <xsl:attribute name="space-before.optimum">1.0em</xsl:attribute>
  <xsl:attribute name="space-before.maximum">1.0em</xsl:attribute>
  <xsl:attribute name="space-after.minimum">0.0em</xsl:attribute>
  <xsl:attribute name="space-after.optimum">0.0em</xsl:attribute>
  <xsl:attribute name="space-after.maximum">0.0em</xsl:attribute>
</xsl:attribute-set>

<xsl:attribute-set name="section.title.level1.properties">
  <xsl:attribute name="font-size">24pt</xsl:attribute>

  <xsl:attribute name="break-before">page</xsl:attribute>

  <xsl:attribute name="space-before.minimum">0.0em</xsl:attribute>
  <xsl:attribute name="space-before.optimum">0.0em</xsl:attribute>
  <xsl:attribute name="space-before.maximum">0.0em</xsl:attribute>

  <!--
  <xsl:attribute name="space-after.minimum">1.5em</xsl:attribute>
  <xsl:attribute name="space-after.optimum">1.5em</xsl:attribute>
  <xsl:attribute name="space-after.maximum">1.5em</xsl:attribute>
  -->
</xsl:attribute-set>

<xsl:attribute-set name="section.title.level2.properties">
  <xsl:attribute name="font-size">18pt</xsl:attribute>
</xsl:attribute-set>

<xsl:attribute-set name="section.title.level3.properties">
  <xsl:attribute name="font-size">14pt</xsl:attribute>
</xsl:attribute-set>

<xsl:attribute-set name="section.title.level4.properties">
  <xsl:attribute name="font-size">12pt</xsl:attribute>
</xsl:attribute-set>

<xsl:attribute-set name="list.item.spacing">
  <xsl:attribute name="space-before.optimum">0.0em</xsl:attribute>
  <xsl:attribute name="space-before.minimum">0.0em</xsl:attribute>
  <xsl:attribute name="space-before.maximum">0.0em</xsl:attribute>
  <xsl:attribute name="space-after.optimum">0.2em</xsl:attribute>
  <xsl:attribute name="space-after.minimum">0.2em</xsl:attribute>
  <xsl:attribute name="space-after.maximum">0.2em</xsl:attribute>
</xsl:attribute-set>

<xsl:attribute-set name="list.block.spacing">
  <xsl:attribute name="space-before.optimum">0.2em</xsl:attribute>
  <xsl:attribute name="space-before.minimum">0.2em</xsl:attribute>
  <xsl:attribute name="space-before.maximum">0.2em</xsl:attribute>

  <xsl:attribute name="space-after.optimum">0.2em</xsl:attribute>
  <xsl:attribute name="space-after.minimum">0.2em</xsl:attribute>
  <xsl:attribute name="space-after.maximum">0.2em</xsl:attribute>

  <xsl:attribute name="margin-left">2pc</xsl:attribute>
</xsl:attribute-set>

<xsl:param name="shade.verbatim" select="1"/>
<xsl:attribute-set name="shade.verbatim.style">
  <xsl:attribute name="background-color">#f0f0f0</xsl:attribute>
  <xsl:attribute name="padding">2pt</xsl:attribute>
</xsl:attribute-set>

<xsl:attribute-set name="monospace.verbatim.properties">
  <xsl:attribute name="font-size">90%</xsl:attribute>
</xsl:attribute-set>

<!--
<xsl:template name="nongraphical.admonition">
  <xsl:variable name="id">
    <xsl:call-template name="object.id"/>
  </xsl:variable>

  <fo:block id="{$id}"
            xsl:use-attribute-sets="nongraphical.admonition.properties">
    <xsl:if test="$admon.textlabel != 0 or title">
      <fo:block keep-with-next.within-column='always'
                xsl:use-attribute-sets="admonition.title.properties">
         <xsl:apply-templates select="." mode="object.title.markup"/>
      </fo:block>
    </xsl:if>

    <fo:block xsl:use-attribute-sets="admonition.properties">
      <xsl:apply-templates/>
    </fo:block>
  </fo:block>
</xsl:template>
-->

<xsl:template name="nongraphical.admonition">
  <xsl:variable name="id">
    <xsl:call-template name="object.id"/>
  </xsl:variable>

  <fo:block space-before.minimum="0.8em"
            space-before.optimum="1em"
            space-before.maximum="1.2em"
            start-indent="0.25in"
            end-indent="0.25in"
            border-top="0.5pt solid black"
            border-bottom="0.5pt solid black"
            padding-top="4pt"
            padding-bottom="4pt"
            id="{$id}">
    <xsl:if test="$admon.textlabel != 0 or title">
      <fo:block keep-with-next='always'
                xsl:use-attribute-sets="admonition.title.properties">
         <xsl:apply-templates select="." mode="object.title.markup"/>
      </fo:block>
    </xsl:if>

    <fo:block xsl:use-attribute-sets="admonition.properties">
      <xsl:apply-templates/>
    </fo:block>
  </fo:block>
</xsl:template>

<xsl:template name="article.titlepage.before.recto">
  <fo:block>&#8203;
    <fo:external-graphic width="8cm" src="url(modsecurity-logo.png)"/>
  </fo:block>
</xsl:template>

</xsl:stylesheet>
