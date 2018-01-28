#ifndef HUDDRAW_H
#define HUDDRAW_H
#include "Base.h"
extern float PosBompP[2][3];
extern int ShowMenuScope;
extern int ControlChosseScope;
extern int Check_Mouse_Menu_Scope(float x,float y);
void gSprite(void);
extern void SetCrossColor(int r ,int g ,int b );
extern float gCrossScale;
extern float TimeReturnColor;
extern int isShowCross;
extern float TimeReturnCross;
extern bool isEnableCross;
extern float MaxSizeCross;
extern float TimeCrossResize;
extern int BompIdPlayer;
extern float BompPost[3];
extern int isBompStatus;
extern int PostVGuiSetting[512][4];
extern int	gResetDataOption ;
extern int ButtonNexDownSPEED[2];
extern int DataMouse;
extern bool isModelButtonSetting;
int const NumshowMaxDeathMsg = 4;
extern int gButtonOptionClose;
extern int gButtonOpticonControl[19];
extern int gButtonOptionSave;
extern char gszData_Config[20][32];
void clientPrinet(int id);
const int MAX_CHANNEL=999;
extern int IsClickMouse;
//int gBodyWeaponView ;
extern int gShowUiMap;
extern int g_iBombEntity;
extern int gBompDrop;
extern int gScoreAllPlayer[3][33];
extern int gPlayerAce[3];
extern int gHMouseClick[2];
extern char gDatakey_Config[32][32]; 
extern int ButtonSoundSpeed[2];
extern int ButtonCheckTabop3[4];
extern int gDataMouseTab3[2];
extern int DataMouse;
extern  int ButtonMouseSpeed[2];
extern int gDataSound[3];
extern int GStartSearch[2];
int SetButtonPOS(int gBut,int x,int y);
int Check_VguiSeting_CRTL(float x,float y,int GStart = 1);
int CheckPost(float x,float y);
int  LoadTgaImage(char *szFileName, int Index,int *SizeW,int *SizeH);
extern int isMenuDraw;
const int UI_ESC = 1;
const int UI_MAP = 2;
const int UI_OPTION_1 = 3;
const int UI_OPTION_2 = 4;
const int UI_OPTION_3 = 5;
#define ME_LEFTCLICK	1
#define ME_RIGHTCLICK	2
#define ME_WHEELCLICK	4
extern HWND HwndGame;
extern float TimeNextClick;
typedef struct gnUserData_s
{
	int hp;
    int acc ;
	int ammo[32];
	int clip;
	int weaponid;
	int iAmmoType;
} gnUserData_t;
typedef struct intrgba_color_s
{     int r;
	  int g;
	  int b;
	  int a;
}intrgba_color_t;
typedef struct g_load_text_add_s
{
	char TextFile[64];
	int index;
	int TextSize[2];

	
}g_load_text_add_t;
typedef struct g_chat_msg_s
{
	char TextChat[512];
	int gShow;
	int r;
	int g;
	int b;
	int a;
	
}g_chat_msg_t;
typedef struct g_draw_text_add_s
{
	int Type;
	int index;
	int Show;
	float TimeStart;
	float TimeCount;
	int Effect;
	float Timein;
	float Timeout;
	int alpha;
	int x;
	int y;
	int r;
	int g;
	int b;
	int a;
	float scaler;
	int center;
	char Font[64];
	char Str[1024];
	int sizefont;

	int bold;

	int boder;
}g_draw_text_add_t;
typedef struct deathMsg_info_s
{
	char gMsgDeathstring[2][64];
	int IsSKill;
	int NumSkill;
	int Team1;
	int Team2;
	int iType;
	int show;
	char szWeapon[64];
}deathMsg_info_t;
enum Effect 
{
	Effect_Nomal, Effect_Addtive
};

typedef struct loadtga_s
{
	byte	id_length;
	byte	colormap_type;
	byte	image_type;
	word	colormap_index;
	word	colormap_length;
	byte	colormap_size;
	word	x_origin;
	word	y_origin;
	word	width;
	word	height;
	byte	pixel_size;
	byte	attributes;
}loadtga_t ;
typedef struct g_draw_icon_s{
	int Show;
	int x;
	int y;
	int size[2];
	int type;
	int iEnt;
	float TimeCount;
	float TimeStart;
	int r;
	int g;
	int b;
	char szFile[512];
	int index;
}g_draw_icon_t;


typedef struct g_player_inf_s
{


	hud_player_info_t gINFent;

	int Team;
	int die;
	


}g_player_inf_t;
class GN_Hud_Draw
{
public:
	GN_Hud_Draw(void);
public :
	void LoadImage();
	void DrawAllHud();
	void DrawFollowIcon(void);
	void DrawImage(int idTextures,int isCreate,int x,int y,float scale,int r, int g , int b, int alpha,int width=0,int height=0);
	void GN_HUD_DrawNumTga(int Num,int Create,int x,int y,int r, int g , int b, int alpha,int  tgaindex[10], int Width);
	int addresource(char *szFile,int checkHas=1);
	int getHasImage(char *szFile);
	void SetScalerDraw(int channel,float Scaler);
	int SetDrawTga(char *szFile , int channel, int Effect,int x,int y,int r, int g,int b, int alpha);
	void SetDrawTga2(float Scaler, float timein, float timeout, float timecount,int center, int Align,int channel);
	void RemoveDraw(int channel);
	void AddChatMSg(char *ChatMsg,int r,int g,int b);
	gnUserData_t gUserdata;
	g_draw_text_add_t ChannelDraw[MAX_CHANNEL];
	g_draw_icon_t ChannelDrawIcon[32];

	void DrawScoreTab(float x,float y);
	float ScoreTabTimeStartDraw;
	int ScoreTabTypeDraw;
	int ScoreTabisShow ;
	float ScorePostMove;
	float TabScoreIndex[2];
	int TabScoreWidth;
	int TabScoreHeight;

	int ChatInBoxDraw;

	char gStringChat[1024];
	int gStringUpperChat;
	float TimeDrawSChat;
	float TimeCountChat;
	int gShowSChat;
	g_chat_msg_t  ChatMsgUI[9];
	g_chat_msg_t scrChatMsgUI[9];
	int NumChatMSG;
	float TimeChatMSG;
	float TimeChatMSGcount;
	void DrawChatUI(int x,int y);
	void AddMsgdeath(char * id1, char * id2,int iskill,int numkill,int team1,int team2,char *Weapon);
	int SetDrawFont(char *Str,int Align,int x,int y,int AlignScr,char *Font,int Size,int bold,int boder,float TimeCount,float Timein,float Timeout,int r,int g,int b,int a,int channel);
	void DrawMsgDeath(int x,int y);
	void DeleteMsgDeath();
	void DeleteMsgchat();
	void DrawTeamName(void);
	void Memu_game() ;
	deathMsg_info_t gMsgDeathINF[NumshowMaxDeathMsg+2];
	float TimeMSGDeathStart;
	int gPublicChat;
	char texyOldChat[64];
	g_player_inf_t VPlayer[33];
	char g_Menu_text[1024];
	int g_Menu_Wait;
	int g_Menu_Drawing;
	int gDrawTeamname;
	int gNumMsgDeath;
	SCREENINFO gScreenInfo;
	bool genableTabscore;
	vgui::ISurface *m_pSurface;
		int KillMsgInfTga[15];
		cvar_t *Cross_color_r, *Cross_color_g, *Cross_color_b;

private:
	hud_player_info_t PlayerInf;
	
	
	int HealthBar;
	int AcBar;
	int WeapomHudN_index[5];
	int playerHud_Index[3];
	int gNumTga_1[10];
	
	int gCreate;
	int LoadResource;
	g_load_text_add_t gTgaList[999];
	
	int CountTga;
	float gscrTime[6];
	int MaxTgaFileHas;
	int gBGChatImage;
	int gPostTextChat;

	intrgba_color_t ColorTeamMSG[2];
	int iLine;
	int TabScoreIcon[4];
};
extern GN_Hud_Draw *gGN_Hud_Draw;

#endif