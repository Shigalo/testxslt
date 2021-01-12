<?xml version="1.0" encoding="windows-1251" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">
  <xsl:template match="/">
    <table border="1">
      <tr bgcolor="#CCCCCC">
        <td align="center">
          <strong>Cli</strong>
        </td>
        <td align="center">
          <strong>Veight</strong>
        </td>
        <td align="center">
          <strong>Color</strong>
        </td>
      </tr>
      <xsl:for-each select="tutorial/enimals/dogs/dog">
        <tr bgcolor="#F5F5F5">
          <td>
            <xsl:value-of select="dogName"/>
          </td>
          <td align="right">
            <xsl:value-of select="dogWeight"/>
            <xsl:value-of select="dogWeight/@caption"/>
          </td>
          <td>
            <xsl:value-of select="dogColor"/>
          </td>
        </tr>
      </xsl:for-each>
    </table>
  </xsl:template>
</xsl:stylesheet>