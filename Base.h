#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include <interface.h>
#include <metahook.h>
#include <Gdiplus.h>
#include <event_api.h>
#include "calcscreen.h"
#include "HLSDK\engine\cdll_int.h"
#include "usercmd.h"
#include <ref_params.h>
#include <VGUI\ISurface.h>
#include <r_studioint.h>
#include <math.h>
#include "vector.h"

#include <gl/gl.h>
//#include <glext.h>

#include <const.h>
#include <triangleapi.h>
#include <cl_entity.h>
#include <ref_params.h>
#include <cvardef.h>
#include <com_model.h>
#include <studio.h>
#include "exportfuncs.h"
#include "engfuncs.h"
#include "r_efx.h"
#include "overview.h"  
#include "msghook.h"
#include <assert.h>

#include <locale>
#include <codecvt>
#include <math.h>

#include <wrect.h>

typedef float vec_t;
typedef float vec2_t[2];
typedef float vec3_t[3];
