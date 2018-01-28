#include "Base.h"
#include "KillMark.h"
#include "parsemsg.h"
#include "HudDraw.h"
pfnUserMsgHook pfnDeathMsgHandler;
static pfnUserMsgHook HealthOrg=NULL;
static pfnUserMsgHook BatteryOrg=NULL;
static pfnUserMsgHook CurWeaponOrg=NULL;
static pfnUserMsgHook AmmoOrg=NULL;
static pfnUserMsgHook AmmoXOrg=NULL;
static pfnUserMsgHook WeaponListOrg=NULL;
static pfnUserMsgHook gMsgChat=NULL;
static pfnUserMsgHook gMsgRadio=NULL;
static pfnUserMsgHook SetFOVOrg=NULL;
static pfnUserMsgHook pmTeamInfo=NULL;
static pfnUserMsgHook pmShowMenu=NULL;
static pfnUserMsgHook  pmResetHUD = NULL;
static pfnUserMsgHook  bompDrop = NULL;
static pfnUserMsgHook bomppickup = NULL;
static pfnUserMsgHook pmScoreAttrib = NULL;
static pfnUserMsgHook MsgDamage = NULL;

void (*pCommandTakeDamage);
extern int FuncGiayNhapModule(const char* pszName, int iSize, void* pbuf);
int EngFunc_AddCommand(char *szCmds, void (*pFunction)(void))
{

	
	
	return gEngfuncs.pfnAddCommand(szCmds, pFunction);
}

int MsgFunc_Damage(const char *pszName, int iSize, void *pbuf)
{

		return pmResetHUD(pszName, iSize, pbuf);
}
int MsgFunc_ResetHUD(const char *pszName, int iSize, void *pbuf)
{	
//	SendScrSize();


	//gnKillMark = new KillMark;
	return pmResetHUD(pszName, iSize, pbuf);
}
void SendScrSize()
{
	//MSG_BeginWrite("Printf");
	//MSG_WriteByte(200);
	//MSG_WriteLong(gGN_Hud_Draw->gScreenInfo.iWidth);
	//MSG_WriteLong(gGN_Hud_Draw->gScreenInfo.iHeight);
	//MSG_EndWrite();
}

int	Engfunc_ClientCmd( char *szCmdString )
{	
	if(strstr(szCmdString,"menuselect"))
	{
		gGN_Hud_Draw->g_Menu_Drawing = 0;
	}
	else if (strstr(szCmdString,"menuselect"))
	{

	}

	

	return gEngfuncs.pfnClientCmd(szCmdString);
}


struct model_s *Engfunc_LoadMapSprite(const char *pszPath)
{
	return g_pOverview->m_pSprite = gEngfuncs.LoadMapSprite(pszPath);
}
byte *Engfunc_COM_LoadFile(char *pszPath, int iUseHunk, int *piLength)
{
	byte *pbResult = gEngfuncs.COM_LoadFile(pszPath, iUseHunk, piLength);

	if (!pbResult)
		return NULL;

	if (g_pOverview && !strncmp(pszPath, "overviews/", 10))
		g_pOverview->m_bCanCheck = true;

	return pbResult;
}
char *Engfunc_COM_ParseFile(char *pszData, char *pszToken)
{
	static char szToken[1024];
	char *pszFile, *pszResult = gEngfuncs.COM_ParseFile(pszData, pszToken);

	if (!g_pOverview || !g_pOverview->m_bCanCheck)
		return pszResult;

	if (!_stricmp(pszToken, "zoom"))
	{
		gEngfuncs.COM_ParseFile(pszResult, szToken);
		g_pOverview->m_flZoom = (float)atof(szToken);
	}
	else if (!_stricmp(pszToken, "origin"))
	{
		pszFile = gEngfuncs.COM_ParseFile(pszResult, szToken);
		g_pOverview->m_vecOrigin.x = (float)atof(szToken);
		pszFile = gEngfuncs.COM_ParseFile(pszFile, szToken);
		g_pOverview->m_vecOrigin.y = (float)atof(szToken);
	}
	else if (!_stricmp(pszToken, "rotated"))
	{
		gEngfuncs.COM_ParseFile(pszResult, szToken);
		g_pOverview->m_bIsRotate = atoi(szToken) != FALSE;
	}
	else if (!_stricmp(pszToken, "height"))
	{	
		pszFile = gEngfuncs.COM_ParseFile(pszResult, szToken);
		g_pOverview->m_iHeight= (float)atof(szToken);
	}

	return pszResult;
}
void Engfunc_COM_FreeFile(void *pBuffer)
{
	if (g_pOverview)
		g_pOverview->m_bCanCheck = false;

	gEngfuncs.COM_FreeFile(pBuffer);
}
//int (*pfnDeathMsgHandler)(const char *,int,void *);
int MsgDeathMsg_Handler(const char *szMsgName,int iSize,void *pBuf)
{
	BEGIN_READ(pBuf,iSize);

	int iKiller = READ_BYTE();
	int iVictim = READ_BYTE();
	int isIcon = READ_BYTE();
	char szWpnName[2048];
	sprintf(szWpnName, "d_%s",READ_STRING());

	hud_player_info_t hPlayer;
	gEngfuncs.pfnGetPlayerInfo(iKiller,&hPlayer);

	char *killer_name = hPlayer.name;
	if ( !killer_name )
		killer_name = "";
	gEngfuncs.pfnGetPlayerInfo(iVictim,&hPlayer);
	char *gnameVic = hPlayer.name;

	if ( !gnameVic )
		gnameVic = "";
	int iskill=1,ishead=0;
	int gWall;
	iskill = (int)(isIcon/10);
	ishead = (int)isIcon - iskill*10 ;
	char buffer [128];
	sprintf(buffer, "%d|%d",iskill, ishead);

	// MessageBox(0,"",buffer,0);
	
	gGN_Hud_Draw->AddMsgdeath( gnameVic,  killer_name,ishead , iskill, gGN_Hud_Draw->VPlayer[iVictim].Team, gGN_Hud_Draw->VPlayer[iKiller].Team,szWpnName);
	if (ishead == 3||ishead == 4||ishead == 5)
	{
		//ishead-=3;
		gWall=1;
	}
	gnKillMark->OnDeathMsg(iskill,ishead, gWall,0,0,0);
	
	return 0;
}

static int Health(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ( pbuf, iSize );
	gGN_Hud_Draw->gUserdata.hp = READ_BYTE();
	return (*HealthOrg)(pszName, iSize, pbuf);
}

static int Battery(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ( pbuf, iSize );
	gGN_Hud_Draw->gUserdata.acc = READ_BYTE();
	return (*BatteryOrg)(pszName, iSize, pbuf);
}

static int CurWeapon(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ( pbuf, iSize );

	int iState = READ_BYTE();
	int iID    = READ_CHAR();
	int iClip  = READ_CHAR();

	if(iState)
	{
	   gGN_Hud_Draw->gUserdata.clip = iClip;
	   gGN_Hud_Draw->gUserdata.weaponid=iID;
	}
	

	return (*CurWeaponOrg)(pszName, iSize, pbuf);
}

int AmmoX(const char *pszName, int iSize, void *pbuf ) 
{
    int retval;
	int ID, Count;
	BEGIN_READ(pbuf, iSize);
	ID = READ_BYTE();
	Count = READ_BYTE();
	if (Count <= 0)
	{
		BEGIN_READ(pbuf, iSize);
		ID = READ_BYTE();
		Count = READ_BYTE();
	}

	gGN_Hud_Draw->gUserdata.ammo[ ID] =Count;
	retval = (*AmmoXOrg)(pszName,iSize,pbuf); 
    return retval;
}


int Ammo (const char *pszName, int iSize, void *pbuf )
{
	int retval;
	gGN_Hud_Draw->gUserdata.ammo[ ((char*)pbuf)[0] ] += ((char*)pbuf)[1];
	retval = (*AmmoOrg)(pszName,iSize,pbuf);
	return retval;
}

static int WeaponList(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ( pbuf, iSize );
	char* name = READ_STRING();
	//if(strstr(name,"weapon_")==name) { name += 7;} 
	//strcpy( Weapon.szName, name );
	gGN_Hud_Draw->gUserdata.iAmmoType = (int)READ_CHAR();	
	


	return (*WeaponListOrg)(pszName, iSize, pbuf);
}
static int MSGRadio(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ( pbuf, iSize );
	
	

	return (*gMsgRadio)(pszName, iSize, pbuf);
}

static int MSGChat(const char* pszName, int iSize, void* pbuf)
{
	int r,g,b;
	BEGIN_READ( pbuf, iSize );
	int IDPlayer = READ_BYTE();

	
	hud_player_info_t PlayerInf;
	gEngfuncs.pfnGetPlayerInfo(IDPlayer,&PlayerInf);
	
	const char* StrMSG = READ_STRING();
	if (strcmp(StrMSG,"#Cstrike_Chat_All"))
	{
		r=37;
		g=120;
		b=164;
	}
	else
	{
		r=193;
		g=181;
		b=159;
	}
	char* StrChat = READ_STRING();
	char* StrChat3 = READ_STRING();
	//gEngfuncs.Con_Printf(" %s",StrChat3);
	char szText[512];

	sprintf(szText,"%s: %s",PlayerInf.name,StrChat3);
	
	gGN_Hud_Draw->AddChatMSg(szText, r, g, b);
	
	return  1;//(*gMsgChat)(pszName, iSize, pbuf);

}


static int SetFOV(const char* pszName, int iSize, void* pbuf)
{	
	BEGIN_READ( pbuf, iSize );
	int iFOV = READ_BYTE();
	if(!iFOV)   { iFOV=90; }
	if (iFOV!=90)
	
	isShowCross = 0;
	
	else
	{
		if (!isShowCross)
		isShowCross = 1;
	}
	//if(iFOV==90){ me.inZoomMode=false; }
	//else           { me.inZoomMode=true;  } 
	//if(cvar.antizoom && me.iFOV!=90 )
	//{ 
	//	me.iFOV = 89; 
	//	((unsigned char*)pbuf)[0]=89;	
	//}
	fCurrentFOV = iFOV;

	return (*SetFOVOrg)(pszName, iSize, pbuf);
}
int MsgFunc_TeamInfo(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf,iSize);
	int px = READ_BYTE();
	char * teamtext = READ_STRING();
	if(!*teamtext) {gGN_Hud_Draw->VPlayer[px].Team = 0; }
	else
	{
		if(strstr(teamtext,"CT"))
		{
			gGN_Hud_Draw->VPlayer[px].Team = 2;
		}
		else if(strstr(teamtext,"TERRORIST"))
		{
			gGN_Hud_Draw->VPlayer[px].Team = 1;
		}
		else
		{
			gGN_Hud_Draw->VPlayer[px].Team = 0;
		}


	}
	gEngfuncs.pfnGetPlayerInfo(px,&gGN_Hud_Draw->VPlayer[px].gINFent);
	return pmTeamInfo(pszName, iSize, pbuf);
}

char *BufferedLocaliseTextString(const char *msg);
int MsgFunc_ShowMenu(const char *pszName, int iSize, void *pbuf)
{
		BEGIN_READ(pbuf, iSize);
	 gGN_Hud_Draw->g_Menu_Drawing = READ_SHORT();
	READ_CHAR();
	int NeedMore = READ_BYTE();

	if (!gGN_Hud_Draw->g_Menu_Wait)
	{
		memset(gGN_Hud_Draw->g_Menu_text,0,1024);
		strncpy(gGN_Hud_Draw->g_Menu_text, READ_STRING(), 1024);
		
	}
	else
	{
		strncat(gGN_Hud_Draw->g_Menu_text, READ_STRING(), 1024 - strlen(gGN_Hud_Draw->g_Menu_text));

	
	}
	gGN_Hud_Draw->g_Menu_Wait =NeedMore; 
	if(gGN_Hud_Draw->g_Menu_text[0] == '#')
	{
		char buffer[1024];
		memset(buffer,0,1024);
		strncpy(buffer, BufferedLocaliseTextString(gGN_Hud_Draw->g_Menu_text), 1024);
		memset(gGN_Hud_Draw->g_Menu_text,0,1024);
		strncpy(gGN_Hud_Draw->g_Menu_text, buffer,1024);
		gGN_Hud_Draw->g_Menu_text[strlen(gGN_Hud_Draw->g_Menu_text)] = '\n';
	}
	gGN_Hud_Draw->g_Menu_text[1023] = 0;
	//gEngfuncs.pfnConsolePrint(gGN_Hud_Draw->g_Menu_text);
	return  pmShowMenu(pszName, iSize, pbuf);
}
inline client_textmessage_t *TextMessageGet(const char *pName) { return gEngfuncs.pfnTextMessageGet(pName); }

void Engfunc_WeaponAnim( int sequence, int body )
{
	//gEngfuncs.pfnConsolePrint("aNim \n");
	return gEventAPI.EV_WeaponAnimation(sequence,body);
}

int MsgFunc_bompdrop(const char *pszName, int iSize, void *pbuf)
{
	gBompDrop=1;
	BEGIN_READ(pbuf, iSize);
	float bompx =READ_COORD();
	BompPost[0] = bompx;
	float bompy =READ_COORD();
	BompPost[1]=bompy;
	float bompz=READ_COORD();
	BompPost[2]=bompz;
	int byte = READ_BYTE();
	if (byte==1)
	isBompStatus= 1;
	else
	isBompStatus = 0;
	BompIdPlayer = 0;
	return bompDrop(pszName, iSize, pbuf);
}
int MsgFunc_bomppickup(const char *pszName, int iSize, void *pbuf)
{
	gBompDrop=0;
	return bomppickup(pszName, iSize, pbuf);
}

char *LocaliseTextString(const char *msg, char *dst_buffer, int buffer_size)
{
	char *dst = dst_buffer;

	for (char *src = (char*)msg; *src != 0 && buffer_size > 0; buffer_size--)
	{
		if (*src == '#')
		{
			static char word_buf[255];
			char *wdst = word_buf, *word_start = src;

			for (++src ; (*src >= 'A' && *src <= 'z') || (*src >= '0' && *src <= '9'); wdst++, src++)
				*wdst = *src;

			*wdst = 0;

			client_textmessage_t *clmsg = TextMessageGet(word_buf);

			if (!clmsg || !(clmsg->pMessage))
			{
				src = word_start;
				*dst = *src;
				dst++, src++;
				continue;
			}

			for (char *wsrc = (char*)clmsg->pMessage; *wsrc != 0; wsrc++, dst++)
				*dst = *wsrc;

			*dst = 0;
		}
		else
		{
			*dst = *src;
			dst++, src++;
			*dst = 0;
		}
	}

	dst_buffer[buffer_size - 1] = 0;
	return dst_buffer;
}

char *BufferedLocaliseTextString(const char *msg)
{
	static char dst_buffer[1024];
	
	LocaliseTextString(msg, dst_buffer, 1024);
	return dst_buffer;
}
int MsgFunc_ScoreAttrib(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);
	int iPlayerID = READ_BYTE();
	int iFlags = READ_BYTE();
	
	if (iFlags & SCOREATTRIB_DEAD)
		gGN_Hud_Draw->VPlayer[iPlayerID].die = 1;
	else 
		gGN_Hud_Draw->VPlayer[iPlayerID].die = 0;
	if (iFlags & SCOREATTRIB_BOMB)
	BompIdPlayer =iPlayerID ;
	return pmScoreAttrib(pszName, iSize, pbuf);
}

	int EngFunc_HookUserMsg(char *szMsgName, pfnUserMsgHook pfn)
	{
			gEngfuncs.Con_Printf("%s \n",szMsgName);
		if (!strcmp(szMsgName, "DeathMsg"))
		{
			pfnDeathMsgHandler = pfn;
			gEngfuncs.pfnHookUserMsg(szMsgName, MsgDeathMsg_Handler);
			return 0 ;
		}
		else if(!strcmp(szMsgName, "Health"))
		{
				HealthOrg = pfn;
				return gEngfuncs.pfnHookUserMsg (szMsgName, Health );
		}
			else if(!strcmp(szMsgName, "Battery"))
		{
				BatteryOrg = pfn;
				return gEngfuncs.pfnHookUserMsg (szMsgName, Battery );
		}
			else if(!strcmp(szMsgName, "CurWeapon"))
		{
				CurWeaponOrg = pfn;
				return gEngfuncs.pfnHookUserMsg (szMsgName, CurWeapon );
		}
			else if (!strcmp(szMsgName,"AmmoX")) 
		{
				 AmmoXOrg = pfn;
				 return gEngfuncs.pfnHookUserMsg (szMsgName, AmmoX );
		}
	
		else if (!strcmp(szMsgName,"Ammo")) 
		{
				 AmmoOrg = pfn;
				 return gEngfuncs.pfnHookUserMsg (szMsgName, Ammo );
		}	
		else if(!strcmp(szMsgName, "WeaponList"))
		{
				WeaponListOrg = pfn;
				return gEngfuncs.pfnHookUserMsg (szMsgName, WeaponList );
		}
		else  if(!strcmp(szMsgName, "SayText"))
		{
			gMsgChat= pfn;
			  return gEngfuncs.pfnHookUserMsg (szMsgName, MSGChat );
		}
		else  if(!strcmp(szMsgName, "SendAudio"))
		{
			gMsgRadio= pfn;
			  return gEngfuncs.pfnHookUserMsg (szMsgName, MSGRadio );
		}
		else if(!strcmp(szMsgName, "SetFOV"))
		{
				SetFOVOrg = pfn;
				return gEngfuncs.pfnHookUserMsg (szMsgName, SetFOV );
		}
		if (!strcmp(szMsgName, "TeamInfo"))
		{
			pmTeamInfo = pfn;
		
			return gEngfuncs.pfnHookUserMsg(szMsgName,MsgFunc_TeamInfo);
		}
		if (!strcmp(szMsgName, "ShowMenu"))
		{
			pmShowMenu = pfn;
			return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_ShowMenu);
		}
		else	if (!strcmp(szMsgName, "ResetHUD"))
		{
			pmResetHUD = pfn;
			return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_ResetHUD);
		}
		else	if (!strcmp(szMsgName, "BombDrop"))
		{
			bompDrop = pfn;
			return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_bompdrop);
		}
		else	if (!strcmp(szMsgName, "BombPickup"))
		{
			bomppickup = pfn;
			return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_bomppickup);
		}
		else if (!strcmp(szMsgName, "ScoreAttrib"))
		{
			pmScoreAttrib = pfn;
			return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_ScoreAttrib);
		}
		else if (!strcmp(szMsgName, "Damage"))
		{
			MsgDamage = pfn;
			return gEngfuncs.pfnHookUserMsg(szMsgName, MsgFunc_Damage);
		}
	
		//if (!strcmp(szMsgName, "printf"))
	//gEngfuncs.pfnHookUserMsg(szMsgName, FuncGiayNhapModule);
		gEngfuncs.Con_Printf(szMsgName);
			return gEngfuncs.pfnHookUserMsg(szMsgName, pfn);
	}