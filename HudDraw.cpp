
#include "HudDraw.h"
#include "DrawText.h"
#include "Shlwapi.h"
#include "calcscreen.h"
#include "Encode.h"
#include "iostream"
#include "KillMark.h"
#include "mathlib.h"
#include <glext.h>
int IndexScrEff[2];
int IndexposBomb[2];
int IndexHaveC4;
float PosBompP[2][3];
int C4Posindex;
int isPressEsc;
int ShowMenuScope;
float TimeMovingScope;
int ControlChosseScope;
float PostMenuScope[32][4];
int BompIdPlayer;
int isBompStatus;
float BompPost[3];
int gBGChatImage2;
void ResetConfig();
int CheckImageOp;
int ButtonCheckTabop3[4];
int gDataMouseTab3[2];
int GStartSearch[2];
int gResetDataOption;
int ButtonNexDownSPEED[2];
int ButtonSoundSpeed[2];
int gControlSound[2][2];
char gszData_Config[20][32] = {"+forward","+back","+moveleft","+moveright","+duck","+speed","+jump",
	"buy","lastinv","slot1","slot2","slot3","slot4","slot5","+attack","+attack2","+reload","drop","+use"};
int gIndexImageMControl[3];
 int ButtonTabOption[3];
int DataMouse;
int ButtonMouseSpeed[2];
int gButtonOpticonControl[19];
int gButtonControlSel ;
char gDatakey_Config[32][32]; 
int gButtonOptionClose;
int gButtonOptionSave;
bool isModelButtonSetting;

int NumVGuiCrtl;
int uiOptionSize[2];
int BGUIOPTION[3];
int gHMouseClick[2];
int g_iRes;
int gScoreAllPlayer[3][33];
int gPlayerAce[3];
 int gPostVguiEsc[5][4];
 int gButtonVGuiEsc[5];
 int IndexVguiEsc[6];
 int TexturesMouse;
 float TimeNextClick;
 int gShowUiMap;
void  DrawTgaLoadList(void);
int IsClickMouse;
int isMenuDraw;
int gDataSound[3];
int idImageMenuScope[7];
int loadNormalFormat (int idWP,const char *filename, int *width, int *height);
int iLocal;
int gCrossType_1_index ;
HWND HwndGame;
HHOOK hKeyboardHook = 0;

float gCrossScale;

int isShowCross;

bool isEnableCross;
float MaxSizeCross;
float TimeCrossResize;
float TimeReturnCross;
float TimeReturnColor;
int gCrossColor[3];
HWND CreateEdit(const HWND hParent,const HINSTANCE hInst,DWORD dwStyle,
                const RECT& rc,const int id,const char* caption)
{
dwStyle|=WS_CHILD|WS_VISIBLE;
return CreateWindowEx(WS_EX_CLIENTEDGE,             //extended styles
                      "class",                   //control 'class' name
                      caption,              //control caption
                      dwStyle,                      //control style 
                      rc.left,                      //position: left
                      rc.top,                       //position: top
                      rc.right,                     //width
                      rc.bottom,                    //height
                      hParent,                      //parent window handle
                      //control's ID
                      reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
                      hInst,                        //application instance
                      0);                           //user defined info
}
//==
	int CSWPN_AMMO[] = {0,9,0,2,12,5,14,6,4,13,10,7,6,4,4,4,6,10,1,10,3,5,4,10,2,11,8,4,2,0,7};







void DrawUiMap();
GN_Hud_Draw *gGN_Hud_Draw;
char Thhext[1024];

int AddPostVguiSeting(int x,int y,int w ,int h)
{
	if (!NumVGuiCrtl) NumVGuiCrtl = 0;
	NumVGuiCrtl+=1;
	int id=NumVGuiCrtl;
	PostVGuiSetting[id][0] = x;
	PostVGuiSetting[id][1] = y;
	PostVGuiSetting[id][2] = w;
	PostVGuiSetting[id][3] = h;
	return id;
}
int Check_VguiSeting_CRTL(float x,float y,int GStart)
{
	
	for (int i = GStart;i<=NumVGuiCrtl+1;i++)
	{
		
		if (int(x)>PostVGuiSetting[i][0]&&int(x)<PostVGuiSetting[i][0]+PostVGuiSetting[i][2]&&int(y)>PostVGuiSetting[i][1]&&int(y)<PostVGuiSetting[i][1]+PostVGuiSetting[i][3])
		{
			//gEngfuncs.Con_Printf("\n g: %i|-| %i",PostVGuiSetting[i][0],PostVGuiSetting[i][1]);

			return i;
		}
	}
	return 0 ;

}
int Check_Mouse_Menu_Scope(float x,float y)
{
	
	for (int i = 1;i<=30+1;i++)
	{
		
		if (int(x)>PostMenuScope[i][0]&&int(x)<PostMenuScope[i][0]+PostMenuScope[i][2]&&int(y)>PostMenuScope[i][1]&&int(y)<PostMenuScope[i][1]+PostMenuScope[i][3])
		{
			//gEngfuncs.Con_Printf("\n g: %i|-| %i",PostVGuiSetting[i][0],PostVGuiSetting[i][1]);

			return i;
		}
	}
	return 0 ;

}

int SetButtonPOS(int gBut,int x,int y)
{
	PostVGuiSetting[gBut][0] = x;
	if (y!=-1)
	PostVGuiSetting[gBut][1] = y;
	return 1;
}
extern int gBodyWeaponView;
void DrawMenuScope()
{
	if (ShowMenuScope!=1) return;
	gExportfuncs.IN_DeactivateMouse();
	int x = 300;
	int y=300;
	int nScope = 6;
	int c=0;
	//int Check_Mouse_Menu_Scope(float x,float y)

	static int iMouseX,iMouseY;
	gEngfuncs.GetMousePosition(&iMouseX,&iMouseY);
	if (ControlChosseScope<1)ControlChosseScope = 1;
	int Addtive = 0;
	for (int i=1;i<=nScope;i++)
	{
		if (i>5) c+=1;
		PostMenuScope[i][0]=x+(i-(c*5)-1)*32;
		PostMenuScope[i][1]=y+c*32;
		PostMenuScope[i][2]=32;
		PostMenuScope[i][3]=32;
		if (ControlChosseScope==i) Addtive = 255;
		else Addtive = 150;
		gBodyWeaponView = ControlChosseScope -1;
		gGN_Hud_Draw->DrawImage(idImageMenuScope[i],0,x+(i-(c*5)-1)*32,y+c*32,1.0,255,255,255,Addtive,32,32);
		
	}
	gGN_Hud_Draw->DrawImage(TexturesMouse,0,iMouseX,iMouseY,1.0,255,255,255,255);
}
void DrawMenuEsc()
{
	IsClickMouse = -1;
	gResetDataOption = 0;
	static int iMouseX,iMouseY;
	isModelButtonSetting = false;
	gButtonControlSel= -1;
				
	
	
	int Width=150,Height=200;
	
//	gGN_Hud_Draw->m_pSurface->DrawGetTextureSize(IndexVguiEsc[0],Width,Height);
	float x =gGN_Hud_Draw->gScreenInfo.iWidth/2 -Width/2;
	float y= gGN_Hud_Draw->gScreenInfo.iHeight/2-Height/2;
	//IndexVguiEsc[0]

	gPostVguiEsc[0][0] = x+13;
	gPostVguiEsc[0][1] = y+30+3;
	gPostVguiEsc[0][2] = 180;
	gPostVguiEsc[0][3] = 24 ;

	gPostVguiEsc[1][0] = x+13;
	gPostVguiEsc[1][1] = y+70+3;
	gPostVguiEsc[1][2] = 180;
	gPostVguiEsc[1][3] = 24 ;

	gPostVguiEsc[2][0] = x+13;
	gPostVguiEsc[2][1] = y+110+3;
	gPostVguiEsc[2][2] = 180;
	gPostVguiEsc[2][3] = 24 ;

	gPostVguiEsc[3][0] = x+13;
	gPostVguiEsc[3][1] = y+150+3;
	gPostVguiEsc[3][2] = 180;
	gPostVguiEsc[3][3] = 24 ;
	gEngfuncs.GetMousePosition(&iMouseX,&iMouseY);
	int gi = CheckPost((float)iMouseX,(float)iMouseY);

	//gEngfuncs.pfnFillRGBABlend(0,0,gGN_Hud_Draw->gScreenInfo.iWidth,gGN_Hud_Draw->gScreenInfo.iHeight,0,0,0,70);

	gGN_Hud_Draw->DrawImage(IndexVguiEsc[0],0,x,y,1.0,255,255,255,255,150,200);
	
	for (int i=1;i<=4;i++)
	if (gi==i)
	gGN_Hud_Draw->DrawImage(IndexVguiEsc[i],0,x+12,y+30+(i-1)*40,1.0,255,255,255,255);
	else
	gGN_Hud_Draw->DrawImage(IndexVguiEsc[i],0,x+12,y+30+(i-1)*40,1.0,255,255,255,150);

	

	//gGN_Hud_Draw->DrawImage(IndexVguiEsc[2],0,x+20,y+70,1.0,255,255,255,200);
	//gGN_Hud_Draw->DrawImage(IndexVguiEsc[3],0,x+20,y+110,1.0,255,255,255,200);
	
	

}
void DrawOptionGui()
{
GStartSearch[0] = 29;
GStartSearch[1] = GStartSearch[0]+2;
char gString[1024];
static int iMouseX,iMouseY;
gEngfuncs.GetMousePosition(&iMouseX,&iMouseY);
//WritePrivateProfileStringA("Config","+attack","MOUSE1","USERCONFIG2.int");
	float x =gGN_Hud_Draw->gScreenInfo.iWidth/2 -uiOptionSize[0]/2;
	float y= gGN_Hud_Draw->gScreenInfo.iHeight/2-uiOptionSize[1]/2;
	//	gEngfuncs.Con_Printf("\n ->> %i",NumVGuiCrtl);

	if (gResetDataOption==0)
	{
		gDataMouseTab3[0]=GetPrivateProfileInt("Config","r_righthand",1,".\\USERCONFIG.ini"); 
		gDataMouseTab3[1]=GetPrivateProfileInt("Config","reverse_mouse",0,".\\USERCONFIG.ini"); 
		gDataSound[0]=GetPrivateProfileInt("Config","volume",50,".\\USERCONFIG.ini"); //max 1
		gDataSound[1]=GetPrivateProfileInt("Config","MP3Volume",50,".\\USERCONFIG.ini"); // max 2
		for (int i= 0;i<19;i++)
		GetPrivateProfileString("Config",gszData_Config[i],"",gDatakey_Config[i],20,".\\USERCONFIG.ini");
		gResetDataOption = 1;
		DataMouse = GetPrivateProfileInt("Config","sensitivity",50,".\\USERCONFIG.ini");
		SetButtonPOS(ButtonMouseSpeed[1],x+155+DataMouse*212/100-7,-1);
		
	}
//gEngfuncs.Con_Printf("\n ->> %i",gDataMouseTab3[0]);
	
		//gEngfuncs.Con_Printf("\nGG-> %i",gDataSound[1]);
	if (!gButtonOptionClose)		
	{
		gButtonOptionClose = AddPostVguiSeting(549,503,68,26);
		gButtonOptionSave = AddPostVguiSeting(480,503,68,26);
		
		ButtonTabOption[0]=AddPostVguiSeting(x+16,y+16,100,24);
		ButtonTabOption[1]=AddPostVguiSeting(x+127,y+16,98,24);
		ButtonTabOption[2]=AddPostVguiSeting(x+230,y+16,45,24);
	}
	//gEngfuncs.pfnFillRGBABlend(x+16,y+16,100,24,0,0,0,200);
	//gEngfuncs.pfnFillRGBABlend(x+127,y+16,98,24,0,0,0,200);

	
		int intGui = 0;
	if (UI_OPTION_1==isMenuDraw)
		intGui = 0;
		
	else if (UI_OPTION_2==isMenuDraw)
	
		intGui = 1;
	
	else if (UI_OPTION_3==isMenuDraw)
		intGui = 2;

	gGN_Hud_Draw->DrawImage(BGUIOPTION[intGui],0,x,y,1.0,255,255,255,255,uiOptionSize[0],uiOptionSize[1]);
	if (isMenuDraw==UI_OPTION_1)
	{
		for (int i =0;i<7;i++)
		{

			if (!gButtonOpticonControl[i])
			gButtonOpticonControl[i] =  AddPostVguiSeting(x+130,y+72+i*20,100,18);
			if (gButtonControlSel!=i||isModelButtonSetting!=true)
			{
				if (gDatakey_Config[i][0]!=0)
				DrawStringFont(x+130,y+72+i*20,gDatakey_Config[i],"Tahoma",14,0,0,255,255,255,255,0);
			}
			else DrawStringFont(x+130,y+72+i*20,"PressKey","Tahoma",14,0,0,255,0,0,255,0);	
		}
		for (int i =0;i<7;i++)
		{
			if (!gButtonOpticonControl[i+7])
			gButtonOpticonControl[i+7] =  AddPostVguiSeting(x+355,y+72+i*20,100,18);
			if (gButtonControlSel!=i+7||isModelButtonSetting!=true)
			{
				if (gDatakey_Config[i+7][0]!=0)
				DrawStringFont(x+355,y+72+i*20,gDatakey_Config[i+7],"Tahoma",14,0,0,255,255,255,255,0);	
			}
			else DrawStringFont(x+355,y+72+i*20,"PressKey","Tahoma",14,0,0,255,0,0,255,0);	
		}
		for (int i =0;i<5;i++)
		{
			if (!gButtonOpticonControl[i+14])
			gButtonOpticonControl[i+14] =  AddPostVguiSeting(x+130,y+238+i*20,100,18);


			if (gButtonControlSel!=i+14||isModelButtonSetting!=true)
			{
			if (gDatakey_Config[i+14][0]!=0)
			DrawStringFont(x+130,y+238+i*20,gDatakey_Config[i+14],"Tahoma",14,0,0,255,255,255,255,0);	
			}
			else DrawStringFont(x+130,y+238+i*20,"PressKey","Tahoma",14,0,0,255,0,0,255,0);	
		  
		}
	
		if (!ButtonMouseSpeed[0])
		{
			ButtonMouseSpeed[1] = AddPostVguiSeting(x+155+DataMouse*212/100-7,y+362,15,12);
			ButtonMouseSpeed[0] = AddPostVguiSeting(x+155,y+350+5,212,5);
			ButtonNexDownSPEED[0] =   AddPostVguiSeting(x+370,y+353+5,212,5);
			ButtonNexDownSPEED[1] =   AddPostVguiSeting(x+132,y+353+5,212,5);
			
		}
		//	gEngfuncs.pfnFillRGBABlend(x+132,y+353,16,16,0,0,0,200);
		//	gEngfuncs.pfnFillRGBABlend(x+370,y+353,16,16,0,0,0,200);
		if (IsClickMouse ==1 )
		{
			int gxPost = iMouseX;
			if (iMouseX>x+155+212)
			gxPost = x+155+212;
			else if (iMouseX<x+155)
			gxPost = x+155;

			SetButtonPOS(ButtonMouseSpeed[1], gxPost-7,-1);
			DataMouse = int((PostVGuiSetting[ButtonMouseSpeed[1]][0]+7 - (x+155) )/212*100);
		}

		if (DataMouse<0)DataMouse = 0;
		if (DataMouse>100) DataMouse = 100;

		SetButtonPOS(ButtonMouseSpeed[1], x+155+DataMouse*212/100-7,-1);
		char gstrChar[4];
		
		sprintf(gstrChar,"%i",DataMouse);
		if (DataMouse<10)sprintf(gstrChar,"0%i",DataMouse);

		gGN_Hud_Draw->DrawImage(gIndexImageMControl[1]	,0,x+155,y+357,1.0,255,255,255,255,DataMouse*212/100,7);

		DrawStringFont(x+400,y+350,gstrChar,"Tahoma",14,0,0,255,255,255,255,0);	

		gGN_Hud_Draw->DrawImage(gIndexImageMControl[0]	,0,PostVGuiSetting[ButtonMouseSpeed[1]][0],PostVGuiSetting[ButtonMouseSpeed[1]][1],1.0,255,255,255,255,15,12);
		
	}
	else if (isMenuDraw==UI_OPTION_2)
	{
		
		//gControlSound
		if (!ButtonSoundSpeed[0])
		{
			ButtonSoundSpeed[0] = AddPostVguiSeting(x+171+gDataSound[0]*192/100-7,y+90,15,12);
			ButtonSoundSpeed[1] = AddPostVguiSeting(x+171+gDataSound[1]*192/100-7,y+160,15,12);
			IsClickMouse = -1;
		}


		if (IsClickMouse == 1||IsClickMouse==2 )
		{
			int gxPost = iMouseX;
			if (iMouseX>x+171+192)
			gxPost = x+171+192;
			else if (iMouseX<x+171)
			gxPost = x+171;

			SetButtonPOS(ButtonSoundSpeed[IsClickMouse-1], gxPost-7,-1);
			gDataSound[IsClickMouse-1] = int((gxPost- (x+171) )/192*100);
			if (gDataSound[IsClickMouse-1] >100) gDataSound[IsClickMouse-1]  = 100;
			if (gDataSound[IsClickMouse-1] <0)  gDataSound[IsClickMouse-1]  = 0;

		}
		
		SetButtonPOS(ButtonSoundSpeed[0], x+171+gDataSound[0]*192/100-7,-1);
		SetButtonPOS(ButtonSoundSpeed[1], x+171+gDataSound[1]*192/100-7,-1);

		gGN_Hud_Draw->DrawImage(gIndexImageMControl[1]	,0,x+171,y+85,1.0,255,255,255,255,gDataSound[0]*192/100,7);
		gGN_Hud_Draw->DrawImage(gIndexImageMControl[1]	,0,x+171,y+154,1.0,255,255,255,255,gDataSound[1]*192/100,7);
		gGN_Hud_Draw->DrawImage(gIndexImageMControl[0]	,0,PostVGuiSetting[ButtonSoundSpeed[0]][0],PostVGuiSetting[ButtonSoundSpeed[0]][1],1.0,255,255,255,255,15,12);
		gGN_Hud_Draw->DrawImage(gIndexImageMControl[0]	,0,PostVGuiSetting[ButtonSoundSpeed[1]][0],PostVGuiSetting[ButtonSoundSpeed[1]][1],1.0,255,255,255,255,15,12);
		char gstrChar[4];
		

		sprintf(gstrChar,"%i",gDataSound[0]);
		if (gDataSound[0]<10)sprintf(gstrChar,"0%i",gDataSound[0]);
		DrawStringFont(x+400,y+80,gstrChar,"Tahoma",14,0,0,255,255,255,255,0);	

		sprintf(gstrChar,"%i",gDataSound[1]);
		if (gDataSound[1]<10)sprintf(gstrChar,"0%i",gDataSound[1]);
		DrawStringFont(x+400,y+148,gstrChar,"Tahoma",14,0,0,255,255,255,255,0);	
	}
	else if (isMenuDraw==UI_OPTION_3)
	{
		if (!ButtonCheckTabop3[0])
		{
			ButtonCheckTabop3[0] = AddPostVguiSeting(x+187,y+166,15,15);

			ButtonCheckTabop3[1] = AddPostVguiSeting(x+249,y+166,15,15);

			ButtonCheckTabop3[2] = AddPostVguiSeting(x+187,y+186,15,15);
			
			ButtonCheckTabop3[3] = AddPostVguiSeting(x+249,y+186,15,15);
		}
			
		

		if (gDataMouseTab3[0]==1)
			gGN_Hud_Draw->DrawImage(CheckImageOp,0,x+249,y+166,1.0,255,255,255,255,16,16);
		else
			gGN_Hud_Draw->DrawImage(CheckImageOp,0,x+188,y+166,1.0,255,255,255,255,16,16);

		if (gDataMouseTab3[1]==1)
			gGN_Hud_Draw->DrawImage(CheckImageOp,0,x+249,y+186,1.0,255,255,255,255,16,16);
		else
			gGN_Hud_Draw->DrawImage(CheckImageOp,0,x+188,y+186,1.0,255,255,255,255,16,16);

		//gEngfuncs.pfnFillRGBABlend(x+187,y+166,16,16,0,0,0,200);
		//gEngfuncs.pfnFillRGBABlend(x+249,y+166,16,16,0,0,0,200);
		//gEngfuncs.pfnFillRGBABlend(x+249,y+186,16,16,0,0,0,200);
	}
}
void DrawUiMap()
{
	static int iMouseX,iMouseY;
	int Width=400,Height=400;
	
	
	float gx =gGN_Hud_Draw->gScreenInfo.iWidth/2 -Width/2;
	float gy= gGN_Hud_Draw->gScreenInfo.iHeight/2-Height/2;

	gEngfuncs.pfnFillRGBABlend(gx-20,gy-20,Width+40,Height+40,70,90,100,170);
	gEngfuncs.pfnFillRGBABlend(gx-16,gy-16,Width+32,Height+32,50,50,50,200);
	if (!g_pOverview->m_pSprite) return;
	Vector2D vecTitles; 
	int i = static_cast<int>(sqrt(static_cast<long double>(g_pOverview->m_pSprite->numframes / (4 * 3))));
	vecTitles = Vector2D(i * 4, i * 3);

	Vector2D vecStartPosOffset, vecScissorSize;
	vecStartPosOffset = Vector2D(gGN_Hud_Draw->gScreenInfo.iWidth / 576,gGN_Hud_Draw->gScreenInfo.iWidth / 432);
	vecScissorSize = Vector2D(Width,Height);


	int iFrame = 0;
	float flScreenAspect = 4.0f / 3.0f;	
	float flAngles = 180 * (M_PI / 180.0);

	if (!g_pOverview->m_bIsRotate)
			flAngles -= float(M_PI / 2);

	Vector2D vecTile;

	
	vecTile.x = 3;
	vecTile.y = 4;
	

	Vector2D vecStep;
	vecStep.x = Width  / vecTitles.x;
	vecStep.y = -(Height / flScreenAspect) / vecTitles.y;

	Vector2D vecStepUp, vecStepRight;
	Vector2D vec1,vec2,vec3,vec4;

	vecStepUp = Vector2D(cos(flAngles + (M_PI / 2)) * vecStep.y, sin(flAngles + (M_PI / 2)) * vecStep.y);
	vecStepRight = Vector2D(cos(flAngles) * vecStep.x, sin(flAngles) * vecStep.x);

	Vector2D vecIn, vecOut;
	vecOut.x = vecScissorSize.x * 0.5 - vecTile.x * vecStepRight.x - vecTile.y * vecStepUp.x ;//+ vecStartPosOffset.x;
	vecOut.y = vecScissorSize.y * 0.5 - vecTile.x * vecStepRight.y - vecTile.y * vecStepUp.y ;//+ vecStartPosOffset.y;
	glColor4f(1, 1, 1, 1);
	glEnable(GL_ALPHA_TEST);
	for (int i = 0; i < vecTitles.y; i++)
	{
		vecIn.x = vecOut.x;
		vecIn.y = vecOut.y;

		for (int j = 0; j < vecTitles.x; j++)	
		{
			gEngfuncs.pTriAPI->SpriteTexture(g_pOverview->m_pSprite, iFrame);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
			
			glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex3f(gx+vecIn.x ,gy+ vecIn.y, 0);
			glTexCoord2f(0, 1);
			glVertex3f(gx+vecIn.x + vecStepRight.x, gy+ vecIn.y + vecStepRight.y, 0);
			glTexCoord2f(1, 1);
			glVertex3f(gx+vecIn.x + vecStepRight.x + vecStepUp.x, gy+ vecIn.y + vecStepRight.y + vecStepUp.y,0);
			glTexCoord2f(1, 0);
			glVertex3f(gx+vecIn.x + vecStepUp.x, gy+vecIn.y + vecStepUp.y, 0);
			glEnd();
			iFrame++;
			vecIn = vecIn + vecStepUp;
		}

		vecOut = vecOut + vecStepRight;
	}
	
}
int CheckPost(float x,float y)
{
	//gEngfuncs.pfnConsolePrint("Post");
	for (int i =0;i<=3;i++)
	{
		if (x>gPostVguiEsc[i][0]&&x<gPostVguiEsc[i][0]+gPostVguiEsc[i][2]&&y>gPostVguiEsc[i][1]&&y<gPostVguiEsc[i][1]+gPostVguiEsc[i][3])
		{
			
			return i+1;
		}
	}
	return 0 ;
}
LRESULT CALLBACK KeyboardCallback(int code,WPARAM wParam,LPARAM lParam) {  
//std::cout << "a key was pressed" << std::endl;
 
   if(code == HC_ACTION && ((DWORD)lParam & 0x80000000) == 0)	// if there is an incoming action and a key was pressed
  {
    switch(wParam)
    {
	
    /*case VK_BACK:
		//memset(Thhext,0,strlen(Thhext)-1);
		{
		if (strlen(Thhext)>0)
		Thhext[strlen(Thhext)-1] =0;
		//gnKillMark->OnDeathMsg(1,1,0,0,0,1);
		}
      break;*/
	case VK_RETURN :
		if (isShowCross!=3)
		isShowCross = 3;
		else
			isShowCross = 2;
		memset(Thhext,0,1024);
	
	//	gnKillMark->OnDeathMsg(1,0,0,0);
		 break;
	case 'x':
		if (ShowMenuScope!=1)
		{
			ShowMenuScope = 1;
			gExportfuncs.IN_DeactivateMouse();
		}
		else
		{
			 ShowMenuScope = 0;
			 gEngfuncs.pfnSetMousePos(gEngfuncs.GetWindowCenterX(),gEngfuncs.GetWindowCenterY()); 
			 gExportfuncs.IN_ActivateMouse();
		}
		break;	
	case VK_ESCAPE :
		
		// DrawMenuEsc();
		if (g_pMetaSave->pEngineFuncs->GetLocalPlayer)
		{
		if (isModelButtonSetting==true) {
			isModelButtonSetting = false;
			
		}
		else
		{
		if (isMenuDraw!=1) isMenuDraw = UI_ESC;

		else isMenuDraw = 0;
		gEngfuncs.pfnSetMouseEnable(isMenuDraw);
		if (isMenuDraw==0){ gEngfuncs.pfnSetMousePos(gEngfuncs.GetWindowCenterX(),gEngfuncs.GetWindowCenterY()); gExportfuncs.IN_ActivateMouse();}

		}
		return (LRESULT)hKeyboardHook;
		}
		//gEngfuncs.pfnConsolePrint("LockKEy");
		//return 0;
		break;
    default:
	//	if (strlen(Thhext)<1024)
	//	Thhext[strlen(Thhext)] = char(wParam);
      break;
	}
	if (strcmp(Thhext,"GIAYNHAP")==0)
	 gEngfuncs.pfnConsolePrint("HackEnable");
   }
  
	return CallNextHookEx(hKeyboardHook,code,wParam,lParam);
}

GN_Hud_Draw::GN_Hud_Draw()
{

	memset(Thhext,0,1024);
	genableTabscore = false;
	LoadResource=0;
	gCreate=0;
	CountTga=0;
	gPostTextChat=0;
	TimeDrawSChat=0.0f;
	TimeCountChat=0.0f;
	gShowSChat=0;
	NumChatMSG=0;
	TimeChatMSG=0.0f;
	TimeChatMSGcount=10.0f;
	gNumMsgDeath=0;
	ColorTeamMSG[1].r=37;
	ColorTeamMSG[1].g=120;
	ColorTeamMSG[1].b=164;
	ColorTeamMSG[1].a=255;
	ColorTeamMSG[0].r=193;
	ColorTeamMSG[0].g=181;
	ColorTeamMSG[0].b=159;
	ColorTeamMSG[0].a=255;
	gResetDataOption = 0;

//	gPublicchat = 0 ;
	//memset(texyO0dChat,0,64);
	gCrossScale = 0.5;
	isShowCross = 0;
	isEnableCross = false;
	TimeReturnCross = 0.0;
	memset(gCrossColor,3,0);


	Cross_color_r = gEngfuncs.pfnRegisterVariable("gnCrossHairColor_r", "0", FCVAR_ARCHIVE);
	Cross_color_g = gEngfuncs.pfnRegisterVariable("gnCrossHairColor_g", "255", FCVAR_ARCHIVE);
	Cross_color_b = gEngfuncs.pfnRegisterVariable("gnCrossHairColor_b", "0", FCVAR_ARCHIVE);
	for (int i=1;i<=32;i++)
	{
		gGN_Hud_Draw->VPlayer[i].die = 0;
		gScoreAllPlayer[0][i] = 0;
		gScoreAllPlayer[1][i] = 0;
		gScoreAllPlayer[2][i]= 0;
		gGN_Hud_Draw->VPlayer[i].Team = 0;

	}
for (int i = 5 ; i >=0 ;i--)
	{
		ChatMsgUI[i].gShow =0;
		scrChatMsgUI[i].gShow =0;
		memset(ChatMsgUI[i].TextChat,0,512) ;
		memset(scrChatMsgUI[i].TextChat,0,512) ;
		scrChatMsgUI[i].TextChat[0]=0;
		ChatMsgUI[i].TextChat[0]=0;
	}
	for (int i = NumshowMaxDeathMsg ; i >0 ;i--)
	{
		
		memset(gMsgDeathINF[i-1].gMsgDeathstring[0],0,64) ;
		memset(gMsgDeathINF[i-1].gMsgDeathstring[1],0,64) ;
		gMsgDeathINF[i-1].gMsgDeathstring[0][0]=0;
		gMsgDeathINF[i-1].gMsgDeathstring[0][1]=0;
	}
	gDataMouseTab3[0]=GetPrivateProfileInt("Config","r_righthand",1,".\\USERCONFIG.ini"); 
	gDataMouseTab3[1]=GetPrivateProfileInt("Config","reverse_mouse",0,".\\USERCONFIG.ini"); 
	gDataSound[0]=GetPrivateProfileInt("Config","volume",50,".\\USERCONFIG.ini"); //max 1
	gDataSound[1]=GetPrivateProfileInt("Config","MP3Volume",50,".\\USERCONFIG.ini"); // max 2
	for (int i= 0;i<19;i++)
	GetPrivateProfileString("Config",gszData_Config[i],"",gDatakey_Config[i],20,".\\USERCONFIG.ini");
	gResetDataOption = 1;
	DataMouse = GetPrivateProfileInt("Config","sensitivity",50,".\\USERCONFIG.ini");
		
	HwndGame = GetActiveWindow();
	
//rc.top+=40;
//rc.bottom+=30;
//HWND hControl = CreateEdit(HwndGame,hInstance,0,rc,200,"");

 hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD,KeyboardCallback, (HINSTANCE)GetWindowLong(HwndGame,GWL_HINSTANCE), GetCurrentThreadId());
  
}
void gSprite(void)
{
	/*SCREENINFO g_sScreenInfo;
	g_sScreenInfo.iSize = sizeof(g_sScreenInfo);
	gEngfuncs.pfnGetScreenInfo(&g_sScreenInfo);
	

	if (g_sScreenInfo.iWidth < 640)
		g_iRes = 320;
	else
		g_iRes = 640;
	if (!g_pSpriteList)
	{
		g_pSpriteList = gEngfuncs.pfnSPR_GetList("sprites/hud.txt", &g_iSpriteCountAllRes);

		if (g_pSpriteList)
		{
			g_iSpriteCount = 0;
			client_sprite_t *p = g_pSpriteList;
			for (int j = 0; j < g_iSpriteCountAllRes; j++)
			{
				if (p->iRes == g_iRes)
					g_iSpriteCount++;
				p++;
			}

 			g_rghSprites = new HL_SPRITE[g_iSpriteCount];
			g_rgrcRects = new wrect_t[g_iSpriteCount];
			g_rgszSpriteNames = new char[g_iSpriteCount * MAX_SPRITE_NAME_LENGTH];

			p = g_pSpriteList;
			int index = 0;
			for (int j = 0; j < g_iSpriteCountAllRes; j++)
			{
				if (p->iRes == g_iRes)
				{
					char sz[256];
					sprintf(sz, "sprites/%s.spr", p->szSprite);
					g_rghSprites[index] = gEngfuncs.pfnSPR_Load(sz);
					g_rgrcRects[index] = p->rc;
					strncpy(&g_rgszSpriteNames[index * MAX_SPRITE_NAME_LENGTH], p->szName, MAX_SPRITE_NAME_LENGTH);
					index++;
				}
				p++; 
			}
		}
	}
	else
	{
		int index = 0;
		client_sprite_t *p = g_pSpriteList;
		for (int j = 0; j < g_iSpriteCountAllRes; j++)
		{
			if (p->iRes == g_iRes)
			{
				char sz[256];
				sprintf(sz, "sprites/%s.spr", p->szSprite);
				g_rghSprites[index] = gEngfuncs.pfnSPR_Load(sz);
				index++;
			}
			p++;
		}
	}

	*/
}
void clientPrinet(int id)
{
	gEngfuncs.pfnConsolePrint("->>>MSG \n");
	gEngfuncs.Con_Printf("->TESTaaaaaa: %i \n",id);
}

void GN_Hud_Draw::LoadImage()
{
	char szTexture[64];
	if (LoadResource!=1)
	{
		ResetConfig();
		iLocal = gEngfuncs.GetLocalPlayer()->index;
		LoadResource=1;
		gScreenInfo.iSize = sizeof(gScreenInfo);
		gEngfuncs.pfnGetScreenInfo(&gScreenInfo);
		CreateInterfaceFn fnEngineCreateInterface = g_pMetaHookAPI->GetEngineFactory();
		m_pSurface = (vgui::ISurface *)fnEngineCreateInterface(VGUI_SURFACE_INTERFACE_VERSION, NULL);

		m_iScreenTexture=m_pSurface->CreateNewTextureID();


			sprintf(szTexture,"CSFIRED//HUD_PLAYER//HP_BG");
			playerHud_Index[0]=m_pSurface->CreateNewTextureID();
			m_pSurface->DrawSetTextureFile(playerHud_Index[0],szTexture,true,true);
			
			sprintf(szTexture,"CSFIRED//HUD_PLAYER//AC_BG");
			playerHud_Index[1]=m_pSurface->CreateNewTextureID();
			m_pSurface->DrawSetTextureFile(playerHud_Index[1],szTexture,true,true);
			
			sprintf(szTexture,"CSFIRED//HUD_PLAYER//AMMO_BG");
			WeapomHudN_index[2]=m_pSurface->CreateNewTextureID();
			m_pSurface->DrawSetTextureFile(WeapomHudN_index[2],szTexture,true,true);
			
			sprintf(szTexture,"CSFIRED//ChatBG");
			gBGChatImage=m_pSurface->CreateNewTextureID();
			m_pSurface->DrawSetTextureFile(gBGChatImage,szTexture,true,true);
			//sprintf(szTexture,"CSFIRED//HUD_PLAYER//AMMO_BG");
			//HealthBar=m_pSurface->CreateNewTextureID();
			//m_pSurface->DrawSetTextureFile(HealthBar,szTexture,true,true);
			
			
			//sprintf(szTexture,"CSFIRED//HUD_PLAYER//AC_BG");
			//AcBar=m_pSurface->CreateNewTextureID();
			//m_pSurface->DrawSetTextureFile(AcBar,szTexture,true,true);

		for (int i = 0; i<=9;i++)
		{
			sprintf(szTexture,"CSFIRED//NUM//%d",i);
			gNumTga_1[i]=m_pSurface->CreateNewTextureID();
			m_pSurface->DrawSetTextureFile(gNumTga_1[i],szTexture,true,true);
		}
		for (int i=0;i<5;i++)
		{
			sprintf(szTexture,"CSFIRED//KILLMSG//SHOT_MULTIKILL%i",i+2);
			KillMsgInfTga[i]=m_pSurface->CreateNewTextureID();
			m_pSurface->DrawSetTextureFile(KillMsgInfTga[i],szTexture,true,true);
		}
		TabScoreIcon[2] = m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(TabScoreIcon[2],"CSFIRED//tscore_ace",true,true);
		TabScoreIcon[3] = m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(TabScoreIcon[3],"CSFIRED//tscore_aceg",true,true);

		TabScoreIcon[1] = m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(TabScoreIcon[1],"CSFIRED//tscore_1st",true,true);
		TabScoreIcon[0] = m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(TabScoreIcon[0],"CSFIRED//tscore_DEATH",true,true);

		KillMsgInfTga[5]=m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(KillMsgInfTga[5],"CSFIRED//KILLMSG//SHOT_MULTIKILLMAX",true,true);

		KillMsgInfTga[6]=m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(KillMsgInfTga[6],"CSFIRED//KILLMSG//SHOT_HEAD",true,true);

		KillMsgInfTga[7]=m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(KillMsgInfTga[7],"CSFIRED//KILLMSG//SHOT_GOLDHEAD",true,true);

		KillMsgInfTga[8]=m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(KillMsgInfTga[8],"CSFIRED//KILLMSG//SHOT_WALL",true,true);

		KillMsgInfTga[9]=m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(KillMsgInfTga[9],"CSFIRED//KILLMSG//SHOT_WALL_HEAD",true,true);

		KillMsgInfTga[10]=m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(KillMsgInfTga[10],"CSFIRED//KILLMSG//SHOT_WALL_GOLDHEAD",true,true);
		
		KillMsgInfTga[11]=m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(KillMsgInfTga[11],"CSFIRED//KILLMSG//SHOT_WALL_GOLDHEAD",true,true);

		KillMsgInfTga[12]=m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(KillMsgInfTga[12],"CSFIRED//KILLMSG//SHOT_FIRSTKILL",true,true);

		KillMsgInfTga[13]=m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(KillMsgInfTga[13],"CSFIRED//KILLMSG//SHOT_LASTKILL",true,true);

		int temp;
		gBGChatImage2 =m_pSurface->CreateNewTextureID();
		LoadTgaImage("CSFIRED//CHATBG_INGAME.tga",gBGChatImage2,&temp,&temp);
		
		IndexVguiEsc[0]= m_pSurface->CreateNewTextureID();
	//	m_pSurface->DrawSetTextureFile(IndexVguiEsc[0],"CSFIRED//BGESC",true,true);
		LoadTgaImage("CSFIRED//BGESC.tga",IndexVguiEsc[0],&temp,&temp);
		IndexVguiEsc[1]= m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(IndexVguiEsc[1],"CSFIRED//Button_Continue",true,true);
		IndexVguiEsc[2]= m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(IndexVguiEsc[2],"CSFIRED//Button_Map",true,true);

		IndexVguiEsc[3]= m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(IndexVguiEsc[3],"CSFIRED//Button_Option",true,true);
		IndexVguiEsc[4]= m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(IndexVguiEsc[4],"CSFIRED//Button_Exit",true,true);


		TexturesMouse =  m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(TexturesMouse,"CSFIRED//Mouse",true,true);
		DrawTgaLoadList();
		TabScoreIndex[0]=m_pSurface->CreateNewTextureID();
		LoadTgaImage("CSFIRED//bl_tab_score.tga",TabScoreIndex[0],&TabScoreWidth,&TabScoreHeight);
		TabScoreIndex[1]=m_pSurface->CreateNewTextureID();
		LoadTgaImage("CSFIRED//gr_tab_score.tga",TabScoreIndex[1],&TabScoreWidth,&TabScoreHeight);

		BGUIOPTION[0] =  m_pSurface->CreateNewTextureID();
		LoadTgaImage("CSFIRED//UI_OPTION//OptionControl.tga",BGUIOPTION[0],&uiOptionSize[0],&uiOptionSize[1]);	
		BGUIOPTION[1] =  m_pSurface->CreateNewTextureID();
		LoadTgaImage("CSFIRED//UI_OPTION//OptionSound.tga",BGUIOPTION[1],&uiOptionSize[0],&uiOptionSize[1]);	
		BGUIOPTION[2] =  m_pSurface->CreateNewTextureID();
		LoadTgaImage("CSFIRED//UI_OPTION//OptionEtc.tga",BGUIOPTION[2],&uiOptionSize[0],&uiOptionSize[1]);

		gIndexImageMControl[0]=  m_pSurface->CreateNewTextureID();
		LoadTgaImage("CSFIRED//UI_OPTION//GuagePoint.tga",gIndexImageMControl[0],&temp,&temp);	
		gIndexImageMControl[1]=  m_pSurface->CreateNewTextureID();
		LoadTgaImage("CSFIRED//UI_OPTION//Guage_full.tga",gIndexImageMControl[1],&temp,&temp);	
		CheckImageOp = m_pSurface->CreateNewTextureID();
		LoadTgaImage("CSFIRED//UI_OPTION//Check.tga",CheckImageOp,&temp,&temp);	

		C4Posindex =  m_pSurface->CreateNewTextureID();
		LoadTgaImage("CSFIRED//C4POS.tga",C4Posindex,&temp,&temp);

		gCrossType_1_index = m_pSurface->CreateNewTextureID();
		LoadTgaImage("CSFIRED//CROSSCTYPE.tga",gCrossType_1_index,&temp,&temp);


		IndexposBomb[0] = m_pSurface->CreateNewTextureID();
		IndexposBomb[1] = m_pSurface->CreateNewTextureID();
		LoadTgaImage("CSFIRED//C4AREA_A.tga",IndexposBomb[0],&temp,&temp);
		LoadTgaImage("CSFIRED//C4AREA_B.tga",IndexposBomb[1],&temp,&temp);

		IndexScrEff[0] = m_pSurface->CreateNewTextureID();
		LoadTgaImage("CSFIRED//GRAY.tga",IndexScrEff[0],&temp,&temp);
		IndexScrEff[1] = m_pSurface->CreateNewTextureID();
		LoadTgaImage("CSFIRED//BLOODEFFECT.tga",IndexScrEff[1],&temp,&temp);
		IndexHaveC4 = m_pSurface->CreateNewTextureID();
		LoadTgaImage("CSFIRED//HAVEC4_EVENT.tga",IndexHaveC4,&temp,&temp);
		for (int i =1 ;i<=6;i++)
		{
			sprintf(szTexture,"CSFIRED//Scope//%i_Scope.tga",i);
			idImageMenuScope[i] =  m_pSurface->CreateNewTextureID();
			LoadTgaImage(szTexture,idImageMenuScope[i],&temp,&temp);	
		}
		
		
	}
}

void GN_Hud_Draw::DrawScoreTab(float x,float y)
{
	if (genableTabscore== false) return;
	int Color[3][3]={{77,167,215},{37,120,164},{71,71,71}};
	
	float MoveMax = 40;
	int Addtive=0;
	
	float CurTime=gEngfuncs.GetClientTime() ;

	if (ScoreTabisShow!=1)
	{
		if (CurTime-ScoreTabTimeStartDraw<=0.3)
		{
			Addtive=int(255/0.3*(CurTime-ScoreTabTimeStartDraw));
			ScorePostMove = MoveMax-MoveMax/0.3*(CurTime-ScoreTabTimeStartDraw) ;
			if (ScorePostMove<0) ScorePostMove = 0;
			if (Addtive>255) Addtive = 255;
		}
		else

		return ;
	}
	else
	{
		if (CurTime-ScoreTabTimeStartDraw<=0.3)
		{
		
			Addtive=255-int(255/0.3*(CurTime-ScoreTabTimeStartDraw));
			ScorePostMove = MoveMax/0.3*(CurTime-ScoreTabTimeStartDraw) ;
		}

		if (ScorePostMove>=MoveMax) {ScorePostMove = MoveMax;Addtive=0;}
		if (Addtive<0) Addtive = 0;
	}
	int gNum[2];
	gNum[0] = 0;
	gNum[1] = 0;
	
	DrawImage(TabScoreIndex[VPlayer[gEngfuncs.GetLocalPlayer()->index].Team-1],0,x - MoveMax+ScorePostMove ,y,1.0,255,255 ,255,255-Addtive,TabScoreWidth,TabScoreHeight);
	char szgScore[10];
	char szDeath[10];
	DrawImage(TabScoreIcon[1],0,x+60- MoveMax+ScorePostMove,y+90+0,1.0,255,255,255,255-Addtive);
	DrawImage(TabScoreIcon[1],0,x+60- MoveMax+ScorePostMove,y+90+190,1.0,255,255,255,255-Addtive);

	for (int i = 1 ;i<=32;i++)
	{
		if (VPlayer[i].Team>0)
		{
			cl_entity_s *pPlayer = NULL;
			pPlayer = gEngfuncs.GetEntityByIndex(i);
			if (pPlayer)
			if (pPlayer->player&&pPlayer->index >0)
			{
				int color;
				float gPost = 0;
				if (VPlayer[i].Team == VPlayer[gEngfuncs.GetLocalPlayer()->index].Team)
				{
					 color = 0 ;		
					 gPost = 0.0;
				}
				else
				{
					 gPost = 190.0;
					 color = 1 ;		
						
					
				}
				if (VPlayer[i].die==1 ) color = 2;

					memset(szgScore,0,10);
					memset(szDeath,0,10);
					szgScore[0]=48;
					szDeath[0]=48;

					if (gScoreAllPlayer[0][i] <0) gScoreAllPlayer[0][i] = 0;

					if (gScoreAllPlayer[1][i] <0) gScoreAllPlayer[1][i] = 0;


					sprintf(szgScore,"%i",gScoreAllPlayer[0][i]);
					sprintf(szDeath,"%i",gScoreAllPlayer[1][i]);
				if (VPlayer[i].gINFent.name[0]!=0)
				{
					gNum[ VPlayer[i].Team-1]+=1;
					int gfNum =gScoreAllPlayer[2][i];
					
					if (gScoreAllPlayer[2][i] <=0||gScoreAllPlayer[2][i]>32) gfNum = gNum[VPlayer[i].Team-1];
					DrawStringFont(x+110- MoveMax+ScorePostMove,y+90+(gfNum-1)*20+gPost,VPlayer[i].gINFent.name,"Tahoma",14,1,0,Color[color][0],Color[color][1],Color[color][2],255-Addtive,0);
					DrawStringFont(x+380- MoveMax+ScorePostMove,y+90+(gfNum-1)*20+gPost,szgScore,"Tahoma",14,1,0,Color[color][0],Color[color][1],Color[color][2],255-Addtive,2);
					DrawStringFont(x+420- MoveMax+ScorePostMove,y+90+(gfNum-1)*20+gPost,szDeath,"Tahoma",14,1,0,Color[color][0],Color[color][1],Color[color][2],255-Addtive,2);
					
					if (VPlayer[i].die==1 ) DrawImage(TabScoreIcon[0],1,x+100- MoveMax+ScorePostMove,y+90+(gfNum-1)*20+gPost+10,1.0,255,255,255,255-Addtive);
					
					for (int ig = 1 ;ig <=3;ig ++)
					if (gPlayerAce[ig-1]==i)
					{
						if ( ig == 1 )
						DrawImage(TabScoreIcon[3],0,x+10- MoveMax+ScorePostMove,y+90+(gfNum-1)*20+gPost,1.0,255,255,255,255-Addtive);
						else
						DrawImage(TabScoreIcon[2],0,x+10- MoveMax+ScorePostMove,y+90+(gfNum-1)*20+gPost,1.0,255,255,255,255-Addtive);

					}
				
				}
			}
		}
	}
}
void GN_Hud_Draw::DrawFollowIcon(void)
{
		
		if (PosBompP[0][0]||PosBompP[0][1])
		{
				vec2_t vecScreen;
				if  (CalcScreen(PosBompP[0] ,vecScreen))
				{
					DrawImage(IndexposBomb[0],1,vecScreen[0],vecScreen[1],1.0,255, 255 , 255,255,64,64);
				}

				if  (CalcScreen(PosBompP[1] ,vecScreen))
				{
					DrawImage(IndexposBomb[1],1,vecScreen[0],vecScreen[1],1.0,255, 255 , 255,255,64,64);
				}
		}
		if (g_iBombEntity && gBompDrop == 1 )
		{
		
			int CanDraw;
			vec2_t vecScreen;
			if (isBompStatus)
			{
				CanDraw = CalcScreen(BompPost ,vecScreen);
				//DrawStringFont(vecScreen[0],vecScreen[1],"-> C4 <-","Tahoma",12,1,1,255,255,255,255,1);
			}
			else
			{
				cl_entity_s *gEntC4 =  gEngfuncs.GetEntityByIndex (g_iBombEntity);
				CanDraw = CalcScreen(gEntC4->origin ,vecScreen);
				//DrawStringFont(vecScreen[0],vecScreen[1],"-> C4 <-","Tahoma",12,1,1,255,255,255,255,1);
			}
			if (CanDraw)
			{
				DrawImage(C4Posindex,1,vecScreen[0],vecScreen[1],1.0,255, 255 , 255,255,64,64);
			}
		}

	float CurTime=gEngfuncs.GetClientTime() ;
	for (int i=0;i<32;i++)
	{
		if (CurTime - ChannelDrawIcon[i].TimeStart >= ChannelDrawIcon[i].TimeCount ) ChannelDrawIcon[i].Show = 0;
		if (ChannelDrawIcon[i].Show)
		{
			cl_entity_s *gEnt = NULL;
			gEnt = gEngfuncs.GetEntityByIndex(ChannelDrawIcon[i].iEnt);
			vec2_t vecScreen;
		
			gEnt->origin[2]+=20;
			if(CalcScreen(gEnt->origin ,vecScreen))
			{
				if ( ChannelDrawIcon[i].type==1)
				DrawImage(gTgaList[ ChannelDrawIcon[i].index].index,1,vecScreen[0],vecScreen[1],1.0,255, 255 , 255,255);
			}
		}

	}
}
void GN_Hud_Draw::DrawTeamName(void)
{
	
	//glBegin(GL_LINES);
   
	//glVertex3f(gScreenInfo.iWidth/2-50,gScreenInfo.iHeight/2-50,0);
    //glVertex3f(gScreenInfo.iWidth/2,gScreenInfo.iHeight/2,0);
   // glEnd();

	if (gDrawTeamname!=1 && gDrawTeamname!=2) return ;
		
	
		
		
		for(int i=1 ;i<=32;i++)
		{
			
			if (i != iLocal)
			{
				cl_entity_s *pPlayer = NULL;
				
				pPlayer = gEngfuncs.GetEntityByIndex(i);
				if(!pPlayer || pPlayer->curstate.iuser1||!pPlayer->player||pPlayer->curstate.health<0.0) continue;	
				if (gGN_Hud_Draw->VPlayer[i].Team==0 ||VPlayer[i].die == 1) continue;

				if (gGN_Hud_Draw->VPlayer[i].Team == gGN_Hud_Draw->VPlayer[iLocal].Team || gDrawTeamname==2 )
				{
				//gEngfuncs.Con_Printf("|%i - %i|",pPlayer->curstate.team,gEngfuncs.GetLocalPlayer()->curstate);
					//gEngfuncs.pfnGetPlayerInfo(pPlayer->index,&gGN_Hud_Draw->VPlayer[i].gINFent);
				
					vec2_t vecScreen;
		
					pPlayer->origin[2]+=30;
					if(CalcScreen(pPlayer->origin ,vecScreen))
					{
				
				
						if (strlen(gGN_Hud_Draw->VPlayer[i].gINFent.name)>0)
						DrawStringFont(vecScreen[0],vecScreen[1],gGN_Hud_Draw->VPlayer[i].gINFent.name,"Tahoma",14,1,1,255,255,255,255,1);
					}
				}
			}
		}
	
}
void SetCrossColor(int r,int g,int b)
{
	gCrossColor[0]=r;
	gCrossColor[1]=g;
	gCrossColor[2]=b;
}
void GN_Hud_Draw::DrawAllHud()
{
		//cl_entity_t *viewent = gEngfuncs.GetViewModel();
		//viewent->curstate.body = gBodyWeaponView;
		if (gCreate!=1)
		{

			gCreate	 =1;
		
			gEngfuncs.pfnGetPlayerInfo(gEngfuncs.GetLocalPlayer()->index,&PlayerInf);
		}

		

			//DrawImage(m_iScreenTexture,0,200,100,1.0,255,255 ,255,255,256,256);



		float CurTime=gEngfuncs.GetClientTime() ;
		if (!gEngfuncs.GetLocalPlayer()->curstate.iuser1)
		{
			
			DrawTeamName();
			if (gUserdata.clip<0) gUserdata.clip = 0;
			if (gUserdata.weaponid<0) gUserdata.weaponid = 0;
			if (gUserdata.hp<0) gUserdata.hp=0;
			if (gUserdata.acc<0) gUserdata.acc=0;
			if (gUserdata.ammo[CSWPN_AMMO[gUserdata.weaponid]]<0)gUserdata.ammo[CSWPN_AMMO[gUserdata.weaponid]]=0;
			DrawImage(playerHud_Index[0],0,150,gScreenInfo.iHeight-45,1,255,255 ,255,255);
			DrawImage(playerHud_Index[1],0,150,gScreenInfo.iHeight-65,1,255,255 ,255,255);
			GN_HUD_DrawNumTga(gUserdata.acc,2,220,gScreenInfo.iHeight-60,255,255 ,255, 255,gNumTga_1,13);
			GN_HUD_DrawNumTga(gUserdata.hp,2,220,gScreenInfo.iHeight-40,255,255 ,255, 255,gNumTga_1,13);
			if (gUserdata.weaponid!=29)
			{
			DrawImage(WeapomHudN_index[2],0,gScreenInfo.iWidth - 210,gScreenInfo.iHeight-25,1,255,255 ,255,255);
			GN_HUD_DrawNumTga(gUserdata.clip,2,gScreenInfo.iWidth-130,gScreenInfo.iHeight-20,255,255 ,255, 255,gNumTga_1,13);
			GN_HUD_DrawNumTga(gUserdata.ammo[CSWPN_AMMO[gUserdata.weaponid]],0,gScreenInfo.iWidth-115,gScreenInfo.iHeight-20,255,255 ,255, 255,gNumTga_1,13);
			}
			if (BompIdPlayer== iLocal )

				DrawImage(IndexHaveC4,1,gScreenInfo.iWidth/2,120,1,255,255 ,255,255,128,128);
			if (gUserdata.hp<20)
			{
				DrawImage(IndexScrEff[1],0,0,0,1.0,255,255 ,255,255,gScreenInfo.iWidth,gScreenInfo.iHeight);
			}
		}
		
		
		for (int i=0;i<MAX_CHANNEL;i++)
		{
			if (CurTime-ChannelDraw[i].TimeStart>= ChannelDraw[i].TimeCount&&ChannelDraw[i].TimeCount>0.0) RemoveDraw(i);
			if (ChannelDraw[i].Show==1)
			{
				if (ChannelDraw[i].Effect==Effect_Addtive)
				{
					
					if (CurTime-ChannelDraw[i].TimeStart<=ChannelDraw[i].Timein)
						ChannelDraw[i].alpha=(CurTime-ChannelDraw[i].TimeStart)*ChannelDraw[i].a/ChannelDraw[i].Timein;
					
					else if (ChannelDraw[i].TimeCount-(CurTime-ChannelDraw[i].TimeStart)<=ChannelDraw[i].Timeout)
						ChannelDraw[i].alpha=(ChannelDraw[i].TimeCount-(CurTime-ChannelDraw[i].TimeStart))*ChannelDraw[i].a/ChannelDraw[i].Timeout;
					if (CurTime-ChannelDraw[i].TimeStart==ChannelDraw[i].Timein) ChannelDraw[i].alpha = ChannelDraw[i].a;
						
					if (ChannelDraw[i].alpha>ChannelDraw[i].a) ChannelDraw[i].alpha= ChannelDraw[i].a;
					if (ChannelDraw[i].alpha<0) ChannelDraw[i].alpha=0;
				}
			//	gEngfuncs.pfnConsolePrint(gTgaList[ChannelDraw[i].index].TextFile);
				if (ChannelDraw[i].Type == 1)
				DrawImage(gTgaList[ChannelDraw[i].index].index,ChannelDraw[i].center,ChannelDraw[i].x,ChannelDraw[i].y,ChannelDraw[i].scaler, ChannelDraw[i].r, ChannelDraw[i].g , ChannelDraw[i].b, ChannelDraw[i].alpha,gTgaList[ChannelDraw[i].index].TextSize[0],gTgaList[ChannelDraw[i].index].TextSize[1]);
				else if (ChannelDraw[i].Type==2)
				DrawStringFont(ChannelDraw[i].x,ChannelDraw[i].y,ChannelDraw[i].Str,ChannelDraw[i].Font,ChannelDraw[i].sizefont,ChannelDraw[i].boder,ChannelDraw[i].bold,ChannelDraw[i].r,ChannelDraw[i].g,ChannelDraw[i].b,ChannelDraw[i].alpha,ChannelDraw[i].center)  ;		
			
			
			
			
			}
		}

		
		DrawFollowIcon();
		DrawMsgDeath(gScreenInfo.iWidth-15,40);
		if (abs(CurTime-TimeReturnCross)>0.1&&isShowCross==3)
		{
			isShowCross = 2;
		}
		if (abs(CurTime-TimeReturnColor)>0.3)
		{
			SetCrossColor(Cross_color_r->value,Cross_color_g->value,Cross_color_b->value);
		}

		if (abs(CurTime-TimeCrossResize)>0.02)
		{
			if (!MaxSizeCross) MaxSizeCross = 1.0;
			if (isShowCross==3&&gCrossScale<=MaxSizeCross) gCrossScale+=0.1;
			else if( isShowCross == 2 &&gCrossScale>0.5) gCrossScale-=0.1;
			if (gCrossScale>MaxSizeCross)gCrossScale = MaxSizeCross;
			else if (gCrossScale<0.5) gCrossScale = 0.5;
			TimeCrossResize = CurTime ;
		}
		if (gUserdata.weaponid==3||gUserdata.weaponid==18||gUserdata.weaponid==13||gUserdata.weaponid==24||fCurrentFOV!=90) isShowCross =0 ; 

		if (isShowCross!=0&& isEnableCross == true&&VPlayer[iLocal].die!=1)
		{
		
			if (!gCrossColor[0]&&!gCrossColor[1]&&!gCrossColor[2])
			SetCrossColor(Cross_color_r->value,Cross_color_g->value,Cross_color_b->value);
			DrawImage(gCrossType_1_index,1,gScreenInfo.iWidth/2,gScreenInfo.iHeight/2,gCrossScale, gCrossColor[0], gCrossColor[1], gCrossColor[2], 255,64,64);
		}		
		Memu_game();


		// DrawMenuScope();



		DrawChatUI(25,gScreenInfo.iHeight-130);
		DrawScoreTab(gScreenInfo.iWidth/2-TabScoreWidth/2,gScreenInfo.iHeight/2-TabScoreHeight/2-0);

		if (isMenuDraw>0)
		{
			gExportfuncs.IN_DeactivateMouse();
			static int iMouseX,iMouseY;
			if (isMenuDraw == UI_ESC )
			DrawMenuEsc();
			else if (isMenuDraw == UI_MAP )
			DrawUiMap();
			else if (isMenuDraw== UI_OPTION_1||isMenuDraw== UI_OPTION_2||isMenuDraw== UI_OPTION_3)
			DrawOptionGui();
			gEngfuncs.GetMousePosition(&iMouseX,&iMouseY);
			DrawImage(TexturesMouse,0,iMouseX,iMouseY,1.0,255,255,255,255);			
		}
			
		//DrawStringFontW(gScreenInfo.iWidth-5+rand() % (5+1+5) + -5,1+ rand() % (5+1+5) + -5 ,L"Hệ thống plugin, module được viết bởi Giấy Nháp","Tahoma",12,1,1,255,255,255,200,2)  ;
}
void GN_Hud_Draw::Memu_game()
{
	return ;
	if (!g_Menu_Drawing||g_Menu_Drawing==2) return;
	int iLineEnd = 0, iLineStart = 0;
	char szPrint[1024];		

	szPrint[1023] = 0;
	int iX, iY;
	int clr=0;
	iY = 100;// (gScreenInfo.iHeight - iLine * 15) / 2;
	iX = 20;
	for (iLineEnd = 0; iLineEnd < 1023 && g_Menu_text[iLineEnd] != '\0'; iLineEnd++)
	{
		if (g_Menu_text[iLineStart+1] == 'r' && g_Menu_text[iLineStart] == '\\')	clr=1;
		else if (g_Menu_text[iLineStart+1] == 'd' && g_Menu_text[iLineStart] == '\\')	clr=2;
		else if (g_Menu_text[iLineStart+1] == 'y' && g_Menu_text[iLineStart] == '\\')	clr=3;
		else if (g_Menu_text[iLineStart+1] == 'w' && g_Menu_text[iLineStart] == '\\')	clr=4;
		if((g_Menu_text[iLineEnd] == '\\' && (g_Menu_text[iLineEnd+1] == 'r' || g_Menu_text[iLineEnd+1] == 'd' || g_Menu_text[iLineEnd+1] == 'y' || g_Menu_text[iLineEnd+1] == 'w')) || g_Menu_text[iLineEnd] == '\n')
		{
			if(g_Menu_text[iLineStart] == '\\' && (g_Menu_text[iLineStart+1] == 'r' || g_Menu_text[iLineStart+1] == 'd' || g_Menu_text[iLineStart+1] == 'y' || g_Menu_text[iLineStart+1] == 'w'))
			{
				if(iLineEnd - iLineStart - 2 >= 0)
					iLineStart += 2;
			}
			memset(szPrint,0,1024);
			strncpy(szPrint, g_Menu_text + iLineStart, iLineEnd - iLineStart);
			iLineStart = iLineEnd;
			if (clr==1)	gEngfuncs.pfnDrawSetTextColor(1.0, 0.3, 0.3);
			else if (clr==2) gEngfuncs.pfnDrawSetTextColor(0.4, 0.4, 0.4);
			else if (clr==3) gEngfuncs.pfnDrawSetTextColor(1.0, 0.93, 0.3);
			else if (clr==4||clr==0) gEngfuncs.pfnDrawSetTextColor(0.95, 0.95, 0.95);

			gEngfuncs.pfnDrawConsoleString(iX, iY+3, szPrint);
			int iLength, iHeight;
			gEngfuncs.pfnDrawConsoleStringLen(szPrint,&iLength,&iHeight);
		
			szPrint[strlen(szPrint)] = 0;
				 gEngfuncs.pfnDrawConsoleString(iX, iY, szPrint);
			iX += iLength;
			
			//if (strlen(szPrint)>5);
			//gEngfuncs.pfnConsolePrint(szPrint));
			
			if (szPrint[0] == 10)
				iX -= 8;
		}
			if (g_Menu_text[iLineEnd] == '\n')
		{
			iY += 16;
			iX = 20;
		}

	}
	
}
void GN_Hud_Draw::AddChatMSg(char *ChatMsg,int r,int g,int b)
{
	//std::string gstringchat(ChatMsg);
	if (ChatMsg[0])
	{
	DeleteMsgchat();

	memset(ChatMsgUI[0].TextChat,0,512);
	strcpy(ChatMsgUI[0].TextChat,ChatMsg);
	ChatMsgUI[0].TextChat[strlen(ChatMsgUI[0].TextChat)-1] = 0;

	//ChatMsgUI[0].TextChat = gstringchat.substr(0,gstringchat.length()-1);
	
	ChatMsgUI[0].gShow = 1;
	ChatMsgUI[0].r = r;
	ChatMsgUI[0].g = g;
	ChatMsgUI[0].b = b;
	ChatMsgUI[0].a = 255;
	
	memset(scrChatMsgUI[0].TextChat,0,512);
	strcpy(scrChatMsgUI[0].TextChat,ChatMsgUI[0].TextChat);
	
	scrChatMsgUI[0].TextChat[strlen(scrChatMsgUI[0].TextChat-1)] = 0;
	scrChatMsgUI[0].gShow = 1;
	scrChatMsgUI[0].r = r;
	scrChatMsgUI[0].g = g;
	scrChatMsgUI[0].b = b;
	scrChatMsgUI[0].a = 255;
	gscrTime[0] = gEngfuncs.GetClientTime();

	}
}
void GN_Hud_Draw::DeleteMsgchat()
{
	memset(ChatMsgUI[7].TextChat,0,512);
	ChatMsgUI[7].TextChat[0]=0;

	for (int i=6;i>=1;i--)
	{
		if (strlen(ChatMsgUI[i-1].TextChat)>0)
		{
			memset(ChatMsgUI[i].TextChat,0,512);
			ChatMsgUI[i].TextChat[0]=0;
			strcpy(ChatMsgUI[i].TextChat,ChatMsgUI[i-1].TextChat);
			//ChatMsgUI[i].TextChat = ChatMsgUI[i-1].TextChat;
			ChatMsgUI[i].gShow =  ChatMsgUI[i-1].gShow;
			ChatMsgUI[i].r =  ChatMsgUI[i-1].r;
			ChatMsgUI[i].g =  ChatMsgUI[i-1].g;
			ChatMsgUI[i].b =  ChatMsgUI[i-1].b;
			ChatMsgUI[i].a =  255;	
		}
		if (strlen(scrChatMsgUI[i-1].TextChat)>0)
		{
			memset(scrChatMsgUI[i].TextChat,0,512);
			strcpy(scrChatMsgUI[i].TextChat,scrChatMsgUI[i-1].TextChat);
			
			scrChatMsgUI[i].gShow =  scrChatMsgUI[i-1].gShow;
			scrChatMsgUI[i].r =  scrChatMsgUI[i-1].r;
			scrChatMsgUI[i].g =  scrChatMsgUI[i-1].g;
			scrChatMsgUI[i].b =  scrChatMsgUI[i-1].b;
			scrChatMsgUI[i].a =  scrChatMsgUI[i-1].a;	
			gscrTime[i] = gscrTime[i-1];
		}
	}
	TimeChatMSG=gEngfuncs.GetClientTime();
	NumChatMSG-=1;
	ChatMsgUI[0].gShow=0;
}

void GN_Hud_Draw::DrawChatUI(int x,int y)
{
	float sysPost;
	float CurTime=gEngfuncs.GetClientTime() ;
	if (ChatInBoxDraw==1)
		{
		
			
			DrawImage(gBGChatImage,0,x-15,y,1.0,255,255 ,255, 255);
			DrawImage(gBGChatImage2,0,x-15,y-148,1.0,255,255 ,255, 255,349,153);
			if (CurTime-TimeDrawSChat>TimeCountChat)
			{
				TimeCountChat=0.7;
				if (gShowSChat!=1)
				{
				TimeCountChat=0.5;
				gShowSChat=1;
				}
				else
				gShowSChat = 0;
				gGN_Hud_Draw->TimeDrawSChat =CurTime;
			}

			if (gPublicChat==0)
			sysPost = DrawStringFontW(x+5-15,y+5,L"Tất cả:","Tahoma",14,1,1,255,255,255,255,0)  ;
			else
			sysPost = DrawStringFontW(x+5-15,y+5,L"Đội:","Tahoma",14,1,1,255,255,255,255,0)  ;

			if (gShowSChat==1)
				gEngfuncs.pfnFillRGBA(x+gPostTextChat+sysPost+10-15,y+10,1,10,255,255,255,255);

			
			if (gStringChat[0]!=0)
			{
			
			//char *gchar = const_cast<char *>(gStringChat.c_str());
			//gEngfuncs.pfnConsolePrint(gStringChat.c_str());
			gPostTextChat= DrawStringFont(x+sysPost+10-15,y+5,gStringChat,"Tahoma",14,1,1,255,255,255,255)  ;	
			
			}
			else gPostTextChat=0;
			
			//gPostTextChat=6.6*(gStringChat.length());
		
		}
	for (int i=0;i<=6;i++)
	{
		if (ChatInBoxDraw==1)
		{
			if (ChatMsgUI[i].gShow == 1)
			{
				//char *gchar = constast<char *>(ChatMsgUI[i].TextChat.c_str());
				DrawStringFont(x+10,y-20-i*15-15,ChatMsgUI[i].TextChat,"Tahoma",13,1,1,ChatMsgUI[i].r,ChatMsgUI[i].g,ChatMsgUI[i].b,ChatMsgUI[i].a);
			}
		}
		else
		{
			if ( i > 4 ) continue;
			if (scrChatMsgUI[i].gShow == 1)
			{
				if (gEngfuncs.GetClientTime() - gscrTime[i] >=5 ) scrChatMsgUI[i].a =255-(gEngfuncs.GetClientTime()-gscrTime[i]-5)*255/1;
				if (scrChatMsgUI[i].a<=0){ scrChatMsgUI[i].a = 0; scrChatMsgUI[i].gShow=0;}
				//char *gchar = constast<char *>(scrChatMsgUI[i].TextChat.c_str());
				DrawStringFont(x+10,y-20-i*15-15,scrChatMsgUI[i].TextChat,"Tahoma",13,1,1,scrChatMsgUI[i].r,scrChatMsgUI[i].g,scrChatMsgUI[i].b,scrChatMsgUI[i].a);
			}
		}
	}
	/*if (ChatMsgUI[5].gShow==1)
	{
		if (ChatMsgUI[5].a > 0) ChatMsgUI[5].a-=5;
		if (ChatMsgUI[5].a<1 ) { ChatMsgUI[5].a = 0;ChatMsgUI[5].gShow=0;}
		char *gchar = const_cast<char *>(ChatMsgUI[5].TextChat.c_str());
		DrawStringFont(x+10,y-20-5*13,gchar,"Tahoma",12,1,1,ChatMsgUI[5].r,ChatMsgUI[5].g,ChatMsgUI[5].b,ChatMsgUI[5].a);

	}*/
	///if (CurTime-TimeChatMSG>=TimeChatMSGcount)
	//{

	//AddMsgdeath("CSFIRED","Cho som Kuteo",0,1,0,1,"");
	//DeleteMsgDeath();
//	DeleteMsgchat();
	//}
	
}
void GN_Hud_Draw::AddMsgdeath(char * id1, char * id2,int iskill,int numkill,int team1,int team2,char *Weapon)
{
	//id1 Vic
	//id2 Killer
	if (strlen(id1) < 2 ||team1<=0 ||team2<=0) return ;
	if (gNumMsgDeath>NumshowMaxDeathMsg-1) DeleteMsgDeath() ;

	memset(gMsgDeathINF[gNumMsgDeath].gMsgDeathstring[0],0,64);
	//gMsgDeathINF[gNumMsgDeath].gMsgDeathstring[0] = std::string(id1);
	strcpy(gMsgDeathINF[gNumMsgDeath].gMsgDeathstring[0],id1);

	gMsgDeathINF[gNumMsgDeath].iType =1;
	if (strlen(id2)<=0)
	{
	gMsgDeathINF[gNumMsgDeath].iType = 0;
	}
	else
	{
	memset(gMsgDeathINF[gNumMsgDeath].gMsgDeathstring[1],0,64);
	strcpy(gMsgDeathINF[gNumMsgDeath].gMsgDeathstring[1],id2);
	//gMsgDeathINF[gNumMsgDeath].gMsgDeathstring[1] = std::string(id2);
	}
	gMsgDeathINF[gNumMsgDeath].IsSKill=iskill;
	gMsgDeathINF[gNumMsgDeath].NumSkill=numkill;
	gMsgDeathINF[gNumMsgDeath].Team1 = team1-1;
	gMsgDeathINF[gNumMsgDeath].Team2 = team2-1;	//sprintf(gMsgDeathINF[4-gNumMsgDeath].szWeapon,"%s",Weapon);
	TimeMSGDeathStart=gEngfuncs.GetClientTime();
	gNumMsgDeath+=1;
	
}
void GN_Hud_Draw::DeleteMsgDeath()
{
	
	//NumMsgDeath-=1;
	for (int i = 0 ; i <NumshowMaxDeathMsg-1 ;i++)
	{
		if (gMsgDeathINF[i+1].gMsgDeathstring[0][0]!=0)
		{
			memset(gMsgDeathINF[i].gMsgDeathstring[0],0,64);
			strcpy(gMsgDeathINF[i].gMsgDeathstring[0], gMsgDeathINF[i+1].gMsgDeathstring[0]);
			memset(gMsgDeathINF[i].gMsgDeathstring[1],0,64);
			strcpy(gMsgDeathINF[i].gMsgDeathstring[1], gMsgDeathINF[i+1].gMsgDeathstring[1]);
		//	gMsgDeathINF[i].gMsgDeathstring[0] = gMsgDeathINF[i+1].gMsgDeathstring[0];
		//	gMsgDeathINF[i].gMsgDeathstring[1] = gMsgDeathINF[i+1].gMsgDeathstring[1];
			gMsgDeathINF[i].IsSKill = gMsgDeathINF[i+1].IsSKill;
			gMsgDeathINF[i].iType = gMsgDeathINF[i+1].iType;
			gMsgDeathINF[i].NumSkill = gMsgDeathINF[i+1].NumSkill;
			gMsgDeathINF[i].Team1 = gMsgDeathINF[i+1].Team1;
			gMsgDeathINF[i].Team2 = gMsgDeathINF[i+1].Team2;
			sprintf(gMsgDeathINF[i].szWeapon,"%s",gMsgDeathINF[i+1].szWeapon);
			//gMsgDeathINF[i+1].gMsgDeathstring[0]=std::string ("");
			memset(gMsgDeathINF[i+1].gMsgDeathstring[0],0,64);
			gMsgDeathINF[i+1].gMsgDeathstring[0][0]=0;
			gMsgDeathINF[i+1].IsSKill=0;
			gMsgDeathINF[i+1].Team1 = 0;
			gMsgDeathINF[i+1].Team2 = 0;
			gMsgDeathINF[i+1].iType = 0;

			}
	}
	TimeMSGDeathStart=gEngfuncs.GetClientTime();
	gNumMsgDeath-=1;
	if (gNumMsgDeath<0) {gNumMsgDeath = 0 ;}
}
void GN_Hud_Draw::DrawMsgDeath(int x,int y)
{
	float CurTime=gEngfuncs.GetClientTime() ;
	
	for (int i =0;i<NumshowMaxDeathMsg;i++)
	{
		int PosAdd = 0 ;
		if (gMsgDeathINF[i].gMsgDeathstring[0][0]!=0)
		{
			if (i<gNumMsgDeath)
			{
				//char *gchar = const_cast<char *>(gMsgDeathINF[i].gMsgDeathstring[0].c_str());
				int gWidth = DrawStringFont(x,y+i*30,gMsgDeathINF[i].gMsgDeathstring[0],"Tahoma",14,1,1,ColorTeamMSG[gMsgDeathINF[i].Team1].r
					,ColorTeamMSG[gMsgDeathINF[i].Team1].g,ColorTeamMSG[gMsgDeathINF[i].Team1].b,ColorTeamMSG[gMsgDeathINF[i].Team1].a,2);
				int gWidth2=0;
				if (gMsgDeathINF[i].iType ==1)
				{
					//char *gchar =  const_cast<char *>(gMsgDeathINF[i].gMsgDeathstring[1].c_str());
					int gx =int(x-(gWidth+80));
					gWidth2 = DrawStringFont(gx,y+i*30,gMsgDeathINF[i].gMsgDeathstring[1],"Tahoma",14,1,1,ColorTeamMSG[gMsgDeathINF[i].Team2].r
					,ColorTeamMSG[gMsgDeathINF[i].Team2].g,ColorTeamMSG[gMsgDeathINF[i].Team2].b,ColorTeamMSG[gMsgDeathINF[i].Team2].a,2);
			
			
					if (gMsgDeathINF[i].NumSkill > 1 )
					{
						int intnum = gMsgDeathINF[i].NumSkill;
						if (intnum>6) intnum = 7;
						DrawImage( KillMsgInfTga[intnum-2],0,gScreenInfo.iWidth-(gWidth+gWidth2+80)-70,y+i*30,1.0,255, 255 , 255,255);
						PosAdd = 60;
					}
					if (gMsgDeathINF[i].IsSKill>0)
					{
						DrawImage( KillMsgInfTga[gMsgDeathINF[i].IsSKill+5],0,gScreenInfo.iWidth-(gWidth+gWidth2+80)-50-PosAdd,y+i*30-6,1.0,255, 255 , 255,255);
					}
				}
			}
		}
	}
	if (CurTime-TimeMSGDeathStart>=5.0) DeleteMsgDeath();
}
void GN_Hud_Draw::GN_HUD_DrawNumTga(int Num,int Create,int x,int y,int r, int g , int b, int alpha,int  tgaindex[10], int Width)
{
	char gChart[10];
	int postAdd;
	sprintf(gChart,"%d", Num );
	std::string gstring(gChart);
	std::string gstring2;
	int StrLen = strlen(gstring.c_str());
	if (Create==1)
	{
		postAdd = StrLen*Width/2;

	}
	else if (Create==2)
	{
		postAdd = StrLen*Width;
	}
	else
	{
		postAdd = 0;
	}
	
	for (int i=0;i< StrLen;i++)
	{
		// std::string gstring2(&gChart[i]);
		int gint = std::stoi( gstring.substr(i,1) );
		if(gint>=0&&gint<=9)		
		DrawImage(tgaindex[gint],0,x+(Width-0)*i-postAdd,y,1.0f,255,255 ,255,255);
		//sprintf(szString2,"%d",gint);
		//gEngfuncs.pfnConsolePrint(&szString2[i]);
	}
	
	//delete gChart;
}
void GN_Hud_Draw::DrawImage(int idTextures,int isCreate,int x,int y,float scale,int r, int g , int b, int alpha,int width,int height)
{
	if (alpha<=0) alpha = 0;
	int Width=width;
	int Height=height;
	if( width <= 0 )
	m_pSurface->DrawGetTextureSize(idTextures,Width,Height);
	if (Width<5&&Height<5) return ;
	Width = floorf(Width*scale);
	Height = floorf(Height*scale);

	if (isCreate ==1) 
	{
		
		x -= Width/2;
		y-= Height/2;
	}

  	glEnable(GL_BLEND);
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
}

int GN_Hud_Draw::addresource(char *szFile,int checkHas)
{
	
	if (checkHas==1)
	if (getHasImage(szFile)!=-5) return -1;
	if (CountTga>999) return -1;
	if (!CountTga) CountTga = 0;
	//int width;
	// height;
	CountTga+=1;
	gTgaList[CountTga].index =m_pSurface->CreateNewTextureID();
	sprintf(gTgaList[CountTga].TextFile,"%s",szFile);
	//gTgaList[CountTga].index = WeapomHudN_index[0];
	//return -1;
	
	if (!LoadTgaImage(szFile, gTgaList[CountTga].index, &gTgaList[CountTga].TextSize[0],&gTgaList[CountTga].TextSize[1])) return -1;
	
	//loadNormalFormat (gTgaList[CountTga].index,szFile, &width, &height);

	
	return CountTga;
}
int GN_Hud_Draw::getHasImage(char *szFile)
{
	if (CountTga>0) 

	for (int i =1 ;i<=CountTga;i++)
	{
		if (!strcmp(szFile,gTgaList[i].TextFile)) return i;
	}

	return -5;
}

int GN_Hud_Draw::SetDrawFont(char *Str,int Align,int x,int y,int AlignScr,char *Font,int Size,int bold,int boder,float TimeCount,float Timein,float Timeout,int r,int g,int b,int a,int channel)
{
	
	
	ChannelDraw[channel].Type = 2;
	
	ChannelDraw[channel].a = a;
	ChannelDraw[channel].alpha = a;
	ChannelDraw[channel].r = r;
	ChannelDraw[channel].g = g;
	ChannelDraw[channel].b =b;
	ChannelDraw[channel].bold=bold;
	ChannelDraw[channel].boder = boder;
	ChannelDraw[channel].sizefont  = Size;
	if (Timein> 0.0) ChannelDraw[channel].Effect  = Effect_Addtive;

	SetDrawTga2(1.0, Timein, Timeout, TimeCount,Align, AlignScr,channel);
		//ChannelDraw[channel].TimeCount = TimeCount;
	//ChannelDraw[channel].Timein = Timein;
	//ChannelDraw[channel].Timeout = Timeout;
	return 0;
}
int GN_Hud_Draw::SetDrawTga(char *szFile , int channel, int Effect,int x,int y,int r, int g,int b, int alpha)
{
	
	if (channel>999||channel<0) return -1 ;
	if (!MaxTgaFileHas) MaxTgaFileHas =0; 
	if (MaxTgaFileHas<channel)MaxTgaFileHas = channel;
	ChannelDraw[channel].Type = 1;
	int gHas = getHasImage(szFile);
	
	if (gHas<0)
	{
		//gHas = addresource(szFile,0);
		return -1 ;
	}
	//return;
	ChannelDraw[channel].index = gHas;
	ChannelDraw[channel].Effect=Effect;
	ChannelDraw[channel].x=x;
	ChannelDraw[channel].y=y;
	ChannelDraw[channel].a=alpha;
	ChannelDraw[channel].alpha=alpha;
	ChannelDraw[channel].r=r;
	ChannelDraw[channel].b=b;
	ChannelDraw[channel].g=g;
	return 1;	
}
void GN_Hud_Draw::SetDrawTga2(float Scaler, float timein, float timeout, float timecount,int center, int ALIGN,int channel)
{
	if (channel>999&&channel<0) return ;

	ChannelDraw[channel].TimeCount=timecount;
	ChannelDraw[channel].scaler=Scaler;
	ChannelDraw[channel].center=center;
	if (ALIGN==1)
	{
		ChannelDraw[channel].x=gScreenInfo.iWidth-ChannelDraw[channel].x;
		
	}
	else if (ALIGN==2)
	{
		
		ChannelDraw[channel].y=gScreenInfo.iHeight-ChannelDraw[channel].y;
	}
	else if (ALIGN==3)
	{
		ChannelDraw[channel].x=gScreenInfo.iWidth-ChannelDraw[channel].x;
		ChannelDraw[channel].y=gScreenInfo.iHeight-ChannelDraw[channel].y;

	}
	else if (ALIGN==4)
	{
		
		ChannelDraw[channel].y=gScreenInfo.iHeight/2+ChannelDraw[channel].y;
	}
	else if (ALIGN==5)
	{
		ChannelDraw[channel].x=gScreenInfo.iWidth/2+ChannelDraw[channel].x;
		
	}
	else if (ALIGN==6)
	{
		ChannelDraw[channel].x=gScreenInfo.iWidth/2+ChannelDraw[channel].x;
		ChannelDraw[channel].y=gScreenInfo.iHeight/2+ChannelDraw[channel].y;
	}
	else if (ALIGN==7)
	{
		ChannelDraw[channel].x=gScreenInfo.iWidth/2+ChannelDraw[channel].x;
		ChannelDraw[channel].y=gScreenInfo.iHeight+ChannelDraw[channel].y;
	}
	else if (ALIGN==8)
	{
		ChannelDraw[channel].x=gScreenInfo.iWidth+ChannelDraw[channel].x;
		ChannelDraw[channel].y=gScreenInfo.iHeight/2+ChannelDraw[channel].y;
	}

	if (ChannelDraw[channel].Effect != Effect_Nomal)
	
	{
		ChannelDraw[channel].alpha = 0;
		ChannelDraw[channel].Timein=timein;
		ChannelDraw[channel].Timeout=timeout;
	}

		ChannelDraw[channel].TimeStart=gEngfuncs.GetClientTime();
	ChannelDraw[channel].Show=1;
}
void GN_Hud_Draw::RemoveDraw(int channel)
{
	ChannelDraw[channel].index=0;
	ChannelDraw[channel].Show=0;
}
void GN_Hud_Draw::SetScalerDraw(int channel,float Scaler)
{
	ChannelDraw[channel].scaler=Scaler;
	
}

int loadNormalFormat (int idWP,const char *filename, int *width, int *height)
{
	
	std::string gString(filename);
	std::wstring wcstring(gString.size(), L' '); 
	wcstring.resize(mbstowcs(&wcstring[0], gString.c_str(), gString.size()));

	Gdiplus::Bitmap bmp(wcstring.c_str());

	Gdiplus::BitmapData data;
	int iWidth = bmp.GetWidth();
	int iHeight = bmp.GetHeight();
	if (iWidth<10||iHeight<10) return 1;
	bmp.LockBits(&Gdiplus::Rect(0, 0, iWidth, iHeight), Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data);
	bmp.UnlockBits(&data);
	BYTE *image = (BYTE *)data.Scan0;
	UINT p = 0; 
	BYTE t = 0;

	for(INT Y = 0; Y < iHeight; ++Y)
	{
		for(INT X = 0; X < iWidth; ++X)
		{
			 t = image[p + 0];

			image[p + 0] = image[p + 2]; 
			image[p + 2] = t;

			p += 4;
		}
	}
	int iTexID = idWP;
	glBindTexture(GL_TEXTURE_2D, iTexID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	*width=iWidth;
	*height=iHeight;
	return iTexID;
}

int LoadTgaImage(char *szFileName, int Index,int *SizeW,int *SizeH)
{
	
	if(!g_pInterface->FileSystem->FileExists(szFileName))
	{
		return -1;
	}

	FileHandle_t pFile = g_pInterface->FileSystem->Open(szFileName,"r");
	if(pFile == NULL)
	{
	//	gLog->WriteLog("TGA file open error:%s",szFileName);
		return -1;
	}

	int fileLength = g_pInterface->FileSystem->Size(pFile);
	byte *buffer = (byte *)malloc(fileLength);
	g_pInterface->FileSystem->Read(buffer,fileLength,pFile);


	int	i, columns, rows, row_inc, row, col;
	byte	*buf_p, *pixbuf, *targa_rgba;
	byte	palette[256][4], red = 0, green = 0, blue = 0, alpha = 0;
	int	readpixelcount, pixelcount;
	qboolean	compressed;
	loadtga_s	targa_header;

	buf_p = (byte *)buffer;
	targa_header.id_length = *buf_p++;
	targa_header.colormap_type = *buf_p++;
	targa_header.image_type = *buf_p++;

	targa_header.colormap_index = buf_p[0] + buf_p[1] * 256;		buf_p += 2;
	targa_header.colormap_length = buf_p[0] + buf_p[1] * 256;		buf_p += 2;
	targa_header.colormap_size = *buf_p;				buf_p += 1;
	targa_header.x_origin = *(short *)buf_p;			buf_p += 2;
	targa_header.y_origin = *(short *)buf_p;			buf_p += 2;
	targa_header.width = *(short *)buf_p;		buf_p += 2;
	targa_header.height = *(short *)buf_p;		buf_p += 2;
	targa_header.pixel_size = *buf_p++;
	targa_header.attributes = *buf_p++;
	if( targa_header.id_length != 0 ) buf_p += targa_header.id_length;	// skip TARGA image comment

	if( targa_header.image_type == 1 || targa_header.image_type == 9 )
	{
		// uncompressed colormapped image
		if( targa_header.pixel_size != 8 )
		{
		//	gLog->WriteLog("TGA processing error:Only 8 bit images supported for type 1 and 9");
			return false;
		}
		if( targa_header.colormap_length != 256 )
		{
//gLog->WriteLog("TGA processing error:Only 8 bit colormaps are supported for type 1 and 9");
			return false;
		}
		if( targa_header.colormap_index )
		{
//gLog->WriteLog("TGA processing error:colormap_index is not supported for type 1 and 9");
			return false;
		}
		if( targa_header.colormap_size == 24 )
		{
			for( i = 0; i < targa_header.colormap_length; i++ )
			{
				palette[i][2] = *buf_p++;
				palette[i][1] = *buf_p++;
				palette[i][0] = *buf_p++;
				palette[i][3] = 255;
			}
		}
		else if( targa_header.colormap_size == 32 )
		{
			for( i = 0; i < targa_header.colormap_length; i++ )
			{
				palette[i][2] = *buf_p++;
				palette[i][1] = *buf_p++;
				palette[i][0] = *buf_p++;
				palette[i][3] = *buf_p++;
			}
		}
		else
		{
	//		gLog->WriteLog("TGA processing error:only 24 and 32 bit colormaps are supported for type 1 and 9");
			return false;
		}
	}
	else if( targa_header.image_type == 2 || targa_header.image_type == 10 )
	{
		// uncompressed or RLE compressed RGB
		if( targa_header.pixel_size != 32 && targa_header.pixel_size != 24 )
		{
	//		gLog->WriteLog( "TGA processing error:Only 32 or 24 bit images supported for type 2 and 10");
			return false;
		}
	}
	else if( targa_header.image_type == 3 || targa_header.image_type == 11 )
	{
		// uncompressed greyscale
		if( targa_header.pixel_size != 8 )
		{
		//	gLog->WriteLog( "TGA processing error:Only 8 bit images supported for type 3 and 11");
			return false;
		}
	}

	columns = targa_header.width;
	rows = targa_header.height;

	int size = columns * rows * 4;
	targa_rgba = (byte *)malloc (size);

	pixbuf = targa_rgba + ( rows - 1 ) * columns * 4;
	row_inc = -columns * 4 * 2;	

	compressed = ( targa_header.image_type == 9 || targa_header.image_type == 10 || targa_header.image_type == 11 );
	for( row = col = 0; row < rows; )
	{
		pixelcount = 0x10000;
		readpixelcount = 0x10000;

		if( compressed )
		{
			pixelcount = *buf_p++;
			if( pixelcount & 0x80 )  // run-length packet
				readpixelcount = 1;
			pixelcount = 1 + ( pixelcount & 0x7f );
		}

		while( pixelcount-- && ( row < rows ) )
		{
			if( readpixelcount-- > 0 )
			{
				switch( targa_header.image_type )
				{
				case 1:
				case 9:
					// colormapped image
					blue = *buf_p++;
					red = palette[blue][0];
					green = palette[blue][1];
					alpha = palette[blue][3];
					blue = palette[blue][2];
					//if( alpha != 255 ) image.flags |= IMAGE_HAS_ALPHA;
					break;
				case 2:
				case 10:
					// 24 or 32 bit image
					blue = *buf_p++;
					green = *buf_p++;
					red = *buf_p++;
					alpha = 255;
					if( targa_header.pixel_size == 32 )
					{
						alpha = *buf_p++;
						//if( alpha != 255 )
							//image.flags |= IMAGE_HAS_ALPHA;
					}
					break;
				case 3:
				case 11:
					// greyscale image
					blue = green = red = *buf_p++;
					alpha = 255;
					break;
				}
			}


			*pixbuf++ = red;
			*pixbuf++ = green;
			*pixbuf++ = blue;
			*pixbuf++ = alpha;
			if( ++col == columns )
			{
				row++;
				col = 0;
				pixbuf += row_inc;
			}
		}
	}

	// Make a texture
	
	
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Index);
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, targa_header.width, targa_header.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, targa_rgba);
	free(targa_rgba);
	free(buffer);
	g_pInterface->FileSystem->Close(pFile);
	
	*SizeW=targa_header.width;
	*SizeH= targa_header.height;
	//TextSize
	return 1;
}


void DrawTgaLoadList(void)
{
	
	FILE* pTgaList;
	static char szBuff[512];
	szBuff[0] = '\0';
	pTgaList = fopen("cstrike//TgaFile.txt", "rt");
	if(pTgaList == NULL)									
	{
		
		return;
	}
	while(!feof(pTgaList))
	{
		fgets(szBuff,sizeof(szBuff)-1,pTgaList);
		if(szBuff[strlen(szBuff)-1] =='\n') szBuff[strlen(szBuff)-1] = '\0';

		if(szBuff[0]=='/' && szBuff[1]=='/' || !szBuff[0]) continue;
		
		gGN_Hud_Draw->addresource(szBuff,1);
	
	}


	
	
	

}
