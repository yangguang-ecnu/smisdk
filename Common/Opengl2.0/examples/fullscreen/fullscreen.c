/* 

   fullscreen.c
   Nate Robins, 1997.

   An example of rendering OpenGL fullscreen using Win32.  Use the
   mouse to select a mode.  Modes that are highlighted in green are
   modes that can be changed to dynamically.  Those in red can't be
   changed to dynamically (they require a reboot).  If you change to a
   mode that has the wrong refresh rate, you can press the right mouse
   button which will take you to the default display mode.  The mode
   that is highlighted in yellow is the current display mode.  Press
   'Esc' to quit.  The program returns to the defalt display mode on
   exit.
   
 */

#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>

HDC hDC;
int win_width, win_height;
int row = 0, col = 0;
int num_modes = 0;
int row_size = 10, col_size = 0;
int num_cols = 3, num_rows = 0;
int current = -1;
int cant = 0;
DEVMODE devmode;
DEVMODE *devmodes;


/* 

   general purpose text routine.  draws a string according to the
   format in a stroke font at x, y after scaling it by the scale
   specified.  x, y and scale are all in window-space [i.e., pixels]
   with origin at the lower-left.

 */

void 
text(GLuint x, GLuint y, GLuint scale, char* format, ...)
{
    va_list args;
    char buffer[255], *p;
    GLfloat font_scale = 119.05F + 33.33F;

    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, win_width, 0, win_height);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glTranslatef((GLfloat) x, (GLfloat) y, 0.0F);

    glScalef(scale/font_scale, scale/font_scale, scale/font_scale);

    for(p = buffer; *p; p++)
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);
  
    glPopAttrib();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void
display()
{
    int i, x, y;

    glViewport(0, 0, win_width, win_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, win_width, 0, win_height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPolygonMode(GL_FRONT, GL_LINE);
    glClear(GL_COLOR_BUFFER_BIT);

    for (i = 0; i < num_modes; i++) {
	x = i / num_rows * col_size;
	y = i % num_rows * row_size;

	glColor3f(1.0F, 1.0F, 1.0F);
	if (current == i)
	    glColor3f(1.0F, 1.0F, 0.0F);
	if (row*row_size == y && col*col_size == x) {
	    /* test to see if the mode that the mouse pointer is above
               is dynamic. */
	    if (ChangeDisplaySettings(&devmodes[i], CDS_TEST) ==
		DISP_CHANGE_SUCCESSFUL) {
		glColor3f(0.0F, 1.0F, 0.0F);
	    } else {
		glColor3f(1.0F, 0.0F, 0.0F);
	    }
	    /* draw a box around the current choice. */
	    glRecti(x, y-2, x+col_size, y+row_size-1);
	}
	text(x, y, row_size, "%3d: %4dx%4d %2d bps %3dhz\n", i,
	     devmodes[i].dmPelsWidth, devmodes[i].dmPelsHeight, 
	     devmodes[i].dmBitsPerPel, devmodes[i].dmDisplayFrequency);
    }

    /* let the user know that they can't choose that one.  "cant"
       decrements so that a few frames of the message are seen
       (otherwise it blinks off too quickly to read). */
    if (cant) {
	glColor3f(1.0F, 0.0F, 0.0F);
	text(0, win_height-20, 20, "Can't dynamically resize to that format.");
	cant--;
    }

    glFlush();
    SwapBuffers(hDC);
}


LONG WINAPI
WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
    int i;
    static PAINTSTRUCT ps;

    switch(uMsg) {
    case WM_PAINT:
	display();
	BeginPaint(hWnd, &ps);
	EndPaint(hWnd, &ps);
	return 0;

    case WM_SIZE:
      	win_width = LOWORD(lParam);
	win_height = HIWORD(lParam);
	num_rows = win_height / row_size;
	col_size = win_width / num_cols;
	PostMessage(hWnd, WM_PAINT, 0, 0);
	return 0;

    case WM_CHAR:
	switch (wParam) {
	case 27:			/* ESC key */
	    PostQuitMessage(0);
	    break;
	}
	return 0;

    case WM_LBUTTONDOWN:
	i = col * num_rows + row;
	if (i < num_modes) {
	    /* test first, then change, just in case. */
	    if (ChangeDisplaySettings(&devmodes[i], CDS_TEST) == 
		DISP_CHANGE_SUCCESSFUL)
	    {
		ChangeDisplaySettings(&devmodes[i], 0);
		current = i;
	    } else {
		cant = 5;
		PostMessage(hWnd, WM_PAINT, 0, 0);
	    }
	}
	return 0;

    case WM_RBUTTONDOWN:
	/* click your heels three times...hang on toto. */
	ChangeDisplaySettings(NULL, 0);
	return 0;

    case WM_MOUSEMOVE:
	col = LOWORD(lParam) / col_size;
	row = (win_height - HIWORD(lParam)) / row_size;
	PostMessage(hWnd, WM_PAINT, 0, 0);
	return 0;

    case WM_CLOSE:
	PostQuitMessage(0);
	return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam); 
} 

HWND
CreateOpenGLWindow(char* title, int x, int y, int width, int height, 
		   BYTE type, DWORD flags)
{
    int         pf;
    HDC         hDC;
    HWND        hWnd;
    WNDCLASS    wc;
    PIXELFORMATDESCRIPTOR pfd;
    static HINSTANCE hInstance = 0;

    /* only register the window class once - use hInstance as a flag. */
    if (!hInstance) {
	hInstance = GetModuleHandle(NULL);
	wc.style         = CS_OWNDC;
	wc.lpfnWndProc   = (WNDPROC)WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = "OpenGL";

	if (!RegisterClass(&wc)) {
	    MessageBox(NULL, "RegisterClass() failed:  "
		       "Cannot register window class.", "Error", MB_OK);
	    return NULL;
	}
    }

    hWnd = CreateWindow("OpenGL", title, WS_POPUP | WS_MAXIMIZE |
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			x, y, width, height, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) {
	MessageBox(NULL, "CreateWindow() failed:  Cannot create a window.",
		   "Error", MB_OK);
	return NULL;
    }

    hDC = GetDC(hWnd);

    /* there is no guarantee that the contents of the stack that become
       the pfd are zeroed, therefore _make sure_ to clear these bits. */
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags;
    pfd.iPixelType   = type;
    pfd.cColorBits   = 32;

    pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) {
	MessageBox(NULL, "ChoosePixelFormat() failed:  "
		   "Cannot find a suitable pixel format.", "Error", MB_OK); 
	return 0;
    } 
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
	MessageBox(NULL, "SetPixelFormat() failed:  "
		   "Cannot set format specified.", "Error", MB_OK);
	return 0;
    } 

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    ReleaseDC(hDC, hWnd);

    return hWnd;
}    

int APIENTRY
WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst,
	LPSTR lpszCmdLine, int nCmdShow)
{
    HGLRC hRC;				/* opengl context */
    HWND hWnd;				/* window */
    MSG msg;				/* message */

    hWnd = CreateOpenGLWindow("minimal", 0, 0, win_width, win_height,
			      PFD_TYPE_RGBA, PFD_DOUBLEBUFFER);
    if (hWnd == NULL)
	exit(1);

    hDC = GetDC(hWnd);
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    ShowWindow(hWnd, SW_SHOW);

    /* get the total number of modes so we can allocate memory for all
       of 'em. */
    num_modes = 0;
    while (EnumDisplaySettings(NULL, num_modes, &devmode))
	num_modes++;

    /* fill an array with all the devmodes so we don't have to keep
       grabbing 'em. */
    devmodes = (DEVMODE*)malloc(sizeof(DEVMODE)*num_modes);
    num_modes = 0;
    while (EnumDisplaySettings(NULL, num_modes, &devmodes[num_modes]))
	num_modes++;

    while(GetMessage(&msg, hWnd, 0, 0)) {
	TranslateMessage(&msg);
	DispatchMessage(&msg);
    }

    /* free the array of devmodes, and click our heels to come back
       from Oz...don't forget Toto. */
    free(devmodes);
    ChangeDisplaySettings(NULL, 0);

    wglMakeCurrent(NULL, NULL);
    ReleaseDC(hDC, hWnd);
    wglDeleteContext(hRC);
    DestroyWindow(hWnd);

    return msg.wParam;
}

