#include "Base.h"

extern int DrawStringFontW(int x,int y,wchar_t *str,char *FontName,int FontSize,int StyleFont,int boder ,int r , int g, int b, int a,int angle=0 ) ;
int DrawStringFont(int x,int y,char *str,char *FontName,int FontSize,int StyleFont,int boder ,int r , int g, int b, int a,int angle = 0)  ;
class GNDrawText
{
public:
	GNDrawText(void);
public :
	void Redraw();
	int GN_Font_SetDraw(int idText ,wchar_t *szTextDraw,int *Width,int *Height);
	void DrawTo(int idTextures,int x,int y,int r, int g , int b,int alpha,int Width,int Height);
	
	int Fonts_LoadChar(wchar_t ch);
	void GN_Font_int(void);
	void GN_Font_SetFont(wchar_t *NameFontWindown, int sizef, int iStyle);
	int GN_Font_DrawText(wchar_t *text,int iOutLine,int Angl,int x, int y, int r, int g, int b, int a );
	//void DrawUnicodeCharAdd(wchar_t wch,vgui::HFont font);
private:
	vgui::HFont g_hFont;
	vgui::ISurface *m_pSurface;
	int create;
	Gdiplus::Graphics *graphicsMain;
	Gdiplus::StringFormat *strformat;
	Gdiplus::Font *hFont;
	Gdiplus::RectF gRect2;
	int gwidth;
	int gheight;
};
extern GNDrawText *gGNDrawText;