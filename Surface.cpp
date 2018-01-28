#include "surface.h"

vgui::ISurface *g_pSurface;

void Surface_Init(vgui::ISurface *pSurface)
{
	g_pSurface = pSurface;
}