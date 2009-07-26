/* indexmat.c */

/*
 * Example/test of the GL_SGI_index_material extension
 *
 * This extension adds the GL_INDEX_OFFSET and GL_INDEX_SHIFT
 * material parameters for CI mode lighting.  With this feature
 * we can combine fog and lighting (for example) in color index
 * mode.
 *
 * Also, we can use glEnable(GL_INDEX_MATERIAL_SGI) and
 * glIndexMaterialSGI() to modify the material's GL_INDEX_OFFSET
 * with the glIndex() command.  This is like glColorMaterial
 * in RGB mode.
 *
 * This demo shows two spinning dodecahedrons using two materials
 * selected via glIndex().
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include "GL/gl.h"

/* Have to access glIndexMaterialSGI through a pointer, ugh! */
static PFNGLINDEXMATERIALSGIPROC glIndexMaterialSGIptr;

char *className = "OpenGL";
char *windowName = "Index Material";
int winX = 0, winY = 0;
int winWidth = 300, winHeight = 300;

HDC hDC;
HGLRC hGLRC;
HPALETTE hPalette;

void (*idleFunc)(void);

/* Struct used to manage color ramps */
struct colorIndexState {
    GLfloat amb[3];	/* ambient color / bottom of ramp */
    GLfloat diff[3];	/* diffuse color / middle of ramp */
    GLfloat spec[3];	/* specular color / top of ramp */
    GLfloat ratio;	/* ratio of diffuse to specular in ramp */
    GLint indexes[3];	/* where ramp was placed in palette */
};

/*
** Each entry in this array corresponds to a color ramp in the
** palette.  The indexes member of each struct is updated to
** reflect the placement of the color ramp in the palette.
*/
#define NUM_COLORS (sizeof(colors) / sizeof(colors[0]))
struct colorIndexState colors[] = {
    {
        { 0.0F, 0.0F, 0.0F },	/* red */
        { 1.0F, 0.0F, 0.0F },
        { 1.0F, 1.0F, 1.0F },
        0.75F, { 0, 0, 0 },
    },
    {
        { 0.0F, 0.0F, 0.0F },	/* green */
        { 0.0F, 1.0F, 0.0F },
        { 1.0F, 1.0F, 1.0F },
        0.75F, { 0, 0, 0 },
    },
};

static GLfloat Yrot = 0.0F;

static GLboolean
checkExtension(const char *name)
{
    const char *p = (const char *) glGetString(GL_EXTENSIONS);

    while (p = strstr(p, name)) {
	const char *q = p + strlen(name);

	if (*q == ' ' || *q == '\0') {
	    return GL_TRUE;
	}
	p = q;
    }
    return GL_FALSE;
}

static void
checkError(char *msg)
{
    GLenum err;

    while ((err = glGetError()) != GL_NO_ERROR) {
	printf("Error %d at %s\n", err, msg);
    }
}

static void
init(void)
{
    GLfloat spec[4] = {0.2F, 0.2F, 0.2F, 1.0F};
    GLfloat dif[4]  = {0.8F, 0.8F, 0.8F, 1.0F};
    GLfloat amb[4]  = {0.2F, 0.2F, 0.2F, 1.0F};
    GLint indexes[3];
    GLint i;

    /* Note: indexes are zero based */
    indexes[0] = 0;
    indexes[1] = colors[0].indexes[1] - colors[0].indexes[0];
    indexes[2] = colors[0].indexes[2] - colors[0].indexes[0];

    glMaterialiv( GL_FRONT_AND_BACK, GL_COLOR_INDEXES, indexes );

    glLightfv( GL_LIGHT0, GL_DIFFUSE, dif );
    glLightfv( GL_LIGHT0, GL_AMBIENT, amb );
    glLightfv( GL_LIGHT0, GL_SPECULAR, spec );

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, 1 );

    /* index material stuff */
    glIndexMaterialSGIptr = (void *) wglGetProcAddress("glIndexMaterialSGI");
    if (!glIndexMaterialSGIptr) {
        printf("Error: renderer doesn't support GL_SGI_index_material\n");
        exit(1);
    }

    /* front and back index offset tracks glIndex(): */
    (*glIndexMaterialSGIptr)( GL_FRONT_AND_BACK, GL_INDEX_OFFSET );
    glEnable( GL_INDEX_MATERIAL_SGI );

    /* test attrib pushing/popping */
    glPushAttrib( GL_LIGHTING_BIT );
    glDisable( GL_INDEX_MATERIAL_SGI );
    (*glIndexMaterialSGIptr)( GL_BACK, GL_INDEX_OFFSET );
    glPopAttrib();

    glGetIntegerv( GL_INDEX_MATERIAL_FACE_SGI, &i );
    assert( i==GL_FRONT_AND_BACK );
    glGetIntegerv( GL_INDEX_MATERIAL_PARAMETER_SGI, &i );
    assert( i==GL_INDEX_OFFSET );
    assert( glIsEnabled(GL_INDEX_MATERIAL_SGI) );

    /* temporary:
    glMateriali( GL_FRONT_AND_BACK, GL_INDEX_SHIFT, -1 );
    glCullFace( GL_FRONT );
    */

    glEnable( GL_DEPTH_TEST );
    checkError("Checkpoint");
}

void
drawDodecahedron(void)
{
#define A (1.61803F)	/* (sqrt(5) + 1) / 2 */
#define B (0.61803F)	/* (sqrt(5) - 1) / 2 */
#define C (1.0F)
    GLfloat vertexes[20][3] = {
	{-A, 0.0F, B }, {-A, 0.0F,-B }, { A, 0.0F,-B }, { A, 0.0F, B },
	{ B,-A, 0.0F }, {-B,-A, 0.0F }, {-B, A, 0.0F }, { B, A, 0.0F },
	{ 0.0F, B,-A }, { 0.0F,-B,-A }, { 0.0F,-B, A }, { 0.0F, B, A },
	{-C,-C, C }, {-C,-C,-C }, { C,-C,-C }, { C,-C, C },
	{-C, C, C }, {-C, C,-C }, { C, C,-C }, { C, C, C },
    };
#undef A
#undef B
#undef C
    GLint polygons[12][5] = {
	{  0, 12, 10, 11, 16 },
	{  1, 17,  8,  9, 13 },
	{  2, 14,  9,  8, 18 },
	{  3, 19, 11, 10, 15 },
	{  4, 14,  2,  3, 15 },
	{  5, 12,  0,  1, 13 },
	{  6, 17,  1,  0, 16 },
	{  7, 19,  3,  2, 18 },
	{  8, 17,  6,  7, 18 },
	{  9, 14,  4,  5, 13 },
	{ 10, 12,  5,  4, 15 },
	{ 11, 19,  7,  6, 16 },
    };
    int i;

    for (i=0; i<12; ++i) {
	GLfloat *p0, *p1, *p2, d;
	GLfloat u[3], v[3], n[3];

	p0 = &vertexes[polygons[i][0]][0];
	p1 = &vertexes[polygons[i][1]][0];
	p2 = &vertexes[polygons[i][2]][0];

	u[0] = p2[0] - p1[0];
	u[1] = p2[1] - p1[1];
	u[2] = p2[2] - p1[2];

	v[0] = p0[0] - p1[0];
	v[1] = p0[1] - p1[1];
	v[2] = p0[2] - p1[2];

	n[0] = u[1]*v[2] - u[2]*v[1];
	n[1] = u[2]*v[0] - u[0]*v[2];
	n[2] = u[0]*v[1] - u[1]*v[0];

	d = 1.0F / (GLfloat) sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
	n[0] *= d;
	n[1] *= d;
	n[2] *= d;

	glBegin(GL_POLYGON);
	glNormal3fv(n);
	glVertex3fv(p0);
	glVertex3fv(p1);
	glVertex3fv(p2);
	glVertex3fv(vertexes[polygons[i][3]]);
	glVertex3fv(vertexes[polygons[i][4]]);
	glEnd();
    }
}

static void
redraw(void)
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPushMatrix();
    glTranslatef( -2.0F, 0.0F, 0.0F );
    glRotatef( Yrot, 0.0F, 1.0F, 0.0F );
    /* Select the material via glIndex */
    glIndexi( colors[0].indexes[0] );
    /*OR glMateriali( GL_FRONT_AND_BACK, GL_INDEX_OFFSET, colors[0].indexes[0] );*/
    drawDodecahedron();
    glPopMatrix();

    glPushMatrix();
    glTranslatef( 2.0F, 0.0F, 0.0F );
    glRotatef( Yrot, 0.0F, 1.0F, 0.0F );
    /* Select the material via glIndex */
    glIndexi( colors[1].indexes[0] );
    /*OR glMateriali( GL_FRONT_AND_BACK, GL_INDEX_OFFSET, colors[1].indexes[0] );*/
    drawDodecahedron();
    glPopMatrix();

    SwapBuffers(hDC);
}

static void
idleRedraw(void)
{
    Yrot += 2.0f;
    redraw();
}

static void
resize(void)
{
    glViewport( 0, 0, winWidth, winHeight );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( -2.0F, 2.0F, -2.0F, 2.0F, 5.0F, 25.0F );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( 0.0F, 0.0F, -15.0F );
}

/*****************************************************************/

void
setupPalette(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd;
    LOGPALETTE* pPal;
    int pixelFormat = GetPixelFormat(hDC);
    int paletteSize;

    DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    /*
    ** Determine if a palette is needed and if so what size.
    */
    if (pfd.dwFlags & PFD_NEED_PALETTE ||
		pfd.iPixelType == PFD_TYPE_COLORINDEX) {
	paletteSize = 1 << pfd.cColorBits;
	if (paletteSize > 4096) {
	    paletteSize = 4096;
	}
    } else {
	return;
    }

    pPal = (LOGPALETTE*)
	malloc(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
    pPal->palVersion = 0x300;
    pPal->palNumEntries = paletteSize;

    /*
    ** Fill the logical palette with color ramps.
    **
    ** Set up the logical palette so that it can be realized
    ** into the system palette as an identity palette.
    **
    ** 1) The default static entries should be present and at the right
    **    location.  The easiest way to do this is to grab them from
    **    the current system palette.
    **
    ** 2) All non-static entries should be initialized to unique values.
    **    The easiest way to do this is to ensure that all of the non-static
    **    entries have the PC_NOCOLLAPSE flag bit set.
    */
    {
	int numRamps = NUM_COLORS;
	int rampSize = (paletteSize - 20) / numRamps;
	int extra = (paletteSize - 20) - (numRamps * rampSize);
	int i, r;

	/*
	** Initialize static entries by copying them from the
	** current system palette.
	*/
	GetSystemPaletteEntries(hDC, 0, paletteSize, &pPal->palPalEntry[0]);

	/*
	** Fill in non-static entries with desired colors.
	*/
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

	/*
	** Initialize any remaining non-static entries.
	*/
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
setupPixelFormat(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd = {
	sizeof(PIXELFORMATDESCRIPTOR),	/* size of this pfd */
	1,				/* version num */
	PFD_DRAW_TO_WINDOW |		/* support window */
	PFD_SUPPORT_OPENGL |		/* support OpenGL */
	PFD_DOUBLEBUFFER,		/* support double-buffering */
	PFD_TYPE_COLORINDEX,		/* color index mode */
	8,				/* 8-bit color depth */
	0, 0, 0, 0, 0, 0,		/* color bits (ignored) */
	0,				/* no alpha buffer */
	0,				/* alpha bits (ignored) */
	0,				/* no accumulation buffer */
	0, 0, 0, 0,			/* accum bits (ignored) */
	16,				/* depth buffer */
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
	MessageBox(WindowFromDC(hDC),
		"ChoosePixelFormat failed\n"
		"This application works best with an 8-bit\n"
		"(256 color) display mode\n",
		"Error",
		MB_ICONERROR | MB_OK);
	exit(1);
    }

    retVal = SetPixelFormat(hDC, SelectedPixelFormat, &pfd);
    if (retVal != TRUE) {
	MessageBox(WindowFromDC(hDC), "SetPixelFormat failed", "Error",
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
    switch (message) {
    case WM_CREATE:
	/*
	** Set up for OpenGL rendering.  Bind the rendering context to
	** the same device context that the palette will be selected into.
	*/
	hDC = GetDC(hWnd);
	setupPixelFormat(hDC);
	setupPalette(hDC);
	hGLRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hGLRC);
	if (!checkExtension("SGI_index_material")) {
	    char message[1024];

	    sprintf(message,
		"SGI_index_material is required by this application\n"
		"but is not supported by the current OpenGL renderer.\n\n"
		"Vendor: %s\nRenderer: %s\nVersion: %s",
		glGetString(GL_VENDOR),
		glGetString(GL_RENDERER),
		glGetString(GL_VERSION));

	    wglMakeCurrent(NULL, NULL);
	    wglDeleteContext(hGLRC);
	    hGLRC = NULL;

	    MessageBox(hWnd, message, "OpenGL Extension Required",
		    MB_ICONERROR | MB_OK);
	    exit(1);
	}
	init();
	idleFunc = idleRedraw;
	return 0;
    case WM_DESTROY:
	/*
	** Finish OpenGL rendering.
	*/
	idleFunc = NULL;
	if (hGLRC) {
	    wglMakeCurrent(NULL, NULL);
	    wglDeleteContext(hGLRC);
	}
	ReleaseDC(hWnd, hDC);
	PostQuitMessage(0);
	return 0;
    case WM_SIZE:
	if (hGLRC) {
	    winWidth = (int) LOWORD(lParam);
	    winHeight = (int) HIWORD(lParam);
	    resize();
	    return 0;
	}
    case WM_PALETTECHANGED:
	/*
	** Update palette mapping if this *is not* the active window.
	*/
	if (hGLRC && hPalette && (HWND) wParam != hWnd) {
	    UnrealizeObject(hPalette);
	    SelectPalette(hDC, hPalette, FALSE);
	    RealizePalette(hDC);
	    redraw();
	    return 0;
	}
	break;
    case WM_QUERYNEWPALETTE:
	/*
	** Update palette mapping if this *is* the active window.
	*/
	if (hGLRC && hPalette) {
	    UnrealizeObject(hPalette);
	    SelectPalette(hDC, hPalette, FALSE);
	    RealizePalette(hDC);
	    redraw();
	    return TRUE;
	}
	break;
    case WM_PAINT:
	/*
	** Update the window.  Don't use the device context returned by
	** BeginPaint as it won't have the right palette selected into it.
	*/
	if (hGLRC) {
	    PAINTSTRUCT ps;

	    BeginPaint(hWnd, &ps);
	    redraw();
	    EndPaint(hWnd, &ps);
	    return 0;
	}
	break;
    case WM_CHAR:
	switch ((int)wParam) {
	case VK_ESCAPE:
	    DestroyWindow(hWnd);
	    return 0;
	default:
	    break;
	}
	break;
    default:
	break;
    }

    /* Deal with any unprocessed messages */
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

    /* Define and register a window class */
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

    /* Figure out a default size for the window */
    winWidth = GetSystemMetrics(SM_CYSCREEN) / 3;
    winHeight = GetSystemMetrics(SM_CYSCREEN) / 3;

    /* Create a window of the previously defined class */
    hWnd = CreateWindow(
	className,		/* Window class's name */
	windowName,		/* Title bar text */
	WS_OVERLAPPEDWINDOW |	/* The window's style */
	WS_CLIPCHILDREN |
	WS_CLIPSIBLINGS,
	winX, winY,		/* Position */
	winWidth, winHeight,	/* Size */
	NULL,			/* Parent window's handle */
	NULL,			/* Menu handle */
	hCurrentInst,		/* Instance handle */
	NULL);			/* No additional data */

    /* Map the window to the screen */
    ShowWindow(hWnd, nCmdShow);

    /* Force the window to repaint itself */
    UpdateWindow(hWnd);

    /* Process Messages */
    while (1) {
	/* execute the idle function while there are no messages to process */
	while (idleFunc &&
	       PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == FALSE)
	{
	    (*idleFunc)();
	}
	if (GetMessage(&msg, NULL, 0, 0) != TRUE) {
	    break;
	}
	TranslateMessage(&msg);
	DispatchMessage(&msg);
    }

    return msg.wParam;
}


