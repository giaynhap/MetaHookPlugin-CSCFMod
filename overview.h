#ifndef OVERVIEW_H
#define OVERVIEW_H
#pragma comment (lib, "gdiplus.lib")
#define M_PI 3.14159265358979323846
extern int gDrawRadar;
extern	int gDrawTeamPost;
class COverview
{
public:
	COverview(void);

public:
	void Draw(void);
	bool CanDraw(void);
	void CalcRefdef(struct ref_params_s *pParams);
	void DrawFrame(void);
	bool IsValidPlayer(cl_entity_s *pEntity);
	bool CalcPoint(const float* origin, float& screenx, float& screeny, int check =1);
	void PlayerESP(void);
	void HUD_DrawText(int x, int y, int r, int g, int b, int a, vgui::HFont font, wchar_t *text);
	void DrawNumRadar2(int gImage,cl_entity_s *gEnt,int r,int g ,int b, int a);
	void DrawNumRadar3(int gImage,float post[3],int r,int g ,int b, int a);
	void DrawNumRadar4(int gImage,float post[3],int r,int g ,int b, int a);
public:
	bool m_bCanCheck, m_bCanDraw;
	model_t *m_pSprite;
	float m_flZoom;
	int m_iHeight;
	Vector2D m_vecOrigin,m_vecEyeOrigin;
	bool m_bIsRotate;
	cvar_t *m_pRect, *m_pRotate, *gRadarx, *gRadary;
	int m_iMaxClient;
	SCREENINFO sScreenInfo;

	float m_fYaw;
	int m_iInited;
	vgui::ISurface *m_pSurface;
	int m_iCircleID;
	int m_iCircleID2;
	int gdaranumg;
	int gdaranum2g;
	float m_fCurrentYaw;
	float m_fYawSin,m_fYawCos;
};
extern COverview *g_pOverview;

#endif