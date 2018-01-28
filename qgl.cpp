#include <metahook.h>
#include <gl/gl.h>
#include <triangleapi.h>
#include "plugins.h"
#include "qgl.h"

void QGL_Init(void)
{
	if (g_iVideoMode != VIDEOMODE_D3D)
	{
		HMODULE hOpenGL = GetModuleHandle("opengl32.dll");

		if (hOpenGL)
		{
			qglAccum = (void (APIENTRY *)(GLenum, GLfloat))GetProcAddress(hOpenGL, "glAccum");
			qglAlphaFunc = (void (APIENTRY *)(GLenum, GLclampf))GetProcAddress(hOpenGL, "glAlphaFunc");
			qglAreTexturesResident = (GLboolean (APIENTRY *)(GLsizei, const GLuint *, GLboolean *))GetProcAddress(hOpenGL, "glAreTexturesResident");
			qglBlendFunc = (void (APIENTRY *)(GLenum, GLenum))GetProcAddress(hOpenGL, "glBlendFunc");
			qglTexImage2D = (void (APIENTRY *)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *))GetProcAddress(hOpenGL, "glTexImage2D");
			qglGenTextures = (void (APIENTRY *)(GLsizei, GLuint *))GetProcAddress(hOpenGL, "glGenTextures");
			qglBindTexture = (void (APIENTRY *)(GLenum, GLuint))GetProcAddress(hOpenGL, "glBindTexture");
			qglTexParameteri = (void (APIENTRY *) (GLenum, GLenum, GLint))GetProcAddress(hOpenGL, "glTexParameteri");
			qglTexImage2D = (void (APIENTRY *)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *))GetProcAddress(hOpenGL, "glTexImage2D");
			qglIsTexture = (GLboolean (APIENTRY *)(GLuint))GetProcAddress(hOpenGL, "glIsTexture");
			qglGetIntegerv = (void (APIENTRY *)(GLenum, GLint *))GetProcAddress(hOpenGL, "glGetIntegerv");
			qglDrawElements = (void (APIENTRY *)(GLenum, GLsizei, GLenum, const GLvoid *))GetProcAddress(hOpenGL, "glDrawElements");
			qglDrawArrays = (void (APIENTRY *)(GLenum, GLint, GLsizei))GetProcAddress(hOpenGL, "glDrawArrays");
			qglEnable = (void (APIENTRY *) (GLenum))GetProcAddress(hOpenGL, "glEnable");
			qglDisable = (void (APIENTRY *) (GLenum))GetProcAddress(hOpenGL, "glDisable");
			qglPushAttrib = (void (APIENTRY *)(GLbitfield))GetProcAddress(hOpenGL, "glPushAttrib");
			qglPopAttrib = (void (APIENTRY *)(void))GetProcAddress(hOpenGL, "glPopAttrib");
			qglDeleteTextures = (void (APIENTRY *)(GLsizei, const GLuint *))GetProcAddress(hOpenGL, "glDeleteTextures");
			qglTexSubImage2D = (void (APIENTRY *)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *))GetProcAddress(hOpenGL, "glTexSubImage2D");
			qglVertex2f = (void (APIENTRY *)(GLfloat, GLfloat))GetProcAddress(hOpenGL, "glVertex2f");
			qglEnd = (void (APIENTRY *)(void))GetProcAddress(hOpenGL, "glEnd");
			qglTexEnvf = (void (APIENTRY *)(GLenum, GLenum, GLfloat))GetProcAddress(hOpenGL, "glTexEnvf");
			qglColor4f = (void (APIENTRY *)(GLfloat, GLfloat, GLfloat, GLfloat))GetProcAddress(hOpenGL, "glColor4f");
			qglColor3f = (void (APIENTRY *)(GLfloat, GLfloat, GLfloat))GetProcAddress(hOpenGL, "glColor3f");
			qglGetTexImage = (void (APIENTRY *)(GLenum, GLint, GLenum, GLenum, GLvoid *))GetProcAddress(hOpenGL, "glGetTexImage");
			qglScissor = (void (APIENTRY *)(GLint, GLint, GLsizei, GLsizei))GetProcAddress(hOpenGL, "glScissor");
			qglColor4ub = (void (APIENTRY *)(GLubyte, GLubyte, GLubyte, GLubyte))GetProcAddress(hOpenGL, "glColor4ub");
			qglBegin = (void (APIENTRY *)(GLenum))GetProcAddress(hOpenGL, "glBegin");
			qglTexCoord2f = (void (APIENTRY *)(GLfloat, GLfloat))GetProcAddress(hOpenGL, "glTexCoord2f");
			//found
			qglVertex3f = (void (APIENTRY *)(GLfloat, GLfloat, GLfloat))GetProcAddress(hOpenGL, "glVertex3f");
			qglPushMatrix = (void (APIENTRY *)(GLvoid))GetProcAddress(hOpenGL, "glPushMatrix");
			qglTranslatef = (void (APIENTRY *)(GLfloat, GLfloat, GLfloat))GetProcAddress(hOpenGL, "glTranslatef");
			qglRotatef = (void (APIENTRY *)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z))GetProcAddress(hOpenGL, "glRotatef");
			qglPopMatrix = (void (APIENTRY *)(GLvoid))GetProcAddress(hOpenGL, "glPopMatrix");
			qglCullFace = (void (APIENTRY *)(GLenum mode))GetProcAddress(hOpenGL, "glCullFace");
			qglShadeModel = (void (APIENTRY *)(GLenum mode))GetProcAddress(hOpenGL, "glShadeModel");
			qglDepthRange = (void (APIENTRY *)(GLclampd zNear, GLclampd zFar))GetProcAddress(hOpenGL, "glDepthRange");

		}
	}

	DWORD base = *(DWORD *)((DWORD)gEngfuncs.pTriAPI->RenderMode + 0x47);

	qglAccum = (void (APIENTRY *)(GLenum, GLfloat))(*(DWORD *)(base - 0xB4));
	qglAlphaFunc = (void (APIENTRY *)(GLenum, GLclampf))(*(DWORD *)(base - 0x374));
	qglAreTexturesResident = (GLboolean (APIENTRY *)(GLsizei, const GLuint *, GLboolean *))(*(DWORD *)(base - 0x3A8));
	qglBlendFunc = (void (APIENTRY *)(GLenum, GLenum))(*(DWORD *)base);
	qglTexImage2D = (void (APIENTRY *)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *))(*(DWORD *)(base - 0x24C));
	qglGenTextures = (void (APIENTRY *)(GLsizei, GLuint *))(*(DWORD *)(base - 0x6C));
	qglBindTexture = (void (APIENTRY *)(GLenum, GLuint))(*(DWORD *)(base - 0x224));
	qglTexParameteri = (void (APIENTRY *) (GLenum, GLenum, GLint))(*(DWORD *)(base - 0x524));
	qglTexImage2D = (void (APIENTRY *)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *))(*(DWORD *)(base + 0x1EC));
	qglIsTexture = (GLboolean (APIENTRY *)(GLuint))(*(DWORD *)(base + 0x250));
	qglGetIntegerv = (void (APIENTRY *)(GLenum, GLint *))(*(DWORD *)(base + 0x3C0));
	qglDrawElements = (void (APIENTRY *)(GLenum, GLsizei, GLenum, const GLvoid *))(*(DWORD *)(base - 0x4C0));
	qglDrawArrays = (void (APIENTRY *)(GLenum, GLint, GLsizei))(*(DWORD *)(base + 0X270));
	qglEnable = (void (APIENTRY *) (GLenum))(*(DWORD *)(base - 0x4BC));
	qglDisable = (void (APIENTRY *) (GLenum))(*(DWORD *)(base + 0x380));
	qglPushAttrib = (void (APIENTRY *)(GLbitfield))(*(DWORD *)(base - 0x1C));
	qglPopAttrib = (void (APIENTRY *)(void))(*(DWORD *)(base + 0xF8));
	qglDeleteTextures = (void (APIENTRY *)(GLsizei, const GLuint *))(*(DWORD *)(base + 0x110));
	qglTexSubImage2D = (void (APIENTRY *)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *))(*(DWORD *)(base + 0x44));
	qglVertex2f = (void (APIENTRY *)(GLfloat, GLfloat))(*(DWORD *)(base + 0x36C));
	qglEnd = (void (APIENTRY *)(void))(*(DWORD *)(base - 0x428));
	qglTexEnvf = (void (APIENTRY *)(GLenum, GLenum, GLfloat))(*(DWORD *)(base - 0x564));
	qglColor4f = (void (APIENTRY *)(GLfloat, GLfloat, GLfloat, GLfloat))(*(DWORD *)(base - 0x468));
	qglColor3f = (void (APIENTRY *)(GLfloat, GLfloat, GLfloat))(*(DWORD *)(base - 0x48C));
	qglGetTexImage = (void (APIENTRY *)(GLenum, GLint, GLenum, GLenum, GLvoid *))(*(DWORD *)(base - 0x3D4));
	qglScissor = (void (APIENTRY *)(GLint, GLint, GLsizei, GLsizei))(*(DWORD *)(base + 0x294));
	qglColor4ub = (void (APIENTRY *)(GLubyte, GLubyte, GLubyte, GLubyte))(*(DWORD *)(base + 0x1E8));

	if (g_dwEngineBuildnum < 4554)
	{
		qglBegin = (void (APIENTRY *)(GLenum))(*(DWORD *)(base + 0x410));
		qglTexCoord2f = (void (APIENTRY *)(GLfloat, GLfloat))(*(DWORD *)(base + 0x46C));
	}
	else
	{
		qglBegin = (void (APIENTRY *)(GLenum))(*(DWORD *)(base + 0x414));
		qglTexCoord2f = (void (APIENTRY *)(GLfloat, GLfloat))(*(DWORD *)(base + 0x474));
	}
}