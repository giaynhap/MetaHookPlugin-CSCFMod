#include <gl/glew.h>
#include "Base.h"
#include "KillMark.h"
#include "DrawText.h"
#include "Global.h"
#include "HudDraw.h"
#include "parsemsg.h"
#include "Winuser.h"
#include "mempatchs.h"
#include <cvardef.h>
#include "mathlib.h"

#include <gl/glut.h>
//#include <gl/glxew.h>
//#include <gl/wglew.h>

efx_api_s gEfxAPI;

extern int gButtonControlSel ;
extern int ButtonTabOption[3];
int g_iBombEntity;
int m_iScreenTexture;
int gBompDrop;
int oldChannel;
int OldReturnIMG;
engine_studio_api_t IEngineStudio;
event_api_s gEventAPI;
cl_enginefunc_t gEngfuncs;
int gBodyWeaponView ;
r_studio_interface_t **pStudioInterface;

Gdiplus::GdiplusStartupInput gdipSIn;
//cl_entity_t **r_currententity;
//#define GetEngfuncsAddress(addr) (g_dwEngineBase+addr-0x1D01000)

//type_R_DrawViewModel g_pfn_R_DrawViewModel ;
unsigned long gdipToken;
 
int Initialize(struct cl_enginefuncs_s *pEnginefuncs, int iVersion)
{
	gBodyWeaponView =0;
	memcpy(&gEngfuncs, pEnginefuncs, sizeof(gEngfuncs));
	MemPatch_Start(MEMPATCH_STEP_INITCLIENT);
	memcpy(&gEventAPI,pEnginefuncs->pEventAPI,sizeof(event_api_s));
	memcpy(&gEfxAPI, pEnginefuncs->pEfxAPI, sizeof(gEfxAPI));
	pEnginefuncs->LoadMapSprite =&Engfunc_LoadMapSprite;
	pEnginefuncs->COM_LoadFile = &Engfunc_COM_LoadFile;
	pEnginefuncs->COM_ParseFile = &Engfunc_COM_ParseFile;
	pEnginefuncs->COM_FreeFile = &Engfunc_COM_FreeFile;
	pEnginefuncs->pfnHookUserMsg = &EngFunc_HookUserMsg;
	pEnginefuncs->pfnClientCmd = &Engfunc_ClientCmd;
	pEnginefuncs->pEventAPI->EV_WeaponAnimation = &Engfunc_WeaponAnim;
	pEnginefuncs->pfnAddCommand = &EngFunc_AddCommand;
	
	MSG_Init();
	//pEnginefuncs->pfnSetCrosshair = Engfunc_CrossHair;
	//g_pfn_R_DrawViewModel = (type_R_DrawViewModel)GetEngfuncsAddress(0x1D45660);
//g_pMetaHookAPI->InlineHook((void *)g_pfn_R_DrawViewModel, R_DrawViewModel, (void *&)g_real_R_DrawViewModel);
	//gEngfuncs.pfnHookEvent("2=Dropped_The_Bomb",)

	return gExportfuncs.Initialize(pEnginefuncs, iVersion);
}
extern int CSWPN_AMMO[];



void HUD_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed)
{
	
	if(cmd)
	{
		if(cmd->buttons & IN_ATTACK) 
		{
			if (gGN_Hud_Draw->gUserdata.clip>0||gGN_Hud_Draw->gUserdata.weaponid==29)
			{
				isShowCross = 3;
				TimeReturnCross = gEngfuncs.GetClientTime() ;
				if (gGN_Hud_Draw->gUserdata.weaponid==29)
				MaxSizeCross = 0.6;
				else if (gGN_Hud_Draw->gUserdata.weaponid==20)
				MaxSizeCross = 1.0;
				else if (gGN_Hud_Draw->gUserdata.weaponid==28)
				MaxSizeCross = 0.8;
				else
				MaxSizeCross = 0.7;

			}
			
		}
		if (cmd->buttons &IN_MOVELEFT||cmd->buttons &IN_MOVERIGHT||cmd->buttons &IN_BACK||cmd->buttons & IN_FORWARD)
			{
				if (gCrossScale<=0.65)
				{
					if (isShowCross!=3)
					MaxSizeCross = 0.6;
					isShowCross = 3;
					TimeReturnCross = gEngfuncs.GetClientTime() ;	
				}
				
				
				

			}
	}
	
	return gExportfuncs.HUD_PostRunCmd(from, to, cmd, runfuncs, time, random_seed);
}
void IN_Accumulate(void)
{
	if (gEngfuncs.GetClientTime()-TimeNextClick <0.3) return;
	if(isMenuDraw>0) return;

	return gExportfuncs.IN_Accumulate();
}
void ResetConfig()
{
	char gChar[32];
					for(int i=0;i<19;i++)
					{
						memset(gChar,0,32);
						if (gDatakey_Config[i][0]!=0)
						{
						
						sprintf(gChar,"bind %s \"\" ",gDatakey_Config[i]);
						gEngfuncs.pfnClientCmd(gChar);
						sprintf(gChar,"bind %s %s",gDatakey_Config[i],gszData_Config[i]);
						gEngfuncs.pfnClientCmd(gChar);
						
						WritePrivateProfileStringA("Config",gszData_Config[i],gDatakey_Config[i],".\\USERCONFIG.ini");
						}
						else
						{
							
							sprintf(gChar,"bind  %s",gszData_Config[i]);
							gEngfuncs.pfnClientCmd(gChar);
							WritePrivateProfileStringA("Config",gszData_Config[i],"",".\\USERCONFIG.ini");
						}

						memset(gChar,0,32);
						sprintf (gChar,"%i",DataMouse);
						WritePrivateProfileStringA("Config","sensitivity",gChar,".\\USERCONFIG.ini");
						memset(gChar,0,32);
						sprintf(gChar,"sensitivity  %f",(float)DataMouse/100.0*20.0);
						gEngfuncs.pfnClientCmd(gChar);


						memset(gChar,0,32);
						sprintf (gChar,"%i",gDataSound[0]);
						WritePrivateProfileStringA("Config","volume",gChar,".\\USERCONFIG.ini");

						memset(gChar,0,32);
						sprintf (gChar,"%i",gDataSound[1]);
						WritePrivateProfileStringA("Config","MP3Volume",gChar,".\\USERCONFIG.ini");

						memset(gChar,0,32);
						sprintf(gChar,"volume  %f",(float)gDataSound[0]/100.0*1.0);
						gEngfuncs.pfnClientCmd(gChar);

						memset(gChar,0,32);
						sprintf(gChar,"MP3Volume  %f",(float)gDataSound[1]/100.0*2.0);
						gEngfuncs.pfnClientCmd(gChar);

						if (gDataMouseTab3[0]==1)
						{
							WritePrivateProfileStringA("Config","r_righthand","1",".\\USERCONFIG.ini");
							gEngfuncs.pfnClientCmd("cl_righthand 1");
						}
						else
						{
							WritePrivateProfileStringA("Config","r_righthand","0",".\\USERCONFIG.ini");
							gEngfuncs.pfnClientCmd("cl_righthand 0");
						}

						if (gDataMouseTab3[1]==1)
						{
							WritePrivateProfileStringA("Config","reverse_mouse","1",".\\USERCONFIG.ini");
							gEngfuncs.pfnClientCmd("m_pitch -0.0022");
						}
						else
						{
							WritePrivateProfileStringA("Config","reverse_mouse","0",".\\USERCONFIG.ini");
							gEngfuncs.pfnClientCmd("m_pitch 0.0022");
						}
					//	gEngfuncs.pfnConsolePrint(gChar);
					}


}
void IN_MouseEvent(int x)
{	

	if (!(x&ME_LEFTCLICK))
		IsClickMouse = 0;
		if (abs(gEngfuncs.GetClientTime()-TimeNextClick) <0.6) return;
		
		

		if (isModelButtonSetting==true )
		{
				char gsz[32];
				memset(gsz,0,32);


				
			if (x&ME_LEFTCLICK)
			{
				sprintf(gsz,"MOUSE1");
			}
			else if (x&ME_RIGHTCLICK)
			{
				sprintf(gsz,"MOUSE2");
			}
			else if (x&ME_WHEELCLICK)
			{
				sprintf(gsz,"MOUSE3");
			}
			else return;

			for (int i= 0;i<19;i++)
			if (strcmp(gDatakey_Config[i],gsz)==0)
			{memset(gDatakey_Config[i],0,32);gDatakey_Config[i][0]=0;}


				memset(gDatakey_Config[gButtonControlSel],0,32);
				strcpy(gDatakey_Config[gButtonControlSel],gsz);


			gButtonControlSel = 0;
			isModelButtonSetting = false;
			//gEngfuncs.Con_Printf("\n-> %s",gsz);
			TimeNextClick = gEngfuncs.GetClientTime();
			return ;
		}


		if (!gHMouseClick[0] ) gHMouseClick[0] = -1;
		gHMouseClick[1]=gHMouseClick[0];
		gHMouseClick[0]=x;
	

	

		if (gHMouseClick[0]==-535) return;
		
		if (x&ME_LEFTCLICK&&isMenuDraw>0)
		{
			int iMouseX,iMouseY;
			gEngfuncs.GetMousePosition(&iMouseX,&iMouseY);
			if (isMenuDraw==UI_ESC)
			{
				int gi = CheckPost((float)iMouseX,(float)iMouseY);
				if (gi==1) {gEngfuncs.pfnSetMousePos(gEngfuncs.GetWindowCenterX(),gEngfuncs.GetWindowCenterY());isMenuDraw = 0;gEngfuncs.pfnSetMouseEnable(0); }
				else if (gi==2) isMenuDraw = UI_MAP;
				else if (gi==3) {isMenuDraw = UI_OPTION_1;gButtonControlSel=false;}
				else if (gi==4) gEngfuncs.pfnClientCmd("quit") ;
				gHMouseClick[0]==-535;
			}
			else if(isMenuDraw ==UI_OPTION_1 ||isMenuDraw ==UI_OPTION_2 ||isMenuDraw ==UI_OPTION_3) 
			{
				int gi = Check_VguiSeting_CRTL((float)iMouseX,(float)iMouseY);
				 if (ButtonTabOption[0] == gi)
				{
						isMenuDraw =UI_OPTION_1;
						
						TimeNextClick = gEngfuncs.GetClientTime();
						return ;
				}
				else if (ButtonTabOption[1] == gi)
				{
						isMenuDraw =UI_OPTION_2;
						
						TimeNextClick = gEngfuncs.GetClientTime();
						return ;
					
				}
				else if (ButtonTabOption[2]==gi)
				{
						isMenuDraw =UI_OPTION_3;
						
						TimeNextClick = gEngfuncs.GetClientTime();
						return ;
				}
				else if (gi == gButtonOptionClose) 
				{
					isMenuDraw =UI_ESC;

				}
				else if (gi == gButtonOptionSave)
				{	
					
					ResetConfig();

					isMenuDraw =UI_ESC;
				}

				if (isMenuDraw ==UI_OPTION_1 )
				{

						if (isModelButtonSetting!=true)
						{
							for (int i=0;i<19;i++)
							{
								 if (gButtonOpticonControl[i]==gi)
								 {
									 isModelButtonSetting = true;
									 gButtonControlSel = i;
								
								 }
							 
							}
					
						 if (ButtonMouseSpeed[1]==gi)
						{
							IsClickMouse=1;
							//SetButtonPOS(ButtonMouseSpeed[1], iMouseX,-1);
							//SetButtonPOS(ButtonMouseSpeed[1],x+155+DataMouse*212/100-7,y+362);

							return gExportfuncs.IN_MouseEvent(x);
						}

						 else if (ButtonMouseSpeed[0] == gi)
						 {
							 SetButtonPOS(ButtonMouseSpeed[1], iMouseX-7,-1);
							IsClickMouse = 1;
							 return gExportfuncs.IN_MouseEvent(x);
						 }
						 else if (ButtonNexDownSPEED[0]== gi )
						 {
								if (DataMouse<100) 
									DataMouse +=1;
						 }
						 else if (ButtonNexDownSPEED[1]== gi )
						 {
								if (DataMouse>0) 
									DataMouse -=1;
						 }

						}

				}
				else if (isMenuDraw ==UI_OPTION_2)
				{
					int gi2 = Check_VguiSeting_CRTL((float)iMouseX,(float)iMouseY,GStartSearch[0]);
					if (ButtonSoundSpeed[0]==gi2) IsClickMouse = 1;
					if (ButtonSoundSpeed[1]==gi2) IsClickMouse = 2;
				}
				else if (isMenuDraw == UI_OPTION_3)
				{

					int gi3 = Check_VguiSeting_CRTL((float)iMouseX,(float)iMouseY,GStartSearch[1]);
					gEngfuncs.Con_Printf("\n CCg: %i",gi3);
					
					if (ButtonCheckTabop3[0]== gi3) { gDataMouseTab3[0] = 0;}
					else if (ButtonCheckTabop3[1]==gi3) gDataMouseTab3[0] = 1;
					else if (ButtonCheckTabop3[2]==gi3) gDataMouseTab3[1] = 0;
					else if (ButtonCheckTabop3[3]==gi3) gDataMouseTab3[1] = 1;

				}
			//	gEngfuncs.Con_Printf("\nCHH: %i || %i || %i || %i \n",iMouseX,iMouseY,gi,gButtonOptionClose);
			}
			TimeNextClick = gEngfuncs.GetClientTime();
			return;
		}
		
		if (isMenuDraw==1) return;
		if (ShowMenuScope==1)
		{
			int iMouseX,iMouseY;
			gEngfuncs.GetMousePosition(&iMouseX,&iMouseY);
			if (x&ME_LEFTCLICK)
			{
				ControlChosseScope = Check_Mouse_Menu_Scope((float)iMouseX,(float)iMouseY);
			}
			return;
		}
	return gExportfuncs.IN_MouseEvent(x);
}
void ResetHud(void)
{
	gGN_Hud_Draw->TimeDrawSChat=0.0f;
	gGN_Hud_Draw->TimeCountChat=0.0f;
	gGN_Hud_Draw->gShowSChat=0;
	gGN_Hud_Draw->NumChatMSG=0;
	gGN_Hud_Draw->TimeChatMSG=0.0f;
	gGN_Hud_Draw->TimeChatMSGcount=10.0f;
	gGN_Hud_Draw->gNumMsgDeath = 0;
	gGN_Hud_Draw->gPublicChat = 0 ;
	gGN_Hud_Draw->gDrawTeamname = 0;
	gGN_Hud_Draw->ScoreTabisShow = 0;
	gGN_Hud_Draw->ScorePostMove = 0.0;
	gGN_Hud_Draw->ScoreTabTimeStartDraw = 0.0;
	gBodyWeaponView = 0;
	isMenuDraw = 0;
	memset(gGN_Hud_Draw->texyOldChat,0,64);
	gGN_Hud_Draw->ScoreTabisShow = 0;
	isModelButtonSetting = false;
	gButtonControlSel= -1;
	gPlayerAce[0] = 0;
	gPlayerAce[1] = 0;
	gPlayerAce[2] = 0;
	gnKillMark->KillmarkReset();
	for (int i=1;i<=32;i++)
	{
		gGN_Hud_Draw->VPlayer[i].die = 0;
		gScoreAllPlayer[0][i] = 0;
		gScoreAllPlayer[1][i] = 0;
		gScoreAllPlayer[2][i] = 0;
		gGN_Hud_Draw->VPlayer[i].Team = 0;

	}

	for (int i=0;i<=4;i++)
	{
		gGN_Hud_Draw->ChatMsgUI[i].gShow =0;
		
	}
	for (int i = NumshowMaxDeathMsg ; i >0 ;i--)
	{
		
		memset(gGN_Hud_Draw->gMsgDeathINF[i-1].gMsgDeathstring[0],0,64) ;
		memset(gGN_Hud_Draw->gMsgDeathINF[i-1].gMsgDeathstring[1],0,64) ;
		gGN_Hud_Draw->gMsgDeathINF[i-1].gMsgDeathstring[0][0]=0;
		gGN_Hud_Draw->gMsgDeathINF[i-1].gMsgDeathstring[0][1]=0;
	}
	
}
void DrawRe();


float CalcFovX (float fov_y, float width, float height)
{
    float   a;
    float   y;

    if (fov_y < 1) fov_y = 1;
    if (fov_y > 179) fov_y = 179;

    y = height / tan (fov_y / 360 * M_PI);
    a = atan (width / y);
    a = a * 360 / M_PI;

    return a;
}

float CalcFovY (float fov_x, float width, float height)
{
    float   a;
    float   x;

    if (fov_x < 1) fov_x = 1;
    if (fov_x > 179) fov_x = 179;

    x = width / tan (fov_x / 360 * M_PI);
    a = atan (height / x);
    a = a * 360 / M_PI;

    return a;
}

DWORD r_currententity;
DWORD dwBoneTransform;
void HUD_DrawTransparentTriangles(void)
{
	
	
//	glEnable(0x84F5);
	//glBindTexture(0x84F5, m_iScreenTexture);
	//glDisable(0x84F5);
		vec3_t gEndpost ;
		
		_VectorCopy( gEngfuncs.GetLocalPlayer()->origin, gEndpost);
		
	/*  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	  
	 glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glBindTexture(GL_TEXTURE_2D,gGN_Hud_Draw->TabScoreIndex[0]);
	glColor4ub(255,255,255,255);
	float angle = gVecAngle[0];
	angle=angle *M_PI/180;

	float angle2 = gVecAngle[1];
	angle2=angle2 *M_PI/180;

	
	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex3f(gEndpost[0]+10+40*cos(angle2),gEndpost[1]+40*cos(angle2),20);
	glTexCoord2f(1,1);
	glVertex3f(gEndpost[0]+10+40*sin(angle)*sin(angle2),gEndpost[1]+40*cos(angle2),20+40*cos(angle));
	glTexCoord2f(1,0);
	glVertex3f(gEndpost[0]+10+40*sin(angle),gEndpost[1],20+40*cos(angle));
	glTexCoord2f(0,0);
	glVertex3f(gEndpost[0]+10,gEndpost[1],20);
	glEnd();
    glDisable(GL_BLEND);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	*/

GLfloat red_light[] =
{1.0, 0.0, 0.0, 1.0}, green_light[] =
{0.0, 1.0, 0.0, 1.0}, white_light[] =
{1.0, 1.0, 1.0, 1.0};

cl_entity_t *viewent = gEngfuncs.GetViewModel();
viewent->curstate.body= gBodyWeaponView;
glViewport(0, 0, gScreenInfo.iWidth, gScreenInfo.iHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
 
  gluPerspective(60.0, gScreenInfo.iWidth /  gScreenInfo.iHeight, 0.1, 1000.0);
  //glOrtho(0, 0, , 0, 0.1, 1000.0);
   // glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
 // glRotatef(90, 1, 0, 0);
  glScalef(1.0,1.3,1.0);
  if (mainViewAngles[0]<0.0)
  glTranslatef(0.0,mainViewAngles[0]/84.0f*0.5,mainViewAngles[0]/84.0f*0.5);
  else
  glTranslatef(0.0,-mainViewAngles[0]/84.0f*0.5,mainViewAngles[0]/84.0f*1.6);
 
  //glScalef(1.0,1.0,0.6);

  

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity;


 

 // IEngineStudio

//  glPushMatrix();

	//glViewport(GL_ZERO, GL_ZERO, GL_, MIRROR_RESOLUTION);

	//glCullFace(GL_FRONT);
	//glColor4f(GL_ONE, GL_ONE, GL_ONE, GL_ONE);

return gExportfuncs.HUD_DrawTransparentTriangles ();
}

int FuncGiayNhapModule(const char* pszName, int iSize, void* pbuf)
{
	
	BEGIN_READ( pbuf, iSize );
	
	int id= READ_LONG();
	if (id==1)
	{
		int id2= READ_BYTE();
		int Channel = READ_LONG();
		if (id2==1)
		{
		
			oldChannel = Channel;
			if (Channel < 0 || Channel >999) return 1;
			char *FileTexture = READ_STRING();
			int x = READ_LONG();
			int y = READ_LONG();
			int Effect = READ_BYTE();
			int r =READ_BYTE();
			int g = READ_BYTE();
			int b = READ_BYTE();
			int a = READ_BYTE();
			OldReturnIMG = gGN_Hud_Draw->SetDrawTga(FileTexture , Channel,Effect,x,y,r,g,b,a);
			//gEngfuncs.Con_Printf("%s|%i|%i|%i|%i|%i|%i|%i|",FileTexture,Channel, Effect,x,y,r,g,b,a);
			float scaler,Timeint,Timeout,TimeCount;
			if (OldReturnIMG < 0 ) {gEngfuncs.pfnConsolePrint("Draw error \n");return 1; }
			int Intscaler = READ_LONG();
			scaler = Intscaler/1000;
			int IntTimeCount = READ_LONG();
			TimeCount = IntTimeCount/1000;
			int IntTimeint = READ_LONG();
			Timeint = IntTimeint/1000;
			int IntTimeout = READ_LONG();
			Timeout = IntTimeout/1000;
			int center =  READ_BYTE();
			int ALIGN = READ_BYTE();
			gGN_Hud_Draw->SetDrawTga2(scaler,Timeint,Timeout,TimeCount,center,ALIGN,oldChannel);
			//gEngfuncs.Con_Printf("%f|%f|%f|%f|%f|%i|%i|%i|",scaler,Timeint,Timeout,TimeCount,center,ALIGN,oldChannel);
			oldChannel = -1;
		}
		else if (id2==2)
		{
			if (Channel>999||Channel<0) return -1 ;
			gGN_Hud_Draw->ChannelDraw[Channel].Type = 2;
			float Timein,Timeout,TimeCount;
			char *strDraw = READ_STRING();	
			sprintf(gGN_Hud_Draw->ChannelDraw[Channel].Str,"%s",strDraw);
			
			char *FontDraw = READ_STRING();
			sprintf(gGN_Hud_Draw->ChannelDraw[Channel].Font,"%s",FontDraw);
			//gEngfuncs.Con_Printf("FontName: %s",gGN_Hud_Draw->ChannelDraw[Channel].Font);


			int fontsize =  READ_BYTE();
			gGN_Hud_Draw->ChannelDraw[Channel].sizefont = fontsize;
			int fontBold =  READ_BYTE();
			gGN_Hud_Draw->ChannelDraw[Channel].bold = fontBold;
			int fontBoder =  READ_BYTE();
			gGN_Hud_Draw->ChannelDraw[Channel].boder = fontBoder;

			int r =READ_BYTE();
			gGN_Hud_Draw->ChannelDraw[Channel].r = r;
			int g = READ_BYTE();
			gGN_Hud_Draw->ChannelDraw[Channel].g = g;
			int b = READ_BYTE();
			gGN_Hud_Draw->ChannelDraw[Channel].b = b;
			int a = READ_BYTE();
			gGN_Hud_Draw->ChannelDraw[Channel].a = a;
			gGN_Hud_Draw->ChannelDraw[Channel].alpha = a;
			int x = READ_LONG();
			gGN_Hud_Draw->ChannelDraw[Channel].x =x;
			int y = READ_LONG();
			gGN_Hud_Draw->ChannelDraw[Channel].y= y;
			
			int Align = READ_BYTE();
			gGN_Hud_Draw->ChannelDraw[Channel].center= Align;
			int Alignscr = READ_BYTE();
			int IntTimeCount = READ_LONG();
			TimeCount = IntTimeCount/1000;
			int IntTimeint = READ_LONG();
			Timein = IntTimeint/1000;
			if (Timein> 0.0) gGN_Hud_Draw->ChannelDraw[Channel].Effect  = Effect_Addtive;
			else gGN_Hud_Draw->ChannelDraw[Channel].Effect  = Effect_Nomal;
			int IntTimeout = READ_LONG();

			if (IntTimeout> 0.0) gGN_Hud_Draw->ChannelDraw[Channel].Effect  = Effect_Addtive;
			
			Timeout = IntTimeout/1000;
			gGN_Hud_Draw->SetDrawTga2(1.0, Timein, Timeout, TimeCount, Align, Alignscr,Channel);
	
			//gEngfuncs.Con_Printf("\n gDrawText: %f | %f | %f | %i | %i | %i",Timein, Timeout, TimeCount, Align, Alignscr,Channel);
		}
	}
	else if (id == 3)
	{
	
		int Channel = READ_LONG();
		float scaler = READ_COORD();
		gGN_Hud_Draw->SetScalerDraw(Channel,scaler);
		//gEngfuncs.Con_Printf("\n channel: %i - %f",Channel,scaler);
	}
	else if (id == 4)
	{
		int isDelete = READ_BYTE();
		
		if (isDelete==1)
		{
			int isChannel = READ_LONG();
			gGN_Hud_Draw->RemoveDraw(isChannel);
		}
	}
	else if (id==195)
	{
		
		int g1 = READ_BYTE();
		gDrawRadar = g1;
		int g2 = READ_BYTE();
		gDrawTeamPost = g2;
		
	}
	else if (id==196)
	{
		char *id1 = READ_STRING();
		char gnameVic[64] ;
		sprintf(gnameVic,"%s",id1);
	
		char *id2 = READ_STRING();
		char gnameKill[64];
		sprintf(gnameKill,"%s",id2);
		char *id3 = READ_STRING();
		char nameWpn[64];
		sprintf(nameWpn,"%s",id3);
		int iskill = READ_BYTE();
		int numkill = READ_BYTE();
		int team1 = READ_BYTE();
		int team2 = READ_BYTE();
	//	gEngfuncs.Con_Printf("\n channel: %s - %s - %i ",gnameVic,gnameKill,iskill);

		gGN_Hud_Draw->AddMsgdeath( gnameVic,  gnameKill, iskill, numkill, team1, team2,nameWpn);

	}
	
	else if (id==197)
	{
		int gKillMark,gWall,gSpical,gHEKNIFE, gRevenge,gHead=0;
		int Combo = READ_BYTE();
		
		 gHead = READ_BYTE();
		 gWall = READ_BYTE();
		 gSpical = READ_BYTE();
		 gHEKNIFE = READ_BYTE();
		 gRevenge=  READ_BYTE();
		if (Combo<1) return 1;
		//gEngfuncs.Con_Printf("\n channel: %i - %i - %i - %i - %i",Combo,gHead,gWall,gRevenge,gHEKNIFE);

		gnKillMark->OnDeathMsg(Combo,gHead, gWall,gSpical,gHEKNIFE,gRevenge);
	}
	else if (id==198)
	{
		
		int body = READ_BYTE();
		if (body<0) body = 0;
		if (body>255) body = 0;
		gBodyWeaponView = body;
	}
	else if (id==199)
	{
		int isdraw = READ_BYTE();
		if (isdraw<0) isdraw = 0;
		if (isdraw>255) isdraw = 0;
		gGN_Hud_Draw->gDrawTeamname = isdraw;
	}
	else if (id == 250)
	{
		int idp = READ_BYTE();
		int gis  = READ_BYTE();
		gGN_Hud_Draw->VPlayer[idp].die = gis;
		//gEngfuncs.Con_Printf("\n call Message : %i - %i",idp,gis);
		
	}
	else if (id == 251)
	{
		int idp = READ_BYTE();
		int stt  = READ_BYTE();
		int score = READ_LONG();
		int death = READ_LONG();
		if (stt<32)
		gScoreAllPlayer[2][idp] = stt;

		gScoreAllPlayer[0][idp] = score;
		gScoreAllPlayer[1][idp]= death;
	
	}
	else if (id == 252)
	{
		int id1 = READ_BYTE();
		int id2 = READ_BYTE();
		int id3 = READ_BYTE();
		int id4 = READ_BYTE();
		int id5 = READ_BYTE();
		int id6 = READ_BYTE();
		int id7 = READ_BYTE();
		int id8 = READ_BYTE();
		int data1 = READ_BYTE();
		gScoreAllPlayer[2][id1]=data1;
		int data2 = READ_BYTE();
		gScoreAllPlayer[2][id2]=data2;
		int data3 = READ_BYTE();
		gScoreAllPlayer[2][id3]=data3;
		int data4 = READ_BYTE();
		gScoreAllPlayer[2][id4]=data4;
		int data5 = READ_BYTE();
		gScoreAllPlayer[2][id5]=data5;
		int data6 = READ_BYTE();
		gScoreAllPlayer[2][id6]=data6;
		int data7 = READ_BYTE();
		gScoreAllPlayer[2][id7]=data7;
		int data8 = READ_BYTE();
		gScoreAllPlayer[2][id8]=data8;
	}
	else if (id == 253 )
	{
		int genable = READ_BYTE();
		if (genable==1)
		gGN_Hud_Draw->genableTabscore  = true;
		else 
		gGN_Hud_Draw->genableTabscore  = false;
	}
	else if (id == 254)
	{
		int id1 = READ_BYTE();
		int id2 = READ_BYTE();
		int id3 = READ_BYTE();
		gPlayerAce[0]=id1;
		gPlayerAce[1]=id2;
		gPlayerAce[2]=id3;
	}
	else if (id == 255 )
	{
		int idp = READ_BYTE();
		gGN_Hud_Draw->VPlayer[idp].Team = 0;
	}
	else if (id == 256)
	{
		SetCrossColor(255,0,0);
		TimeReturnColor = gEngfuncs.GetClientTime();
	}
	else if (id == 257)
	{
		int id1 = READ_BYTE();
		if (id1==1) isEnableCross = true;
		else if (id1==0) isEnableCross = false;
	}
	else if (id == 258)
	{
		float p1 = READ_COORD(); PosBompP[0][0]  = p1;
		float p2 = READ_COORD(); PosBompP[0][1]  = p2;
		float p3 = READ_COORD(); PosBompP[0][2]  = p3;

		float p11 = READ_COORD(); PosBompP[1][0]  = p11;
		float p22 = READ_COORD(); PosBompP[1][1]  = p22;
		float p33 = READ_COORD(); PosBompP[1][2]  = p33;

	}
	return 1; 
}

int HUD_Redraw(float time, int intermission)
{
	
	gEngfuncs.pfnHookUserMsg("Printf",FuncGiayNhapModule);
	if(g_pOverview->CanDraw()) g_pOverview->Draw();
	gnKillMark->Redraw();
	
	int iResult = gExportfuncs.HUD_Redraw(time, intermission);
	//gGNDrawText->Redraw();
	gGN_Hud_Draw->DrawAllHud();

	//ptexture + sizeof(mstudiotexture_t *) ;


	return iResult;
}

int (__fastcall *g_pfnCHudAmmo_DrawCrosshair)(void *, int, float flTime, int weaponid) = (int (__fastcall *)(void *, int, float, int))0x1940430;

int __fastcall CHudAmmo_DrawCrosshair(void *pthis, int, float flTime, int weaponid)
{
	return 1;
}

int (__fastcall *g_pfnHudSniperScope_Draw)(void *, int,float flTime) = (int (__fastcall *)(void *,int,float))0x1961AD0;
int __fastcall HudSniperScope_Draw(void *p,int i,float f)
{
	return 1;
};


void(*g_pfnEV_HLDM_DecalGunshot)(pmtrace_t *pTrace, int iBulletType, float scale, int r, int g, int b, bool bCreateSparks, char cTextureType) = (void(*)(pmtrace_t *, int, float, int, int, int, bool, char))0x19020B0;
void(*g_pfnEV_HLDM_FireBullets)(int idx, float *forward, float *right, float *up, int cShots, float *vecSrc, float *vecDirShooting, float *vecSpread, float flDistance, int iBulletType, int iTracerFreq, int *tracerCount, int iPenetration) = (void(*)(int, float *, float *, float *, int, float *, float *, float *, float, int, int, int *, int))0x1902460;

void EV_HLDM_DecalGunshot(pmtrace_t *pTrace, int iBulletType, float scale, int r, int g, int b, bool bCreateSparks, char cTextureType);
void EV_HLDM_FireBullets(int idx, float *forward, float *right, float *up, int cShots, float *vecSrc, float *vecDirShooting, float *vecSpread, float flDistance, int iBulletType, int iTracerFreq, int *tracerCount, int iPenetration);


typedef enum
{
	BULLET_NONE = 0,
	BULLET_PLAYER_9MM,
	BULLET_PLAYER_MP5,
	BULLET_PLAYER_357,
	BULLET_PLAYER_BUCKSHOT,
	BULLET_PLAYER_CROWBAR,

	BULLET_MONSTER_9MM,
	BULLET_MONSTER_MP5,
	BULLET_MONSTER_12MM,

	BULLET_PLAYER_45ACP,
	BULLET_PLAYER_338MAG,
	BULLET_PLAYER_762MM,
	BULLET_PLAYER_556MM,
	BULLET_PLAYER_50AE,
	BULLET_PLAYER_57MM,
	BULLET_PLAYER_357SIG
}
Bullet;

void EV_HLDM_DecalGunshot(pmtrace_t *ptr, int iBulletType, float scale, int r, int g, int b, bool bCreateSparks, char cTextureType)
{
	physent_t *pe;
	pe = gEngfuncs.pEventAPI->EV_GetPhysent(ptr->ent);

	if (!pe->iuser1&&pe->solid != SOLID_BSP)
	{
		
		SetCrossColor(255,0,0);
		TimeReturnColor = gEngfuncs.GetClientTime();
	}
}

void EV_HLDM_FireBullets(int idx, float *forward, float *right, float *up, int cShots, float *vecSrc, float *vecDirShooting, float *vecSpread, float flDistance, int iBulletType, int iTracerFreq, int *tracerCount, int iPenetration)
{
	pmtrace_t tr;

	for (int iShot = 1; iShot <= cShots; iShot++)
	{

		vec3_t vecDir, vecEnd;
		float x, y, z;

		if (iBulletType == BULLET_PLAYER_BUCKSHOT)
		{
			do
			{
				x = gEngfuncs.pfnRandomFloat(-0.5, 0.5) + gEngfuncs.pfnRandomFloat(-0.5, 0.5);
				y = gEngfuncs.pfnRandomFloat(-0.5, 0.5) + gEngfuncs.pfnRandomFloat(-0.5, 0.5);
				z = x * x + y * y;
			} while (z > 1);

			for (int i = 0; i < 3; i++)
			{
				vecDir[i] = vecDirShooting[i] + x * vecSpread[0] * right[i] + y * vecSpread[1] * up[i];
				vecEnd[i] = vecSrc[i] + flDistance * vecDir[i];
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				vecDir[i] = vecDirShooting[i] + vecSpread[0] * right[i] + vecSpread[1] * up[i];
				vecEnd[i] = vecSrc[i] + flDistance * vecDir[i];
			}
		}

		
		gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction(false, false);
		gEngfuncs.pEventAPI->EV_PushPMStates();
		gEngfuncs.pEventAPI->EV_SetSolidPlayers(idx - 1);
		gEngfuncs.pEventAPI->EV_SetTraceHull(2);
		gEngfuncs.pEventAPI->EV_PlayerTrace(vecSrc, vecEnd, PM_STUDIO_IGNORE, -1, &tr);
		gEngfuncs.pEventAPI->EV_PopPMStates();
	
		if (tr.fraction == 1.0)
			continue;

		if (gEngfuncs.PM_PointContents(tr.endpos, NULL) != CONTENTS_WATER)
			continue;
		
	}

	g_pfnEV_HLDM_FireBullets(idx, forward, right, up, cShots, vecSrc, vecDirShooting, vecSpread, flDistance, iBulletType, iTracerFreq, tracerCount, iPenetration);

}

void HUD_Init(void)
{	
	//GetSprite()
	g_pOverview = new COverview;
	gnKillMark = new KillMark;
	gGNDrawText = new GNDrawText;
	
	gGN_Hud_Draw =new 	GN_Hud_Draw;	
	Gdiplus::GdiplusStartup (&gdipToken, &gdipSIn, NULL);

	g_pMetaHookAPI->InlineHook(g_pfnCHudAmmo_DrawCrosshair, CHudAmmo_DrawCrosshair, (void *&)g_pfnCHudAmmo_DrawCrosshair);
	g_pMetaHookAPI->InlineHook(g_pfnHudSniperScope_Draw, HudSniperScope_Draw, (void *&)g_pfnHudSniperScope_Draw);

	if (g_dwEngineBuildnum >= 5971)
	{
		DWORD dwClientBase = g_pMetaHookAPI->GetModuleBase(GetModuleHandle("client.dll"));
		g_pfnEV_HLDM_DecalGunshot = (void(*)(pmtrace_t *, int, float, int, int, int, bool, char))(dwClientBase + 0x20B0);
		g_pfnEV_HLDM_FireBullets = (void(*)(int, float *, float *, float *, int, float *, float *, float *, float, int, int, int *, int))(dwClientBase + 0x2460);
	
	}

g_pMetaHookAPI->InlineHook(g_pfnEV_HLDM_DecalGunshot, EV_HLDM_DecalGunshot, (void *&)g_pfnEV_HLDM_DecalGunshot);
	g_pMetaHookAPI->InlineHook(g_pfnEV_HLDM_FireBullets, EV_HLDM_FireBullets, (void *&)g_pfnEV_HLDM_FireBullets);


	return gExportfuncs.HUD_Init();
	//return 1;
}


void HUD_DrawNormalTriangles(void)
{
	cl_entity_t *pEnt = gEngfuncs.GetEntityByIndex(1);

/*
	for (int j = 0; j < 1024; j++)
	{
		cl_entity_t *pEnt = gEngfuncs.GetEntityByIndex(j);
		
		if (pEnt)
		{
			if (pEnt->curstate.skin == CONTENTS_WATER)
			{
				pEnt->baseline.effects = EF_NODRAW;
				pEnt->baseline.renderamt = 0;
				pEnt->baseline.rendermode = kRenderTransAdd;

				pEnt->prevstate.effects = EF_NODRAW;
				pEnt->prevstate.renderamt = 0;
				pEnt->prevstate.rendermode = kRenderTransAdd;

				pEnt->curstate.effects = EF_NODRAW;
				pEnt->curstate.renderamt = 0;
				pEnt->curstate.rendermode = kRenderTransAdd;
				
			}
		}
	}*/
	return gExportfuncs.HUD_DrawNormalTriangles();
}
void V_CalcRefdef(struct ref_params_s *pParams)
{
	
	VectorCopy(pParams->viewangles,mainViewAngles);
	VectorCopy(pParams->vieworg,mainViewOrigin);
	g_pOverview->CalcRefdef(pParams);
	gnKillMark->LoadTextures();
	gGN_Hud_Draw->LoadImageA();



	return gExportfuncs.V_CalcRefdef(pParams);
}

void int_gHud(void);
int HUD_VidInit(void)
{
	//if (!g_iVideoMode) exit(1);
	
	gScreenInfo.iSize = sizeof(gScreenInfo);
	gEngfuncs.pfnGetScreenInfo(&gScreenInfo);
	displayCenterX=gScreenInfo.iWidth/2;
	displayCenterY=gScreenInfo.iHeight/2;
	
	//gSprite();
	//gEngfuncs.pfnConsolePrint("\n\n====>RESET \n\n");
	ResetHud();
	int_gHud();
	return gExportfuncs.HUD_VidInit();
}
void CL_CameraOffset( float *ofs )
{
	
	return gExportfuncs.CL_CameraOffset(ofs);
}
int HUD_AddEntity(int iType, struct cl_entity_s *pEntity, const char *pszModel)
{
	if(pszModel && strstr(pszModel,"backpack"))
	{
		g_iBombEntity = pEntity->index;
		//g_fBombEntityTime = g_flTime + 0.5f;
	}
	return gExportfuncs.HUD_AddEntity(iType, pEntity, pszModel);
}
int gKeyEn(int keynum)
{
if(	keynum!=132&&keynum!=133&&keynum!=134&&keynum!=239&&keynum!=240&&keynum!=241&&keynum!=242&&keynum!=243&&keynum!=244&&keynum!=245&&keynum!=9&&keynum!=144&&keynum!=255
	&&keynum!=175&&keynum!=148) return 1;
return 0;
}
int gEditkey(int keynum,int shift)
{
	if (shift==1)
	{
	if (keynum==48) return 41; //0
	if (keynum==49) return 33;//1
	if (keynum==50) return 64;//2
	if (keynum==51) return 35;//3
	if (keynum==52) return 36;//4
	if (keynum==53) return 37;//5
	if (keynum==54) return 94;//6
	if (keynum==55) return 39;//7
	if (keynum==56) return 42;//8
	if (keynum==57) return 40;//9
	if (keynum==96) return 126;//`
	if (keynum==45) return 95;//-
	if (keynum==61) return 43;//=
	if (keynum==44) return 60;//,
	if (keynum==46) return 62;//.
	if (keynum==59) return 58;//;
	if (keynum==34) return 44;//'
	if (keynum==47) return 63;///
	if (keynum==92) return 124;///n
	}
	else
	{
		if (keynum==170) return 48;//0
		if (keynum==166) return 49;//1
		if (keynum==167) return 50;//2
		if (keynum==168) return 51;//3
		if (keynum==163) return 52;//4
		if (keynum==164) return 53;//5
		if (keynum==165) return 54;//6
		if (keynum==160) return 55;//7
		if (keynum==161) return 56;//8
		if (keynum==162) return 57;//9
		if (keynum==174) return 43;//+
		
		if (keynum==172) return 47;///
		if (keynum==171) return 46;//.
		if (keynum==173) return 95;//-
	}
	return 0;
}
int HUD_Key_Event(int eventcode, int keynum, const char* pszCurrentBinding)
{
	
	if (isModelButtonSetting== true )
	{
		if (keynum==96||keynum==27|| keynum == 127 || keynum == 13|| keynum == 169|| keynum == 132|| keynum == 0241) return 0 ;
		
		
		char gsz[32];
		memset(gsz,0,32);
	//	char gchar = (char)keynum;
		if (keynum==32)
		sprintf(gsz,"SPACE");
		else if (133==keynum)
		
		sprintf(gsz,"CTRL");
		else if (keynum == 134)
		sprintf(gsz,"SHIFT");
		else if (keynum==175)
		sprintf(gsz,"CAPSLOCK");	
		else if (keynum==166)
		sprintf(gsz,"KP_END");	
		else if (keynum==167)
		sprintf(gsz,"KP_DOWNARROW");	
		else if (keynum==168)
		sprintf(gsz,"KP_PGDN");
		else if (keynum==163)
		sprintf(gsz,"KP_LEFTARROW");
		else if (keynum==164)
		sprintf(gsz,"KP_5");
		else if (keynum==165)
		sprintf(gsz,"KP_RIGHTARROW");
		else if (keynum==160)
		sprintf(gsz,"KP_HOME");
		else if (keynum==161)
		sprintf(gsz,"KP_UPARROW");
		else if (keynum==169)
		sprintf(gsz,"KP_PGUP");
		else if (keynum==174)
		sprintf(gsz,"KP_PLUS");
		else if (keynum==172)
		sprintf(gsz,"KP_SLASH");
		else if (keynum==42)
		sprintf(gsz,"*");
		else if (keynum==171)
		sprintf(gsz,"KP_DEL");
		else if (keynum==152)
		sprintf(gsz,"END");
		else if (keynum==151)
		sprintf(gsz,"HOME");
		else if (keynum==149)
		sprintf(gsz,"PGDN");
		else if (keynum==150)
		sprintf(gsz,"PGUP");
		else if (keynum==147)
		sprintf(gsz,"INS");
		else if (keynum==135)
		sprintf(gsz,"F1");
		else if (keynum==136)
		sprintf(gsz,"F2");
		else if (keynum==137)
		sprintf(gsz,"F3");
		else if (keynum==138)
		sprintf(gsz,"F4");
		else if (keynum==139)
		sprintf(gsz,"F5");
		else if (keynum==140)
		sprintf(gsz,"F6");
		else if (keynum==141)
		sprintf(gsz,"F7");
		else if (keynum==142)
		sprintf(gsz,"F8");
		else if (keynum==143)
		sprintf(gsz,"F9");
		else if (keynum==144)
		sprintf(gsz,"F10");
		else if (keynum==145)
		sprintf(gsz,"F11");
		else if (keynum==146)
		sprintf(gsz,"F12");
		else if (keynum==170)
		sprintf(gsz,"KP_INS");
		else if (keynum==128)
		sprintf(gsz,"UPARROW");
		else if (keynum==129)
		sprintf(gsz,"DOWNARROW");
		else if (keynum==131)
		sprintf(gsz,"RIGHTARROW");
		else if (keynum==130)
		sprintf(gsz,"LEFTARROW");
		else if (keynum==61)
		sprintf(gsz,"=");
		else if (keynum==44)
		sprintf(gsz,",");
		else
		{
			if ( (keynum>=97&&keynum<=122)||(keynum>=45&&keynum<=59)||(keynum>90&&keynum<95)||(keynum=39))
			{
				gsz[0] = keynum; 
			}
			else return 0;
		}

		
		for (int i= 0;i<19;i++)
			if (strcmp(gDatakey_Config[i],gsz)==0)
			{memset(gDatakey_Config[i],0,32);gDatakey_Config[i][0]=0;}


		memset(gDatakey_Config[gButtonControlSel],0,32);
		strcpy(gDatakey_Config[gButtonControlSel],gsz);

		gButtonControlSel = 0;

		isModelButtonSetting = false;
		
		//gEngfuncs.Con_Printf("\n-> %s",gsz);
		return 0;

	}

	if (isMenuDraw>0) return 0;
	if (keynum==9)
	{
		if (gGN_Hud_Draw->genableTabscore==true)
		if (eventcode==1)
		{
			if (gGN_Hud_Draw->ScoreTabisShow!=1)
			{
			
				gGN_Hud_Draw->ScoreTabisShow =1;
				gGN_Hud_Draw->ScoreTabTimeStartDraw = gEngfuncs.GetClientTime();
			}
		}
		else if (eventcode==0)
		{
			gGN_Hud_Draw->ScoreTabTimeStartDraw = gEngfuncs.GetClientTime();
			gGN_Hud_Draw->ScoreTabisShow =0;
		}
		return 0;
	}

	if (keynum==13||keynum==169){ 
		
		if (eventcode==0)
		{
			if (gGN_Hud_Draw->ChatInBoxDraw!=1) 
			{
				memset(gGN_Hud_Draw->gStringChat,0,1024);
				gGN_Hud_Draw->gStringChat[0]=0;


				gGN_Hud_Draw->ChatInBoxDraw = 1;
				gGN_Hud_Draw->gStringUpperChat=0;
			}
			else{
					char szText[1024];
					if (gGN_Hud_Draw->gPublicChat!=1)
					sprintf(szText,"say %s",gGN_Hud_Draw->gStringChat);
					else
					sprintf(szText,"say_team %s",gGN_Hud_Draw->gStringChat);
					sprintf(gGN_Hud_Draw->texyOldChat,"%s",gGN_Hud_Draw->gStringChat);
					gGN_Hud_Draw->ChatInBoxDraw = 0;
					gEngfuncs.pfnClientCmd(szText);
					
				}
		}

	return 0;
	}
	if (keynum==134)
	{
		if (eventcode==1)
		{
			gGN_Hud_Draw->gStringUpperChat=1;
			return 0;
		}else{
			gGN_Hud_Draw->gStringUpperChat=0;return 0;}
	}
	if (gGN_Hud_Draw->ChatInBoxDraw==1)
	{
		
		if (eventcode==1)
		{	
				gEngfuncs.Con_Printf("%i",keynum);
			if (keynum==136) {gGN_Hud_Draw->gPublicChat=0;return 0;}
			if (keynum==137) {gGN_Hud_Draw->gPublicChat=1;return 0;}
			if (keynum>=135&&keynum<=146) return gExportfuncs.HUD_Key_Event(eventcode,keynum,pszCurrentBinding);
			if (keynum==134)
			{
				gGN_Hud_Draw->gStringUpperChat=1;
				return 0;
			}
			if (keynum==127)
			{
				//gGN_Hud_Draw->gStringChat = gGN_Hud_Draw->gStringChat.substr( 0,gGN_Hud_Draw->gStringChat.length()-1);
				gGN_Hud_Draw->gStringChat[strlen(gGN_Hud_Draw->gStringChat)-1]=0;
				return 0;
			}
			if (keynum==27)
			{
				gGN_Hud_Draw->ChatInBoxDraw = 0;
				return 0;
			}
			if (keynum==128)
			{
				if (strlen(gGN_Hud_Draw->texyOldChat)>=1 && gGN_Hud_Draw->texyOldChat )
				memset(gGN_Hud_Draw->gStringChat,0,1024);
				strcpy(gGN_Hud_Draw->gStringChat,gGN_Hud_Draw->texyOldChat);
			//	gGN_Hud_Draw->gStringChat = std::string(gGN_Hud_Draw->texyOldChat);
				gGN_Hud_Draw->gShowSChat=1;
				gGN_Hud_Draw->TimeCountChat=1.0;
			}
			if (keynum>=128&&keynum<=131) return 0;
			if ( gKeyEn( keynum)==1)
			{
				char gchar;
				
				int gKey2 = gEditkey(keynum,gGN_Hud_Draw->gStringUpperChat);
				if (gKey2!=0)
					gchar=(char)gKey2;
				else
				{
				if (gGN_Hud_Draw->gStringUpperChat!=1)
				 gchar = (char)keynum;
				else
				 gchar= toupper((char)keynum);
				}
				gGN_Hud_Draw->TimeDrawSChat = gEngfuncs.GetClientTime();
				gGN_Hud_Draw->gShowSChat=1;
				gGN_Hud_Draw->TimeCountChat=1.0;
				//gGN_Hud_Draw->gStringUpperChat=0;
				//gEngfuncs.Con_Printf(&gchar);
				//std::string gcharStr(&gchar);
				// =gGN_Hud_Draw->gStringChat +  gcharStr.substr(0,1);
				gGN_Hud_Draw->gStringChat[strlen(gGN_Hud_Draw->gStringChat)] = gchar;
			}
		}
		else if (keynum=107)
		{

			return 0;
		}
		
		return 0;
	}
	return gExportfuncs.HUD_Key_Event(eventcode,keynum,pszCurrentBinding);
}


