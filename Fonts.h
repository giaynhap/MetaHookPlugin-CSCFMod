
#ifndef _FONT_H
#define _FONT_H


#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_STROKER_H
#define FONTS_MAX_BUFFER	2000
#define FONT_PAGE_MAX	8


struct CHARINFO
{
	int m_iTexture;
	int m_iWidth;
	int m_iHeight;
	int m_iDeltaWidth;
	int m_iDeltaHeight;
	int m_iAdvWidth;
	int m_iAdvHeight;
	BYTE *pTexture;
	float m_fCoord[2][2];
	int m_iCharWidth;
	wchar_t wChar;
};




class CFont
{
public:
	void Init(void);
	void Shutdown(void);

	int		DrawString(wchar_t* szText,int x , int y, int iMaxWidth, int iHeightAdd = 0);
	void	SetSize(int iWidth, int iHeight);
	void	SetColor(int, int, int);
	void	SetAlpha(int);
	int		GetLen(wchar_t* szText);
	int		GetHeight(wchar_t* szText, int iMaxWidth, int iHeightAdd);
	void	C2W(wchar_t *pwstr, size_t len, const char *str);

private:
	int		AddCharInToPage(CHARINFO *pChar);
	void	LoadStr(wchar_t* _strText);
	int		CheckCharExists(wchar_t ch);
	int		LoadChar(wchar_t ch);
	CHARINFO *GetChar(wchar_t ch);

private:
	FT_Library  m_sLibrary;
	FT_Face     m_sFace;

	int		m_iColor[4];
	int		m_iPage[64][FONT_PAGE_MAX];
	int		m_iPageCount[64][FONT_PAGE_MAX];
	bool	m_bPageFull[64][FONT_PAGE_MAX];

	int m_iCharCount;
	int m_iWidth,m_iHeight;
	int m_iBuffer[FONTS_MAX_BUFFER];
	CHARINFO m_tFontTexture[FONTS_MAX_BUFFER];
	
	
};



extern CFont gFont;


#endif