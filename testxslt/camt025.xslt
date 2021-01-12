<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="text" indent="yes"/>
  <xsl:template match="/Document">
    <xsl:text>{F:/</xsl:text>
    <xsl:apply-templates select="Rct/MsgHdr" />
  </xsl:template>

  <xsl:template match="MsgHdr">
    <xsl:value-of select="concat(substring(CreDtTm, 3, 2), substring(CreDtTm, 6, 2), substring(CreDtTm, 9, 2), '/00001GRC0000/', MsgId, '}{2:/7/7100/799/01/00001GRC0000')" />
  </xsl:template>


</xsl:stylesheet>