#include <metahook.h>
#include "exportfuncs.h"
#include "KillMark.h"
#include "parsemsg.h"
#include "Global.h"
#include <math.h> 

#include "HudDraw.h"
#include <locale>
#include <codecvt>
#include "mempatchs.h"
extern int g_iVideoMode;
extern int g_iVideoQuality;
//////// lights
#define R_STUDIOLIGHTING_SIG "\x51\xDB\x05\x2A\x2A\x2A\x2A\x8A\x4C\x24\x10\xB8\x01\x00\x00\x00\x84\xC8\xD9\x5C\x24\x00"

#define R_STUDIOLIGHTING_SIG_NEW "\x55\x8B\xEC\x51\xDB\x05\x2A\x2A\x2A\x2A\x8A\x4D\x10\xB8\x01\x00\x00\x00\x84\xC8\xD9\x5D\xFC"


#define STUDIO_NF_FULLBRIGHT  0x0004

typedef struct
{
    void (*R_StudioLighting)(float *lv, int bone, int flags, vec3_t normal);
	void (*R_StudioSetupLighting)(alight_t *plighting);
}studiofuncs_t;

studiofuncs_t gStudioFuncs;

//////// lights




cl_exportfuncs_t gExportfuncs;
mh_interface_t *g_pInterface;
metahook_api_t *g_pMetaHookAPI;
mh_enginesave_t *g_pMetaSave;
DWORD g_dwEngineBuildnum;


int loadNormalFormat (int idWP,const wchar_t *filename, int *width, int *height)
{
	
Gdiplus::Bitmap bmp(filename);

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

#define CL_ADDTORESOURCELIST_SIGNAL "\x8B\x44\x24\x04\x8B\x88\x84\x00\x00\x00\x85\xC9"

#define CL_GETMODELBYINDEX_SIGNAL "\x83\xEC\x10\x56\x57\x8B\x7C\x24\x1C\x8B\x34\xBD\x50\xE9\xF1\x02\x85\xF6"
void CL_AddToResourceList(resource_t *pResource, resource_t *pList);

void IPlugins::Init(metahook_api_t *pAPI, mh_interface_t *pInterface, mh_enginesave_t *pSave)
{
	g_pInterface = pInterface;
	g_pMetaHookAPI = pAPI;
	g_pMetaSave = pSave;
	
}

void IPlugins::Shutdown(void)
{
	delete g_pOverview;
}



DWORD g_dwEngineBase, g_dwEngineSize;

void (*g_pfnCL_AddToResourceList)(resource_t *pResource, resource_t *pList);
hook_t *g_phCL_AddToResourceList;




void IPlugins::LoadEngine(void)
{

	g_dwEngineBuildnum = g_pMetaHookAPI->GetEngineBuildnum();
	g_dwEngineBase = g_pMetaHookAPI->GetEngineBase();
	g_dwEngineSize = g_pMetaHookAPI->GetEngineSize();

	//MemPatch_Start(MEMPATCH_STEP_LOADENGINE);
	g_pfnCL_AddToResourceList = (void (*)(resource_t *, resource_t *))g_pMetaHookAPI->SearchPattern((void *)g_dwEngineBase, g_dwEngineSize, CL_ADDTORESOURCELIST_SIGNAL, sizeof(CL_ADDTORESOURCELIST_SIGNAL) - 1);
	g_phCL_AddToResourceList = g_pMetaHookAPI->InlineHook(g_pfnCL_AddToResourceList, CL_AddToResourceList, (void *&)g_pfnCL_AddToResourceList);
	
	
}

void CL_AddToResourceList(resource_t *pResource, resource_t *pList)
{
if(strstr(pResource->szFileName, "models"))
{
	//gEngfuncs.pfnConsolePrint("\n");
	//gEngfuncs.pfnConsolePrint(pResource->szFileName);
	//gEngfuncs.pfnConsolePrint("\n");

	
		mstudiotexture_t* ptexture;
		mstudiobodyparts_t * pbodypart;
		studiohdr_t*pstudio;
		CreateInterfaceFn fnEngineCreateInterface = g_pMetaHookAPI->GetEngineFactory();
		vgui::ISurface *m_pSurface = (vgui::ISurface *)fnEngineCreateInterface(VGUI_SURFACE_INTERFACE_VERSION, NULL);
		m_iScreenTexture =  m_pSurface->CreateNewTextureID();
		
		model_t *pModel = IEngineStudio.Mod_ForName(pResource->szFileName,false) ;
		pstudio = (studiohdr_t*) IEngineStudio.Mod_Extradata(pModel);
		ptexture = (mstudiotexture_t *)(((byte *)pstudio)+pstudio->textureindex);
		

		pbodypart = (mstudiobodyparts_t *)(((byte *)pstudio)+pstudio->bodypartindex);
		for (int i=0;i<pstudio->numbodyparts;i++)
		{
			char Model[32];
			if (pbodypart[i].nummodels>2) 
				sprintf(Model,"\n\n%s\n\n\n",pModel->name);
		}
		
		/*for (int i =0 ; i<pstudio->numtextures;i++) 
		{
			

				//std::string gString(ptexture[i].name) ;
		
				//int gTextures = m_pSurface->CreateNewTextureID();
				gString.replace(gString.end()-4,gString.end(),".png");
				gString.insert(0,"TEXTURES/");
				int width;
				int height;
				//std::wstring wcstring(gString.size(), L' '); 
				//wcstring.resize(mbstowcs(&wcstring[0], gString.c_str(), gString.size()));
			
			   // loadNormalFormat (ptexture[i].index,wcstring.c_str(),&width,&height);
	
			//	gEngfuncs.pfnConsolePrint(gString.c_str());
				
		}*/


}
	g_pfnCL_AddToResourceList(pResource, pList);
}

alight_t *g_savedLighting= new alight_t[10];

void R_StudioSetupLighting(alight_t *plighting)
{
	for (int i=0;i<2;i++)
	memcpy(&g_savedLighting[i], plighting, sizeof(alight_t));
	gStudioFuncs.R_StudioSetupLighting(plighting);
}
int ilight=0;
int gnewxlight=0;
float sdfdsf =0.0;
void R_StudioLighting(float *lv, int bone, int flags, vec3_t normal)
{

	//gEngfuncs.Con_Printf("1123");
	//if(flags & STUDIO_NF_FULLBRIGHT)
	//{
		//save the lighting strength at first.

		//int r_ambientlight = g_savedLighting.ambientlight;

	//	float r_shadelight = g_savedLighting.shadelight;

		//changed to whatever you want
	
		gnewxlight+=1;
		
	
	if (abs(sdfdsf-gEngfuncs.GetClientTime())>1.0)
	{
		sdfdsf = gEngfuncs.GetClientTime();
		gnewxlight=0;
		ilight+=1;
		if (ilight>1) ilight=0;
	}

	for (int i=0;i<2;i++)
	{
		g_savedLighting[i].shadelight = 160;
		g_savedLighting[i].ambientlight = 168;
		g_savedLighting[i].color[0]=255;
		g_savedLighting[i].color[1]=255*i;
		g_savedLighting[i].color[2]=255*i;
		
	}

		//then call the original StudioSetupLighting to modify engine's r_ambientlight & r_shadelight.

		gStudioFuncs.R_StudioSetupLighting(&g_savedLighting[ilight]);



		//then call the original func the calculate the lighting value (lv)

		gStudioFuncs.R_StudioLighting(lv, bone, flags, normal);


	
		//or you can just set *lv = 1 if you want it fullbright, if you do this way, there is no need to save or change the global lighting strength.

		

		//then set the lighting value back;

		//g_savedLighting.ambientlight = r_ambientlight;

		//g_savedLighting.shadelight = r_shadelight;

		//then call the original StudioSetupLighting to set engine's r_ambientlight & r_shadelight back.

		//gStudioFuncs.R_StudioSetupLighting(&g_savedLighting);
		
	   gStudioFuncs.R_StudioLighting(lv, bone, flags, normal);

}
int HUD_UpdateClientData(client_data_t *cdata, float time)
{
	
	//cdata->fov = -40;	
	
	//gEngfuncs.Con_Printf("aaaaa");
	// return 1 if in anything in the client_data struct has been changed, 0 otherwise

	return gExportfuncs.HUD_UpdateClientData(cdata,time);
}

void IPlugins::LoadClient(cl_exportfuncs_t *pExportFunc)
{
	memcpy(&gExportfuncs, pExportFunc, sizeof(gExportfuncs));

	gnKillMark = new KillMark();
	//g_pMetaHookAPI->InlineHook((void *)0x1943340,MsgDeathMsg_Handler,(void *&)pfnDeathMsgHandler);

	pExportFunc->Initialize = Initialize;
	pExportFunc->HUD_Redraw = HUD_Redraw;
	pExportFunc->HUD_Init = HUD_Init;
	pExportFunc->V_CalcRefdef = V_CalcRefdef;
	pExportFunc->HUD_VidInit =HUD_VidInit;
	pExportFunc->HUD_GetStudioModelInterface=HUD_GetStudioModelInterface;
	pExportFunc->HUD_Key_Event = HUD_Key_Event;
	pExportFunc->HUD_DrawTransparentTriangles = HUD_DrawTransparentTriangles;
	pExportFunc->HUD_AddEntity = HUD_AddEntity;
	pExportFunc->HUD_DrawNormalTriangles = HUD_DrawNormalTriangles;
	pExportFunc->IN_MouseEvent = IN_MouseEvent;
	pExportFunc->IN_Accumulate = IN_Accumulate;
	pExportFunc->HUD_PostRunCmd = HUD_PostRunCmd;
	pExportFunc->CL_CameraOffset=CL_CameraOffset;
	pExportFunc->HUD_UpdateClientData =HUD_UpdateClientData;
	
}

/////lights

void InstallHook(void)
{
	if(g_dwEngineBuildnum >= 5953)
		gStudioFuncs.R_StudioLighting = (void (*)(float *, int, int, vec3_t))g_pMetaHookAPI->SearchPattern((void *)g_dwEngineBase, g_dwEngineSize, R_STUDIOLIGHTING_SIG_NEW, sizeof(R_STUDIOLIGHTING_SIG_NEW)-1);
	else
		gStudioFuncs.R_StudioLighting = (void (*)(float *, int, int, vec3_t))g_pMetaHookAPI->SearchPattern((void *)g_dwEngineBase, g_dwEngineSize, R_STUDIOLIGHTING_SIG, sizeof(R_STUDIOLIGHTING_SIG)-1);
	g_pMetaHookAPI->InlineHook(gStudioFuncs.R_StudioLighting, R_StudioLighting, (void *&)gStudioFuncs.R_StudioLighting);  
	g_pMetaHookAPI->InlineHook(gStudioFuncs.R_StudioSetupLighting, R_StudioSetupLighting, (void *&)gStudioFuncs.R_StudioSetupLighting);  
}

/////lights

int HUD_GetStudioModelInterface(int iVersion, struct r_studio_interface_s **ppStudioInterface, struct engine_studio_api_s *pEngineStudio)
{



	memcpy(&IEngineStudio, pEngineStudio, sizeof(engine_studio_api_t));

	gStudioFuncs.R_StudioSetupLighting = pEngineStudio->StudioSetupLighting;

		//InstallHook();
	

	 r_currententity= ((DWORD)pEngineStudio->GetCurrentEntity + 0x1);

	 dwBoneTransform =  ((DWORD)pEngineStudio->StudioGetBoneTransform + 0x1);
	 g_iVideoMode = (IEngineStudio.IsHardware() == 1);
	
	
	//if (!g_iVideoMode) MessageBox(HwndGame,"Tai phien ban này chi ho tro Opengl.","Error",16);
	if(g_iVideoMode)
	{

		char *p = (char *)glGetString(GL_VERSION);
		if(*p)
		{
			char s = *p;
		
			if(s == '1')
			{
				
				g_iVideoQuality = 0;
			}
			else 
			{
				
				g_iVideoQuality = 1;
			}
		}
	}
	return gExportfuncs.HUD_GetStudioModelInterface(iVersion, ppStudioInterface, pEngineStudio);
}









void IPlugins::ExitGame(int iResult)
{
}


EXPOSE_SINGLE_INTERFACE(IPlugins, IPlugins, METAHOOK_PLUGIN_API_VERSION);
