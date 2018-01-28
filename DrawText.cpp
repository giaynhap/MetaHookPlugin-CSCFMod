#include "Base.h"
#include "DrawText.h"
#include "exportfuncs.h"
#include "surface.h"
#include "Color.h"

#include <IEngineSurface.h>
#include <WinGDI.h>
#include <locale>
#include <codecvt>

//GN_Hud_Draw *gGN_Hud_Draw;

HDC hMemDC = NULL;
HDC hDC = NULL;

#define ADJUST_HEIGHT 15
bool Drawed = true;

float glR, glG, glB , glA;

#define MAX_CHAR       128
extern IEngineSurface *staticSurface;
int idText;
int idText2;
int idText3;
BYTE* ConvertToImage (Gdiplus::Bitmap *pBitmap,int *width,int *hight);
GNDrawText *gGNDrawText;
//vgui::HFont ghFont;
	float gHH = 1.0f;









 GNDrawText::GNDrawText(void)
{
		create = 0;
	
}


void GNDrawText::Redraw()
{
	if (create  != 1 )
	{
		
		
		create = 1;
		CreateInterfaceFn fnEngineCreateInterface = g_pMetaHookAPI->GetEngineFactory();
	   // vgui::ISurface *pSurface = (vgui::ISurface *)fnEngineCreateInterface(VGUI_SURFACE_INTERFACE_VERSION, NULL);
		m_pSurface = (vgui::ISurface *)fnEngineCreateInterface(VGUI_SURFACE_INTERFACE_VERSION, NULL);
		
		
		idText = m_pSurface->CreateNewTextureID();
		//	gGN_Hud_Draw->LoadImage();
	
		
	}
	
	//	int gWidth;
	//	int gHeight;
		
		//GN_Font_SetFont(L"Tahoma",12,1);
		//GN_Font_DrawText(L"GDI+ Giay Nhap text2 gtest - Font Tahoma Size 12",1,1,400,220, 255,255,255,255 );
		//GN_Font_SetFont(L"Tahoma",18,1);

		//GN_Font_DrawText(L"GDI+ Giay Nhap - Font Tahoma Size 24",1,0,100,280, 255,255,255,255 );

		//GN_Font_SetFont(L"Tahoma",12,2);
		//GN_Font_DrawText(L"GDI+ Giay Nhap - Font \nTahoma Size 12",1,0,5,400, 255,255,255,255 );


	//	hud_player_info_t PlayerInf;
		//gEngfuncs.pfnGetPlayerInfo(gEngfuncs.GetLocalPlayer()->index,&PlayerInf);
		//std::string gString(PlayerInf.name) ;
		//std::wstring wcstring(gString.size(), L' '); 
		//wcstring.resize(mbstowcs(&wcstring[0], gString.c_str(), gString.size()));
		//char *gwchart;
		//gwchart = const_cast<char *>(PlayerInf.name);
	//	gFont.DrawString(L"FreeType Giay Nhap - Font Tahoma Size 12 ",0,340,600,30);
	//	DrawStringFont(200,10,gwchart,"Tahoma",12,1,1,255,255,255,255)  ;
	//	DrawStringFont(200,25,"Giay Nhap Test Windown Font 16 - style (No boder)","Tahoma",16,1,0,255,255,255,255)  ;
	//	DrawStringFont(200,50,"Giay Nhap Test Windown Font 24 - style","Tahoma",24,1,1,255,255,255,255)  ;
		//selectFont(48, ANSI_CHARSET, "Comic Sans MS");

		//gEngfuncs.pfnDrawSetTextColor(1.0f,0.2f,0.2f);
		//gEngfuncs.pfnDrawConsoleString(600,500,"Giay NHap Skinner");
		
		//gGN_Hud_Draw->DrawBarHealth();
	   
}

int  GNDrawText::GN_Font_DrawText(wchar_t *text,int iOutLine,int Angl,int x, int y, int r, int g, int b, int a )
{
	static int idTextures=0;
	if (idTextures==0) idTextures= m_pSurface->CreateNewTextureID();
	int Width;
	int Height;
	GN_Font_SetDraw(idTextures ,text,&Width,&Height);
	if (iOutLine==1)
		{
			DrawTo(idTextures,x-1-Width*Angl,y, 0, 0, 0,a,Width,Height);
			DrawTo(idTextures,x+1-Width*Angl,y,0, 0, 0,a,Width,Height);
			DrawTo(idTextures,x-Width*Angl,y-1,0, 0, 0,a,Width,Height);
			DrawTo(idTextures,x-Width*Angl,y+1,0, 0, 0,a,Width,Height);
		}
	DrawTo(idTextures,x-Width*Angl,y,r,g,b,a,Width,Height);
	return 1;
}

void GNDrawText::DrawTo(int idTextures,int x,int y,int r, int g , int b,int alpha,int Width,int Height)
{
	if (alpha<=0) alpha = 0;
	if (Width<=0 || Height <=0 )
	m_pSurface->DrawGetTextureSize(idTextures, Width,Height);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, idTextures);
	glColor4ub(r, g, b, alpha);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3i(x, y + Height*gHH, 0);
	glTexCoord2f(1, 1);
	glVertex3i(x + Width*gHH, y + Height*gHH, 0);
	glTexCoord2f(1, 0);
	glVertex3i(x + Width*gHH, y, 0);
	glTexCoord2f(0, 0);
	glVertex3i(x, y, 0);
	glEnd();
}
void GNDrawText::GN_Font_int(void)
{
	Gdiplus::Bitmap *pBitma2p = new Gdiplus::Bitmap(500,20);
	graphicsMain = new Gdiplus::Graphics (pBitma2p);
	strformat = Gdiplus::StringFormat::GenericTypographic()->Clone();  
	delete pBitma2p;
}
void GNDrawText::GN_Font_SetFont(wchar_t *NameFontWindown, int sizef, int iStyle)
{
		
LOGFONTW lfont;  
memset( &lfont, 0, sizeof(lfont) );  

lfont.lfHeight = -sizef; //
lfont.lfWidth  = 0;
if (iStyle< 2 )
lfont.lfWeight      = 400*(iStyle+1);
else
{
	if (iStyle==2)
	lfont.lfItalic      = TRUE; 
	else if (iStyle==3)
	lfont.lfUnderline   = TRUE;   
}      
lfont.lfCharSet     = GB2312_CHARSET; 
lfont.lfQuality         = 5;
lfont.lfPitchAndFamily  = DEFAULT_PITCH | FF_SWISS; 
wcscpy( lfont.lfFaceName, (NameFontWindown) ); 
HDC hHDC= ::GetDC(NULL);
hFont = new Gdiplus::Font( hHDC, &lfont );  
::ReleaseDC( NULL, hHDC ); 
}
int GNDrawText::GN_Font_SetDraw(int idText ,wchar_t *szTextDraw,int *Width,int *Height)
{
		Gdiplus::SolidBrush *ghbrush = new  Gdiplus::SolidBrush(Gdiplus::Color(0,0,255));
		int strLen = wcslen(szTextDraw);
		graphicsMain->MeasureString(szTextDraw,strLen,hFont,Gdiplus::PointF(0,0),&gRect2);
		Gdiplus::Bitmap *pBitmap = new Gdiplus::Bitmap(gRect2.Width,gRect2.Height);
		Gdiplus::Graphics graphics(pBitmap);
		graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
		graphics.Clear(Gdiplus::Color(0,0,0));
		strformat->SetAlignment( Gdiplus::StringAlignmentNear );  
		strformat->SetLineAlignment( Gdiplus::StringAlignmentNear );  
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);	
		graphics.DrawString(szTextDraw,strLen,hFont,Gdiplus::RectF(0,0,gRect2.Width,gRect2.Height), strformat ,ghbrush);
		BYTE *buffer = ConvertToImage (pBitmap,&gwidth,&gheight);
		glBindTexture(GL_TEXTURE_2D, idText);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, gwidth, gheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		*Width = gRect2.Width;
		*Height = gRect2.Height;
		delete pBitmap;
		return 1;

}
BYTE* ConvertToImage (Gdiplus::Bitmap *pBitmap,int *width, int *height)
{
	int bitmapWidth = pBitmap->GetWidth();
	int bitmapHeight = pBitmap->GetHeight();
	Gdiplus::Rect bitmapArea (0, 0, bitmapWidth, bitmapHeight);
	Gdiplus::BitmapData bitmapData;
	pBitmap->LockBits (&bitmapArea, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);
	pBitmap->UnlockBits(&bitmapData);
	BYTE *imageData = (BYTE *)bitmapData.Scan0;
	int stride = bitmapData.Stride;

	UINT p = 0; 

	for(INT Y = 0; Y < bitmapWidth; ++Y)
	{
		for(INT X = 0; X < bitmapHeight; ++X)
		{
			
			imageData[p + 3] = imageData[p + 0];
			if (imageData[p + 3]<0) imageData[p + 3]=0;
			imageData[p + 0] = 0xFF;
			imageData[p + 2] =0xFF;
			imageData[p + 1]=0xFF;
			p+=4;
		}
	}
	*width = bitmapWidth;
	*height = bitmapHeight;
	return imageData;
}


void CreateBitmap(BITMAP *bmp, LONG width, LONG height) 
{ 
   FillMemory(bmp, sizeof(*bmp), 0); 
   bmp->bmWidth = width; 
   bmp->bmHeight = height; 
   bmp->bmWidthBytes = ((width + 7) / 8 + 1) & (~1); 
   bmp->bmPlanes = 1; 
   bmp->bmBitsPixel = 1; 
   bmp->bmBits = GlobalAlloc(GMEM_FIXED, bmp->bmWidthBytes*height); 
} 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char *GetBitmapBits(HDC hDC, HBITMAP hBmp, int* size) 
{ 
   BITMAP bi; 
   unsigned char   *bits; 
   struct { 
       BITMAPINFOHEADER bih; 
       RGBQUAD col[2]; 
   }bic; 

   BITMAPINFO *binf = (BITMAPINFO *)&bic; 
   GetObject(hBmp, sizeof(bi), &bi); 
   *size = bi.bmHeight*(((bi.bmWidth + 31) & (~31)) / 8); 
   bits = (unsigned char *)GlobalAlloc(GPTR, *size); 
   binf->bmiHeader.biSize = sizeof(binf->bmiHeader); 
   binf->bmiHeader.biWidth = bi.bmWidth; 
   binf->bmiHeader.biHeight = bi.bmHeight; 
   binf->bmiHeader.biPlanes = 1; 
   binf->bmiHeader.biBitCount = 1; 
   binf->bmiHeader.biCompression = BI_RGB; 
   binf->bmiHeader.biSizeImage = *size; 
   binf->bmiHeader.biXPelsPerMeter = 1; 
   binf->bmiHeader.biYPelsPerMeter = 1; 
   binf->bmiHeader.biClrUsed = 0; 
   binf->bmiHeader.biClrImportant = 0; 
   GetDIBits(hDC, hBmp, 0, bi.bmHeight, bits, binf, DIB_RGB_COLORS); 
   return bits; 
} 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char *CreateFontBitmap(HDC hDC, HFONT hFont, char *str, PSIZEL size) 
{ 
   BITMAP  bmp; 
   HBITMAP hbmp; 
   unsigned char    *pbm; 
   int     len = lstrlen(str); 
   
   HFONT   hFontOld = (HFONT)SelectObject(hDC, hFont); 

   GetTextExtentPoint32(hDC, str, len, size); 
   
   CreateBitmap(&bmp, size->cx, size->cy); 
   hbmp = CreateBitmapIndirect(&bmp); 

   GlobalFree(bmp.bmBits); 
   DeleteObject(&bmp); 

   if(hbmp && hMemDC)
   { 
     HBITMAP hPrevBmp = (HBITMAP)SelectObject(hMemDC, hbmp); 
     HFONT   hPrevFont; 
     int     size0; 
     BITMAP  bi; 

     SetBkColor(hMemDC, RGB(0,0,0)); 
     SetTextColor(hMemDC, RGB(0,0,255)); 
     SetBkMode(hMemDC, OPAQUE); 

     hPrevFont = (HFONT)SelectObject(hMemDC, hFont); 


	// BeginPath (hMemDC);
	 TextOut(hMemDC, 0, 0, str, len);
	  
	 
	// EndPath(hMemDC);
     GetObject(hbmp, sizeof(bi), &bi); 
	
     pbm = GetBitmapBits(hMemDC, hbmp, &size0); 
	 
     size->cx = ((bi.bmWidth+31)&(~31)); 
     size->cy = bi.bmHeight; 

	 GlobalFree(bi.bmBits); 
     DeleteObject(&bi); 

	 DeleteObject(hPrevFont); 
	 DeleteObject(hPrevBmp);
	 DeleteObject(hbmp); 
   } 
   SelectObject(hDC, hFontOld); 
    UINT p = 0; 
  
   return pbm; 
} 

void DrawSetTextColorEx(float r, float g, float b, float a)
{
	glR = r;
	glG = g;
	glB = b;
	glA = b;
}

int DrawStringFont(int x,int y,char *str,char *FontName,int FontSize,int StyleFont,int boder ,int r , int g, int b, int a,int angle ) 
{ 
   LOGFONT lf; 
   HFONT   hFont; 
   unsigned char *lpBitmap;
   SIZE size; 
   
   if(!hDC)
   {
	 hDC = wglGetCurrentDC();
	 hMemDC = CreateCompatibleDC(hDC);
   }
   HDC hDC2 = wglGetCurrentDC();


   FillMemory(&lf, sizeof(lf), 0); 
   lf.lfHeight = FontSize;
   lf.lfCharSet = ANSI_CHARSET; 

   if (StyleFont< 2 )
	  lf.lfWeight = 400*(StyleFont+1); 
	else
	{
		if (StyleFont==2)
		lf.lfItalic      = TRUE; 
		else if (StyleFont==3)
		lf.lfUnderline   = TRUE;   
		else if (StyleFont = 4)
		{
		lf.lfWeight = 800;
		lf.lfItalic      = TRUE; 
		}
		else if (StyleFont = 5)
		{
		lf.lfWeight = 800;
		lf.lfUnderline      = TRUE; 
		}
		else if (StyleFont = 6)
		{
			lf.lfWeight = 800;
			lf.lfUnderline      = TRUE; 
			lf.lfItalic      = TRUE; 
		}
		else if (StyleFont = 7)
		{
			lf.lfWeight = 400;
			lf.lfUnderline      = TRUE; 
			lf.lfItalic      = TRUE; 
		}
	}      
   lf.lfOutPrecision = OUT_STROKE_PRECIS; 
   lf.lfClipPrecision = CLIP_CHARACTER_PRECIS; 
   lstrcpy(lf.lfFaceName, FontName); 
	
   hFont = CreateFontIndirect(&lf); 
   GlobalFree(&lf);
    RECT rect = { 0, 0, 0, 0 };
   lpBitmap = CreateFontBitmap(hDC, hFont, str, &size); 
   SelectObject(hDC2, hFont);
  DrawText(hDC2,str, strlen(str), &rect, DT_CALCRECT | DT_NOPREFIX | DT_SINGLELINE);
   
   int textWidth = int(abs(rect.right - rect.left));
   DeleteObject(hFont);
  
	  DeleteDC(hDC2);
glDisable(GL_TEXTURE_2D);                      
glDisable(GL_LIGHTING);               
glEnable(GL_COLOR_MATERIAL);

int post_add = 0;
if (angle==1)
	post_add=-textWidth/2;
else if (angle == 2) post_add = -textWidth;

 int g2 =(x + 1+post_add)%2;
 
if (boder==1)
{
	glColor4ub(0, 0, 0, a);
	glRasterPos2f((float)x + 1+post_add, (float)y + 1 + ADJUST_HEIGHT); 
	//glRasterPos2f((int (x + 1+post_add)+1), (int (y + 1 + ADJUST_HEIGHT))); 
	glBitmap(size.cx, size.cy, 0.0, -1.0, 0.0, 0.0, lpBitmap);
	
//	glRasterPos2f((int(x + 1+post_add)-1), (int(y + 1 + ADJUST_HEIGHT))); 
	glBitmap(size.cx, size.cy, -1.0, 0.0, 0.0, 0.0, lpBitmap);
	
	//glRasterPos2f((int(x + 1+post_add)), (int(y + 1 + ADJUST_HEIGHT)+1)); 
	glBitmap(size.cx, size.cy, 0.0, 1.0f, 0.0, 0.0, lpBitmap);
	
	//glRasterPos2f((int(x + 1+post_add)), (int(y + 1 + ADJUST_HEIGHT)-1)); 
	glBitmap(size.cx, size.cy, 1.0f, 0.0, 0.0, 0.0, lpBitmap);
}

DrawSetTextColorEx( r , g, b, a);
//glColor3f(glR, glG, glB);
glColor4ub(r, g, b, a);
glRasterPos2f((float)x + 1+post_add, (float)y + 1 + ADJUST_HEIGHT); 
glBitmap(size.cx, size.cy, 0.0, 0.0, 0.0f, 0.0f, lpBitmap);



glEnable(GL_TEXTURE_2D);
Drawed = true;
GlobalFree(lpBitmap); 
return textWidth;
} 


///;/////////////////////////////// Wchar

unsigned char *CreateFontBitmapW(HDC hDC, HFONT hFont, wchar_t *str, PSIZEL size) 
{ 
   BITMAP  bmp; 
   HBITMAP hbmp; 
   unsigned char    *pbm; 
   int     len = wcslen(str); 
   
   HFONT   hFontOld = (HFONT)SelectObject(hDC, hFont); 

   GetTextExtentPoint32W(hDC, str, len, size); 
   
   CreateBitmap(&bmp, size->cx, size->cy); 
   hbmp = CreateBitmapIndirect(&bmp); 

   GlobalFree(bmp.bmBits); 
   DeleteObject(&bmp); 

   if(hbmp && hMemDC)
   { 
     HBITMAP hPrevBmp = (HBITMAP)SelectObject(hMemDC, hbmp); 
     HFONT   hPrevFont; 
     int     size0; 
     BITMAP  bi; 

     SetBkColor(hMemDC, RGB(0,0,0)); 
     SetTextColor(hMemDC, RGB(0,0,255)); 
     SetBkMode(hMemDC, OPAQUE); 

     hPrevFont = (HFONT)SelectObject(hMemDC, hFont); 


	// BeginPath (hMemDC);
	 TextOutW(hMemDC, 0, 0, str, len);
	  
	 
	// EndPath(hMemDC);
     GetObject(hbmp, sizeof(bi), &bi); 
	
     pbm = GetBitmapBits(hMemDC, hbmp, &size0); 
	 
     size->cx = ((bi.bmWidth+31)&(~31)); 
     size->cy = bi.bmHeight; 

	 GlobalFree(bi.bmBits); 
     DeleteObject(&bi); 

	 DeleteObject(hPrevFont); 
	 DeleteObject(hPrevBmp);
	 DeleteObject(hbmp); 
   } 
   SelectObject(hDC, hFontOld); 
    UINT p = 0; 
  
   return pbm; 
} 

int DrawStringFontW(int x,int y,wchar_t *str,char *FontName,int FontSize,int StyleFont,int boder ,int r , int g, int b, int a,int angle ) 
{ 
   LOGFONT lf; 
   HFONT   hFont; 
   unsigned char *lpBitmap;
   SIZE size; 
   
   if(!hDC)
   {
	 hDC = wglGetCurrentDC();
	 hMemDC = CreateCompatibleDC(hDC);
   }
   HDC hDC2 = wglGetCurrentDC();


   FillMemory(&lf, sizeof(lf), 0); 
   lf.lfHeight = FontSize;
   lf.lfCharSet = ANSI_CHARSET; 

   if (StyleFont< 2 )
	  lf.lfWeight = 400*(StyleFont+1); 
	else
	{
		if (StyleFont==2)
		lf.lfItalic      = TRUE; 
		else if (StyleFont==3)
		lf.lfUnderline   = TRUE;   
		else if (StyleFont = 4)
		{
		lf.lfWeight = 800;
		lf.lfItalic      = TRUE; 
		}
		else if (StyleFont = 5)
		{
		lf.lfWeight = 800;
		lf.lfUnderline      = TRUE; 
		}
		else if (StyleFont = 6)
		{
			lf.lfWeight = 800;
			lf.lfUnderline      = TRUE; 
			lf.lfItalic      = TRUE; 
		}
		else if (StyleFont = 7)
		{
			lf.lfWeight = 400;
			lf.lfUnderline      = TRUE; 
			lf.lfItalic      = TRUE; 
		}
	}      
   lf.lfOutPrecision = OUT_STROKE_PRECIS; 
   lf.lfClipPrecision = CLIP_CHARACTER_PRECIS; 
   lstrcpy(lf.lfFaceName, FontName); 
	
   hFont = CreateFontIndirect(&lf); 
   GlobalFree(&lf);
    RECT rect = { 0, 0, 0, 0 };
   lpBitmap = CreateFontBitmapW(hDC, hFont, str, &size); 
   SelectObject(hDC2, hFont);
  DrawTextW(hDC2,str, wcslen(str), &rect, DT_CALCRECT | DT_NOPREFIX | DT_SINGLELINE);
   
   int textWidth = int(abs(rect.right - rect.left));
   DeleteObject(hFont);
  
	  DeleteDC(hDC2);
glDisable(GL_TEXTURE_2D);                      
glDisable(GL_LIGHTING);               
glEnable(GL_COLOR_MATERIAL);

int post_add = 0;
if (angle==1)
	post_add=-textWidth/2;
else if (angle == 2) post_add = -textWidth;

 int g2 =(x + 1+post_add)%2;
 
if (boder==1)
{
	glColor4ub(0, 0, 0, a);
	glRasterPos2f((float)x + 1+post_add, (float)y + 1 + ADJUST_HEIGHT); 
	//glRasterPos2f((int (x + 1+post_add)+1), (int (y + 1 + ADJUST_HEIGHT))); 
	glBitmap(size.cx, size.cy, 0.0, -1.0, 0.0, 0.0, lpBitmap);
	
//	glRasterPos2f((int(x + 1+post_add)-1), (int(y + 1 + ADJUST_HEIGHT))); 
	glBitmap(size.cx, size.cy, -1.0, 0.0, 0.0, 0.0, lpBitmap);
	
	//glRasterPos2f((int(x + 1+post_add)), (int(y + 1 + ADJUST_HEIGHT)+1)); 
	glBitmap(size.cx, size.cy, 0.0, 1.0f, 0.0, 0.0, lpBitmap);
	
	//glRasterPos2f((int(x + 1+post_add)), (int(y + 1 + ADJUST_HEIGHT)-1)); 
	glBitmap(size.cx, size.cy, 1.0f, 0.0, 0.0, 0.0, lpBitmap);
}

DrawSetTextColorEx( r , g, b, a);
//glColor3f(glR, glG, glB);
glColor4ub(r, g, b, a);
glRasterPos2f((float)x + 1+post_add, (float)y + 1 + ADJUST_HEIGHT); 
glBitmap(size.cx, size.cy, 0.0, 0.0, 0.0f, 0.0f, lpBitmap);



glEnable(GL_TEXTURE_2D);
Drawed = true;
GlobalFree(lpBitmap); 
return textWidth;
} 
