#ifndef KILLMARK_H
#define KILLMARK_H
#include "Base.h"
class KillMark
{
	float KILLCOMBOTIME;
	float KILLEFFECTTIME;
public:
	KillMark();
public :
	void LoadTextures();
	void OnDeathMsg(int combokill,int gHead=0,int gWall=0,int SpecialKill=0,int heKnife=0,int Revenge=0);
	void Redraw();
	void DrawImage(int idTextures,int isCreate,int x,int y,float scale,int r, int g , int b, int alpha,int width = 0,int height = 0);
	int gHeadShot;
	void KillmarkReset();

private:
	float AlphaImage[2];
	float _startDrawTime;
	float _lastComboKillTime;
	float _TimeRandom;
	int _killMarkType;
	int _currentRenderingMark;
	int _markTextures[17];
	int _markEffectTextures[7];
	int _comboKill;
	int _markTextureWidth;
	int _markTextureHeight;
	int _markEffectTextureWidth;
	int _markEffectTextureHeight;
	int RandomPos[2];
	float gScaler[2] ;
	int gIsHide;
	int gIsHide2;
	int gHeKnife;
	int gkWall;
	int gRevenge;
	vgui::ISurface *m_pSurface;
	SCREENINFO gScreenInfo;
	int gCreate;
	int gSpecialKill;
};
extern KillMark *gnKillMark;

#endif