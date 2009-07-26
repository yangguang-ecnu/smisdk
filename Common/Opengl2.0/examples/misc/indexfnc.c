/* indexfnc.c */

/*
 * Test/demo of the GL_SGI_index_func extension
 *
 * This extension adds the glIndexFuncSGI() function which let's
 * one cull fragments depending on the fragment's index value
 * compared to a reference value, very much like glAlphaFunc().
 *
 * This demo draws a line, triangle, and image using a ramp of
 * color indexes.  The space bar enables/disables the index test.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include "GL/gl.h"

#if defined(GL_SGI_index_func)
/* Have to access glIndexFuncSGI through a pointer, ugh! */
static PFNGLINDEXFUNCSGIPROC glIndexFuncSGIptr;
#endif

char *className = "OpenGL";
char *windowName = "Index Func";
int winX = 0, winY = 0;
int winWidth = 300, winHeight = 300;

HDC hDC;
HGLRC hGLRC;
HPALETTE hPalette;

void (*idleFunc)(void);

void setPaletteIndex(int i, float r, float g, float b);

static GLfloat Zrot = 0.0F;
static GLboolean DoTest = GL_FALSE;
static GLubyte *Image;

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
    GLint i, j;
    GLfloat f;

    glIndexFuncSGIptr = (void *) wglGetProcAddress("glIndexFuncSGI");
    if (!glIndexFuncSGIptr) {
        printf("Note: renderer doesn't support GL_SGI_index_func extension.\n");
    }

    setPaletteIndex( 10, 0.0F, 0.5F, 0.5F );
    glClearIndex( 10.0f );
    for (i=11;i<240;i++) {
        GLfloat t = (GLfloat) (i-11) / (GLfloat) (240-11);
        setPaletteIndex( i, t, t, 0.0F );
    }

    Image = (GLubyte *) malloc( 32*32*sizeof(GLubyte) );

    for (i = 0; i < 32; i++)
	for (j = 0; j < 32; j++) {
	    double d;

	    d = sqrt((i - 16) * (i - 16) + (j - 16) * (j - 16)) / 23.0;
	    Image[(32 * i) + j] = 11 + (int)((240 - 11) * d);
	}

#ifdef GL_SGI_index_func
    if (glIndexFuncSGIptr) {
        /* Sanity checks */
        glEnable( GL_INDEX_TEST_SGI );
        (*glIndexFuncSGIptr)( GL_GEQUAL, 10.0F );
        glPushAttrib( GL_COLOR_BUFFER_BIT );
        glDisable( GL_INDEX_TEST_SGI );
        (*glIndexFuncSGIptr)( GL_LESS, 99.0F );
        glPopAttrib();
        assert( glIsEnabled(GL_INDEX_TEST_SGI) );
        glGetIntegerv( GL_INDEX_TEST_FUNC_SGI, &i );
        assert( i==GL_GEQUAL );
        glGetFloatv( GL_INDEX_TEST_REF_SGI, &f );
        assert( f==10.0f );
    }
#endif
    checkError("after init");
}

static void
redraw(void)
{
    glClear( GL_COLOR_BUFFER_BIT );

    glPushMatrix();

    glRotatef( Zrot, 0.0F, 0.0F, 1.0F );

#ifdef GL_SGI_index_func
    if (glIndexFuncSGIptr) {
        if (DoTest) {
            glEnable( GL_INDEX_TEST_SGI );
            (*glIndexFuncSGIptr)( GL_GREATER, 128.0F );
        }
        else {
            glDisable( GL_INDEX_TEST_SGI );

        }
    }
#endif

    glBegin( GL_LINES );
    glIndexi(  10 );   glVertex2f( -1.0F, 0.0F );
    glIndexi( 240 );   glVertex2f(  1.0F, 0.0F );
    glEnd();

    glBegin( GL_TRIANGLES );
    glIndexi(  11 );   glVertex2f( 0.0F, 1.0F );
    glIndexi( 100 );   glVertex2f( 0.5F, 0.1F );
    glIndexi( 240 );   glVertex2f( -0.5F, 0.1F );
    glEnd();

    glRasterPos2f( 0.5F, -0.2F );
    glDrawPixels( 32, 32, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, Image );

    glPopMatrix();

    SwapBuffers(hDC);
    checkError("after swap buffers");
}

static void
idleRedraw(void)
{
    Zrot += 2.0f;
    redraw();
}

static void
resize(void)
{
    glViewport( 0, 0, winWidth, winHeight );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( -1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

/*****************************************************************/

void
setPaletteIndex(int i, float r, float g, float b)
{
    PALETTEENTRY pe;

    pe.peRed = (BYTE) (255.0F * r);
    pe.peGreen = (BYTE) (255.0F * g);
    pe.peBlue = (BYTE) (255.0F * b);
    pe.peFlags = PC_NOCOLLAPSE;
    SetPaletteEntries(hPalette, i, 1, &pe);
    UnrealizeObject(hPalette);
}

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
	int extra = paletteSize - 20;
	int i;

	/*
	** Initialize static entries by copying them from the
	** current system palette.
	*/
	GetSystemPaletteEntries(hDC, 0, paletteSize, &pPal->palPalEntry[0]);

	/*
	** Initialize any remaining non-static entries.
	*/
	for (i=0; i<extra; ++i) {
	    int index = 10+i;
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
	if (!checkExtension("SGI_index_func")) {
	    char message[1024];

	    sprintf(message,
		"SGI_index_func is required by this application\n"
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
	    hGLRC = NULL;
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
	case VK_SPACE:
	    DoTest = !DoTest;
	    break;
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
