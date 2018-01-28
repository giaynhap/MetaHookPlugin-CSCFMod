#include "Base.h"
#include "Surface.h"
#include "HudDraw.h"

#include <glext.h>
#include "qgl.h"

void Sys_ErrorEx(const char *error);
GLuint R_CompileShader(const char *vscode, const char *fscode, const char *vsfile, const char *fsfile);

GLuint gStencilRenderOp = GL_NOTEQUAL;
vgui::HFont g_hFont;
int create = 0;
int gTgaSkin=0;
COverview *g_pOverview;
int gTextures;
int BompIAlpha = 0;
#define SQROOT2	1.41
float gVecAngle[3];
 int gDrawRadar;
	int gDrawTeamPost;
	float gZvec;
	float flOverViewZoom = 10.0f;
	void calcpostangle(float gAngle,float R,Vector2D *gvec1,Vector2D *gvec2,Vector2D *gvec3,Vector2D *gvec4);

extern "C"
{
	PROC (WINAPI *qwglGetProcAddress)(LPCSTR) = NULL;

	PFNGLCREATESHADEROBJECTARBPROC qglCreateShaderObjectARB = NULL;
	PFNGLDELETEOBJECTARBPROC qglDeleteObjectARB = NULL;
	PFNGLSHADERSOURCEARBPROC qglShaderSourceARB = NULL;
	PFNGLCOMPILESHADERARBPROC qglCompileShaderARB = NULL;
	PFNGLCREATEPROGRAMOBJECTARBPROC qglCreateProgramObjectARB = NULL;
	PFNGLATTACHOBJECTARBPROC qglAttachObjectARB = NULL;
	PFNGLLINKPROGRAMARBPROC qglLinkProgramARB = NULL;
	PFNGLUSEPROGRAMOBJECTARBPROC qglUseProgramObjectARB = NULL;
	PFNGLGETUNIFORMLOCATIONARBPROC qglGetUniformLocationARB = NULL;
	PFNGLUNIFORM1IARBPROC qglUniform1iARB = NULL;
	PFNGLUNIFORM2IARBPROC qglUniform2iARB = NULL;
	PFNGLUNIFORM3IARBPROC qglUniform3iARB = NULL;
	PFNGLUNIFORM4IARBPROC qglUniform4iARB = NULL;
	PFNGLUNIFORM1FARBPROC qglUniform1fARB = NULL;
	PFNGLUNIFORM2FARBPROC qglUniform2fARB = NULL;
	PFNGLUNIFORM3FARBPROC qglUniform3fARB = NULL;
	PFNGLUNIFORM4FARBPROC qglUniform4fARB = NULL;
	PFNGLUNIFORM3FVARBPROC qglUniform3fvARB = NULL;
	PFNGLUNIFORM4FVARBPROC qglUniform4fvARB = NULL;
	PFNGLGETSHADERIVPROC qglGetShaderiv = NULL;
	PFNGLGETSHADERINFOLOGPROC qglGetShaderInfoLog = NULL;
	PFNGLGETINFOLOGARBPROC qglGetInfoLogARB = NULL;
}

COverview::COverview(void)
{
	

	m_pRect = gEngfuncs.pfnRegisterVariable("gn_Radar_rect", "200", FCVAR_ARCHIVE);
	
	m_pRotate = gEngfuncs.pfnRegisterVariable("gn_Radar_rotate", "1", FCVAR_ARCHIVE);

	gRadarx = gEngfuncs.pfnRegisterVariable("gnRadar_x", "-50", FCVAR_ARCHIVE);
	gRadary = gEngfuncs.pfnRegisterVariable("gnRadar_y", "-50", FCVAR_ARCHIVE);
	
	sScreenInfo.iSize = sizeof(sScreenInfo);
	gEngfuncs.pfnGetScreenInfo(&sScreenInfo);

	m_iInited = 0;
	m_pSurface = NULL;
	m_fCurrentYaw = 0;
	
	

}
bool COverview::CalcPoint(const float* origin, float& screenx, float& screeny, int check)
{
	float dx  = (origin[0] - m_vecEyeOrigin.x)*1.8;
	float dy  = (origin[1] - m_vecEyeOrigin.y)*1.8;

	float x =  dx*  m_fYawSin   -  dy*m_fYawCos;
	float y =  dx*(-m_fYawCos)  -  dy*m_fYawSin;

	screenx = (m_pRect->value/2)+(x/(1.8*flOverViewZoom))*m_flZoom;
	screeny = (m_pRect->value/2)+(y/(1.8*flOverViewZoom))*m_flZoom;

	// check in circle
	if (check==1)
	{
		float fRadius = sqrt((x/(1.8*flOverViewZoom) * m_flZoom) * (x/(1.8*flOverViewZoom))*m_flZoom + (y/(1.8*flOverViewZoom))*m_flZoom * (y/(1.8*flOverViewZoom))*m_flZoom);
		if(fRadius > m_pRect->value / 4) return false;
		return true;
	}
	return true;

}
float CalcDist(const float* origin1,const float* origin2)
{
	
	float dist = sqrt((origin1[0]-origin2[0])*(origin1[0]-origin2[0]) + (origin1[1]-origin2[1])*(origin1[1]-origin2[1]) );
	return dist;
}
float calcAngle(const float* origin1,const float* origin2)
{
	float originvic[2];
	float gdist = CalcDist(origin1,origin2);
	
	
	//float gdist2 = CalcDist(origin1,originvic);
	float gAngle = acos(abs(origin1[0]-origin2[0])/gdist)* 180.0 / M_PI;
	//if (origin1[0]<=origin2[0] && origin1[1]>=
	if (origin1[0]<=origin2[0]&&origin1[1]<origin2[1])
		gAngle = 360 - gAngle;
	if (origin1[0]>= origin2[0] && origin1[1]>=origin2[1])
		gAngle = 180-gAngle;
	if (origin1[0]>= origin2[0] && origin1[1] <= origin2[1])
		gAngle = 180 + gAngle;
	return gAngle;
}
bool COverview::IsValidPlayer(cl_entity_s *pEntity)
{

	if (pEntity && pEntity->model && pEntity->model->name && !(pEntity->curstate.messagenum < gEngfuncs.GetLocalPlayer()->curstate.messagenum) && pEntity->player && (pEntity->index != gEngfuncs.GetLocalPlayer()->index))
		return true;

	return false;
}
void COverview::PlayerESP(void)
{
	if (gDrawTeamPost==0) return;
	float x,y;
	
	cl_entity_s *loactent =  gEngfuncs.GetEntityByIndex (gEngfuncs.GetLocalPlayer()->index);
	for(int i=1;i<33;i++)
	{
		cl_entity_s *pEntity = gEngfuncs.GetEntityByIndex(i);
		if(IsValidPlayer(pEntity)&& (gGN_Hud_Draw->VPlayer[i].die != 1))
		{
			if (gGN_Hud_Draw->VPlayer[i].Team == gGN_Hud_Draw->VPlayer[gEngfuncs.GetLocalPlayer()->index].Team || gDrawTeamPost==2 )
			if(CalcPoint(pEntity->origin,x,y))
			{
				//gEngfuncs.pfnFillRGBA(x+gRadarx->value,y+gRadary->value,2,2,255,255,0,255);
				m_pSurface->DrawSetTexture(gdaranumg);
				m_pSurface->DrawSetColor(255,255,255,255);
				m_pSurface->DrawTexturedRect(x+gRadarx->value-4,y+gRadary->value-4,x+gRadarx->value+4,y+gRadary->value+4);
			}
			else
			DrawNumRadar2(gdaranum2g,pEntity,255,255,255,255);
		}
	}
}
void COverview::DrawFrame(void)
{
	//_____y
	float fDelta = m_fYaw - m_fCurrentYaw;
	float fDelta2;
	if(fDelta >0.5)
	{
		if(fDelta > 15)
		{
			m_fCurrentYaw = m_fYaw - 15;
		}
		else
		{
			fDelta2 = fDelta / 60;
			fDelta2 = max(0.01,fDelta2);

			m_fCurrentYaw += fDelta2;
		}
	}
	else if(fDelta<-0.5)
	{
		if(fDelta <-15)
		{
			m_fCurrentYaw = m_fYaw + 15;
		}
		else
		{
			fDelta2 = (-fDelta) / 60;
			fDelta2 = max(0.01,fDelta2);
			m_fCurrentYaw -= fDelta2;
		}
	}
	else m_fCurrentYaw = m_fYaw;

	Vector2D vec1,vec2,vec3,vec4;
	
	calcpostangle( m_fCurrentYaw, m_pRect->value/2,&vec1,&vec2,&vec3,&vec4);

	//m_pSurface->DrawSetTexture(m_iCircleID2);
	//m_pSurface->DrawSetColor(255,255,255,255);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glBindTexture(GL_TEXTURE_2D,m_iCircleID2);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(vec1.x+gRadarx->value, vec1.y+gRadary->value, 0.0);
	glTexCoord2f(0, 1);
	glVertex3f(vec2.x+gRadarx->value, vec2.y+gRadary->value, 0.0);
	glTexCoord2f(1, 1);
	glVertex3f(vec3.x+gRadarx->value, vec3.y+gRadary->value, 0.0);
	glTexCoord2f(1, 0);
	glVertex3f(vec4.x+gRadarx->value, vec4.y+gRadary->value, 0.0);
	glEnd();

}

void calcpostangle(float gAngle,float R,Vector2D *gvec1,Vector2D *gvec2,Vector2D *gvec3,Vector2D *gvec4)
{
	
	float X,Y;
	Vector2D vec1,vec2,vec3,vec4;
	float fAngle = gAngle* (M_PI / 180.0);
	float i = (SQROOT2 *R);
	X = R - cos(fAngle )*i ;
	Y = R - sin(fAngle ) *i;
	vec1.x=X;
	vec1.y=Y;

	fAngle = (270+gAngle)* (M_PI / 180.0);
	X = R - cos(fAngle )*i;
	Y = R - sin(fAngle )*i;
	vec2.x=X;
	vec2.y=Y;

	vec3.x = R*2-vec1.x;
	vec3.y = R*2-vec1.y;

	vec4.x = R*2-vec2.x;
	vec4.y = R*2-vec2.y;
	*gvec1 = vec1;
	*gvec2 = vec2;
	*gvec3 = vec3;
	*gvec4 = vec4;
}



void COverview::CalcRefdef(struct ref_params_s *pParams)
{
	gVecAngle[0] = pParams->viewangles[0];
	gVecAngle[1] = pParams->viewangles[1];
	gVecAngle[2] = pParams->viewangles[2];
	m_fYaw = m_pRotate->value?pParams->viewangles[1]:90;
	m_fYawSin = sin(m_fYaw*(M_PI/180.0));
	m_fYawCos = cos(m_fYaw*(M_PI/180.0));

	m_vecEyeOrigin.x = pParams->vieworg[0];
	m_vecEyeOrigin.y = pParams->vieworg[1];
	gZvec = pParams->vieworg[2];
	





	if (!m_iInited)
	{
		glClearStencil( 0 );
		CreateInterfaceFn fnEngineCreateInterface = g_pMetaHookAPI->GetEngineFactory();
		
		m_pSurface = (vgui::ISurface *)fnEngineCreateInterface(VGUI_SURFACE_INTERFACE_VERSION, NULL);
		
		
		
		m_iCircleID = m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(m_iCircleID,"CSFIRED/cf",true,true);
		m_iCircleID2 = m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(m_iCircleID2,"CSFIRED/cf2",true,true);

		gdaranumg=m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(gdaranumg,"CSFIRED/RADANUM",true,true);

		gdaranum2g=m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(gdaranum2g,"CSFIRED/RADANUM2",true,true);


		m_iInited = 1;

	}
	return;
}
void gnDrawPoint(float x,float y, int r,int g ,int b ,int a);
void DrawCircle(float x1,float y1,float R) 
{


float  angle,x2,y2;
	     glColor3f(1.0,1.0,1.0);
	      
	   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	      
	    glBegin(GL_TRIANGLE_FAN);

		glVertex2f(x1,y1);
		      
		for (angle=1.0f;angle<361.0f;angle+=0.2)
		{
		x2 = x1+sin(angle)* R;
		y2 = y1+cos(angle)* R;
		glVertex2f(x2,y2);  
		} 
           glEnd();
 
}
void COverview::Draw(void)
{
	/*
	if (gTgaSkin!=1)
	{
		
		gTgaSkin = 1;
	//	model_t *pModel = IEngineStudio.Mod_ForName("models/v_ak47.mdl",false) ;
		mstudiotexture_t* ptexture;
		studiohdr_t*pstudio;
		
		model_t *pModel = IEngineStudio.Mod_ForName("models/v_knife.mdl",false) ;
		pstudio = (studiohdr_t*) IEngineStudio.Mod_Extradata(pModel);
		ptexture = (mstudiotexture_t *)(((byte *)pstudio)+pstudio->textureindex);
		for (int i =0 ; i<pstudio->numtextures;i++) 
		{
			std::string gString(ptexture[i].name) ;
			
	
			
		
			
			//if ( gString.find("hand")!=std::string::npos)
			//{
				gTextures = m_pSurface->CreateNewTextureID();
				gString.replace(gString.end()-4,gString.end(),".png");
				gString.insert(0,"TEXTURES/");
				int width;
				int height;
			
				//std::wstring_convert< std::codecvt<wchar_t,char,std::mbstate_t> > conv ;
				//char *gChar = gString.c_str()
			
				std::wstring wcstring(gString.size(), L' '); // Overestimate number of code points.
				wcstring.resize(mbstowcs(&wcstring[0], gString.c_str(), gString.size())); // Shrink to fit.

			    loadNormalFormat (gTextures,wcstring.c_str(),&width,&height);
				//gString.c_str()
				//m_pSurface->DrawSetTextureRGBA(gTextures,buffer,width,height,true,true);
				ptexture[i].index=gTextures;
				//ptexture[i].width = width;
			//	ptexture[i].height=height;
				gEngfuncs.pfnConsolePrint(gString.c_str());
				//m_pSurface->drawsettextures

			//}
		}
	}
	*/
	//glEnable(GL_TEXTURE_2D);
	Vector2D vecTitles; 
	int i = static_cast<int>(sqrt(static_cast<long double>(m_pSprite->numframes / (4 * 3))));
	vecTitles = Vector2D(i * 4, i * 3);

	Vector2D vecStartPosOffset, vecScissorSize;
	vecStartPosOffset = Vector2D(sScreenInfo.iWidth / 576, sScreenInfo.iHeight / 432);
	vecScissorSize = Vector2D(m_pRect->value, m_pRect->value);


	int iFrame = 0;
	float flScreenAspect = 4.0f / 3.0f;	
	float flAngles = (m_fYaw + 90.0) * (M_PI / 180.0);

	float z ;
	z  = gZvec - (10+1.2*m_pRect->value)*4.1f/g_pOverview->m_flZoom;
	if (m_bIsRotate)
			flAngles -= float(M_PI / 2);

	Vector2D vecTile;

	if (m_bIsRotate)
	{
		vecTile.x = 3 + m_flZoom * (1.0 / 1024.0) * m_vecOrigin.y - (1.0/1024) * m_flZoom * m_vecEyeOrigin.y;
		vecTile.y = -(-4 + m_flZoom * (1.0 / 1024.0) * m_vecOrigin.x - (1.0/1024) * m_flZoom * m_vecEyeOrigin.x);
	}
	else
	{
		vecTile.x = 3 + m_flZoom * (1.0 / 1024.0) * m_vecOrigin.x - (1.0/1024) * m_flZoom * m_vecEyeOrigin.x;
		vecTile.y = 4 + m_flZoom * (1.0 / 1024.0) * m_vecOrigin.y - (1.0/1024) * m_flZoom * m_vecEyeOrigin.y;
	}

	Vector2D vecStep;
	vecStep.x = (2 * 4096.0f / flOverViewZoom) / vecTitles.x;
	vecStep.y = -(2 * 4096.0f / (flOverViewZoom * flScreenAspect)) / vecTitles.y;

	Vector2D vecStepUp, vecStepRight;
	Vector2D vec1,vec2,vec3,vec4;

	vecStepUp = Vector2D(cos(flAngles + (M_PI / 2)) * vecStep.y, sin(flAngles + (M_PI / 2)) * vecStep.y);
	vecStepRight = Vector2D(cos(flAngles) * vecStep.x, sin(flAngles) * vecStep.x);

	Vector2D vecIn, vecOut;
	vecOut.x = vecScissorSize.x * 0.5 - vecTile.x * vecStepRight.x - vecTile.y * vecStepUp.x + vecStartPosOffset.x;
	vecOut.y = vecScissorSize.y * 0.5 - vecTile.x * vecStepRight.y - vecTile.y * vecStepUp.y + vecStartPosOffset.y;
	// glClearStencil( 0 );
	
//	glColorMask(GL_FALSE | GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	
	glClear( GL_STENCIL_BUFFER_BIT );
	glDisable(GL_STENCIL_TEST);
    glStencilMask(0x0);

	glEnable(GL_STENCIL_TEST);
	glEnable(GL_ALPHA_TEST);
	
	
	glStencilMask(-1u);
	glStencilFunc(GL_ALWAYS, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
	
	
	DrawCircle(gRadarx->value+m_pRect->value/2,gRadary->value+m_pRect->value/2,m_pRect->value/4+2) ;
       
	glStencilMask(0x1);
    glStencilFunc(GL_LEQUAL, 0x1, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

	


	//  glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
	//glStencilFunc(GL_EQUAL, 1, 1);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	//glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

	/*glStencilMask(0x00); 
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glDepthMask	(GL_TRUE);
	*/

	//gEngfuncs.pTriAPI->RenderMode(kRenderTransAlpha);
	
	//gEngfuncs.pTriAPI->CullFace(TRI_NONE);
	//gEngfuncs.pTriAPI->Color4f(1, 1, 1, 1);
	//glScissor(0,0, 256,256);
	//glEnable( GL_SCISSOR_TEST );

	for (int i = 0; i < vecTitles.y; i++)
	{
		vecIn.x = vecOut.x;
		vecIn.y = vecOut.y;

		for (int j = 0; j < vecTitles.x; j++)	
		{
			gEngfuncs.pTriAPI->SpriteTexture(m_pSprite, iFrame);
			
			//glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x8370 );
			//glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x8370 );
			glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex3f(gRadarx->value+vecIn.x ,gRadary->value+ vecIn.y, z);
			glTexCoord2f(0, 1);
			glVertex3f(gRadarx->value+vecIn.x + vecStepRight.x, gRadary->value+ vecIn.y + vecStepRight.y, z);
			glTexCoord2f(1, 1);
			glVertex3f(gRadarx->value+vecIn.x + vecStepRight.x + vecStepUp.x, gRadary->value+ vecIn.y + vecStepRight.y + vecStepUp.y,z);
			glTexCoord2f(1, 0);
			glVertex3f(gRadarx->value+vecIn.x + vecStepUp.x, gRadary->value+vecIn.y + vecStepUp.y, z);
			glEnd();

			iFrame++;
			vecIn = vecIn + vecStepUp;
		}

		vecOut = vecOut + vecStepRight;
	}
	//m_pSurface->DrawSetTexture(m_iCircleID);
	//m_pSurface->DrawSetColor(255,255,255,255);
	//m_pSurface->DrawTexturedRect(gRadarx->value,gRadary->value,m_pRect->value+gRadarx->value,m_pRect->value+gRadary->value);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_ALPHA_TEST );
	
	
	//DrawCircle(200.0,300.0,m_pRect->value/4+2) ;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glBindTexture(GL_TEXTURE_2D,m_iCircleID);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex3f(gRadarx->value,gRadary->value+m_pRect->value,0);
	glTexCoord2f(1,1);
	glVertex3f(gRadarx->value+m_pRect->value,gRadary->value+m_pRect->value,0);
	glTexCoord2f(1,0);
	glVertex3f(gRadarx->value+m_pRect->value,gRadary->value,0);
	glTexCoord2f(0,0);
	glVertex3f(gRadarx->value,gRadary->value,0);
	glEnd();

	DrawFrame();
	PlayerESP();
		

	if (gBompDrop)
	{
		cl_entity_s *gEntC4 =  gEngfuncs.GetEntityByIndex (g_iBombEntity);
		
		if (gEntC4 && g_iBombEntity  )
		{
		
			float x,y;
			if(CalcPoint(gEntC4->origin ,x,y))
			{
				BompIAlpha-=10;
				if (BompIAlpha<=0) BompIAlpha = 255;
				m_pSurface->DrawSetTexture(gdaranumg);
				m_pSurface->DrawSetColor(255,0,0,BompIAlpha);
				m_pSurface->DrawTexturedRect(x+gRadarx->value-4,y+gRadary->value-4,x+gRadarx->value+4,y+gRadary->value+4);
				// gnDrawPoint(x+gRadarx->value,y+gRadary->value,255,0,0,255);
			}
			else
			DrawNumRadar2(gdaranum2g,gEntC4,255,0,255,255);




		}
	}



	if (create!=1)
	{
		create=1;
		 g_hFont = m_pSurface->CreateFont();
		// m_pSurface->AddGlyphSetToFont(g_hFont, "Tahoma", 48, 800, 0, 0, 0, 0, 0);
		 m_pSurface->AddGlyphSetToFont(g_hFont, "Tahoma", 12, 800,0, 0, vgui::ISurface::FONTFLAG_OUTLINE, 0, 0);

	
	}
	//gEngfuncs.pTriAPI->RenderMode(kRenderNonaml);
	/* m_pSurface->DrawFlushText();
	 HUD_DrawText(200, 500, 0, 255, 0, 255, g_hFont, L"Surface Draw Text \n");
	 HUD_DrawText(201, 500, 40, 40, 40, 255, g_hFont, L"Surface Draw Text \n");
	 HUD_DrawText(199, 500, 40, 40, 40, 255, g_hFont, L"Surface Draw Text \n");
	 HUD_DrawText(200, 501, 40, 40, 40, 255, g_hFont, L"Surface Draw Text \n");
	 HUD_DrawText(200, 499, 40, 40, 40, 255, g_hFont, L"Surface Draw Text \n");
	 */
}
void COverview::DrawNumRadar2(int gImage,cl_entity_s *gEnt,int r,int g ,int b, int a)
{
			float x,y;
			float origin1[2];
			float origin2[2];
			CalcPoint(gEnt->origin ,x,y);
			origin2[0] =x;
			origin2[1] = y;
			CalcPoint(gEngfuncs.GetLocalPlayer()->origin ,x,y);
			origin1[0] = x;
			origin1[1] = y;
			float angle = calcAngle( origin1,origin2);
			float gangle2 ;
			gangle2 = angle+45;
			if (gangle2>360) gangle2 = gangle2-360;
			int gw;
			int gh;
			m_pSurface->DrawGetTextureSize(gImage,gw,gh);

			float gx = m_pRect->value/2+gRadarx->value + cos((360-angle)* (M_PI / 180.0))*m_pRect->value/4 -gw/2;
			float gy =m_pRect->value/2+gRadary->value + sin((360-angle)* (M_PI / 180.0))*m_pRect->value/4 - gh/2 ;
			Vector2D vec1,vec2,vec3,vec4;
	
			calcpostangle(360-gangle2,8,&vec1,&vec2,&vec3,&vec4);
			//m_pSurface->DrawSetTexture(gImage);
			//m_pSurface->DrawSetColor(r,g,b,a);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
			glBindTexture(GL_TEXTURE_2D,gImage);
			glColor4ub(r,g,b,a);
			gEngfuncs.pTriAPI->Begin(TRI_QUADS);
			gEngfuncs.pTriAPI->TexCoord2f(0, 0);
			gEngfuncs.pTriAPI->Vertex3f(vec1.x+gx, vec1.y+gy, 0.0);
			gEngfuncs.pTriAPI->TexCoord2f(0, 1);
			gEngfuncs.pTriAPI->Vertex3f(vec2.x+gx, vec2.y+gy, 0.0);
			gEngfuncs.pTriAPI->TexCoord2f(1, 1);
			gEngfuncs.pTriAPI->Vertex3f(vec3.x+gx, vec3.y+gy, 0.0);
			gEngfuncs.pTriAPI->TexCoord2f(1, 0);
			gEngfuncs.pTriAPI->Vertex3f(vec4.x+gx, vec4.y+gy, 0.0);
			gEngfuncs.pTriAPI->End();


}
void gnDrawPoint(float x,float y, int r,int g ,int b ,int a)
{
	//gEngfuncs.pfnFillRGBA(x-1,y-1,3,3,r,g,b,a);
	//gEngfuncs.pfnFillRGBA(x+2,y,1,1,r,g,b,a);
	//gEngfuncs.pfnFillRGBA(x+-2,y,1,1,r,g,b,a);
	//gEngfuncs.pfnFillRGBA(x,y-2,1,1,r,g,b,a);
	//gEngfuncs.pfnFillRGBA(x,y+2,1,1,r,g,b,a);

	

}
bool COverview::CanDraw(void)
{
	return m_pSprite && gDrawRadar && !gEngfuncs.GetLocalPlayer()->curstate.iuser1;
}

void  COverview::HUD_DrawText(int x, int y, int r, int g, int b, int a, vgui::HFont font, wchar_t *text)
{
    int startX = x;
    int fontTall = m_pSurface->GetFontTall(font);
    m_pSurface->DrawSetTextPos(x, y);
    m_pSurface->DrawSetTextColor(r, g, b, a);
    m_pSurface->DrawSetTextFont(font);

    for (int i = 0; i < wcslen(text); i++)
    {
        if (text[i] == '\n')
        {
            x = startX;
            y += fontTall;

            continue;
        }

        m_pSurface->DrawUnicodeCharAdd(text[i]);

        int w1, w2, w3;
        m_pSurface->GetCharABCwide(font, text[i], w1, w2, w3);

        x += w1 + w2 + w3;
    }
}
