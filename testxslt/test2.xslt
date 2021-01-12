<?xml version="1.0" encoding="windows-1251" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">
  <xsl:output method="text" indent="yes"/>
  <xsl:key name="Tags" match="Tag5" use="No" />

  <xsl:template match="/info">
    <xsl:text>ACTIONID|NO|DESCRIPTION|NAME|DBOMInf1|DBOMInf2</xsl:text>
    <xsl:value-of select="'&#13;'" />
    <xsl:apply-templates select="Tag1/Tag2" />
  </xsl:template>

  <xsl:template match="Tag2">
    <xsl:apply-templates select="key('Tags', normalize-space(Tag3/PartNo))" />
    <xsl:value-of select="concat(DBOMInf1, '|', DBOMInf2, '&#13;')" />
  </xsl:template>

  <xsl:template match="Tag5">
    <xsl:value-of select="concat(Id, '|', No, '|', Name, '|', Description, '|')" />
  </xsl:template>
</xsl:stylesheet>