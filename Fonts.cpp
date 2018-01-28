#include "base.h"
#include "Fonts.h"
#include "TriAPI.h"
#include <iostream>

vgui::ISurface *g_pSurface;
CFont gFont;

void CFont::Init(void)
{
	int iError = 0;
	iError = FT_Init_FreeType(&m_sLibrary);
	
	if(iError)
	{
		
		gEngfuncs.pfnConsolePrint("[CFont::Init] failed to init freetype \n");
		return;
	}
	//m_sFace->style_flags =FT_STYLE_FLAG_BOLD;
	iError = FT_New_Face(m_sLibrary, "MainFont.ttf", 0, &m_sFace );

	if(iError == FT_Err_Unknown_File_Format)
	{
		gEngfuncs.pfnConsolePrint("[CFont::Init] failed to recognize the font file \n");
		return;
	}
	else if(iError)
	{
		gEngfuncs.pfnConsolePrint("[CFont::Init] failed to use the font file \n");
		return;
	}

	m_iCharCount = 0;
	m_iWidth = 10;
	m_iHeight = 10;
	m_iColor[0] = m_iColor[1] = m_iColor[2] = m_iColor[3] = 255;

	FT_Select_Charmap(m_sFace, FT_ENCODING_UNICODE);
	FT_Set_Pixel_Sizes(m_sFace, m_iWidth, m_iHeight);

	for (int i=0; i < 63; i++)
	{
		for (int j=0; j < FONT_PAGE_MAX; j++)
		{
			m_iPage[i][j] = 0;
			m_iPageCount[i][j] = 0;
			m_bPageFull[i][j] = false;
		}
	}
	gEngfuncs.pfnConsolePrint("Init Font Complete \n");
	for(int i=0; i<FONTS_MAX_BUFFER; i++)
		m_tFontTexture[i].m_iTexture = 0;
		CreateInterfaceFn fnEngineCreateInterface = g_pMetaHookAPI->GetEngineFactory();
		g_pSurface = (vgui::ISurface *)fnEngineCreateInterface(VGUI_SURFACE_INTERFACE_VERSION, NULL);
	return;
}

void CFont::Shutdown(void)
{
	FT_Done_Face(m_sFace);
	FT_Done_FreeType(m_sLibrary);
}

int CFont::CheckCharExists(wchar_t ch)
{
	for (int i = 0; i < m_iCharCount; i++)
	{
		if(65536 * m_iWidth + ch == m_iBuffer[i])
			return i;
	}

	return -1;
}

int CFont::AddCharInToPage(CHARINFO *pChar)
{
	int iRealWidth = m_iWidth + 4;

	// calc max chars
	int iLineChars = 256 / iRealWidth - 1;
	int iMaxChars = iLineChars * iLineChars;
	int i = 0;

	for(i=0; i < FONT_PAGE_MAX; i++)
	{
		if(m_bPageFull[m_iWidth][i])
			continue;

		if(!m_iPage[m_iWidth][i])
		{
			// Create a new empty page
			m_iPage[m_iWidth][i] = g_pSurface->CreateNewTextureID();
			g_pSurface->DrawSetTexture(m_iPage[m_iWidth][i]);

			// bind null texture
			static BYTE byteNull[256 * 256 * 4];
			memset(byteNull, 0, sizeof(byteNull));
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 256, 256 , 0, GL_RGBA,GL_UNSIGNED_BYTE,byteNull);
		}

		m_iPageCount[m_iWidth][i] ++;

		if(m_iPageCount[m_iWidth][i] == iMaxChars)
			m_bPageFull[m_iWidth][i] = true;
		
		//calc pos
		int V = (m_iPageCount[m_iWidth][i]-1) / (iLineChars); // hard to calculate >.<
		int H = (m_iPageCount[m_iWidth][i]-1) % iLineChars; // hard to calculate >.<

		float fTexCoordX = float(H*iRealWidth + 2) / 256.0;
		float fTexCoordY = float(V*iRealWidth + 2) / 256.0;
		float fTexCoordX2 = float(H*iRealWidth + 2 + pChar->m_iWidth) / 256.0;
		float fTexCoordY2 = float(V*iRealWidth + 2 + pChar->m_iHeight) / 256.0;

		pChar->m_fCoord[0][0] = fTexCoordX;
		pChar->m_fCoord[0][1] = fTexCoordX2;
		pChar->m_fCoord[1][0] = fTexCoordY;
		pChar->m_fCoord[1][1] = fTexCoordY2;

		glBindTexture(GL_TEXTURE_2D, m_iPage[m_iWidth][i]);
		pChar->m_iTexture = m_iPage[m_iWidth][i];
		pChar->m_iCharWidth = m_iWidth;
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexEnvf(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_REPLACE);
		glTexSubImage2D(GL_TEXTURE_2D, 0, H*iRealWidth + 2, V*iRealWidth + 2, pChar->m_iWidth, pChar->m_iHeight, GL_RGBA,GL_UNSIGNED_BYTE,pChar->pTexture);

		break;
	}

	return m_iPage[m_iWidth][i];
}

int CFont::LoadChar(wchar_t ch)
{
	int iCallBack = CheckCharExists(ch);
	if(iCallBack > -1)
		return iCallBack;
	

	if(FT_Load_Char(m_sFace,ch , FT_LOAD_FORCE_AUTOHINT|FT_LOAD_TARGET_NORMAL  ))
		return -1;
	
	FT_Glyph sGlyph;
	
	if(FT_Get_Glyph( m_sFace->glyph, &sGlyph ))
		return 0;
	// FT_Stroker stroker;
	//FT_Stroker_New(m_sLibrary, &stroker);
    //FT_Stroker_Set(stroker,
      //              (int)(1 * 64),
        //            FT_STROKER_LINECAP_ROUND,
          //          FT_STROKER_LINEJOIN_ROUND,
            //        0);
	//FT_Glyph_StrokeBorder(&sGlyph, stroker, 0, 1);
	
	FT_Render_Glyph( m_sFace->glyph,   FT_RENDER_MODE_LCD );
	FT_Glyph_To_Bitmap( &sGlyph, ft_render_mode_normal, 0, 1 );
	FT_BitmapGlyph sBitmapGlyph = (FT_BitmapGlyph)sGlyph;

	static int id = 0;
	if (!id)
		id = g_pSurface->CreateNewTextureID();

	m_tFontTexture[m_iCharCount].m_iTexture = id;
	FT_Bitmap sBitmap = sBitmapGlyph->bitmap;

	m_tFontTexture[m_iCharCount].m_iAdvWidth		= m_sFace->glyph->advance.x / 64.0f;
	m_tFontTexture[m_iCharCount].m_iAdvHeight		= m_sFace->size->metrics.y_ppem; 
	m_tFontTexture[m_iCharCount].m_iDeltaWidth		= (float)sBitmapGlyph->left;
	m_tFontTexture[m_iCharCount].m_iDeltaHeight		= (float)sBitmapGlyph->top - sBitmap.rows;
	m_tFontTexture[m_iCharCount].m_iWidth			= sBitmap.width;
	m_tFontTexture[m_iCharCount].m_iHeight			= sBitmap.rows;
	m_tFontTexture[m_iCharCount].wChar = ch;

	BYTE* pBuf = new byte[m_tFontTexture[m_iCharCount].m_iWidth  * m_tFontTexture[m_iCharCount].m_iHeight * 4];
	for(int j=0; j  < m_tFontTexture[m_iCharCount].m_iHeight ; j++)
	{
		for(int i=0; i < m_tFontTexture[m_iCharCount].m_iWidth; i++)
		{
			unsigned char iByte =  (i>=m_tFontTexture[m_iCharCount].m_iWidth || j>=m_tFontTexture[m_iCharCount].m_iHeight) ? 0 : sBitmap.buffer[i + m_tFontTexture[m_iCharCount].m_iWidth*j];
			pBuf[(4*i + (m_tFontTexture[m_iCharCount].m_iHeight - j - 1) * m_tFontTexture[m_iCharCount].m_iWidth * 4)  ] = 255;
			pBuf[(4*i + (m_tFontTexture[m_iCharCount].m_iHeight - j - 1) * m_tFontTexture[m_iCharCount].m_iWidth * 4)+1] = 255;
			pBuf[(4*i + (m_tFontTexture[m_iCharCount].m_iHeight - j - 1) * m_tFontTexture[m_iCharCount].m_iWidth * 4)+2] = 255;
		//	if (iByte>100) iByte = 255;
			//else iByte =0;
			pBuf[(4*i + (m_tFontTexture[m_iCharCount].m_iHeight - j - 1) * m_tFontTexture[m_iCharCount].m_iWidth * 4)+3] = iByte;
		}
	}

	m_tFontTexture[m_iCharCount].pTexture = pBuf;
	AddCharInToPage(&m_tFontTexture[m_iCharCount]);

	delete[] pBuf;
	m_iBuffer[m_iCharCount] = 65536*m_iWidth + ch;
	m_iCharCount ++ ;
	return (m_iCharCount-1);
}

CHARINFO *CFont::GetChar(wchar_t ch)
{
	 return &m_tFontTexture[LoadChar(ch)];
}

int CFont::GetLen(wchar_t* szText)
{
	int iRet = 0;
	int iLen = (int)wcslen(szText);
 
	for(int i = 0 ; i <iLen ; i ++)
		iRet += GetChar(szText[i])->m_iAdvWidth;

	return iRet;
}

int CFont::GetHeight(wchar_t* szText, int iMaxWidth, int iHeightAdd)
{
	int iRet = 0, iHeight = 0;
	int iLen = (int)wcslen(szText);
 
	for (int i = 0 ; i <iLen ; i ++)
	{
		CHARINFO *pCharInfo = GetChar(szText[i]);
		
		if (szText[i] == '\n')
		{
			iRet = 0; 
			iHeight += m_iHeight + iHeightAdd;
			continue;
		}

		iRet += pCharInfo->m_iAdvWidth;

		if (iRet > iMaxWidth)
		{
			iRet = 0;
			iHeight += m_iHeight + iHeightAdd;
		}
	}

	return iHeight;
}

int CFont::DrawString(wchar_t* szText, int x , int y, int iMaxWidth, int iHeightAdd)
{
	int iDrawX = x;
	int iDrawY = y;
	int iLen = (int)wcslen(szText);
	//return 1;
	//Tri_Enable(GL_BLEND);
	//Tri_BlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//glColor4ub(r, g, b, alpha);
	glColor4ub(m_iColor[0], m_iColor[1], m_iColor[2], m_iColor[3]);	
	int iLastDrawTexture = 0;
	
	for(int i = 0 ; i <iLen ; i ++)
	{
		if(szText[i] =='\n')
		{
			iDrawX = x ; 
			iDrawY += iHeightAdd;
			continue;
		}
		CHARINFO* pCharInfo = GetChar(szText[i]);

		 // Update iHeightAdd. Should be small than char
		if(iHeightAdd < pCharInfo->m_iHeight)
			iHeightAdd = pCharInfo->m_iHeight;

		if(iLastDrawTexture != pCharInfo->m_iTexture) 
		{
			//g_pSurface->DrawSetTexture(pCharInfo->m_iTexture);
			glBindTexture(GL_TEXTURE_2D, pCharInfo->m_iTexture);
			iLastDrawTexture = pCharInfo->m_iTexture;
		}
	  
		
		//glEnable(GL_BLEND);
			gEngfuncs.pTriAPI->Begin ( TRI_QUADS );

			gEngfuncs.pTriAPI->TexCoord2f(pCharInfo->m_fCoord[0][0], pCharInfo->m_fCoord[1][1]);
			gEngfuncs.pTriAPI->Vertex3f(iDrawX+pCharInfo->m_iDeltaWidth , iDrawY - pCharInfo->m_iHeight - pCharInfo->m_iDeltaHeight   ,  0);
			
			gEngfuncs.pTriAPI->TexCoord2f(pCharInfo->m_fCoord[0][1], pCharInfo->m_fCoord[1][1]); 
			gEngfuncs.pTriAPI->Vertex3f(iDrawX+pCharInfo->m_iDeltaWidth + pCharInfo->m_iWidth,iDrawY - pCharInfo->m_iHeight - pCharInfo->m_iDeltaHeight    ,  0);
			
			gEngfuncs.pTriAPI->TexCoord2f(pCharInfo->m_fCoord[0][1], pCharInfo->m_fCoord[1][0]); 
			gEngfuncs.pTriAPI->Vertex3f(iDrawX+pCharInfo->m_iDeltaWidth + pCharInfo->m_iWidth,iDrawY - pCharInfo->m_iDeltaHeight,  0);
			
			gEngfuncs.pTriAPI->TexCoord2f(pCharInfo->m_fCoord[0][0],pCharInfo->m_fCoord[1][0]); 
			gEngfuncs.pTriAPI->Vertex3f(iDrawX+pCharInfo->m_iDeltaWidth  , iDrawY- pCharInfo->m_iDeltaHeight, 0);
			
		gEngfuncs.pTriAPI->End();
			
			/*
			glBegin(GL_QUADS);
			glTexCoord2f(pCharInfo->m_fCoord[0][0], pCharInfo->m_fCoord[1][1]);
			glVertex3i(iDrawX,iDrawY- pCharInfo->m_iHeight- pCharInfo->m_iDeltaHeight, 0);
			glTexCoord2f(pCharInfo->m_fCoord[0][1], pCharInfo->m_fCoord[1][1]);
			glVertex3i(iDrawX+pCharInfo->m_iDeltaWidth, iDrawY- pCharInfo->m_iHeight- pCharInfo->m_iDeltaHeight, 0);
			glTexCoord2f(pCharInfo->m_fCoord[0][1], pCharInfo->m_fCoord[1][0]);
			glVertex3i(iDrawX+pCharInfo->m_iDeltaWidth, iDrawY- pCharInfo->m_iDeltaHeight, 0);
			glTexCoord2f(pCharInfo->m_fCoord[0][0], pCharInfo->m_fCoord[1][0]);
			glVertex3i(iDrawX,iDrawY- pCharInfo->m_iDeltaHeight, 0);
			glEnd();
			
			glBegin(GL_QUADS);
			glTexCoord2f(pCharInfo->m_fCoord[0][0], pCharInfo->m_fCoord[1][1]);
			glVertex3i(iDrawX,iDrawY+				pCharInfo->m_iHeight, 0);
			glTexCoord2f(pCharInfo->m_fCoord[0][1], pCharInfo->m_fCoord[1][1]);
			glVertex3i(iDrawX+ pCharInfo->m_iAdvWidth, iDrawY+pCharInfo->m_iHeight, 0);
			glTexCoord2f(pCharInfo->m_fCoord[0][1], pCharInfo->m_fCoord[1][0]);
			glVertex3i(iDrawX+ pCharInfo->m_iAdvWidth, iDrawY, 0);
			glTexCoord2f(pCharInfo->m_fCoord[0][0], pCharInfo->m_fCoord[1][0]);
			glVertex3i(iDrawX,						iDrawY, 0);
			glEnd();
			*/

		iDrawX += pCharInfo->m_iAdvWidth;
		if(iDrawX > x + iMaxWidth)
		{
			iDrawX = x;
			iDrawY += iHeightAdd/*pCharInfo->m_iHeight*/;
		}
	}
	return iDrawX;
}

void CFont::C2W(wchar_t *pwstr, size_t len, const char *str)
{
	if(str)
    {
		size_t nu = strlen(str);
		size_t n =(size_t)::MultiByteToWideChar(CP_ACP,0,(const char *)str,(int)nu,NULL,0);
		if(n>=len)n=len-1;
		::MultiByteToWideChar(CP_ACP,0,(const char *)str,(int)nu,pwstr,(int)n);
		pwstr[n]=0;
    }
}

void CFont::LoadStr(wchar_t* _strText)
{
	if (!_strText)
		return;

	size_t nLen = wcslen(_strText); 
	for(int i = 0 ; i <(int)nLen ; i ++)
		LoadChar(_strText[i]);
}

void CFont::SetSize(int iWidth, int iHeight)
{
	if(m_iWidth == iWidth && m_iHeight == iHeight)
		return;

	m_iWidth = iWidth;
	m_iHeight = iHeight;
	FT_Set_Pixel_Sizes(m_sFace, iWidth, iHeight);
}

void CFont::SetColor(int r, int g, int b)
{
	m_iColor[0] = r;
	m_iColor[1] = g;
	m_iColor[2] = b;
}

void CFont::SetAlpha(int iAlpha)
{
	m_iColor[3] = iAlpha;
}