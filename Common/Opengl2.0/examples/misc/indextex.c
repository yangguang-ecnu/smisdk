/*
** Color Index Texture Example
*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <GL/gl.h>

#if !defined(M_PI)
#define M_PI 3.14159265F
#endif

char *className = "OpenGL";
char *windowName = "Color Index Texture";
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
        { 0.0F, 0.0F, 0.0F },
        { 0.1F, 0.6F, 0.3F },
        { 1.0F, 1.0F, 1.0F },
        0.75F, { 0, 0, 0 },
    },
    {
        { 0.0F, 0.0F, 0.0F },
        { 0.0F, 0.2F, 0.5F },
        { 1.0F, 1.0F, 1.0F },
        0.75F, { 0, 0, 0 },
    },
    {
        { 0.0F, 0.05F, 0.05F },
        { 0.6F, 0.0F, 0.8F },
        { 1.0F, 1.0F, 1.0F },
        0.75F, { 0, 0, 0 },
    },
};

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

void
drawTorus(void)
{
    int numMajor = 32;
    int numMinor = 24;
    float majorRadius = 0.6F;
    float minorRadius = 0.2F;
    double majorStep = 2.0F*M_PI / numMajor;
    double minorStep = 2.0F*M_PI / numMinor;
    int i, j;

    for (i=0; i<numMajor; ++i) {
	double a0 = i * majorStep;
	double a1 = a0 + majorStep;
	GLfloat x0 = (GLfloat) cos(a0);
	GLfloat y0 = (GLfloat) sin(a0);
	GLfloat x1 = (GLfloat) cos(a1);
	GLfloat y1 = (GLfloat) sin(a1);

	glBegin(GL_TRIANGLE_STRIP);
	for (j=0; j<=numMinor; ++j) {
	    double b = j * minorStep;
	    GLfloat c = (GLfloat) cos(b);
	    GLfloat r = minorRadius * c + majorRadius;
	    GLfloat z = minorRadius * (GLfloat) sin(b);

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
setCheckTexture(void)
{
    BOOL textureReplaceEnv = FALSE;
    int texWidth = 64;
    int texHeight = 64;
    GLubyte *texPixels, *p;
    int texSize;
    int i, j;

    texSize = texWidth*texHeight*1*sizeof(GLubyte);
    if (textureReplaceEnv) {
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    } else {
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    texPixels = (GLubyte *) malloc(texSize);
    if (texPixels == NULL) {
	return;
    }

    p = texPixels;
    for (i=0; i<texHeight; ++i) {
	for (j=0; j<texWidth; ++j) {
	    int rampIndex = textureReplaceEnv ? 1 : 0;
	    if ((i ^ j) & 4) {
		*p = colors[0].indexes[rampIndex] & 0xff;
	    } else {
		*p = colors[1].indexes[rampIndex] & 0xff;
	    }
	    p += 1;
	}
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_COLOR_INDEX8_EXT,
		 texWidth, texHeight, 0,
		 GL_COLOR_INDEX, GL_UNSIGNED_BYTE, texPixels);

    free(texPixels);
}

/*****************************************************************/

void
setProjection(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /*
    ** Preserve the aspect ratio of objects in the scene.
    */
    if (winWidth > winHeight) {
	GLfloat aspect = (GLfloat) winWidth / (GLfloat) winHeight;
	glFrustum(-0.5F*aspect, 0.5F*aspect, -0.5F, 0.5F, 1.0F, 3.0F);
    } else {
	GLfloat aspect = (GLfloat) winHeight / (GLfloat) winWidth;
	glFrustum(-0.5F, 0.5F, -0.5F*aspect, 0.5F*aspect, 1.0F, 3.0F);
    }
    glMatrixMode(GL_MODELVIEW);
}

void
init(void)
{
    GLfloat matDiff[4] = { 0.65F, 0.05F, 0.20F, 0.60F };
    GLfloat matSpec[4] = { 0.50F, 0.50F, 0.50F, 1.00F };
    GLfloat matShine = 20.00F;
    GLint indexes[3];
    GLfloat light0Pos[4] = { 0.70F, 0.70F, 1.25F, 0.00F };

    glClearColor(colors[2].diff[0], colors[2].diff[1], colors[2].diff[2], 1.0F);
    glClearIndex((GLfloat) colors[2].indexes[1]);

    setProjection();
    glTranslatef(0.0F, 0.0F, -2.0F);

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

    indexes[0] = 0;
    indexes[1] = colors[0].indexes[1] - colors[0].indexes[0];
    indexes[2] = colors[0].indexes[2] - colors[0].indexes[0];
    glMaterialiv(GL_FRONT_AND_BACK, GL_COLOR_INDEXES, indexes);
    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
    glEnable(GL_LIGHT0);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    setCheckTexture();
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_CULL_FACE);
}

void
doRedraw(void)
{
    static GLfloat x, y, z;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glRotatef(x, 1.0F, 0.0F, 0.0F);
    glRotatef(y, 0.0F, 1.0F, 0.0F);
    glRotatef(z, 0.0F, 0.0F, 1.0F);

    drawTorus();

    glPopMatrix();

    SwapBuffers(hDC);

    x += 5.0F;
    if (x > 360.0F) x -= 360.0F;
    y += 7.0F;
    if (y > 360.0F) y -= 360.0F;
    z += 9.0F;
    if (z > 360.0F) z -= 360.0F;
}

void
redraw(void)
{
    idleFunc = doRedraw;
}

void
resize(void)
{
    setProjection();
    glViewport(0, 0, winWidth, winHeight);
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
	if (!checkExtension("SGI_index_texture")) {
	    char message[1024];

	    sprintf(message,
		"SGI_index_texture is required by this application\n"
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
	idleFunc = doRedraw;
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
