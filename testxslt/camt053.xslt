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
    <xsl:value-of select="concat( '/00001GRC0000/', MsgId, '}{2:/7/7100/712/')" />
    <xsl:apply-templates select="../Stmt/Bal/Amt" />
    <xsl:value-of select="concat('/00001GRC0000}{3:/PNS/', ../Stmt/Id, '}{4:')" />
  </xsl:template>

  <xsl:template match="Amt">
    <xsl:variable name="a" select="current()"/>
    <xsl:choose>
      <xsl:when test="$a > 0">01</xsl:when>
      <xsl:otherwise>00</xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="Stmt">
    <xsl:value-of select="concat('&#13;:20:', Id)" />
    <xsl:value-of select="concat('&#13;:21:', ../GrpHdr/OrgnlBizQry/MsgId)" />
  </xsl:template>

</xsl:stylesheet>