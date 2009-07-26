/*
** Vertex Array Torus
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <assert.h>

#include <windows.h>
#include <GL/gl.h>

#define NUM_MAJOR 32
#define NUM_MINOR 24

#define X_MIN -1
#define X_MAX 1
#define Y_MIN -1
#define Y_MAX 1

char *className = "OpenGL";
char *windowName = "Vertex Array Torus";
int winX, winY;
int winWidth, winHeight;

#ifdef GL_SGI_cull_vertex
    PFNGLCULLPARAMETERFVSGIPROC	CullParameterfvSGI;
#endif

#ifdef GL_SGI_compiled_vertex_array
    PFNGLLOCKARRAYSSGIPROC		LockArraysSGI;
    PFNGLUNLOCKARRAYSSGIPROC		UnlockArraysSGI;
#endif

HDC hDC;
HGLRC hGLRC;
HPALETTE hPalette;
void (*idleFunc)(HDC hDC);
BOOL trackingMotion = FALSE;

HMENU hMainPup, hTexturePup, hVxCullPup, hLightPup, hOutlinePup, 
	hPmodePup, hDmodePup, hTexEnvPup, hShadePup, hDitherPup;

#define IDM_TEXTURE_ENABLED     100
#define IDM_TEXTURE_DISABLED    101
#define IDM_VXCULL_ENABLED     	102
#define IDM_VXCULL_DISABLED    	103
#define IDM_PMODE_TSTRIP	106
#define IDM_PMODE_QSTRIP	107
#define IDM_PMODE_LSTRIP	108
#define IDM_PMODE_TRIANGLES	109
#define IDM_PMODE_QUADS		110
#define IDM_PMODE_LINES		111
#define IDM_PMODE_POINTS	112
#define IDM_PMODE_LLOOP		113
#define IDM_PMODE_TFAN		114
#define IDM_PMODE_POLYGON	115
#define IDM_LIGHT_ENABLED     	120
#define IDM_LIGHT_DISABLED    	121
#define IDM_APPLICATION_EXIT    122
#define IDM_DMODE_VERTEX	125
#define IDM_DMODE_DRAW_ARRAYS	127
#define IDM_DMODE_DRAW_ELEMENTS	128
#define IDM_OUTLINE_ENABLED    	129
#define IDM_OUTLINE_DISABLED    130
#define IDM_TEXENV_REPLACE      131
#define IDM_TEXENV_MODULATE     132
#define IDM_TEXENV_BLEND        133
#define IDM_TEXENV_ADD          134
#define IDM_DITHER_ENABLED      140
#define IDM_DITHER_DISABLED     141
#define IDM_SHADE_FLAT          150
#define IDM_SHADE_SMOOTH        151

/* motion tracking globals */

GLfloat objectXform[4][4];
float angle, axis[3], trans[3];

BOOL up_pressed, down_pressed, left_pressed, right_pressed;

BOOL redrawContinue = FALSE;
BOOL objectSelected = FALSE;
BOOL colorIndexMode = FALSE;
enum MoveModes { MoveNone, MoveView, MoveObject, MovePlane };
enum MoveModes mode = MoveNone;


/* struct used to manage color ramps */
struct colorIndexState {
    GLfloat amb[3];	/* ambient color / bottom of ramp */
    GLfloat diff[3];	/* diffuse color / middle of ramp */
    GLfloat spec[3];	/* specular color / top of ramp */
    GLfloat ratio;	/* ratio of diffuse to specular in ramp */
    GLint indexes[3];	/* where ramp was placed in palette */
};

#define NUM_COLORS (sizeof(colors) / sizeof(colors[0]))
struct colorIndexState colors[] = {
    {
        { 0.0F, 0.0F, 0.0F },
        { 1.0F, 0.0F, 0.0F },
        { 1.0F, 1.0F, 1.0F },
        0.75F, { 0, 0, 0 },
    },
    {
        { 0.0F, 0.05F, 0.05F },
        { 0.9F, 0.0F, 1.0F },
        { 1.0F, 1.0F, 1.0F },
        1.0F, { 0, 0, 0 },
    },
    {
        { 0.0F, 0.0F, 0.0F },
        { 1.0F, 0.9F, 0.1F },
        { 1.0F, 1.0F, 1.0F },
        0.75F, { 0, 0, 0 },
    },
    {
        { 0.0F, 0.0F, 0.0F },
        { 0.1F, 1.0F, 0.9F },
        { 1.0F, 1.0F, 1.0F },
        0.75F, { 0, 0, 0 },
    },
};

GLint lit_tex_indexes[3];

/* vtorus globals */

#define M_PI 3.14159265F

void drawTorusVarray(void);
void drawTorusVarrayTex(void);
void drawTorus(void);

BOOL tracking = FALSE;
BOOL rgba = FALSE;

BOOL doTexture = FALSE;
int texenv = IDM_TEXENV_REPLACE;
BOOL doColors = FALSE;
BOOL doNormals = TRUE;
BOOL doLighting = TRUE;
BOOL ortho = TRUE;
BOOL localEye = FALSE;
BOOL vxCull = FALSE;
BOOL cull = TRUE;
BOOL frontBuffer = FALSE;
BOOL twoSided = FALSE;
BOOL lightLocalViewer = FALSE;
BOOL dither = TRUE;
BOOL shade = GL_SMOOTH;
BOOL doOutline = FALSE;
GLenum cullFace = GL_BACK;
BOOL vcache = TRUE;
void (*drawFunction)(void) = drawTorusVarray;

static int texWidth = 128, texHeight = 128;
static int needTexImage = TRUE;

int stride = 0;

static GLfloat yes[1] = {1.0F};
static GLfloat no[1] = {0.0F};

int curx = 0, cury = 0;
float lastPos[3] = {0.0F, 0.0F, 0.0F}; 
float angle = 0.0F;
float axis[3] = {0.0F, 0.0F, 0.0F};

GLfloat envColor[4] = { 0.5f, 0.3f, 0.75f, 1.0f };

void
matrixIdentity(GLfloat matrix[16])
{
    matrix[ 0] = 1.0F;
    matrix[ 1] = 0.0F;
    matrix[ 2] = 0.0F;
    matrix[ 3] = 0.0F;
    matrix[ 4] = 0.0F;
    matrix[ 5] = 1.0F;
    matrix[ 6] = 0.0F;
    matrix[ 7] = 0.0F;
    matrix[ 8] = 0.0F;
    matrix[ 9] = 0.0F;
    matrix[10] = 1.0F;
    matrix[11] = 0.0F;
    matrix[12] = 0.0F;
    matrix[13] = 0.0F;
    matrix[14] = 0.0F;
    matrix[15] = 1.0F;
}

#define MAX_ROWS 100
#define MAX_COLS 100
static GLuint elements[MAX_ROWS*MAX_COLS];

static GLenum vertexFormats[] = { GL_V3F, GL_N3F_V3F};
static GLenum texFormats[] = { GL_T2F_V3F, GL_T2F_N3F_V3F};
static GLenum colorFormats[] = { GL_C3F_V3F, GL_C4F_N3F_V3F};
static GLenum indexFormats[] = { GL_IUI_V3F_SGI, GL_IUI_N3F_V3F_SGI};
static int numRGBColors[] = { 3, 4 };

/* Elements can be up to T4, C4, N3, V4 */
static GLubyte interleavedData[(NUM_MAJOR+1)*NUM_MINOR*2*16*4];
static int interleavedDataSize = 0;
static GLenum interleavedFormat = 0;


/* Basic primitive mode globals */
static GLuint numRows, numCols;
static GLuint vertexesPerFrame = 0;
static GLuint primMode = 0;
static GLenum primModes[] = {
    GL_TRIANGLE_STRIP, 
    GL_QUAD_STRIP, 
    GL_LINE_STRIP,
    GL_TRIANGLES,
    GL_QUADS,
    GL_LINES,
    GL_POINTS,
    GL_LINE_LOOP,
    GL_TRIANGLE_FAN,
    GL_POLYGON,
};
static char *primModeStrings[] = {
    "GL_TRIANGLE_STRIP", 
    "GL_QUAD_STRIP", 
    "GL_LINE_STRIP",
    "GL_TRIANGLES",
    "GL_QUADS",
    "GL_LINES",
    "GL_POINTS",
    "GL_LINE_LOOP",
    "GL_TRIANGLE_FAN",
    "GL_POLYGON",
};
static int primModeIDMs[] = {
    IDM_PMODE_TSTRIP,
    IDM_PMODE_QSTRIP,
    IDM_PMODE_LSTRIP,
    IDM_PMODE_TRIANGLES,
    IDM_PMODE_QUADS,
    IDM_PMODE_LINES,
    IDM_PMODE_POINTS,
    IDM_PMODE_LLOOP,
    IDM_PMODE_TFAN,
    IDM_PMODE_POLYGON,
};
static GLuint numPrimModes = sizeof(primModes)/sizeof(GLenum);

/* Draw Modes */
typedef enum drawModeEnum {
    DM_Vertex = 0,
    DM_DrawArrays,
    DM_DrawElements,
} drawModeEnum;

static drawModeEnum drawModes[] = {
    DM_Vertex,
    DM_DrawArrays,
    DM_DrawElements,
};
static GLuint numDrawModes = sizeof(drawModes)/sizeof(drawModeEnum);
static GLuint drawMode = DM_DrawArrays;
static GLuint firstDrawMode = DM_Vertex, lastDrawMode = DM_DrawElements;
static char *drawModeStrings[] = {
    "glVertex",
    "DrawArrays",
    "DrawElements",
};
static int drawModeIDMs[] = {
    IDM_DMODE_VERTEX,
    IDM_DMODE_DRAW_ARRAYS,
    IDM_DMODE_DRAW_ELEMENTS,
};


int
getFormatSize(GLenum format)
{
    switch (format) {
	case GL_N3F_V3F:
	case GL_C3F_V3F:
	    return 24;
	case GL_T2F_N3F_V3F:
	    return 32;
	case GL_C4F_N3F_V3F:
	    return 40;
	case GL_T2F_V3F:
	    return 20;
	case GL_IUI_V3F_SGI:
	    return 16;
	case GL_IUI_N3F_V3F_SGI:
	    return 28;
    }
    return 0;
}

void
setCheckedTexture(void)
{
    int texSize;
    void *textureBuf;
    int i,j;

    if (!needTexImage) return;

    /* malloc for rgba as worst case */
    texSize = texWidth*texHeight*4;

    textureBuf = malloc(texSize);
    if (NULL == textureBuf) return;

    if (rgba) {
	GLubyte *p = (GLubyte *)textureBuf;
	for (i=0; i < texWidth; i++) {
	    for (j=0; j < texHeight; j++) {
		if ((i ^ j) & 8) {
		    p[0] = 0xff; p[1] = 0xff; p[2] = 0xff; p[3] = 0xff;
		} else {
		    p[0] = 0x08; p[1] = 0x08; p[2] = 0x08; p[3] = 0xff;
		}
		p += 4;
	    }
	}
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texWidth, texHeight, 
		    0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuf);
    } else {
	GLubyte *pBuf = (GLubyte *)textureBuf;
	GLubyte c0, c1;

	if (texenv == IDM_TEXENV_REPLACE) {
	    c0 = colors[0].indexes[1];
	    c1 = colors[2].indexes[2];
	} else {
	    c0 = colors[0].indexes[0] & 0xff;
	    c1 = colors[2].indexes[0] & 0xff;
	}

	for (i=0; i<texHeight; ++i) {
	    for (j=0; j<texWidth; ++j) {
		if ((i ^ j) & 8) {
		    *pBuf = c0;
		} else {
		    *pBuf = c1;
		}
		pBuf ++;
	    }
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_COLOR_INDEX8_EXT,
		     texWidth, texHeight, 0,
		      GL_COLOR_INDEX, GL_UNSIGNED_BYTE, textureBuf);
    }
    free(textureBuf);
    needTexImage = FALSE;
}

void
calcTorusData(void)
{
    static GLubyte *basePtr;
    int numMajor = NUM_MAJOR;
    int numMinor = NUM_MINOR;
    int padding;
    float majorRadius = 0.6F;
    float minorRadius = 0.2F;
    double majorStep = 2.0F*M_PI / numMajor;
    double minorStep = 2.0F*M_PI / numMinor;
    int i, j;
    int count;
    float red, green, blue, colorStep = 0.01F;
    float texS, texT, texStep_s, texStep_t;
    int formatSize;
    int colorOffset, normalOffset, vertexOffset;
    int numColors;

    formatSize = getFormatSize(interleavedFormat);
    assert(0 != formatSize);

    if (doColors) {
	numColors = rgba ? numRGBColors[doNormals] : 1;
    } else {
	numColors = 0;
    }

    colorOffset = doTexture ? 2*sizeof(GLfloat) : 0;
    assert(sizeof(GLfloat) == sizeof(GLuint));
    normalOffset = colorOffset + numColors*sizeof(GLfloat);
    vertexOffset = doNormals ? normalOffset + 3*sizeof(GLfloat) 
			     : normalOffset;

    stride = formatSize;
    padding = 0;

    if (doTexture) {
	texStep_s = 1.0F/numMajor;
	texStep_t = 1.0F/numMinor;
	texS = texT = 0.0F;
    }

    count = (numMajor+1)*(numMinor+1);
    for (i=0; i<count; i++) elements[i] = i;

    basePtr = interleavedData;

    for (i=0; i <= numMajor; ++i) {
	double a0;
	GLfloat x0;
	GLfloat y0;

	if (i == numMajor) {
	    a0 = 0;
	} else {
	    a0 = i * majorStep;
	} 
	x0 = (GLfloat)cos(a0);
	y0 = (GLfloat)sin(a0);

	if (doColors) {
	    if (rgba) {
		red = i%3 * 0.1F;
		green = (i+1)%3 * 0.1F;
		blue = (i+2)%3 * 0.1F;
	    } else {
		red = (GLfloat)(i & 0x7);
	    }
	} 

	for (j=0; j<=numMinor; j++) {
	    double b = j * minorStep;
	    GLfloat c = (GLfloat)cos(b);
	    GLfloat r = minorRadius * c + majorRadius;
	    GLfloat z = minorRadius * (GLfloat)sin(b);
	    GLfloat *vertexPtr;

	    if (doTexture) {
		GLfloat *texPtr = (GLfloat*)(basePtr);
		texPtr[0] = texS;
		texPtr[1] = texT;
	    }
	    if (doColors) {
		/* color */
		if (rgba) {
		    GLfloat *colorPtr = (GLfloat*)(basePtr+colorOffset);
		    colorPtr[0] = red;
		    colorPtr[1] = green;
		    colorPtr[2] = blue;
		    if (numColors > 3) {
			colorPtr[3] = 1.0F; /* alpha */
		    }
		    red = 1.0F-red; 
		} else {
		    GLuint *colorPtr = (GLuint*)(basePtr+colorOffset);
		    colorPtr[0] = (GLuint)red;
		}
	    }
	    if (doNormals) {
		GLfloat *normalPtr = (GLfloat*)(basePtr+normalOffset);
		normalPtr[0] = x0*c;
		normalPtr[1] = y0*c;
		normalPtr[2] = z/minorRadius;
	    } 
	    /* vertex */
	    vertexPtr = (GLfloat*)(basePtr+vertexOffset);
	    vertexPtr[0] = x0*r;
	    vertexPtr[1] = y0*r;
	    vertexPtr[2] = z;

	    if (doTexture) {
	       texT += texStep_t;
	    }

	    basePtr += stride;
	}
	if (doTexture) {
	   texS += texStep_s;
	   texT = 0.0F;
	}
    }
    interleavedDataSize = (numMajor+1)*numMinor*stride;
    vertexesPerFrame = numMajor*numMinor;
}


void userSettings(int doPrint)
{
    static GLfloat eyeDir[4] = {0.0F, 0.0F, 1.0F, 0.0F};
    static GLfloat eyePos[4] = {0.0F, 0.0F, 0.0F, 1.0F};

    /* if lighting or no colors or texture, need normals */
    if (doLighting || !(doColors || doTexture)) {
	doNormals = 1;
    }
    if (doColors) {
	doLighting = FALSE;
	doTexture = FALSE;
    }
    if ((DM_DrawArrays == drawModes[drawMode])) {
	drawMode = DM_DrawElements - firstDrawMode;
    }

    /* set up interleaved format */
    if (doColors) {
	if (rgba) {
	    interleavedFormat = colorFormats[doNormals];
	} else {
	    interleavedFormat = indexFormats[doNormals];
	}
    } else if (doTexture) {
	interleavedFormat = texFormats[doNormals];
    } else {
	interleavedFormat = vertexFormats[doNormals];
    }

    switch ( texenv )
    {
    case IDM_TEXENV_REPLACE:
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	break;
    case IDM_TEXENV_MODULATE:
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	break;
    case IDM_TEXENV_BLEND:
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );
	glTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, envColor );
	break;
    case IDM_TEXENV_ADD:
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
	break;
    }

    if ( dither )
        glEnable( GL_DITHER );
    else
        glDisable( GL_DITHER );

    if (twoSided) {
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    } else {
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
    }

    if (lightLocalViewer) {
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    } else {
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
    }

    if (doLighting) {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel( shade );
	if (!rgba) {
	    glIndexi(colors[0].indexes[1]);
	}
    } else {
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glShadeModel( shade );
	if (rgba) {
	    glColor3f(1.0F,1.0F,1.0F);
	} else {
	    /* this moves the value up the ramp from the texel */
	    glIndexi(colors[0].indexes[1] - colors[0].indexes[0]);
	}
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (ortho) {
	glOrtho(-1, 1, -1, 1, 1, 3);
    } else {
	glFrustum(-0.5, 0.5, -0.5, 0.5, 1, 3);
    }

#ifdef GL_SGI_cull_vertex
    if (CullParameterfvSGI) {
	if (localEye) {
	    (*CullParameterfvSGI)(GL_CULL_VERTEX_EYE_POSITION_SGI, eyePos);
	} else {
	    (*CullParameterfvSGI)(GL_CULL_VERTEX_EYE_POSITION_SGI, eyeDir);
	}
    }
#endif

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0F, 0.0F, -2.0F);

    calcTorusData();
    switch(drawModes[drawMode]) {
    case DM_DrawArrays: 		/* not supported yet */
    case DM_DrawElements:
	if (doTexture) {
	    drawFunction = drawTorusVarrayTex;
	} else {
	    drawFunction = drawTorusVarray;
	}
	break;
    case DM_Vertex:
	drawFunction = drawTorus;
	break;
    }


    if (frontBuffer) {
	glDrawBuffer(GL_FRONT);
    } else {
	glDrawBuffer(GL_BACK);
    }

    if (doTexture) {
	setCheckedTexture();
	glEnable(GL_TEXTURE_2D);
	glMaterialiv(GL_FRONT, GL_COLOR_INDEXES, lit_tex_indexes);
	glMaterialiv(GL_BACK, GL_COLOR_INDEXES, lit_tex_indexes);
    } else {
	glDisable(GL_TEXTURE_2D);
	glMaterialiv(GL_FRONT, GL_COLOR_INDEXES, colors[0].indexes);
	glMaterialiv(GL_BACK, GL_COLOR_INDEXES, colors[2].indexes);
    }

#ifdef GL_SGI_cull_vertex
    if (vxCull) {
	glEnable(GL_CULL_VERTEX_SGI);
    } else {
	glDisable(GL_CULL_VERTEX_SGI);
    }
#endif

    if (cull) {
	glEnable(GL_CULL_FACE);
	glCullFace(cullFace);
    } else {
	glDisable(GL_CULL_FACE);
    }

    if (doOutline) {
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(0.0f, 1.0f);
    } else {
	glDisable(GL_POLYGON_OFFSET_FILL);
    }

    {
	unsigned int i;
	MENUITEMINFO miinfo;

	miinfo.cbSize = sizeof(miinfo);
	miinfo.fMask = MIIM_STATE;

	miinfo.fState = MFS_UNCHECKED;
	SetMenuItemInfo( hTexEnvPup, IDM_TEXENV_REPLACE, FALSE, &miinfo);
	if (rgba) {
	    SetMenuItemInfo( hTexEnvPup, IDM_TEXENV_MODULATE, FALSE, &miinfo);
	    SetMenuItemInfo( hTexEnvPup, IDM_TEXENV_BLEND, FALSE, &miinfo);
	} else {
	    SetMenuItemInfo( hTexEnvPup, IDM_TEXENV_ADD, FALSE, &miinfo);
	}
	miinfo.fState = MFS_CHECKED;
	SetMenuItemInfo( hTexEnvPup, texenv, FALSE, &miinfo );

	miinfo.fState = MFS_UNCHECKED;
	SetMenuItemInfo( hShadePup, IDM_SHADE_FLAT, FALSE, &miinfo );
	SetMenuItemInfo( hShadePup, IDM_SHADE_SMOOTH, FALSE, &miinfo );
	miinfo.fState = MFS_CHECKED;
	if ( shade == GL_FLAT )
	    SetMenuItemInfo( hShadePup, IDM_SHADE_FLAT, FALSE, &miinfo );
	else
	    SetMenuItemInfo( hShadePup, IDM_SHADE_SMOOTH, FALSE, &miinfo );

	miinfo.fState = MFS_UNCHECKED;
	SetMenuItemInfo( hDitherPup, IDM_DITHER_ENABLED, FALSE, &miinfo );
	SetMenuItemInfo( hDitherPup, IDM_DITHER_DISABLED, FALSE, &miinfo );
	miinfo.fState = MFS_CHECKED;
	if ( dither )
	    SetMenuItemInfo( hDitherPup, IDM_DITHER_ENABLED, FALSE, &miinfo );
	else
	    SetMenuItemInfo( hDitherPup, IDM_DITHER_DISABLED, FALSE, &miinfo );

	if (vxCull) {
	    miinfo.fState = MFS_CHECKED;
	    SetMenuItemInfo(hVxCullPup, IDM_VXCULL_ENABLED, FALSE, &miinfo);
	    miinfo.fState = MFS_UNCHECKED;
	    SetMenuItemInfo(hVxCullPup, IDM_VXCULL_DISABLED, FALSE, &miinfo);
	} else {
	    miinfo.fState = MFS_CHECKED;
	    SetMenuItemInfo(hVxCullPup, IDM_VXCULL_DISABLED, FALSE, &miinfo);
	    miinfo.fState = MFS_UNCHECKED;
	    SetMenuItemInfo(hVxCullPup, IDM_VXCULL_ENABLED, FALSE, &miinfo);
	}
	if (doLighting) {
	    miinfo.fState = MFS_CHECKED;
	    SetMenuItemInfo(hLightPup, IDM_LIGHT_ENABLED, FALSE, &miinfo);
	    miinfo.fState = MFS_UNCHECKED;
	    SetMenuItemInfo(hLightPup, IDM_LIGHT_DISABLED, FALSE, &miinfo);
	} else {
	    miinfo.fState = MFS_CHECKED;
	    SetMenuItemInfo(hLightPup, IDM_LIGHT_DISABLED, FALSE, &miinfo);
	    miinfo.fState = MFS_UNCHECKED;
	    SetMenuItemInfo(hLightPup, IDM_LIGHT_ENABLED, FALSE, &miinfo);
	}
	if (doTexture) {
	    miinfo.fState = MFS_CHECKED;
	    SetMenuItemInfo(hTexturePup, IDM_TEXTURE_ENABLED, FALSE, &miinfo);
	    miinfo.fState = MFS_UNCHECKED;
	    SetMenuItemInfo(hTexturePup, IDM_TEXTURE_DISABLED, FALSE, &miinfo);
	} else {
	    miinfo.fState = MFS_CHECKED;
	    SetMenuItemInfo(hTexturePup, IDM_TEXTURE_DISABLED, FALSE, &miinfo);
	    miinfo.fState = MFS_UNCHECKED;
	    SetMenuItemInfo(hTexturePup, IDM_TEXTURE_ENABLED, FALSE, &miinfo);
	}
	if (doOutline) {
	    miinfo.fState = MFS_CHECKED;
	    SetMenuItemInfo(hOutlinePup, IDM_OUTLINE_ENABLED, FALSE, &miinfo);
	    miinfo.fState = MFS_UNCHECKED;
	    SetMenuItemInfo(hOutlinePup, IDM_OUTLINE_DISABLED, FALSE, &miinfo);
	} else {
	    miinfo.fState = MFS_CHECKED;
	    SetMenuItemInfo(hOutlinePup, IDM_OUTLINE_DISABLED, FALSE, &miinfo);
	    miinfo.fState = MFS_UNCHECKED;
	    SetMenuItemInfo(hOutlinePup, IDM_OUTLINE_ENABLED, FALSE, &miinfo);
	}
	for (i=0; i < numPrimModes; i++) {
	    miinfo.fState = (i == primMode) ? MFS_CHECKED : MFS_UNCHECKED;
	    SetMenuItemInfo(hPmodePup, primModeIDMs[i], FALSE, &miinfo);
	}
	for (i=0; i < numDrawModes; i++) {
	    miinfo.fState = (i == drawMode) ? MFS_CHECKED : MFS_UNCHECKED;
	    SetMenuItemInfo(hDmodePup, drawModeIDMs[i], FALSE, &miinfo);
	}
    }
}


void
drawTorusVarrayTex(void)
{
    int numMajor = NUM_MAJOR;
    int numMinor = NUM_MINOR;
    int i, j;
    int curElement = 0;
    int count;

    count = (numMajor+1)*(numMinor+1);

    glInterleavedArrays(interleavedFormat, stride, interleavedData);

#ifdef GL_SGI_compiled_vertex_array
    if (LockArraysSGI)
	(*LockArraysSGI)(0, count);
#endif

    {
	int numElements;
	int majorPos = 0, majorPos1 = 0;
	for (i=0; i<numMajor; ++i) {
	    numElements = 0;
	    for (j=0; j<=numMinor; ++j) {
		majorPos = i*(numMinor+1); /* this row */
		majorPos1 = majorPos + numMinor+1; /* next row */
		elements[numElements++] = majorPos + j;
		elements[numElements++] = majorPos1 + j;
	    }
	    glDrawElements(GL_TRIANGLE_STRIP, numElements, 
				GL_UNSIGNED_INT, elements);
	}
    }

#ifdef GL_SGI_compiled_vertex_array
    if (LockArraysSGI)
	(*UnlockArraysSGI)();
#endif
}

void
drawTorusVarray(void)
{
    int numMajor = NUM_MAJOR;
    int numMinor = NUM_MINOR;
    int i, j;
    int curElement = 0;
    int count;

    count = (numMajor+1)*(numMinor+1);

    glInterleavedArrays(interleavedFormat, stride, interleavedData);

#ifdef GL_SGI_compiled_vertex_array
    if (LockArraysSGI)
	(*LockArraysSGI)(0, count);
#endif

    {
	int numElements;
	int majorPos = 0, majorPos1 = 0;
	for (i=0; i<numMajor; ++i) {
	    numElements = 0;
	    for (j=0; j<=numMinor; j++) {
		majorPos = i*(numMinor+1); 		/* this row */
		majorPos1 = majorPos + numMinor+1; 	/* next row */
		elements[numElements++] = majorPos + j;
		elements[numElements++] = majorPos1 + j;
	    }
	    glDrawElements(GL_TRIANGLE_STRIP, numElements, 
				GL_UNSIGNED_INT, elements);
	}
    }

#ifdef GL_SGI_compiled_vertex_array
    if (LockArraysSGI)
	(*UnlockArraysSGI)();
#endif
}

void
drawTorus(void)
{
    int numMajor = NUM_MAJOR;
    int numMinor = NUM_MINOR;
    float majorRadius = 0.6F;
    float minorRadius = 0.2F;
    double majorStep = 2.0F*M_PI / numMajor;
    double minorStep = 2.0F*M_PI / numMinor;
    int i, j;

    for (i=0; i<numMajor; ++i) {
	double a0 = i * majorStep;
	double a1 = a0 + majorStep;
	GLfloat x0 = (GLfloat)cos(a0);
	GLfloat y0 = (GLfloat)sin(a0);
	GLfloat x1 = (GLfloat)cos(a1);
	GLfloat y1 = (GLfloat)sin(a1);

	glBegin(GL_TRIANGLE_STRIP);
	for (j=0; j<=numMinor; ++j) {
	    double b = j * minorStep;
	    GLfloat c = (GLfloat)cos(b);
	    GLfloat r = minorRadius * c + majorRadius;
	    GLfloat z = minorRadius * (GLfloat)sin(b);

	    glNormal3f(x0*c, y0*c, z/minorRadius);
	    glTexCoord2f(i/(GLfloat) numMajor, j/(GLfloat) numMinor);
	    glVertex3f(x0*r, y0*r, z);

	    glNormal3f(x1*c, y1*c, z/minorRadius);
	    glTexCoord2f((i+1)/(GLfloat) numMajor, j/(GLfloat) numMinor);
	    glVertex3f(x1*r, y1*r, z);
	}
	glEnd();
    }
}

void 
init(HDC hDC)
{
    GLfloat light0Pos[4] = { 0.70F, 0.70F, 1.25F, 0.00F };
    GLfloat matAmb[4] = { 0.01F, 0.01F, 0.01F, 1.00F };
    GLfloat matDiff[4] = { 0.65F, 0.05F, 0.20F, 0.60F };
    GLfloat matSpec[4] = { 0.50F, 0.50F, 0.50F, 1.00F };
    GLfloat matShine = 20.00F;

    /* Init extensions */
#ifdef GL_SGI_cull_vertex
    CullParameterfvSGI = (void *) wglGetProcAddress("glCullParameterfvSGI");
#endif

#ifdef GL_SGI_compiled_vertex_array
    LockArraysSGI = (void *) wglGetProcAddress("glLockArraysSGI");
    if (LockArraysSGI) {
	UnlockArraysSGI = (void *) wglGetProcAddress("glUnlockArraysSGI");
	assert(NULL != UnlockArraysSGI);
    } 
#endif

    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, matShine);
    if (!rgba) {
	glMaterialiv(GL_FRONT, GL_COLOR_INDEXES, colors[0].indexes);
	glMaterialiv(GL_BACK, GL_COLOR_INDEXES, colors[2].indexes);
	glClearIndex((GLfloat) (colors[1].indexes[0] + 
		(colors[1].indexes[1] - colors[1].indexes[0])/2));
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glEnable(GL_DEPTH_TEST);

    matrixIdentity((GLfloat *) objectXform);

    userSettings(1);

    glMatrixMode(GL_MODELVIEW);

}

void
resize(HDC hDC)
{
    glViewport(0, 0, winWidth, winHeight);
}

void
doRedraw(HDC hDC)
{
    if ( down_pressed )  trans[1] -= 0.05F;
    if ( up_pressed )    trans[1] += 0.05F;
    if ( left_pressed )  trans[0] -= 0.05F;
    if ( right_pressed ) trans[0] += 0.05F;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* update transformations */
    if (mode == MoveObject || mode == MoveView) {
	glPushMatrix();
	glLoadIdentity();
	glRotatef(angle, axis[0], axis[1], axis[2]);
	glMultMatrixf((GLfloat *) objectXform);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) objectXform);
	glPopMatrix();
    }
    glPushMatrix();
    glTranslatef( trans[0], trans[1], trans[2] );
    glMultMatrixf((GLfloat *) objectXform);

    (*drawFunction)();
    if (doOutline) {
	GLfloat color[4];
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (doLighting)
	    glDisable(GL_LIGHTING);
	if (doTexture)
	    glDisable(GL_TEXTURE_2D);
	if (rgba) {
	    glGetFloatv(GL_CURRENT_COLOR, color);
	    glColor3f(0.0f, 0.0f, 0.0f);
	} else {
	    glGetFloatv(GL_CURRENT_INDEX, color);
	    glIndexi(0);
	}
	(*drawFunction)();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (doLighting)
	    glEnable(GL_LIGHTING);
	if (doTexture)
	    glEnable(GL_TEXTURE_2D);
	if (rgba) {
	    glColor4fv(color);
	} else {
	    glIndexfv(color);
	}
    }
    glPopMatrix();

    SwapBuffers(hDC);

}

/*****************************************************************/

void
idleRedraw(HDC hDC)
{
    if (!redrawContinue) {
        idleFunc = NULL;
    }
    doRedraw(hDC);
}

void
redraw(HDC hDC)
{
    if (!idleFunc) {
	if (redrawContinue) {
	    idleFunc = idleRedraw;
	} else {
	    doRedraw(hDC);
	}
    }
}

/*****************************************************************/


/* these functions implement a simple trackball-like motion control */
float lastPos[3];
DWORD lastTime;
int startX, startY;

void
ptov(int x, int y, int width, int height, float v[3])
{
    float d, a;

    /* project x,y onto a hemi-sphere centered within width, height */
    v[0] = (2.0F*x - width) / width;
    v[1] = (height - 2.0F*y) / height;
    d = (float) sqrt(v[0]*v[0] + v[1]*v[1]);
    v[2] = (float) cos((M_PI/2.0F) * ((d < 1.0F) ? d : 1.0F));
    a = 1.0F / (float) sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] *= a;
    v[1] *= a;
    v[2] *= a;
}

void
startMotion(HDC hDC, DWORD time, int button, int x, int y)
{
    if (button == 1) {
	if (objectSelected) {
	    mode = MoveObject;
	} else {
	    mode = MoveView;
	}
    } else if (button == 2) {
	mode = MovePlane;
    } else {
	return;
    }

    trackingMotion = TRUE;
    redrawContinue = FALSE;
    startX = x;
    startY = y;
    ptov(x, y, winWidth, winHeight, lastPos);
}

void
stopMotion(HDC hDC, DWORD time, int button, int x, int y)
{
    if ((button == 1 && mode == MoveView) ||
	(button == 1 && mode == MoveObject) ||
	(button == 2 && mode == MovePlane))
    {
	trackingMotion = FALSE;
    } else {
	return;
    }

    if (startX != x || startY != y) {
	redrawContinue = TRUE;
    } else {
	angle = 0.0F;
	redrawContinue = FALSE;
    }
    if (!redrawContinue) {
	mode = MoveNone;
    }
    redraw(hDC);
}

void
trackMotion(HDC hDC, DWORD time, int x, int y)
{
    if (trackingMotion) {
	float curPos[3], dx, dy, dz;

	ptov(x, y, winWidth, winHeight, curPos);

	dx = curPos[0] - lastPos[0];
	dy = curPos[1] - lastPos[1];
	dz = curPos[2] - lastPos[2];
	angle = 90.0F * (float) sqrt(dx*dx + dy*dy + dz*dz);

	axis[0] = lastPos[1]*curPos[2] - lastPos[2]*curPos[1];
	axis[1] = lastPos[2]*curPos[0] - lastPos[0]*curPos[2];
	axis[2] = lastPos[0]*curPos[1] - lastPos[1]*curPos[0];

	lastPos[0] = curPos[0];
	lastPos[1] = curPos[1];
	lastPos[2] = curPos[2];
	redraw(hDC);
    }
}

/*****************************************************************/

void
setupPalette(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd;
    LOGPALETTE* pPal;
    int pixelFormat = GetPixelFormat(hDC);
    int paletteSize;

    (void) DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
    if (!(pfd.dwFlags & PFD_NEED_PALETTE || pfd.iPixelType == PFD_TYPE_COLORINDEX)) {
	return;
    }

    paletteSize = 1 << pfd.cColorBits;
    pPal = (LOGPALETTE*)
	malloc(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
    pPal->palVersion = 0x300;
    pPal->palNumEntries = paletteSize;

    /* start with a copy of the current system palette */
    (void) GetSystemPaletteEntries(hDC, 0, paletteSize, &pPal->palPalEntry[0]);

    if (pfd.iPixelType == PFD_TYPE_RGBA) {
	/* fill in an RGBA color palette */
	int redMask = (1 << pfd.cRedBits) - 1;
	int greenMask = (1 << pfd.cGreenBits) - 1;
	int blueMask = (1 << pfd.cBlueBits) - 1;
	int i;

	for (i=0; i<paletteSize; ++i) {
	    pPal->palPalEntry[i].peRed =
		    (((i >> pfd.cRedShift) & redMask) * 255) / redMask;
	    pPal->palPalEntry[i].peGreen =
		    (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
	    pPal->palPalEntry[i].peBlue =
		    (((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask;
	    pPal->palPalEntry[i].peFlags = 0;
	}
    } else {
	/* fill in a Color Index ramp color palette */
	int numRamps = NUM_COLORS;
	int rampSize = (paletteSize - 20) / numRamps;
	int extra = (paletteSize - 20) - (numRamps * rampSize);
	int i, r;

	for (r=0; r<numRamps; ++r) {
	    int rampBase = r * rampSize + 10;
	    PALETTEENTRY *pe = &pPal->palPalEntry[rampBase];
	    int diffSize = (int) (rampSize * colors[r].ratio);
	    int specSize = rampSize - diffSize;

	    for (i=0; i<rampSize; ++i) {
		GLfloat *c0, *c1;
		GLint a;

		if (i < diffSize) {
		    c0 = colors[r].amb;
		    c1 = colors[r].diff;
		    a = (i * 255) / (diffSize - 1);
		} else {
		    c0 = colors[r].diff;
		    c1 = colors[r].spec;
		    a = ((i - diffSize) * 255) / (specSize - 1);
		}

		pe[i].peRed = (BYTE) (a * (c1[0] - c0[0]) + 255 * c0[0]);
		pe[i].peGreen = (BYTE) (a * (c1[1] - c0[1]) + 255 * c0[1]);
		pe[i].peBlue = (BYTE) (a * (c1[2] - c0[2]) + 255 * c0[2]);
		pe[i].peFlags = PC_NOCOLLAPSE;
	    }

	    colors[r].indexes[0] = rampBase;
	    colors[r].indexes[1] = rampBase + (diffSize-1);
	    colors[r].indexes[2] = rampBase + (rampSize-1);
	}
	lit_tex_indexes[0] = 0;
	lit_tex_indexes[1] = (GLint)(rampSize*colors[0].ratio)-1;
	lit_tex_indexes[2] = rampSize-1;

	for (i=0; i<extra; ++i) {
	    int index = numRamps*rampSize+10+i;
	    PALETTEENTRY *pe = &pPal->palPalEntry[index];

	    pe->peRed = (BYTE) 0;
	    pe->peGreen = (BYTE) 0;
	    pe->peBlue = (BYTE) 0;
	    pe->peFlags = PC_NOCOLLAPSE;
	}
    }

    hPalette = CreatePalette(pPal);
    free(pPal);

    if (hPalette) {
	SelectPalette(hDC, hPalette, FALSE);
	RealizePalette(hDC);
    }
}

void
setupDC(HDC hDC)
{
    byte pixelType = rgba ? PFD_TYPE_RGBA : PFD_TYPE_COLORINDEX;
    PIXELFORMATDESCRIPTOR pfd = {
	sizeof(PIXELFORMATDESCRIPTOR),	/* size of this pfd */
	1,				/* version num */
	PFD_DRAW_TO_WINDOW |		/* support window */
	PFD_SUPPORT_OPENGL |		/* support OpenGL */
	PFD_DOUBLEBUFFER,		/* support double buffering */
	pixelType,			/* RGBA type */
	8,				/* 8-bit color depth */
	0, 0, 0, 0, 0, 0,		/* color bits (ignored) */
	0,				/* no alpha buffer */
	0,				/* alpha bits (ignored) */
	0,				/* no accumulation buffer */
	0, 0, 0, 0,			/* accum bits (ignored) */
	16,				/* 16-bit depth buffer */
	0,				/* no stencil buffer */
	0,				/* no auxiliary buffers */
	PFD_MAIN_PLANE,			/* main layer */
	0,				/* reserved */
	0, 0, 0,			/* no layer, visible, damage masks */
    };
    int SelectedPixelFormat;
    BOOL retVal;

    SelectedPixelFormat = ChoosePixelFormat(hDC, &pfd);
    if (SelectedPixelFormat == 0) {
	(void) MessageBox(WindowFromDC(hDC),
		"Failed to find acceptable pixel format.",
		"OpenGL application error",
		MB_ICONERROR | MB_OK);
	exit(1);
    }

    retVal = SetPixelFormat(hDC, SelectedPixelFormat, &pfd);
    if (retVal != TRUE) {
	(void) MessageBox(WindowFromDC(hDC),
		"Failed to set pixel format.",
		"OpenGL application error",
		MB_ICONERROR | MB_OK);
	exit(1);
    }
}

LRESULT APIENTRY
WndProc(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    int needRedraw = 0;

    /* Handle messages to which the application must respond */
    switch (message) {
    case WM_CREATE:
	hDC = GetDC(hWnd);
	setupDC(hDC);
	setupPalette(hDC);
	hGLRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hGLRC);
	init(hDC);
	return 0;
    case WM_DESTROY:
	if (hGLRC) {
	    wglMakeCurrent(NULL, NULL);
	    wglDeleteContext(hGLRC);
	}
	idleFunc = NULL;
	ReleaseDC(hWnd, hDC);
	PostQuitMessage(0);
	return 0;
    case WM_SIZE:
	if (hGLRC) {
	    winWidth = (int) LOWORD(lParam);
	    winHeight = (int) HIWORD(lParam);
	    resize(hDC);
	    return 0;
	}
	break;
    case WM_PALETTECHANGED:
	if (hPalette != NULL && (HWND) wParam != hWnd) {
	    UnrealizeObject(hPalette);
	    SelectPalette(hDC, hPalette, FALSE);
	    RealizePalette(hDC);
	    redraw(hDC);
	    return 0;
	}
	break;
    case WM_QUERYNEWPALETTE:
	if (hPalette != NULL) {
	    UnrealizeObject(hPalette);
	    SelectPalette(hDC, hPalette, FALSE);
	    RealizePalette(hDC);
	    redraw(hDC);
	    return TRUE;
	}
	break;
    case WM_PAINT:
	if (hGLRC) {
	    PAINTSTRUCT ps;
	    BeginPaint(hWnd, &ps);
	    redraw(hDC);
	    EndPaint(hWnd, &ps);
	    return 0;
	}
	break;
    case WM_LBUTTONDOWN:
	if (hGLRC) {
	    int x = (int) LOWORD(lParam);
	    int y = (int) HIWORD(lParam);
	    SetCapture(hWnd);
	    startMotion(hDC, 0, 1, x, y);
	    return 0;
	}
	break;
    case WM_LBUTTONUP:
	if (hGLRC) {
	    int x = (int) LOWORD(lParam);
	    int y = (int) HIWORD(lParam);
	    ReleaseCapture();
	    stopMotion(hDC, 0, 1, x, y);
	    return 0;
	}
	break;
    case WM_MOUSEMOVE:
	if (hGLRC) {
	    int x = (int) LOWORD(lParam);
	    int y = (int) HIWORD(lParam);
	    trackMotion(hDC, 0, x, y);
	}
	break;
    case WM_KEYDOWN:
	switch ( wParam )
	{
	case VK_LEFT:
	    left_pressed = TRUE;
            needRedraw = 1;
	    break;
	case VK_RIGHT:
	    right_pressed = TRUE;
            needRedraw = 1;
	    break;
	case VK_UP:
	    up_pressed = TRUE;
            needRedraw = 1;
	    break;
	case VK_DOWN:
	    down_pressed = TRUE;
            needRedraw = 1;
	    break;
	}
        if (needRedraw) {
            userSettings(0);
            doRedraw(hDC);
        }
	break;
    case WM_KEYUP:
	switch ( wParam )
	{
	case VK_LEFT:
	    left_pressed = FALSE;
            needRedraw = 1;
	    break;
	case VK_RIGHT:
	    right_pressed = FALSE;
            needRedraw = 1;
	    break;
	case VK_UP:
	    up_pressed = FALSE;
            needRedraw = 1;
	    break;
	case VK_DOWN:
	    down_pressed = FALSE;
            needRedraw = 1;
	    break;
	}
        if (needRedraw) {
            userSettings(0);
            doRedraw(hDC);
        }
	break;
    case WM_CHAR:	
	switch ((int)wParam) {
	case 27: /* Escape */
	    PostQuitMessage(0);
	    break;
	}
	if (needRedraw) {
	    userSettings(1);
	    doRedraw(hDC);
	}
	return 0;
	break;
    case WM_CONTEXTMENU:
	{
	    TrackPopupMenu(hMainPup, TPM_LEFTALIGN | TPM_RIGHTBUTTON,
	    		LOWORD(lParam), HIWORD(lParam), 0, hWnd, NULL);
	    break;
        }
    case WM_COMMAND:
	{
	    switch (LOWORD(wParam)) {
	    case IDM_DITHER_ENABLED:
		dither = TRUE;
		userSettings( 1 );
		redraw( hDC );
		break;
	    case IDM_DITHER_DISABLED:
		dither = FALSE;
		userSettings( 1 );
		redraw( hDC );
		break;
	    case IDM_SHADE_FLAT:
		shade = GL_FLAT;
		userSettings( 1 );
		redraw( hDC );
		break;
	    case IDM_SHADE_SMOOTH:
		shade = GL_SMOOTH;
		userSettings( 1 );
		redraw( hDC );
		break;
	    case IDM_TEXTURE_DISABLED:
		doTexture = FALSE;
		userSettings(1);
		redraw(hDC);
		break;
	    case IDM_TEXTURE_ENABLED:
		doTexture = TRUE;
		userSettings(1);
		redraw(hDC);
		break;
	    case IDM_VXCULL_DISABLED:
		vxCull = FALSE;
		userSettings(1);
		redraw(hDC);
		break;
	    case IDM_VXCULL_ENABLED:
		vxCull = TRUE;
		userSettings(1);
		redraw(hDC);
		break;
	    case IDM_LIGHT_DISABLED:
		doLighting = FALSE;
		userSettings(1);
		redraw(hDC);
		break;
	    case IDM_LIGHT_ENABLED:
		doLighting = TRUE;
		userSettings(1);
		redraw(hDC);
		break;
	    case IDM_PMODE_TSTRIP:
	    case IDM_PMODE_QSTRIP:
	    case IDM_PMODE_LSTRIP:
	    case IDM_PMODE_TRIANGLES:
	    case IDM_PMODE_QUADS:
	    case IDM_PMODE_LINES:
	    case IDM_PMODE_POINTS:
	    case IDM_PMODE_LLOOP:
	    case IDM_PMODE_TFAN:
	    case IDM_PMODE_POLYGON:
		primMode = LOWORD(wParam) - IDM_PMODE_TSTRIP;
		userSettings(1);
		redraw(hDC);
		break;
	    case IDM_DMODE_DRAW_ARRAYS:
	    case IDM_DMODE_DRAW_ELEMENTS:
	    case IDM_DMODE_VERTEX:
		drawMode = LOWORD(wParam) - drawModeIDMs[firstDrawMode];
		userSettings(1);
		redraw(hDC);
		break;
	    case IDM_OUTLINE_ENABLED:
		doOutline = TRUE;
		userSettings(1);
		redraw(hDC);
		break;
	    case IDM_OUTLINE_DISABLED:
		doOutline = FALSE;
		userSettings(1);
		redraw(hDC);
		break;
	    case IDM_TEXENV_REPLACE:
	    case IDM_TEXENV_MODULATE:
	    case IDM_TEXENV_ADD:
	    case IDM_TEXENV_BLEND:
		/* In Color Index mode, change the texture if the environment
		 * changes!
		 */
		if (!rgba && texenv != (int)wParam) {
		    needTexImage = TRUE;
		}
		texenv = wParam;
		userSettings( 1 );
		redraw( hDC );
		break;
	    case IDM_APPLICATION_EXIT:
		PostQuitMessage(0);
		break;
	    }
	}
	break;
    default:
	break;
    }

    /* Make sure all messages get returned to Windows. */
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int APIENTRY
WinMain(
    HINSTANCE hCurrentInst,
    HINSTANCE hPreviousInst,
    LPSTR lpszCmdLine,
    int nCmdShow)
{
    WNDCLASS wndClass;
    HWND hWnd;
    MSG msg;

    /* Define and register the window class */
    wndClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hCurrentInst;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = className;

    RegisterClass(&wndClass);

    {
      HDC hdc = GetDC( 0 );
      int bpp = GetDeviceCaps( hdc, BITSPIXEL );

      if ( bpp <= 8 ) rgba = 0;
      else rgba = 1;

      ReleaseDC( 0, hdc );
    }

    /* Get the size of the screen */
    winWidth = GetSystemMetrics(SM_CYSCREEN) / 3;
    winHeight = GetSystemMetrics(SM_CYSCREEN) / 3;

    /* Create a window of the previously defined class */
    hWnd = CreateWindow(
	className,		/* Window class's name */
	windowName,		/* Title bar text */
	WS_OVERLAPPEDWINDOW |	/* The window's style */
	WS_CLIPCHILDREN |
	WS_CLIPSIBLINGS,
	winX, winY,		/* X position */
	winWidth, winHeight,	/* Size */
	NULL,			/* Parent window's handle */
	NULL,			/* Menu handle */
	hCurrentInst,		/* Instance handle */
	NULL);			/* No additional data */

    /* Create a popup menu */
    {
	unsigned int i;
	hMainPup = CreatePopupMenu();

	hTexturePup = CreatePopupMenu();
	AppendMenu(hTexturePup, MF_STRING, IDM_TEXTURE_ENABLED, "Enabled");
	AppendMenu(hTexturePup, MF_STRING, IDM_TEXTURE_DISABLED, "Disabled");
	AppendMenu(hMainPup, MF_POPUP|MF_STRING, (UINT) hTexturePup, "texture");

	hTexEnvPup = CreatePopupMenu();
	AppendMenu( hTexEnvPup, MF_STRING, IDM_TEXENV_REPLACE, "Replace" );
	AppendMenu( hTexEnvPup, MF_STRING, IDM_TEXENV_MODULATE, "Modulate" );
	AppendMenu( hTexEnvPup, MF_STRING, IDM_TEXENV_BLEND, "Blend" );
	AppendMenu( hTexEnvPup, MF_STRING, IDM_TEXENV_ADD, "Add" );
	AppendMenu( hMainPup, MF_POPUP|MF_STRING, (UINT) hTexEnvPup, "tex env" );
	if (rgba) {
	    EnableMenuItem(hTexEnvPup, IDM_TEXENV_ADD,
			   MF_BYCOMMAND|MF_GRAYED);
	} else {
	    EnableMenuItem(hTexEnvPup, IDM_TEXENV_MODULATE,
			   MF_BYCOMMAND|MF_GRAYED);
	    EnableMenuItem(hTexEnvPup, IDM_TEXENV_BLEND,
			   MF_BYCOMMAND|MF_GRAYED);
	}

	hVxCullPup = CreatePopupMenu();
	AppendMenu(hVxCullPup, MF_STRING, IDM_VXCULL_ENABLED, "Enabled");
	AppendMenu(hVxCullPup, MF_STRING, IDM_VXCULL_DISABLED, "Disabled");
	AppendMenu(hMainPup, MF_POPUP|MF_STRING, (UINT) hVxCullPup, "vx cull");

	hLightPup = CreatePopupMenu();
	AppendMenu(hLightPup, MF_STRING, IDM_LIGHT_ENABLED, "Enabled");
	AppendMenu(hLightPup, MF_STRING, IDM_LIGHT_DISABLED, "Disabled");
	AppendMenu(hMainPup, MF_POPUP|MF_STRING, (UINT) hLightPup, "lighting");

	hShadePup = CreatePopupMenu();
	AppendMenu( hShadePup, MF_STRING, IDM_SHADE_FLAT, "Flat" );
	AppendMenu( hShadePup, MF_STRING, IDM_SHADE_SMOOTH, "Smooth" );
	AppendMenu( hMainPup, MF_POPUP|MF_STRING, (UINT) hShadePup, "shade model" );

	hOutlinePup = CreatePopupMenu();
	AppendMenu(hOutlinePup, MF_STRING, IDM_OUTLINE_ENABLED, "Enabled");
	AppendMenu(hOutlinePup, MF_STRING, IDM_OUTLINE_DISABLED, "Disabled");
	AppendMenu(hMainPup, MF_POPUP|MF_STRING, (UINT) hOutlinePup, "outline");

	hPmodePup = CreatePopupMenu();
	for (i=0; i < numPrimModes; i++) {
	    AppendMenu(hPmodePup, MF_STRING, 
			primModeIDMs[i], primModeStrings[i]);
	}
	AppendMenu(hMainPup, MF_POPUP|MF_STRING, (UINT) hPmodePup, "prim mode");

	hDmodePup = CreatePopupMenu();
	for (i=0; i < numDrawModes; i++) {
	    AppendMenu(hDmodePup, MF_STRING, 
			drawModeIDMs[i], drawModeStrings[i]);
	}
	AppendMenu(hMainPup, MF_POPUP|MF_STRING, (UINT) hDmodePup, "draw mode");

	hDitherPup = CreatePopupMenu();
	AppendMenu( hDitherPup, MF_STRING, IDM_DITHER_ENABLED, "Enabled" );
	AppendMenu( hDitherPup, MF_STRING, IDM_DITHER_DISABLED, "Disabled" );
	AppendMenu( hMainPup, MF_POPUP|MF_STRING, (UINT) hDitherPup, "dither" );

	AppendMenu(hMainPup, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMainPup, MF_STRING, IDM_APPLICATION_EXIT, "Exit");
    }
    userSettings(1);

    /* Map the window to the screen */
    ShowWindow(hWnd, nCmdShow);

    /* Force the window to repaint itself */
    UpdateWindow(hWnd);

    /* Message loop */
    while (1) {
	while (idleFunc &&
	       PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == FALSE)
	{
	    (*idleFunc)(hDC);
	}
	if (GetMessage(&msg, NULL, 0, 0) != TRUE) {
	    return msg.wParam;
	}
	TranslateMessage(&msg);
	DispatchMessage(&msg);
    }
}

