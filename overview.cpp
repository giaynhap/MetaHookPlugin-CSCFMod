#include "Base.h"
#include "Surface.h"
#include "HudDraw.h"
#include <qgl.h>
#include <glext.h>
extern int g_iVideoMode;
void Sys_ErrorEx(const char *error);
//void Sys_ErrorEx(const char *error);
GLuint R_CompileShader(const char *vscode, const char *fscode, const char *vsfile, const char *fsfile);

int RadarIconBomp_index[2];
int gRadarBG2;
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
	float flOverViewZoom = 20.0f;
	void calcpostangle(float gAngle,float R,Vector2D *gvec1,Vector2D *gvec2,Vector2D *gvec3,Vector2D *gvec4);
int program = 0;

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
	PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;

	// FrameBuffer (FBO) gen, bin and texturebind
	PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT ;
	PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT ;
	PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT ;
	PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT ;
	 PFNGLISRENDERBUFFEREXTPROC glIsRenderbufferEXT;
	PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT;
	PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT;
	PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT;
	PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT;
}

typedef struct
{
	int base;
	int center;
	int radius;
	int blurdist;
}uniform_t;

uniform_t uniform;

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

	if (pEntity && pEntity->model && pEntity->model->name  && pEntity->player && (pEntity->index != gEngfuncs.GetLocalPlayer()->index))
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
				if (BompIdPlayer==i)
				m_pSurface->DrawSetColor(255,0,0,255);
				else
				m_pSurface->DrawSetColor(255,255,255,255);
				m_pSurface->DrawTexturedRect(x+gRadarx->value-4,y+gRadary->value-4,x+gRadarx->value+4,y+gRadary->value+4);
			}
			else
			{
				if (BompIdPlayer==i)
					DrawNumRadar2(gdaranum2g,pEntity,255,0,0,255);
				
				else
				DrawNumRadar2(gdaranum2g,pEntity,255,255,255,255);
			}
		}
	}
	
//	RadarIconBomp_index[0]
//	RadarIconBomp_index[1]

	if (PosBompP[0][0])
	{
		float x,y;

		int gw;
		int gh;
		m_pSurface->DrawGetTextureSize(RadarIconBomp_index[0],gw,gh);
		if(CalcPoint(PosBompP[0],x,y))
		{
			
			m_pSurface->DrawSetTexture(RadarIconBomp_index[0]);
			
			m_pSurface->DrawSetColor(255,255,255,255);
			m_pSurface->DrawTexturedRect(x+gRadarx->value-gw/2,y+gRadary->value-gh/2,x+gRadarx->value+gw/2,y+gRadary->value+gh/2);




		}
		else
		DrawNumRadar4(RadarIconBomp_index[0],PosBompP[0],255,255,255,255);
		if(CalcPoint(PosBompP[1],x,y))
		{
			
			m_pSurface->DrawSetTexture(RadarIconBomp_index[1]);
			
			m_pSurface->DrawSetColor(255,255,255,255);
			m_pSurface->DrawTexturedRect(x+gRadarx->value-gw/2,y+gRadary->value-gh/2,x+gRadarx->value+gw/2,y+gRadary->value+gh/2);
		}
		else
		DrawNumRadar4(RadarIconBomp_index[1],PosBompP[1],255,255,255,255);
	}
	if (gBompDrop)
	{
		
		if ( g_iBombEntity  )
		{
			float gOriginl[3];
			memset(gOriginl,3,0);
			float x,y;
			if (isBompStatus==1)
			{
				gOriginl[0]=BompPost[0];
				gOriginl[1]=BompPost[1];
				gOriginl[2]=BompPost[2];
			}
			else
			{
				cl_entity_s *gEntC4 =  gEngfuncs.GetEntityByIndex (g_iBombEntity);
				gOriginl[0]=gEntC4->origin[0];
				gOriginl[1]=gEntC4->origin[1];
				gOriginl[2]=gEntC4->origin[2];
			}
				if(CalcPoint(gOriginl,x,y))
				{
					BompIAlpha-=10;
					if (BompIAlpha<=0) BompIAlpha = 255;
					m_pSurface->DrawSetTexture(gdaranumg);
					m_pSurface->DrawSetColor(255,0,0,BompIAlpha);
					m_pSurface->DrawTexturedRect(x+gRadarx->value-4,y+gRadary->value-4,x+gRadarx->value+4,y+gRadary->value+4);
					// gnDrawPoint(x+gRadarx->value,y+gRadary->value,255,0,0,255);
				}
				else
				{
				
					DrawNumRadar3(gdaranum2g,gOriginl,255,0,0,255);
					
				}
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
		gDrawTeamPost= 1;
		gDrawRadar=1;

		
		CreateInterfaceFn fnEngineCreateInterface = g_pMetaHookAPI->GetEngineFactory();
		
		m_pSurface = (vgui::ISurface *)fnEngineCreateInterface(VGUI_SURFACE_INTERFACE_VERSION, NULL);
		
		
		
		m_iCircleID = m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(m_iCircleID,"CSFIRED/RADARBG",true,true);
		m_iCircleID2 = m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(m_iCircleID2,"CSFIRED/RADARBG2",true,true);

		gdaranumg=m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(gdaranumg,"CSFIRED/RADANUM",true,true);

		gdaranum2g=m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(gdaranum2g,"CSFIRED/RADANUM2",true,true);
		gRadarBG2 = m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(gRadarBG2,"CSFIRED/RADARBG1",true,true);


		RadarIconBomp_index[0] =  m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(RadarIconBomp_index[0],"CSFIRED/radarbomba",true,true);
		RadarIconBomp_index[1] =  m_pSurface->CreateNewTextureID();
		m_pSurface->DrawSetTextureFile(RadarIconBomp_index[1],"CSFIRED/radarbombb",true,true);


		m_iInited = 1;
	}
	
	return;
}

void int_gHud(void)
{
	if (!g_iVideoMode) return ;
	HMODULE hOpenGL = GetModuleHandle("opengl32.dll");
	*(FARPROC *)&qglGetString = GetProcAddress(hOpenGL, "glGetString");
	*(FARPROC *)&qwglGetProcAddress = GetProcAddress(hOpenGL, "wglGetProcAddress");
	
	//检查扩展
	const char *extension = (const char *)qglGetString(GL_EXTENSIONS);
	if (!strstr(extension, "GL_ARB_shader_objects") || !strstr(extension, "GL_ARB_vertex_shader") || !strstr(extension, "GL_ARB_fragment_shader"))
	{
		//gDrawRadar=0;
		//Sys_ErrorEx("Error Shader");
		return ;
	}
	qglCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)qwglGetProcAddress("glCreateShaderObjectARB");
	qglDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)qwglGetProcAddress("glDeleteObjectARB");
	qglShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)qwglGetProcAddress("glShaderSourceARB");
	qglCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)qwglGetProcAddress("glCompileShaderARB");
	qglCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)qwglGetProcAddress("glCreateProgramObjectARB");
	qglAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)qwglGetProcAddress("glAttachObjectARB");
	qglLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)qwglGetProcAddress("glLinkProgramARB");
	qglUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)qwglGetProcAddress("glUseProgramObjectARB");
	qglGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)qwglGetProcAddress("glGetUniformLocationARB");

	qglUniform1iARB = (PFNGLUNIFORM1IARBPROC)qwglGetProcAddress("glUniform1iARB");
	qglUniform2iARB = (PFNGLUNIFORM2IARBPROC)qwglGetProcAddress("glUniform2iARB");
	qglUniform3iARB = (PFNGLUNIFORM3IARBPROC)qwglGetProcAddress("glUniform3iARB");
	qglUniform4iARB = (PFNGLUNIFORM4IARBPROC)qwglGetProcAddress("glUniform4iARB");
	qglUniform1fARB = (PFNGLUNIFORM1FARBPROC)qwglGetProcAddress("glUniform1fARB");
	qglUniform2fARB = (PFNGLUNIFORM2FARBPROC)qwglGetProcAddress("glUniform2fARB");
	qglUniform3fARB = (PFNGLUNIFORM3FARBPROC)qwglGetProcAddress("glUniform3fARB");
	qglUniform4fARB = (PFNGLUNIFORM4FARBPROC)qwglGetProcAddress("glUniform4fARB");
	qglUniform3fvARB = (PFNGLUNIFORM3FVARBPROC)qwglGetProcAddress("glUniform3fvARB");
	qglUniform4fvARB = (PFNGLUNIFORM4FVARBPROC)qwglGetProcAddress("glUniform4fvARB");

	qglGetShaderiv = (PFNGLGETSHADERIVPROC)qwglGetProcAddress("glGetShaderiv");
	qglGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)qwglGetProcAddress("glGetShaderInfoLog");
	qglGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)qwglGetProcAddress("glGetInfoLogARB");
	 glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	  glGenFramebuffersEXT		= (PFNGLGENFRAMEBUFFERSEXTPROC)		wglGetProcAddress("glGenFramebuffersEXT");
	  glBindFramebufferEXT		= (PFNGLBINDFRAMEBUFFEREXTPROC)		wglGetProcAddress("glBindFramebufferEXT");
	  glFramebufferTexture2DEXT	= (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
	  glCheckFramebufferStatusEXT	= (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");

	
	  glIsRenderbufferEXT = (PFNGLISRENDERBUFFEREXTPROC) wglGetProcAddress("glIsRenderbufferEXT");
     glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC) wglGetProcAddress("glBindRenderbufferEXT");
     glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC) wglGetProcAddress("glDeleteRenderbuffersEXT");
     glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC) wglGetProcAddress("glGenRenderbuffersEXT");
     glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC) wglGetProcAddress("glRenderbufferStorageEXT");
	//加载着色器
	const char *vscode = (const char *)gEngfuncs.COM_LoadFile("resource/hud_drawroundrect.vsh", 5, 0);
	const char *fscode = (const char *)gEngfuncs.COM_LoadFile("resource/hud_drawroundrect.fsh", 5, 0);
	if(!vscode || !fscode)
	{
		Sys_ErrorEx("Error 1");
		return ;
	}
	program = R_CompileShader(vscode, fscode, "hud_drawroundrect.vsh", "hud_drawroundrect.fsh");
	if(!program)
	{
		Sys_ErrorEx("Error2");
		return ;
	}
	uniform.base = qglGetUniformLocationARB(program, "base");
	uniform.center = qglGetUniformLocationARB(program, "center");
	uniform.radius = qglGetUniformLocationARB(program, "radius");
	uniform.blurdist = qglGetUniformLocationARB(program, "blurdist");


	

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
	
	//if (m_flZoom<=0) return ; 
	Vector2D vecTitles; 
	int i = static_cast<int>(sqrt(static_cast<long double>(m_pSprite->numframes / (4 * 3))));
	vecTitles = Vector2D(i * 4, i * 3);

	Vector2D vecStartPosOffset, vecScissorSize;
	vecStartPosOffset = Vector2D(sScreenInfo.iWidth / 576, sScreenInfo.iHeight / 432);
	vecScissorSize = Vector2D(m_pRect->value, m_pRect->value);


	int iFrame = 0;
	float flScreenAspect = 4.0f / 3.0f;	
	float flAngles = (m_fYaw + 90.0) * (M_PI / 180.0);

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
//	if (m_bIsRotate==1)
//	{
	//	vecStep.x = (2 * 4096.0f / (flOverViewZoom * flScreenAspect)) / vecTitles.y; 
	//	vecStep.y = -(2 * 4096.0f / flOverViewZoom) / vecTitles.x;
//	}
	//else{
	vecStep.x = (2 * 4096.0f / flOverViewZoom) / vecTitles.x;
	vecStep.y = -(2 * 4096.0f / (flOverViewZoom * flScreenAspect)) / vecTitles.y;
	//}
	
	Vector2D vecStepUp, vecStepRight;
	Vector2D vec1,vec2,vec3,vec4;
	vecStepUp = Vector2D(cos(flAngles + (M_PI / 2)) * vecStep.y, sin(flAngles + (M_PI / 2)) * vecStep.y);
	vecStepRight = Vector2D(cos(flAngles) * vecStep.x, sin(flAngles) * vecStep.x);

	
	Vector2D vecIn, vecOut;
	vecOut.x = vecScissorSize.x * 0.5 - vecTile.x * vecStepRight.x - vecTile.y * vecStepUp.x + vecStartPosOffset.x;
	vecOut.y = vecScissorSize.y * 0.5 - vecTile.x * vecStepRight.y - vecTile.y * vecStepUp.y + vecStartPosOffset.y;
	float gx_g=m_pRect->value/2,gy_g=m_pRect->value/2;

	qglUseProgramObjectARB(program);
	qglUniform1iARB(uniform.base, 0);
	qglUniform2fARB(uniform.center, (float)(gx_g+gRadarx->value), (float)(gy_g +gRadary->value));
	qglUniform1fARB(uniform.radius, (float)(m_pRect->value/4));
	qglUniform1fARB(uniform.blurdist, 2.0);
	//gEngfuncs.Con_Printf("iRotate: %i",m_bIsRotate);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glBindTexture(GL_TEXTURE_2D,gRadarBG2);
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

	for (int i = 0; i < vecTitles.y; i++)
	{
		vecIn.x = vecOut.x;
		vecIn.y = vecOut.y;

		for (int j = 0; j < vecTitles.x; j++)	
		{
			gEngfuncs.pTriAPI->SpriteTexture(m_pSprite, iFrame);
			glColor4ub(255,255,255,200);
			//glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x8370 );
			//glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x8370 );
			glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex3f(gRadarx->value+vecIn.x ,gRadary->value+ vecIn.y, 0);
			glTexCoord2f(0, 1);
			glVertex3f(gRadarx->value+vecIn.x + vecStepRight.x, gRadary->value+ vecIn.y + vecStepRight.y, 0);
			glTexCoord2f(1, 1);
			glVertex3f(gRadarx->value+vecIn.x + vecStepRight.x + vecStepUp.x, gRadary->value+ vecIn.y + vecStepRight.y + vecStepUp.y,0);
			glTexCoord2f(1, 0);
			glVertex3f(gRadarx->value+vecIn.x + vecStepUp.x, gRadary->value+vecIn.y + vecStepUp.y, 0);
			glEnd();
			iFrame++;
			
			vecIn = vecIn + vecStepUp;
		}
		
		vecOut = vecOut + vecStepRight;
	}
	//m_pSurface->DrawSetTexture(m_iCircleID);
	//m_pSurface->DrawSetColor(255,255,255,255);
	//m_pSurface->DrawTexturedRect(gRadarx->value,gRadary->value,m_pRect->value+gRadarx->value,m_pRect->value+gRadary->value);
//	glDisable(GL_STENCIL_TEST);
	//glDisable(GL_ALPHA_TEST );
	
if (g_iVideoMode)
{
	qglUseProgramObjectARB(0);
	
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
}
	DrawFrame();
	PlayerESP();
		

	



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

void COverview::DrawNumRadar3(int gImage,float post[3],int r,int g ,int b, int a)
{
			float x,y;
			float origin1[2];
			float origin2[2];
			CalcPoint(post ,x,y);
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



void COverview::DrawNumRadar4(int gImage,float post[3],int r,int g ,int b, int a)
{
			float x,y;
			float origin1[2];
			float origin2[2];
			CalcPoint(post ,x,y);
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
		
			//m_pSurface->DrawSetTexture(gImage);
			//m_pSurface->DrawSetColor(r,g,b,a);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
			glBindTexture(GL_TEXTURE_2D,gImage);
			glColor4ub(r,g,b,a);
			gEngfuncs.pTriAPI->Begin(TRI_QUADS);
			gEngfuncs.pTriAPI->TexCoord2f(0, 0);
			gEngfuncs.pTriAPI->Vertex3f(gx, gy, 0.0);
			gEngfuncs.pTriAPI->TexCoord2f(0, 1);
			gEngfuncs.pTriAPI->Vertex3f(gx, gy+gw, 0.0);
			gEngfuncs.pTriAPI->TexCoord2f(1, 1);
			gEngfuncs.pTriAPI->Vertex3f(gx+gh, gy+gw, 0.0);
			gEngfuncs.pTriAPI->TexCoord2f(1, 0);
			gEngfuncs.pTriAPI->Vertex3f(gx+gw, gy, 0.0);
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
	return m_pSprite && gDrawRadar==1 && !gEngfuncs.GetLocalPlayer()->curstate.iuser1;
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
void Sys_ErrorEx(const char *error)
{
	if(g_pMetaSave->pEngineFuncs)
		g_pMetaSave->pEngineFuncs->pfnClientCmd("escape\n");
	MessageBox(NULL, error, "[GiayNhap]Error!!", MB_ICONERROR);
	exit(0);
}

qboolean GL_IsShaderError(GLuint shader, const char *filename)
{
	char compiler_log[1000];
	int blen;

	qglGetShaderiv(shader, GL_INFO_LOG_LENGTH , &blen);
	if(blen > 1)
	{
		int slen;
		qglGetInfoLogARB(shader, 1000, &slen, compiler_log);
		//if(!strstr(compiler_log, "error"))
		//{
		//	gEngfuncs.Con_Printf("Shader %s compiled with warning: %s\n", filename, compiler_log);
	//		return false;
		//}
		gEngfuncs.Con_Printf("Shader %s compiled with error: %s\n", filename, compiler_log);
		return false;
	}

	return false;
}

//编译着色器
GLuint R_CompileShader(const char *vscode, const char *fscode, const char *vsfile, const char *fsfile)
{
	GLuint vs = qglCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	qglShaderSourceARB(vs, 1, &vscode, NULL);
	qglCompileShaderARB(vs);
	if(GL_IsShaderError(vs, vsfile))
	{
		
		qglDeleteObjectARB(vs);
		return 0;
	}

	GLuint fs = qglCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	qglShaderSourceARB(fs, 1, &fscode, NULL);
	qglCompileShaderARB(fs);
	if(GL_IsShaderError(fs, fsfile))
	{
		
		qglDeleteObjectARB(fs);
		return 0;
	}	

	GLuint prog = qglCreateProgramObjectARB();
	qglAttachObjectARB(prog, vs);
	qglAttachObjectARB(prog, fs);
	qglLinkProgramARB(prog);

	return prog;
}

