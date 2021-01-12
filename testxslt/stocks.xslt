<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="2.0">
  <xsl:output method="text" indent="yes"/>
  <xsl:template match="/Document">
    <xsl:text>{F:/</xsl:text>
    <xsl:apply-templates select="BkToCstmrStmt/GrpHdr" />
    <xsl:apply-templates select="BkToCstmrStmt/Stmt" />
  </xsl:template>

  <xsl:template match="GrpHdr">
    <xsl:value-of select="concat(substring(CreDtTm, 3, 2), substring(CreDtTm, 6, 2), substring(CreDtTm, 9, 2))" />
    <xsl:value-of select="concat( '/00001GRC0000/', MsgId, '}{2:/7/7100/712&#13;')" />

    <xsl:for-each select="/Document/BkToCstmrStmt/Stmt">
      <xsl:value-of select="concat('id = ', Id, '&#13;')"/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="Stmt">
    <xsl:value-of select="concat('&#13;:20:', Id)" />
    newline
  </xsl:template>

</xsl:stylesheet>