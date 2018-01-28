#include "Base.h"
#include "Graphic.h"
#include "KillMark.h"
#include "Global.h"
#include "exportfuncs.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

#include <math.h>

#include "HudDraw.h"
//cl_enginefunc_t gEngfuncs;
int isLoadKill;
int PostVGuiSetting[512][4];
KillMark *gnKillMark;
float TimeLoadSetHudKillmark;
KillMark::KillMark()
{
_startDrawTime = 0;
_lastComboKillTime = 0;
_killMarkType = 0;
_comboKill = 0;
_currentRenderingMark = 0;
KILLCOMBOTIME = 5.0f;
KILLEFFECTTIME = 1.3f;
_markTextureWidth = 0;
_markTextureHeight = 0;
}
void KillMark::KillmarkReset()
{
_startDrawTime = 0;
_lastComboKillTime = 0;
_killMarkType = 0;
_comboKill = 0;
_TimeRandom = 0.0;
_currentRenderingMark = 0;
KILLCOMBOTIME = 5.0f;
KILLEFFECTTIME = 1.3f;
_markTextureWidth = 0;
_markTextureHeight = 0;
gIsHide = 0;
gIsHide2 = 0;
gScaler[0] = 0.0;
gScaler[1] = 0.0;
}
void KillMark::OnDeathMsg(int combokill,int gHead,int gWall,int SpecialKill,int heKnife,int Revenge)
{
	gkWall= gWall;
	gRevenge = Revenge;
	gSpecialKill = SpecialKill;
	
	float currentTime = gEngfuncs.GetClientTime();
	_startDrawTime = currentTime ;
	AlphaImage[0] = 255;
	AlphaImage[1] = 255;
	gIsHide = 0;
	gIsHide2 = 0;
	gScaler[0] = 0.0;
	gScaler[1] = 0.0;
	if (gHead<0) gHead=0;
	gHeadShot=gHead;
	gHeKnife = heKnife;
	//_comboKill ++;
	if (combokill>6) combokill = 6;
	_comboKill = combokill;
	
}
void KillMark::LoadTextures()
{
	// base textures

	
	if (isLoadKill!=1)
	{
		isLoadKill =1;
	gEngfuncs.Con_Printf("LoadKill \n");
		gScreenInfo.iSize = sizeof(gScreenInfo);
	gEngfuncs.pfnGetScreenInfo(&gScreenInfo);
	CreateInterfaceFn fnEngineCreateInterface = g_pMetaHookAPI->GetEngineFactory();

	char szTexture[64];
	m_pSurface = (vgui::ISurface *)fnEngineCreateInterface(VGUI_SURFACE_INTERFACE_VERSION, NULL);
	for(int i=1 ; i< 7 ;i++)
	{
		sprintf(szTexture,"CSFIRED/KILL_MARK/badge_multi%d",i);
//		_markTextures[i] = gGraphic->LoadTgaImage(szTexture);

		_markTextures[i] = m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(_markTextures[i],szTexture,true,true);
	}

	_markTextures[7] =  m_pSurface->CreateNewTextureID();
	m_pSurface->DrawSetTextureFile(_markTextures[7],"CSFIRED/KILL_MARK/badge_knife",true,true);

	_markTextures[8] =  m_pSurface->CreateNewTextureID();
	m_pSurface->DrawSetTextureFile(_markTextures[8],"CSFIRED/KILL_MARK/badge_grenade",true,true);
	
	_markTextures[9] =  m_pSurface->CreateNewTextureID();
	m_pSurface->DrawSetTextureFile(_markTextures[9],"CSFIRED/KILL_MARK/badge_headshot",true,true);

	_markTextures[10] =  m_pSurface->CreateNewTextureID();
	m_pSurface->DrawSetTextureFile(_markTextures[10],"CSFIRED/KILL_MARK/badge_headshot_gold",true,true);
	
	_markTextures[11] =  m_pSurface->CreateNewTextureID();
	m_pSurface->DrawSetTextureFile(_markTextures[11],"CSFIRED/KILL_MARK/badge_wallshot",true,true);

	_markTextures[12] =  m_pSurface->CreateNewTextureID();
	m_pSurface->DrawSetTextureFile(_markTextures[12],"CSFIRED/KILL_MARK/badge_headwallshot",true,true);

	_markTextures[13] =  m_pSurface->CreateNewTextureID();
	m_pSurface->DrawSetTextureFile(_markTextures[13],"CSFIRED/KILL_MARK/badge_headwallshot_gold",true,true);

	_markTextures[14] =  m_pSurface->CreateNewTextureID();
	m_pSurface->DrawSetTextureFile(_markTextures[14],"CSFIRED/KILL_MARK/FIRSTKILL",true,true);

	_markTextures[15] =  m_pSurface->CreateNewTextureID();
	m_pSurface->DrawSetTextureFile(_markTextures[15],"CSFIRED/KILL_MARK/LastKill",true,true);

	_markTextures[16] =  m_pSurface->CreateNewTextureID();
	m_pSurface->DrawSetTextureFile(_markTextures[16],"CSFIRED/KILL_MARK/Revenge",true,true);

	m_pSurface->DrawGetTextureSize(_markTextures[1],_markTextureWidth,_markTextureHeight);
	
	for(int i=2 ; i< 7 ;i++)
	{
			sprintf(szTexture,"CSFIRED/KILL_MARK/fx/%d.tga",i);
		// = gGraphic->LoadTgaImage(szTexture);
			_markEffectTextures[i] = m_pSurface->CreateNewTextureID();
		//	m_pSurface->DrawSetTextureFile(_markEffectTextures[i],szTexture,true,true);
			LoadTgaImage( szTexture, _markEffectTextures[i],&_markEffectTextureWidth,&_markEffectTextureHeight );
	}
	//m_pSurface->DrawGetTextureSize(_markEffectTextures[2],_markEffectTextureWidth,_markEffectTextureHeight);
	}
	gGN_Hud_Draw->LoadImage();
}
void KillMark::Redraw()
{
	float currentTime = gEngfuncs.GetClientTime();

	if (abs(currentTime-TimeLoadSetHudKillmark)<0.01)  return ;
	TimeLoadSetHudKillmark = currentTime;
	if(currentTime - _startDrawTime > KILLCOMBOTIME) return;

	float fDelta = currentTime - _startDrawTime ;
//	if(fDelta < 0) return;

	int centerX = 0;
	int centerY = 0;

	//return ;
	
	centerX = gScreenInfo.iWidth/2;
	centerY =gScreenInfo.iHeight-150;	
	if (fDelta< 0.4)
	{
				
	if (abs(currentTime-_TimeRandom)>0.01)
	{
		_TimeRandom = currentTime;
		if (gScaler[0] < 1.2)
		{
			gScaler[0]+=0.2;
			gIsHide = 1;
		}
		if (gScaler[0]>1.0 && gIsHide == 1)
		{
			gScaler[0]-=0.1;
		}
		if(gScaler[1]<2.5)
		{	
			if (gIsHide==1)
			gScaler[1]+=0.4;
			else
			gScaler[1]+=0.2;

		}
		else
		gIsHide2 = 1;
		if (gIsHide2==1 )
		{
		if (gScaler[1]>1.0) 
		gScaler[1]-=1.0;
		else
		gScaler[1] = 1.0;
		if (gScaler[1]<1.0) gScaler[1] = 1.0;
		AlphaImage[1]-=20;
		}
	}
	RandomPos[0] = rand() % (10+1+10) + -10;
	RandomPos[1] = rand() % (5+1+5) + -5;
	}
	else
	{
		
		//{
			if (gScaler[1]>1.0) 
			gScaler[1]-=0.6;
			else
			gScaler[1] = 1.0;
		//}
		gScaler[0] = 1.0;
		RandomPos[0] = 0;
		RandomPos[1] = 0;

		if (AlphaImage[1]>0)
		AlphaImage[1]-=20;
		
		
	
		if (fDelta > 0.7)
		{
			if (AlphaImage[0]>0)
			AlphaImage[0]-=20;
			

		}
	}
	int gTgaInt=0;
		//}
		//gGraphic->DrawTgaImageScale(_markTextures[_comboKill],centerX,centerY,gScaler,255);
		if (gSpecialKill==0)
		{
			gTgaInt = _comboKill;

			if (_comboKill==1)
			{
				if (gHeadShot==1)
				{
					if (gkWall==1) gTgaInt=12; else gTgaInt=9;
				}
				else if (gHeadShot==2)
				{
					if (gkWall==1) gTgaInt=13; else gTgaInt=10;
				}
				else if (gHeKnife==1) gTgaInt=8;
				
				else if (gHeKnife==2) gTgaInt=7;
				
				else if (gkWall==1) gTgaInt=11;
				
			}
		}
		else
		{
			if (gSpecialKill==1) gTgaInt = 14;
			else if (gSpecialKill==2)  gTgaInt = 15;
			//gRevenge
			//gTgaInt=;
		}
		
		
		KillMark::DrawImage(_markTextures[gTgaInt],1,centerX+RandomPos[0],centerY+ RandomPos[1],gScaler[0],255,255,255,AlphaImage[0]);
		if (gRevenge==1)
		KillMark::DrawImage(_markTextures[16],1,centerX+RandomPos[0],centerY+ RandomPos[1]-60,gScaler[0],255,255,255,AlphaImage[0]);

		
		if (_comboKill>1)
		KillMark::DrawImage(_markEffectTextures[_comboKill],1,centerX+RandomPos[0],centerY+ RandomPos[1],gScaler[1],255,255,255,AlphaImage[1],_markEffectTextureWidth,_markEffectTextureHeight);
	
}
void KillMark::DrawImage(int idTextures,int isCreate,int x,int y,float scale,int r, int g , int b, int alpha,int width,int height)
{
	if (alpha<=0) alpha = 0;
	int Width = width;
	int Height = height;
	if (width<1||height<1)
	m_pSurface->DrawGetTextureSize(idTextures, Width,Height);
	
	//gEngfuncs.pTriAPI->RenderMode(kRenderTransAlpha);
	//m_pSurface->DrawSetTexture(idTextures);
	//m_pSurface->DrawSetColor(Color(r,g,b,alpha));
	if (isCreate ==1) 
	{
		Width = floorf(Width*scale);
		Height = floorf(Height*scale);
		x -= Width/2;
		y-= Height/2;
	}
	
//	m_pSurface->DrawTexturedRect(x,y,x+Width,y+Height);

	
  	glEnable(GL_BLEND);
	//glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	

	glBindTexture(GL_TEXTURE_2D,idTextures);
	glColor4ub(r,g,b,alpha);

	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex3f(x,y+Height,0);

	glTexCoord2f(1,1);
	glVertex3f(x+Width,y+Height,0);
		
	glTexCoord2f(1,0);
	glVertex3f(x+Width,y,0);
	glTexCoord2f(0,0);
	glVertex3f(x,y,0);
	glEnd();
	//gEngfuncs.pTriAPI->RenderMode(kRenderTransTexture);



	//gEngfuncs.Con_Printf("DrawKill x=\"%i\"\ny=\"%i\"",x,y);

}