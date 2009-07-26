#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <windows.h>
#include <GL/gl.h>

#include "resource.h"

#define RB_MSG_ERROR        0
#define RB_MSG_WARNING      1
#define RB_MSG_INFORMATION  2

static DWORD s_window_width  = 640;
static DWORD s_window_height = 480;
static BOOL  s_index_texture_extension = FALSE;
static BOOL  s_lock_arrays_extension = FALSE;

typedef struct
{
    float st[2];
    float rgb[3];
    float xyz[3];
} RBIVertex;

typedef struct 
{
    RBIVertex     *interleaved_array;
    int            num_vertex_rows;
    int            num_cols;
    int            num_vertices;
    int            verts_per_row;

    int            num_element_rows;
    unsigned int **elements_array;
} RBDrawElementsData;

RBDrawElementsData s_elements_data;

const float RED[]   = { 1.0F, 0.0F, 0.0F };
const float GREEN[] = { 0.0F, 1.0F, 0.0F };
const float BLUE[]  = { 0.0F, 0.0F, 1.0F };

const char *APPNAME = "OpenGL RasterBench 1.00";
const char *CFGNAME = "rb.cfg";
const char *LOGNAME = "rb.log";

HGLRC s_hglrc;
HDC   s_hGLDC;
HWND  s_hwndOpenGL;
BOOL  s_config_specified_on_cmdline;
BOOL  s_noprompt;
BOOL  s_everything;
BOOL  s_lock_arrys = FALSE;

HPALETTE s_hPalette = NULL;
int      s_redIndex, s_greenIndex, s_blueIndex, s_clearIndex;

int  RBInit( HINSTANCE, int, LPSTR );
void RBShutdown( void );
void RBGo( HINSTANCE, int );

void RBSetupPalette( HDC hDC );
int  RBCreateContext( HWND hWnd );
void RBDestroyContext( HWND hWnd );

void RBInitDialog( HWND hDlg );
void RBReadConfigFromDialog( HWND hDlg );

void RBConfigOpenGL( void );
HWND RBCreateOpenGLWindow( HINSTANCE, int );

void RBSelectColorMode( HMENU hMenu, WPARAM wID, HWND hDlg );
void RBSelectDepthFunc( HMENU hMenu, WPARAM wID );
void RBSelectFogCurve( HMENU hMenu, WPARAM wID );
void RBSelectTriangleSize( HMENU hMenu, WPARAM wID );

void RBSelectTextureEnvironment( HMENU hMenu, WPARAM wID );
void RBSelectTextureFormat( HMENU hMenu, WPARAM wID );
void RBSelectTextureHeight( HMENU hMenu, WPARAM wID );
void RBSelectTextureWidth( HMENU hMenu, WPARAM wID );
void RBSelectTextureOrientation( HMENU hMenu, WPARAM wID );
void RBSelectTextureFilter( HMENU hMenu, WPARAM wID );
void RBCreateTexture( void );
void RBSelectZoom( HMENU hMenu, WPARAM wID );

void RBStoreConfigToDisk( void );

int  RBMenuItemIsChecked( HWND hWnd, WPARAM wID );
void RBToggleMenuCheck( HMENU hMenu, WPARAM wID );

void RBDoTest( HWND hWnd );
void RBDoHatTest( HWND hWnd );
void RBDoHatStripTest( HWND hWnd );
void RBDoHatDrawElementsTest( HWND hWnd );
void RBDoClearTest( HWND hWnd );

void RBBuildData( HWND hWnd );

void RBLogDialog( HINSTANCE hInstance );
void RBGenerateLog( const char *logfilename );

void RBHelp( void );

#define RB_CANCEL 0
#define RB_OK     1

void RbMessage( int type, const char *msg );

typedef struct
{
    unsigned long triangles_rendered;
    float         elapsed_time;
    float         triangles_second;
    float         mpixels_second;

    float         area_filled;
    unsigned long num_screen_triangles;
} RBStats_s;

typedef struct
{
    int    size;
    
    int    rgb_mode;
    int    do_depth_test;
    int    do_depth_writes;
    GLenum depth_func;
    int    depth_size;
    float  depth_clear;
    
    int    do_fog;
    int    do_cheap_fog;
    GLenum fog_curve;
    
    int      do_dither;
    unsigned triangle_size;
    GLenum   draw_buffer;
    
    GLenum shade_model;
    
    int    do_texture;
    int    random_texture;
    GLenum perspective_correction;
    GLenum texture_environment;
    int    texture_width;
    int    texture_height;
    float  ftexture_width, ftexture_height;
    int    texture_format;
    int    texture_orientation;
    float  texture_zoom;
    GLenum texture_filter;
    
    int    texture_clamp_s, texture_clamp_t;
    
    int    primitive;
    
    int    iterations;
} RBConfig_s;

RBConfig_s s_config =
{
    sizeof( s_config ),
        
        TRUE,         // rgb_mode
        FALSE,        // do_depth_test
        TRUE,         // do_depth_writes
        GL_LEQUAL,    // depth_func
        16,           // depth_size
        1.0F,         // clear depth
        
        FALSE,        // do_fog
        FALSE,        // do_cheap_fog
        GL_LINEAR,    // fog_curve
        
        FALSE,            // do_dither
        50,               // triangle_size
        GL_FRONT,         // draw_buffer
        
        GL_SMOOTH,        // shade_model
        
        FALSE,            // do_texture
        TRUE,             // random texture
        GL_NICEST,        // perspective_correction
        GL_DECAL,         // texture_environment
        64, 64,           // texture_width and texture_height
        64.0F, 64.0F,
        0,                // texture_format
        0,                // texture_orientation
        1.0F,             // texture zoom
        GL_NEAREST,	     // texture_filter
        
        FALSE, FALSE,     // clamp_s, clamp_t
        
        IDM_TRIANGLES,    // primitive
        
        1                 // iterations
};

RBStats_s  s_stats;

BOOL CALLBACK StatsDialogProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK DialogProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

BOOL s_done = FALSE;

int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    
    /*
    ** execute the configuration dialog box
    */
    while ( !s_done )
    {
        if ( RBInit( hInstance, nCmdShow, lpCmdLine ) )
        {
            RBGo( hInstance, nCmdShow );
            if ( !s_noprompt )
                RBLogDialog( hInstance );
            RBGenerateLog( LOGNAME );
            
            if ( s_noprompt ) 
                s_done = 1;
        }
        else
        {
            break;
        }
    }
    
    return 0;
}

/*
** RBInit
*/
int RBInit( HINSTANCE hInstance, int nCmdShow, LPSTR lpCmdLine )
{
    RBConfig_s cfg;

    if ( strstr( lpCmdLine, "/?" ) || ( strstr( lpCmdLine, "-?" ) ) )
    {
        RBHelp();
        return FALSE;
    }

    if ( !SetPriorityClass( GetCurrentProcess(), REALTIME_PRIORITY_CLASS ) )
    {
        OutputDebugString( "Could not set priority class\n" );
        return 0;
    }
    if ( !SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL ) )
    {
        OutputDebugString( "Could not set thread priority\n" );
        return 0;
    }
    
    /*
    ** set a default iteration count
    */
    s_config.iterations = 1;
    
    /*
    ** see if we're not supposed to prompt
    */
    if ( strstr( lpCmdLine, "-noprompt" ) )
    {
        s_noprompt = 1;
    }
    
    if ( strstr( lpCmdLine, "-everything" ) )
    {
        s_noprompt = 1;
        s_everything = 1;
    }

    /*
    ** see if an alternate config file has been specified
    */
    if ( strstr( lpCmdLine, "-config" ) )
    {
        FILE       *fp;
        const char *name;
        
        name = strstr( lpCmdLine, "-config" ) + strlen( "-config" );
        
        while ( *name && *name == ' ' )
            name++;
        
        if ( ( fp = fopen( name, "rb" ) ) != 0 )
        {
            fread( &cfg, sizeof( cfg ), 1, fp );
            fclose( fp );
            if ( cfg.size == sizeof( cfg ) )
            {
                s_config = cfg;
                s_config_specified_on_cmdline = TRUE;
            }
        }
    }
    /*
    ** or try and load the old one
    */
    else
    {
        FILE *fp;
        
        if ( ( fp = fopen( CFGNAME, "rb" ) ) != 0 )
        {
            fread( &cfg, sizeof( cfg ), 1, fp );
            
            if ( cfg.size == sizeof( cfg ) )
                s_config = cfg;
            fclose( fp );
        }
    }
    
    /*
    ** see if a non-default window width and height have been specified
    */
    {
        const char *widthstring  = strstr( lpCmdLine, "-width:" );
        const char *heightstring = strstr( lpCmdLine, "-height:" );
        
        if ( widthstring )
        {
            widthstring += strlen( "-width:" );
            s_window_width  = atoi( widthstring );
        }
        if ( heightstring )
        {
            heightstring += strlen( "-height:" );
            s_window_height = atoi( heightstring );
        }
        
        if ( s_window_width < 0 || s_window_height < 0 )
        {
            RbMessage( RB_MSG_ERROR, "Invalid command line window size" );
            return FALSE;
        }
    }
    
    if ( strstr( lpCmdLine, "-checkerboard" ) )
    {
        s_config.random_texture = FALSE;
    }
    
    if ( !s_noprompt )
    {
        int rval;
        
        RBCreateOpenGLWindow( hInstance, nCmdShow );
        if ( !RBCreateContext( s_hwndOpenGL ) )
            return FALSE;

        
        rval = DialogBox( hInstance, MAKEINTRESOURCE( IDD_DIALOG1 ), 0, DialogProc );
        
        RBDestroyContext( s_hwndOpenGL );
        
        return rval;
    }
    else
    {
        return TRUE;
    }
}

/*
** DialogProc
*/
BOOL CALLBACK DialogProc( HWND hWnd, 
                         UINT uMsg,
                         WPARAM wParam,
                         LPARAM lParam )
{
    WORD    wNotifyCode = HIWORD( wParam );
    HMENU   hMenu       = GetMenu( hWnd );
    
    switch( uMsg )
    {
    case WM_INITDIALOG:
        RBInitDialog( hWnd );
        return TRUE;
    case WM_COMMAND:
        switch ( HIWORD( wParam ) )
        {
        case BN_CLICKED:
            {
                switch( wParam )
                {
                case IDOK:
                    RBReadConfigFromDialog( hWnd );
                    RBStoreConfigToDisk();
                    EndDialog( hWnd, RB_OK );
                    return TRUE;
                case IDCANCEL:
                    EndDialog( hWnd, RB_CANCEL );
                    s_done = TRUE;
                    return TRUE;
                    
                case IDM_COLORMODE_RGB:
                case IDM_COLORMODE_INDEX:
                    RBSelectColorMode( hMenu, wParam, hWnd );
                    return TRUE;
                    
                case IDM_TRIANGLES:
                case IDM_TRIANGLE_STRIP:
                case IDM_DRAW_ELEMENTS:
                case IDM_CLEAR:
                    CheckMenuItem( hMenu, IDM_TRIANGLES, MF_BYCOMMAND | MF_UNCHECKED );
                    CheckMenuItem( hMenu, IDM_TRIANGLE_STRIP, MF_BYCOMMAND | MF_UNCHECKED );
                    CheckMenuItem( hMenu, IDM_DRAW_ELEMENTS, MF_BYCOMMAND | MF_UNCHECKED );
                    CheckMenuItem( hMenu, IDM_CLEAR, MF_BYCOMMAND | MF_UNCHECKED );

                    CheckMenuItem( hMenu, wParam, MF_BYCOMMAND | MF_CHECKED );
                    break;
                    
                case IDM_DRAW_FRONT:
                    CheckMenuItem( hMenu, IDM_DRAW_FRONT, MF_BYCOMMAND | MF_CHECKED );
                    CheckMenuItem( hMenu, IDM_DRAW_BACK, MF_BYCOMMAND | MF_UNCHECKED );
                    return TRUE;
                case IDM_DRAW_BACK:
                    CheckMenuItem( hMenu, IDM_DRAW_FRONT, MF_BYCOMMAND | MF_UNCHECKED );
                    CheckMenuItem( hMenu, IDM_DRAW_BACK, MF_BYCOMMAND | MF_CHECKED );
                    return TRUE;
                    
                case IDM_GL_FLAT:
                    CheckMenuItem( hMenu, IDM_GL_FLAT,   MF_BYCOMMAND | MF_CHECKED );
                    CheckMenuItem( hMenu, IDM_GL_SMOOTH, MF_BYCOMMAND | MF_UNCHECKED );
                    return TRUE;
                case IDM_GL_SMOOTH:
                    CheckMenuItem( hMenu, IDM_GL_SMOOTH, MF_BYCOMMAND | MF_CHECKED );
                    CheckMenuItem( hMenu, IDM_GL_FLAT, MF_BYCOMMAND | MF_UNCHECKED );
                    return TRUE;
                    
                case IDM_DEPTHSIZE_16:
                    CheckMenuItem( hMenu, IDM_DEPTHSIZE_16, MF_BYCOMMAND | MF_CHECKED );
                    CheckMenuItem( hMenu, IDM_DEPTHSIZE_32, MF_BYCOMMAND | MF_UNCHECKED );
                    return TRUE;
                case IDM_DEPTHSIZE_32:
                    CheckMenuItem( hMenu, IDM_DEPTHSIZE_32, MF_BYCOMMAND | MF_CHECKED );
                    CheckMenuItem( hMenu, IDM_DEPTHSIZE_16, MF_BYCOMMAND | MF_UNCHECKED );
                    return TRUE;
                    
                case IDM_DEPTHBUFFER_ENABLE:
                case IDM_TEXTURE_ENABLE:
                case IDM_TEXTURE_PERSPECTIVE_CORRECT:
                case IDM_DITHER:
                case IDM_FOG_ENABLE:
                case IDM_FOG_CHEAP:
                case IDM_DEPTHMASK:
                case IDM_CLAMP_S:
                case IDM_CLAMP_T:
                    RBToggleMenuCheck( hMenu, wParam );
                    return TRUE;
                    
                case IDM_DEPTHBUFFER_CLEAR0:
                    CheckMenuItem( hMenu, IDM_DEPTHBUFFER_CLEAR1, MF_BYCOMMAND | MF_UNCHECKED );
                    CheckMenuItem( hMenu, IDM_DEPTHBUFFER_CLEAR0, MF_BYCOMMAND | MF_CHECKED );
                    return TRUE;
                case IDM_DEPTHBUFFER_CLEAR1:
                    CheckMenuItem( hMenu, IDM_DEPTHBUFFER_CLEAR1, MF_BYCOMMAND | MF_CHECKED );
                    CheckMenuItem( hMenu, IDM_DEPTHBUFFER_CLEAR0, MF_BYCOMMAND | MF_UNCHECKED );
                    return TRUE;
                    
                case IDM_FOG_LINEAR:
                case IDM_FOG_EXP:
                case IDM_FOG_EXP2:
                    RBSelectFogCurve( hMenu, wParam );
                    return TRUE;
                    
                case IDM_GL_LESS:
                case IDM_GL_LEQUAL:
                case IDM_GL_EQUAL:
                case IDM_GL_GEQUAL:
                case IDM_GL_GREATER:
                case IDM_GL_ALWAYS:
                case IDM_GL_NEVER:
                case IDM_GL_NOTEQUAL:
                    RBSelectDepthFunc( hMenu, wParam );
                    return TRUE;
                    
                case IDM_ZOOM_1x1:
                case IDM_ZOOM_1x2:
                case IDM_ZOOM_1x4:
                case IDM_ZOOM_1x8:
                    RBSelectZoom( hMenu, wParam );
                    return TRUE;

                case IDM_SIZE_3:
                case IDM_SIZE_10:
                case IDM_SIZE_25:
                case IDM_SIZE_50:
                case IDM_SIZE_100:
                case IDM_SIZE_1000:
                case IDM_SIZE_10000:
                    RBSelectTriangleSize( hMenu, wParam );
                    return TRUE;
                    
                case IDM_TEXTURE_REPLACE:
                case IDM_TEXTURE_ADD:
                case IDM_TEXTURE_MODULATE:
                case IDM_TEXTURE_DECAL:
                    RBSelectTextureEnvironment( hMenu, wParam );
                    return TRUE;
#ifdef NOTYET                    
                case IDM_TEXTURE_CI:
                case IDM_TEXTURE_RGB:
                    RBSelectTextureFormat( hMenu, wParam );
                    return TRUE;
#endif
                    
                case IDM_TEXTURE_WIDTH_1:
                case IDM_TEXTURE_WIDTH_2:
                case IDM_TEXTURE_WIDTH_4:
                case IDM_TEXTURE_WIDTH_8:
                case IDM_TEXTURE_WIDTH_16:
                case IDM_TEXTURE_WIDTH_32:
                case IDM_TEXTURE_WIDTH_64:
                case IDM_TEXTURE_WIDTH_128:
                case IDM_TEXTURE_WIDTH_256:
                case IDM_TEXTURE_WIDTH_512:
                    RBSelectTextureWidth( hMenu, wParam );
                    return TRUE;
                    
                case IDM_TEXTURE_HEIGHT_1:
                case IDM_TEXTURE_HEIGHT_2:
                case IDM_TEXTURE_HEIGHT_4:
                case IDM_TEXTURE_HEIGHT_8:
                case IDM_TEXTURE_HEIGHT_16:
                case IDM_TEXTURE_HEIGHT_32:
                case IDM_TEXTURE_HEIGHT_64:
                case IDM_TEXTURE_HEIGHT_128:
                case IDM_TEXTURE_HEIGHT_256:
                case IDM_TEXTURE_HEIGHT_512:
                    RBSelectTextureHeight( hMenu, wParam );
                    return TRUE;
                    
                case IDM_TEXTURE_0:
                case IDM_TEXTURE_90:
                case IDM_TEXTURE_180:
                case IDM_TEXTURE_270:
                    RBSelectTextureOrientation( hMenu, wParam );
                    return TRUE;
                    
                case IDM_TEXTURE_NEAREST:
                case IDM_TEXTURE_LINEAR:
                    RBSelectTextureFilter( hMenu, wParam );
                    return TRUE;
}
}
return FALSE;
default:
    return FALSE;
}
return FALSE;
}

return FALSE;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uMsg )
    {
    case WM_CHAR:
	if (wParam == VK_ESCAPE) {
	    DestroyWindow(hWnd);
	    return 0;
	}
	break;
    case WM_DESTROY:
        PostQuitMessage( 0 );
        return 0;
    }
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

int RBCreateContext( HWND hWnd )
{
    byte pixelType = s_config.rgb_mode ? PFD_TYPE_RGBA : PFD_TYPE_COLORINDEX;
    PIXELFORMATDESCRIPTOR pfd = 
    { 
        sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
            1,                     // version number 
            PFD_DRAW_TO_WINDOW |   // support window 
            PFD_SUPPORT_OPENGL |   // support OpenGL 
            PFD_DOUBLEBUFFER,      // double buffered 
            pixelType,             // RGBA type 
            16,                    // 16-bit color depth 
            0, 0, 0, 0, 0, 0,      // color bits ignored 
            0,                     // no alpha buffer 
            0,                     // shift bit ignored 
            0,                     // no accumulation buffer 
            0, 0, 0, 0,            // accum bits ignored 
            s_config.depth_size,   // 16-bit z-buffer 
            0,                     // no stencil buffer 
            0,                     // no auxiliary buffer 
            PFD_MAIN_PLANE,        // main layer 
            0,                     // reserved 
            0, 0, 0                // layer masks ignored 
    };
    
    int pf;
    
    s_hGLDC = GetDC( hWnd );

    /*
    ** force into RGBA mode if color index in > 8bpp mode
    */
    pfd.cColorBits = GetDeviceCaps( s_hGLDC, BITSPIXEL );
    if ( !s_config.rgb_mode && pfd.cColorBits > 8 )
    {
       pfd.iPixelType = PFD_TYPE_RGBA;
       s_config.rgb_mode = 1;
    }
    
    if ( ( pf = ChoosePixelFormat( s_hGLDC, &pfd ) ) != 0 )
    {
        DescribePixelFormat( s_hGLDC, pf, sizeof( pfd ), &pfd );
        SetPixelFormat( s_hGLDC, pf, &pfd );
        
        s_hglrc = wglCreateContext( s_hGLDC );
        wglMakeCurrent( s_hGLDC, s_hglrc );
        RBSetupPalette( s_hGLDC );
        return 1;
    }
    else
    {
        PVOID lpMsgBuf;
        
        FormatMessage( 
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR) &lpMsgBuf,
            0,
            NULL 
            );
        
        // Display the string.
        MessageBox( NULL, lpMsgBuf, "GetLastError", MB_OK|MB_ICONINFORMATION );
        
        // Free the buffer.
        LocalFree( lpMsgBuf );
        ReleaseDC( hWnd, s_hGLDC );
        return 0;
    }
}

void RBSetupPalette( HDC hDC )
{
    PIXELFORMATDESCRIPTOR pfd;
    LOGPALETTE* pPal;
    int pixelFormat = GetPixelFormat(hDC);
    int paletteSize, status;
    
    status = DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
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
        int ii;
        int nColors = paletteSize - 20;
        float hStep = (360.0f / (float)nColors);
        PALETTEENTRY *pe = &pPal->palPalEntry[0];
        
        for (ii = 10; ii < paletteSize-10; ii++) {
            /* HSV_to_RGB lifted from Foley, et. al. */
            
            float h = ii * hStep, s = 1.0f, v = 1.0f;
            float r, g, b;
            float ih, fh, p, q, t;
            
            h = h / 60.0f;
            ih = (float) floor(h);
            fh = h - ih;
            
            p = v * (1.0f - s);
            q = v * (1.0f - (s * fh));
            t = v * (1.0f - (s * (1.0f - fh)));
            
            switch ((int) ih) {
            case 0:
                r = v; g = t; b = p;
                break;
            case 1:
                r = q; g = v; b = p;
                break;
            case 2:
                r = p; g = v; b = t;
                break;
            case 3:
                r = p; g = q; b = v;
                break;
            case 4:
                r = t; g = p; b = v;
                break;
            case 5:
                r = v; g = p; b = q;
                break;
            }
            
            pe[ii].peRed   = (BYTE) (r * 255);
            pe[ii].peGreen = (BYTE) (g * 255);
            pe[ii].peBlue  = (BYTE) (b * 255);
            pe[ii].peFlags = PC_NOCOLLAPSE;
        }
        
        s_redIndex   = 10 + 0;
        s_greenIndex = 10 + (nColors >> 1);
        s_blueIndex  = 10 + nColors - 1;
        s_clearIndex = 0;
    }
    
    s_hPalette = CreatePalette(pPal);
    free(pPal);
    
    if ( s_hPalette ) {
        SelectPalette(hDC, s_hPalette, FALSE);
        RealizePalette(hDC);
    }
}

void RBShutdown( void )
{
}

void RBDestroyContext( HWND hWnd )
{
    wglMakeCurrent( NULL, NULL );
    if ( s_hGLDC )
        ReleaseDC( hWnd, s_hGLDC );
    if ( s_hglrc )
        wglDeleteContext( s_hglrc );
    
    s_hGLDC = 0;
    s_hglrc = 0;
}

void RBGo( HINSTANCE hInstance, int nCmdShow )
{
    MSG msg;
    
    memset( &s_stats, 0, sizeof( s_stats ) );
    
    RBCreateOpenGLWindow( hInstance, nCmdShow );
    
    /*
    ** show and update window
    */
    ShowWindow( s_hwndOpenGL, nCmdShow );
    UpdateWindow( s_hwndOpenGL );

    /*
    ** create data
    */
    if ( s_elements_data.interleaved_array )
    {
        free( s_elements_data.interleaved_array );
        s_elements_data.interleaved_array = 0;
    }
    if ( s_elements_data.elements_array )
    {
        int i;

        for ( i = 0; i < s_elements_data.num_element_rows; i++ )
        {
            free( s_elements_data.elements_array[i] );
        }

        free( s_elements_data.elements_array );
        s_elements_data.elements_array = 0;
    }

    RBBuildData( s_hwndOpenGL );
    
    if ( s_everything ) 
    {
        FILE* every;
        int zi, si, qi;
        
        every = fopen("every.log", "wt");
        
        RBCreateContext( s_hwndOpenGL );
        s_config.iterations = 200;
        for (zi = 0; zi < 4; zi++) {
            switch (zi) {
            case 0:
                s_config.do_depth_test = 0;
                s_config.do_depth_writes = 0;
                break;
            case 1:
                s_config.do_depth_test = 1;
                s_config.depth_func = GL_ALWAYS;
                s_config.do_depth_writes = 1;
                break;
            case 2:
                s_config.do_depth_test = 1;
                s_config.do_depth_writes = 1;
                s_config.depth_func = GL_LEQUAL;
                break;
            case 3:
                s_config.do_depth_test = 1;
                s_config.do_depth_writes = 0;
                s_config.depth_func = GL_EQUAL;
                break;
            }
            
            for (s_config.do_dither = 0; s_config.do_dither <= 1; s_config.do_dither++) {
                for (si = 0; si < 2; si++) {
                    switch (si) {
                    case 0:
                        s_config.shade_model = GL_FLAT;
                        break;
                    case 1:
                        s_config.shade_model = GL_SMOOTH;
                        break;
                    }
                    
                    for (qi = 0; qi < 5; qi++) {
                        switch (qi) {
                        case 0:
                            s_config.do_texture = 0;
                            s_config.perspective_correction = GL_FASTEST;
                            s_config.texture_environment = GL_REPLACE;
                            break;
                        case 1:
                            s_config.do_texture = 1;
                            s_config.perspective_correction = GL_FASTEST;
                            s_config.texture_environment = GL_REPLACE;
                            break;
                        case 2:
                            s_config.do_texture = 1;
                            s_config.perspective_correction = GL_FASTEST;
                            s_config.texture_environment = s_config.rgb_mode ? GL_MODULATE : GL_ADD;
                            break;
                        case 3:
                            s_config.do_texture = 1;
                            s_config.perspective_correction = GL_NICEST;
                            s_config.texture_environment = GL_REPLACE;
                            break;
                        case 4:
                            s_config.do_texture = 1;
                            s_config.perspective_correction = GL_NICEST;
                            s_config.texture_environment = s_config.rgb_mode ? GL_MODULATE : GL_ADD;
                            break;
                        }
                        
                        RBConfigOpenGL();
                        s_stats.triangles_rendered = 0;
                        s_stats.area_filled = 0.0F;
                        RBDoTest( s_hwndOpenGL );
                        
                        {
                            char msg[80];
                            
                            sprintf(msg,
                                "zi=%d, d=%d, si=%d, qi=%d % 7.3f MPixel/s",
                                zi,
                                s_config.do_dither,
                                si,
                                qi,
                                s_stats.mpixels_second);
                            SetWindowText( s_hwndOpenGL, msg );
                        }
                        if ( s_config.draw_buffer == GL_BACK )
                            SwapBuffers( s_hGLDC );
                        
                        fprintf(every, "% 7.3f", s_stats.mpixels_second);
                    }
                }
                fprintf(every, "\n");
            }
        }
        
        fclose(every);
        RBDestroyContext( s_hwndOpenGL );
        
    } else {
        RBCreateContext( s_hwndOpenGL );
        RBConfigOpenGL();
        
        RBDoTest( s_hwndOpenGL );
        
        if ( s_config.draw_buffer == GL_BACK )
            SwapBuffers( s_hGLDC );
    }
    
    if ( s_noprompt )
        PostMessage( s_hwndOpenGL, WM_CLOSE, 0, 0 );
    
    while ( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
    
    RBDestroyContext( s_hwndOpenGL );
}

HWND RBCreateOpenGLWindow( HINSTANCE hInstance, int nCmdShow )
{
    WNDCLASSEX wc;
    
    /*
    ** register a window class
    */
    memset( &wc, 0, sizeof( wc ) );
    
    wc.cbSize            = sizeof( wc );
    wc.style             = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc       = WndProc;
    wc.cbClsExtra        = 0;
    wc.cbWndExtra        = 0;
    wc.hInstance         = hInstance;
    wc.hIcon             = LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor           = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground     = (HBRUSH) GetStockObject( WHITE_BRUSH );
    wc.lpszMenuName      = NULL;
    wc.lpszClassName     = "RB Canvas";
    wc.hIconSm           = LoadIcon( NULL, IDI_APPLICATION );
    
    RegisterClassEx( &wc );
    
    /*
    ** create a window
    */
    if ( ( s_hwndOpenGL = CreateWindow( "RB Canvas",
        APPNAME,
        WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        0, 0, s_window_width, s_window_height,
        NULL,
        NULL,
        hInstance,
        NULL ) ) == NULL )
    {
        return NULL;
    }
    
    return s_hwndOpenGL;
}

void RBInitDialog( HWND hDlg )
{
    char         buffer[1024];
    HMENU        hMenu;
    char         iterationsbuffer[1024];
    char         vendorbuffer[1024];
    

    if ( strstr( glGetString( GL_EXTENSIONS ), "GL_SGI_index_texture " ) )
        s_index_texture_extension = TRUE;

    strcpy( vendorbuffer, glGetString( GL_VENDOR ) );
    strupr( vendorbuffer );
    
    sprintf( buffer, "\n  OEM:\t\t%s  \n  Version:\t%s\n  Renderer:\t%s\n", glGetString( GL_VENDOR ), glGetString( GL_VERSION ), glGetString( GL_RENDERER ) );
    SetDlgItemText( hDlg, IDC_OEMINFO, buffer );
    
    hMenu = GetMenu( hDlg );
    
    if ( s_config.do_depth_test )
        CheckMenuItem( hMenu, IDM_DEPTHBUFFER_ENABLE, MF_BYCOMMAND | MF_CHECKED );
    else
        CheckMenuItem( hMenu, IDM_DEPTHBUFFER_ENABLE, MF_BYCOMMAND | MF_UNCHECKED );
    
    if ( s_config.do_depth_writes )
        CheckMenuItem( hMenu, IDM_DEPTHMASK, MF_BYCOMMAND | MF_CHECKED );
    else
        CheckMenuItem( hMenu, IDM_DEPTHMASK, MF_BYCOMMAND | MF_UNCHECKED );
    
    if ( s_config.depth_size == 16 )
        SendMessage( hDlg, WM_COMMAND, IDM_DEPTHSIZE_16, 0 );
    else
        SendMessage( hDlg, WM_COMMAND, IDM_DEPTHSIZE_32, 0 );
    
    if ( s_config.depth_clear == 0.0F )
        SendMessage( hDlg, WM_COMMAND, IDM_DEPTHBUFFER_CLEAR0, 0 );
    else
        SendMessage( hDlg, WM_COMMAND, IDM_DEPTHBUFFER_CLEAR1, 0 );
    
    if ( s_config.primitive == IDM_TRIANGLES )
        SendMessage( hDlg, WM_COMMAND, IDM_TRIANGLES, 0 );
    else if ( s_config.primitive == IDM_TRIANGLE_STRIP )
        SendMessage( hDlg, WM_COMMAND, IDM_TRIANGLE_STRIP, 0 );
    else if ( s_config.primitive == IDM_DRAW_ELEMENTS )
        SendMessage( hDlg, WM_COMMAND, IDM_DRAW_ELEMENTS, 0 );
    else if ( s_config.primitive == IDM_CLEAR )
        SendMessage( hDlg, WM_COMMAND, IDM_CLEAR, 0 );
    
    switch ( s_config.depth_func )
    {
    case GL_LESS:
        SendMessage( hDlg, WM_COMMAND, IDM_GL_LESS, 0 );
        break;
    case GL_LEQUAL:
        SendMessage( hDlg, WM_COMMAND, IDM_GL_LEQUAL, 0 );
        break;
    case GL_EQUAL:
        SendMessage( hDlg, WM_COMMAND, IDM_GL_EQUAL, 0 );
        break;
    case GL_GEQUAL:
        SendMessage( hDlg, WM_COMMAND, IDM_GL_GEQUAL, 0 );
        break;
    case GL_GREATER:
        SendMessage( hDlg, WM_COMMAND, IDM_GL_GREATER, 0 );
        break;
    case GL_ALWAYS:
        SendMessage( hDlg, WM_COMMAND, IDM_GL_ALWAYS, 0 );
        break;
    case GL_NEVER:
        SendMessage( hDlg, WM_COMMAND, IDM_GL_NEVER, 0 );
        break;
    case GL_NOTEQUAL:
        SendMessage( hDlg, WM_COMMAND, IDM_GL_NOTEQUAL, 0 );
        break;
    }
    
    if ( s_config.do_fog )
        CheckMenuItem( hMenu, IDM_FOG_ENABLE, MF_BYCOMMAND | MF_CHECKED );
    else
        CheckMenuItem( hMenu, IDM_FOG_ENABLE, MF_BYCOMMAND | MF_UNCHECKED );
    
    if ( s_config.do_cheap_fog )
        CheckMenuItem( hMenu, IDM_FOG_CHEAP, MF_BYCOMMAND | MF_CHECKED );
    else
        CheckMenuItem( hMenu, IDM_FOG_CHEAP, MF_BYCOMMAND | MF_UNCHECKED );
    
    switch ( s_config.fog_curve )
    {
    case GL_LINEAR:
        SendMessage( hDlg, WM_COMMAND, IDM_FOG_LINEAR, 0 );
        break;
    case GL_EXP:
        SendMessage( hDlg, WM_COMMAND, IDM_FOG_EXP, 0 );
        break;
    case GL_EXP2:
        SendMessage( hDlg, WM_COMMAND, IDM_FOG_EXP2, 0 );
        break;
    }
    
    if ( s_config.rgb_mode )
        SendMessage( hDlg, WM_COMMAND, IDM_COLORMODE_RGB, 0 );
    else
        SendMessage( hDlg, WM_COMMAND, IDM_COLORMODE_INDEX, 0 );
    
    if ( s_config.do_dither )
        CheckMenuItem( hMenu, IDM_DITHER, MF_BYCOMMAND | MF_CHECKED );
    else
        CheckMenuItem( hMenu, IDM_DITHER, MF_BYCOMMAND | MF_UNCHECKED );
    
    if ( s_config.draw_buffer == GL_FRONT )
        SendMessage( hDlg, WM_COMMAND, IDM_DRAW_FRONT, 0 );
    else
        SendMessage( hDlg, WM_COMMAND, IDM_DRAW_BACK, 0 );
    
    if ( s_config.shade_model == GL_FLAT )
        SendMessage( hDlg, WM_COMMAND, IDM_GL_FLAT, 0 );
    else
        SendMessage( hDlg, WM_COMMAND, IDM_GL_SMOOTH, 0 );
    
    sprintf( iterationsbuffer, "%d", s_config.iterations );
    SetDlgItemText( hDlg, IDC_ITERATIONS, iterationsbuffer );
    
    switch ( s_config.triangle_size )
    {
    case 3:
        SendMessage( hDlg, WM_COMMAND, IDM_SIZE_3, 0 );
        break;
    case 10:
        SendMessage( hDlg, WM_COMMAND, IDM_SIZE_10, 0 );
        break;
    case 25:
        SendMessage( hDlg, WM_COMMAND, IDM_SIZE_25, 0 );
        break;
    case 50:
        SendMessage( hDlg, WM_COMMAND, IDM_SIZE_50, 0 );
        break;
    case 100:
        SendMessage( hDlg, WM_COMMAND, IDM_SIZE_100, 0 );
        break;
    case 1000:
        SendMessage( hDlg, WM_COMMAND, IDM_SIZE_1000, 0 );
        break;
    case 10000:
        SendMessage( hDlg, WM_COMMAND, IDM_SIZE_10000, 0 );
        break;
    }
    
    /*
    ** configure texture mapping
    */
    if ( s_config.texture_zoom == 1.0F )
        SendMessage( hDlg, WM_COMMAND, IDM_ZOOM_1x1, 0 );
    else if ( s_config.texture_zoom == 2.0F )
        SendMessage( hDlg, WM_COMMAND, IDM_ZOOM_1x2, 0 );
    else if ( s_config.texture_zoom == 4.0F )
        SendMessage( hDlg, WM_COMMAND, IDM_ZOOM_1x4, 0 );
    else
        SendMessage( hDlg, WM_COMMAND, IDM_ZOOM_1x8, 0 );
    
    if ( s_config.do_texture )
        CheckMenuItem( hMenu, IDM_TEXTURE_ENABLE, MF_BYCOMMAND | MF_CHECKED );
    else
        CheckMenuItem( hMenu, IDM_TEXTURE_ENABLE, MF_BYCOMMAND | MF_UNCHECKED );
    
    if ( s_config.perspective_correction == GL_NICEST )
        CheckMenuItem( hMenu, IDM_TEXTURE_PERSPECTIVE_CORRECT, MF_BYCOMMAND | MF_CHECKED );
    else
        CheckMenuItem( hMenu, IDM_TEXTURE_PERSPECTIVE_CORRECT, MF_BYCOMMAND | MF_UNCHECKED );
    
    if ( s_config.texture_clamp_s )
        CheckMenuItem( hMenu, IDM_CLAMP_S, MF_BYCOMMAND | MF_CHECKED );
    else
        CheckMenuItem( hMenu, IDM_CLAMP_S, MF_BYCOMMAND | MF_UNCHECKED );
    
    if ( s_config.texture_clamp_t )
        CheckMenuItem( hMenu, IDM_CLAMP_T, MF_BYCOMMAND | MF_CHECKED );
    else
        CheckMenuItem( hMenu, IDM_CLAMP_T, MF_BYCOMMAND | MF_UNCHECKED );
    
    
    if ( !s_index_texture_extension )
    {
        if ( s_config.texture_environment == GL_ADD )
            s_config.texture_environment = GL_DECAL;
        EnableMenuItem( hMenu, IDM_TEXTURE_ADD, MF_BYCOMMAND | MF_GRAYED );
    }
    
    switch ( s_config.texture_environment )
    {
    case GL_ADD:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_ADD, 0 );
        break;
    case GL_DECAL:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_DECAL, 0 );
        break;
    case GL_REPLACE:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_REPLACE, 0 );
        break;
    case GL_MODULATE:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_MODULATE, 0 );
        break;
    }
    
    switch ( s_config.texture_width )
    {
    case 1:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_WIDTH_1, 0 );
        break;
    case 2:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_WIDTH_2, 0 );
        break;
    case 4:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_WIDTH_4, 0 );
        break;
    case 8:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_WIDTH_8, 0 );
        break;
    case 16:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_WIDTH_16, 0 );
        break;
    case 32:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_WIDTH_32, 0 );
        break;
    case 64:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_WIDTH_64, 0 );
        break;
    case 128:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_WIDTH_128, 0 );
        break;
    case 256:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_WIDTH_256, 0 );
        break;
    case 512:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_WIDTH_512, 0 );
        break;
    }
    
    switch ( s_config.texture_height )
    {
    case 1:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_HEIGHT_1, 0 );
        break;
    case 2:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_HEIGHT_2, 0 );
        break;
    case 4:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_HEIGHT_4, 0 );
        break;
    case 8:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_HEIGHT_8, 0 );
        break;
    case 16:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_HEIGHT_16, 0 );
        break;
    case 32:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_HEIGHT_32, 0 );
        break;
    case 64:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_HEIGHT_64, 0 );
        break;
    case 128:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_HEIGHT_128, 0 );
        break;
    case 256:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_HEIGHT_256, 0 );
        break;
    case 512:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_HEIGHT_512, 0 );
        break;
    }
    
    switch ( s_config.texture_orientation )
    {
    case 0:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_0, 0 );
        break;
    case 90:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_90, 0 );
        break;
    case 180:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_180, 0 );
        break;
    case 270:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_270, 0 );
        break;
    }
    
    switch ( s_config.texture_filter )
    {
    case GL_NEAREST:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_NEAREST, 0 );
        break;
    case GL_LINEAR:
        SendMessage( hDlg, WM_COMMAND, IDM_TEXTURE_LINEAR, 0 );
        break;
    }

#ifdef NOTYET
    if ( !s_index_texture_extension )
    {
        EnableMenuItem( hMenu, IDM_TEXTURE_CI, MF_BYCOMMAND | MF_GRAYED );
        if ( s_config.texture_format == IDM_TEXTURE_CI )
            s_config.texture_format = IDM_TEXTURE_RGB;
    }
    SendMessage( hDlg, WM_COMMAND, s_config.texture_format, 0 );
#endif
    
}

void RBReadConfigFromDialog( HWND hDlg )
{
    char  buffer[1024];
    char *endptr;
    HMENU hMenu = GetMenu( hDlg );
    
    /*
    ** configure test parameters
    */
    GetWindowText( GetDlgItem( hDlg, IDC_ITERATIONS ), buffer, 1023 );
    s_config.iterations = strtol( buffer, &endptr, 10 );
    
    if ( RBMenuItemIsChecked( hMenu, IDM_DRAW_FRONT ) )
        s_config.draw_buffer = GL_FRONT;
    else
        s_config.draw_buffer = GL_BACK;
    
        /*
        ** configure attributes
    */
    s_config.rgb_mode = RBMenuItemIsChecked( hMenu, IDM_COLORMODE_RGB );
    
    s_config.do_dither = RBMenuItemIsChecked( hMenu, IDM_DITHER );
   
    if ( RBMenuItemIsChecked( hMenu, IDM_SIZE_3 ) )
        s_config.triangle_size = 3;
    else if ( RBMenuItemIsChecked( hMenu, IDM_SIZE_10 ) )
        s_config.triangle_size = 10;
    else if ( RBMenuItemIsChecked( hMenu, IDM_SIZE_25 ) )
        s_config.triangle_size = 25;
    else if ( RBMenuItemIsChecked( hMenu, IDM_SIZE_50 ) )
        s_config.triangle_size = 50;
    else if ( RBMenuItemIsChecked( hMenu, IDM_SIZE_100 ) )
        s_config.triangle_size = 100;
    else if ( RBMenuItemIsChecked( hMenu, IDM_SIZE_1000 ) )
        s_config.triangle_size = 1000;
    else if ( RBMenuItemIsChecked( hMenu, IDM_SIZE_10000 ) )
        s_config.triangle_size = 10000;
    
    if ( RBMenuItemIsChecked( hMenu, IDM_GL_SMOOTH ) )
        s_config.shade_model = GL_SMOOTH;
    else 
        s_config.shade_model = GL_FLAT;
    
    if ( RBMenuItemIsChecked( hMenu, IDM_TRIANGLES ) )
        s_config.primitive = IDM_TRIANGLES;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TRIANGLE_STRIP ) )
        s_config.primitive = IDM_TRIANGLE_STRIP;
    else if ( RBMenuItemIsChecked( hMenu, IDM_DRAW_ELEMENTS ) )
        s_config.primitive = IDM_DRAW_ELEMENTS;
    else if ( RBMenuItemIsChecked( hMenu, IDM_CLEAR ) )
        s_config.primitive = IDM_CLEAR;
    
        /*
        ** configure depth buffer variables
    */
    s_config.do_depth_test   = RBMenuItemIsChecked( hMenu, IDM_DEPTHBUFFER_ENABLE );
    s_config.do_depth_writes = RBMenuItemIsChecked( hMenu, IDM_DEPTHMASK );
    if ( RBMenuItemIsChecked( hMenu, IDM_DEPTHSIZE_16 ) )
        s_config.depth_size = 16;
    else
        s_config.depth_size = 32;
    
    if ( RBMenuItemIsChecked( hMenu, IDM_DEPTHBUFFER_CLEAR0 ) )
        s_config.depth_clear = 0.0F;
    else
        s_config.depth_clear = 1.0F;
    
    if ( RBMenuItemIsChecked( hMenu, IDM_GL_LESS ) )
        s_config.depth_func = GL_LESS;
    else if ( RBMenuItemIsChecked( hMenu, IDM_GL_LEQUAL ) )
        s_config.depth_func = GL_LEQUAL;
    else if ( RBMenuItemIsChecked( hMenu, IDM_GL_EQUAL ) )
        s_config.depth_func = GL_EQUAL;
    else if ( RBMenuItemIsChecked( hMenu, IDM_GL_GREATER ) )
        s_config.depth_func = GL_GREATER;
    else if ( RBMenuItemIsChecked( hMenu, IDM_GL_GEQUAL ) )
        s_config.depth_func = GL_GEQUAL;
    else if ( RBMenuItemIsChecked( hMenu, IDM_GL_ALWAYS ) )
        s_config.depth_func = GL_ALWAYS;
    else if ( RBMenuItemIsChecked( hMenu, IDM_GL_NEVER ) )
        s_config.depth_func = GL_NEVER;
    else if ( RBMenuItemIsChecked( hMenu, IDM_GL_NOTEQUAL ) )
        s_config.depth_func = GL_NOTEQUAL;
    
        /*
        ** configure texture map variables
    */
    s_config.do_texture = RBMenuItemIsChecked( hMenu, IDM_TEXTURE_ENABLE );
    if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_PERSPECTIVE_CORRECT ) )
        s_config.perspective_correction = GL_NICEST;
    else
        s_config.perspective_correction = GL_FASTEST;
    
    if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_0 ) )
        s_config.texture_orientation = 0;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_90 ) )
        s_config.texture_orientation = 90;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_180 ) )
        s_config.texture_orientation = 180;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_270 ) )
        s_config.texture_orientation = 270;
    
    if ( RBMenuItemIsChecked( hMenu, IDM_ZOOM_1x1 ) )
        s_config.texture_zoom = 1.0F;
    else if ( RBMenuItemIsChecked( hMenu, IDM_ZOOM_1x2 ) )
        s_config.texture_zoom = 2.0F;
    else if ( RBMenuItemIsChecked( hMenu, IDM_ZOOM_1x4 ) )
        s_config.texture_zoom = 4.0F;
    else if ( RBMenuItemIsChecked( hMenu, IDM_ZOOM_1x8 ) )
        s_config.texture_zoom = 8.0F;

#ifdef NOTYET    
    if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_CI ) )
        s_config.texture_format = IDM_TEXTURE_CI;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_RGB ) )
        s_config.texture_format = IDM_TEXTURE_RGB;
#endif
    
    if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_DECAL ) )
        s_config.texture_environment = GL_DECAL;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_REPLACE ) )
        s_config.texture_environment = GL_REPLACE;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_MODULATE ) )
        s_config.texture_environment = GL_MODULATE;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_ADD ) )
        s_config.texture_environment = GL_ADD;
    
    if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_WIDTH_1 ) )
        s_config.texture_width = 1;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_WIDTH_2 ) )
        s_config.texture_width = 2;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_WIDTH_4 ) )
        s_config.texture_width = 4;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_WIDTH_8 ) )
        s_config.texture_width = 8;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_WIDTH_16 ) )
        s_config.texture_width = 16;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_WIDTH_32 ) )
        s_config.texture_width = 32;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_WIDTH_64 ) )
        s_config.texture_width = 64;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_WIDTH_128 ) )
        s_config.texture_width = 128;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_WIDTH_256 ) )
        s_config.texture_width = 256;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_WIDTH_512 ) )
        s_config.texture_width = 512;
    
    if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_HEIGHT_1 ) )
        s_config.texture_height = 1;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_HEIGHT_2 ) )
        s_config.texture_height = 2;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_HEIGHT_4 ) )
        s_config.texture_height = 4;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_HEIGHT_8 ) )
        s_config.texture_height = 8;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_HEIGHT_16 ) )
        s_config.texture_height = 16;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_HEIGHT_32 ) )
        s_config.texture_height = 32;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_HEIGHT_64 ) )
        s_config.texture_height = 64;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_HEIGHT_128 ) )
        s_config.texture_height = 128;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_HEIGHT_256 ) )
        s_config.texture_height = 256;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_HEIGHT_512 ) )
        s_config.texture_height = 512;
    
    if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_NEAREST ) )
        s_config.texture_filter = GL_NEAREST;
    else if ( RBMenuItemIsChecked( hMenu, IDM_TEXTURE_LINEAR ) )
        s_config.texture_filter = GL_LINEAR;
    
    s_config.ftexture_width  = ( float ) s_config.texture_width;
    s_config.ftexture_height = ( float ) s_config.texture_height;
    
    
    s_config.texture_clamp_s = RBMenuItemIsChecked( hMenu, IDM_CLAMP_S );
    s_config.texture_clamp_s = RBMenuItemIsChecked( hMenu, IDM_CLAMP_T );
    
    /*
    ** configure fog parameters
    */
    s_config.do_fog       = RBMenuItemIsChecked( hMenu, IDM_FOG_ENABLE );
    s_config.do_cheap_fog = RBMenuItemIsChecked( hMenu, IDM_FOG_CHEAP );
    if ( RBMenuItemIsChecked( hMenu, IDM_FOG_LINEAR ) )
        s_config.fog_curve = GL_LINEAR;
    else if ( RBMenuItemIsChecked( hMenu, IDM_FOG_EXP ) )
        s_config.fog_curve = GL_EXP;
    else if ( RBMenuItemIsChecked( hMenu, IDM_FOG_EXP2 ) )
        s_config.fog_curve = GL_EXP2;
    
}

void RBConfigOpenGL( void )
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(0.0, s_window_width, 0.0, s_window_height, 0.0, 1.0);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    glDrawBuffer( s_config.draw_buffer );
    
    glShadeModel( s_config.shade_model );
    
    glDisable( GL_CULL_FACE );
    
    if (s_config.rgb_mode)
        glClearColor( 0.2F, 0.0F, 0.4F, 0.0F );
    else
        glClearIndex( (float) s_clearIndex );
    
    if ( s_config.do_fog )
    {
        float color[4] = { 1.0F, 1.0F, 1.0F, 1.0F };
        
        glEnable( GL_FOG );
        glFogi( GL_FOG_MODE, s_config.fog_curve );
        glFogfv( GL_FOG_COLOR, color );
        
        if ( s_config.do_cheap_fog )
        {
            glHint( GL_FOG_HINT, GL_FASTEST );
        }
        else
        {
            glHint( GL_FOG_HINT, GL_NICEST );
        }
    }
    
    if ( s_config.do_depth_test )
        glEnable( GL_DEPTH_TEST );
    else
        glDisable( GL_DEPTH_TEST );
    
    glDepthMask( ( GLboolean ) s_config.do_depth_writes );
    glClearDepth( s_config.depth_clear );
    glDepthFunc( s_config.depth_func );
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    if ( s_config.do_dither )
        glEnable( GL_DITHER );
    else
        glDisable( GL_DITHER );
    
    if ( s_config.do_texture )
    {
        glEnable( GL_TEXTURE_2D );
        glHint( GL_PERSPECTIVE_CORRECTION_HINT, s_config.perspective_correction );
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, s_config.texture_environment );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, s_config.texture_filter );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, s_config.texture_filter );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_config.texture_clamp_s ? GL_CLAMP : GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, s_config.texture_clamp_t ? GL_CLAMP : GL_REPEAT );
        RBCreateTexture();
    } else {
        glDisable( GL_TEXTURE_2D );
    }
}

int RBMenuItemIsChecked( HMENU hMenu, WPARAM wID )
{
    MENUITEMINFO mif;
    
    mif.cbSize = sizeof( mif );
    mif.fMask  = MIIM_STATE;
    
    GetMenuItemInfo( hMenu, wID, FALSE, &mif );
    
    return ( ( mif.fState & MFS_CHECKED ) != 0 );
}

void RBToggleMenuCheck( HMENU hMenu, WPARAM wID )
{
    if ( RBMenuItemIsChecked( hMenu, wID ) )
        CheckMenuItem( hMenu, wID, MF_BYCOMMAND | MF_UNCHECKED );
    else
        CheckMenuItem( hMenu, wID, MF_BYCOMMAND | MF_CHECKED );
}

#define CHECKITEM( A ) CheckMenuItem( hMenu, A, MF_BYCOMMAND | ( ( wID == A ) ? MF_CHECKED : MF_UNCHECKED ) )

void RBSelectColorMode( HMENU hMenu, WPARAM wID, HWND hDlg )
{
    CHECKITEM( IDM_COLORMODE_RGB );
    CHECKITEM( IDM_COLORMODE_INDEX );

    if ( wID == IDM_COLORMODE_INDEX && !s_index_texture_extension )
    {
        EnableMenuItem( hMenu, 3, MF_BYPOSITION | MF_GRAYED );
    }
    else
    {
        EnableMenuItem( hMenu, 3, MF_BYPOSITION | MF_ENABLED );
    }
    DrawMenuBar( hDlg );
}

void RBSelectDepthFunc( HMENU hMenu, WPARAM wID )
{
    CHECKITEM( IDM_GL_LESS );
    CHECKITEM( IDM_GL_LEQUAL );
    CHECKITEM( IDM_GL_EQUAL );
    CHECKITEM( IDM_GL_GEQUAL );
    CHECKITEM( IDM_GL_GREATER );
    CHECKITEM( IDM_GL_ALWAYS );
    CHECKITEM( IDM_GL_NEVER );
    CHECKITEM( IDM_GL_NOTEQUAL );
}

void RBSelectTextureEnvironment( HMENU hMenu, WPARAM wID )
{
    CHECKITEM( IDM_TEXTURE_ADD );
    CHECKITEM( IDM_TEXTURE_DECAL );
    CHECKITEM( IDM_TEXTURE_MODULATE );
    CHECKITEM( IDM_TEXTURE_REPLACE );
}

#ifdef NOTYET
void RBSelectTextureFormat( HMENU hMenu, WPARAM wID )
{
    CHECKITEM( IDM_TEXTURE_CI );
    CHECKITEM( IDM_TEXTURE_RGB );
}
#endif

void RBSelectTextureHeight( HMENU hMenu, WPARAM wID )
{
    CHECKITEM( IDM_TEXTURE_HEIGHT_1 );
    CHECKITEM( IDM_TEXTURE_HEIGHT_2 );
    CHECKITEM( IDM_TEXTURE_HEIGHT_4 );
    CHECKITEM( IDM_TEXTURE_HEIGHT_8 );
    CHECKITEM( IDM_TEXTURE_HEIGHT_16 );
    CHECKITEM( IDM_TEXTURE_HEIGHT_32 );
    CHECKITEM( IDM_TEXTURE_HEIGHT_64 );
    CHECKITEM( IDM_TEXTURE_HEIGHT_128 );
    CHECKITEM( IDM_TEXTURE_HEIGHT_256 );
    CHECKITEM( IDM_TEXTURE_HEIGHT_512 );
}

void RBSelectTextureWidth( HMENU hMenu, WPARAM wID )
{
    CHECKITEM( IDM_TEXTURE_WIDTH_1 );
    CHECKITEM( IDM_TEXTURE_WIDTH_2 );
    CHECKITEM( IDM_TEXTURE_WIDTH_4 );
    CHECKITEM( IDM_TEXTURE_WIDTH_8 );
    CHECKITEM( IDM_TEXTURE_WIDTH_16 );
    CHECKITEM( IDM_TEXTURE_WIDTH_32 );
    CHECKITEM( IDM_TEXTURE_WIDTH_64 );
    CHECKITEM( IDM_TEXTURE_WIDTH_128 );
    CHECKITEM( IDM_TEXTURE_WIDTH_256 );
    CHECKITEM( IDM_TEXTURE_WIDTH_512 );
}

void RBSelectTextureOrientation( HMENU hMenu, WPARAM wID )
{
    CHECKITEM( IDM_TEXTURE_0 );
    CHECKITEM( IDM_TEXTURE_90 );
    CHECKITEM( IDM_TEXTURE_180 );
    CHECKITEM( IDM_TEXTURE_270 );
}

void RBSelectTextureFilter( HMENU hMenu, WPARAM wID )
{
    CHECKITEM( IDM_TEXTURE_NEAREST );
    CHECKITEM( IDM_TEXTURE_LINEAR );
}

void RBSelectFogCurve( HMENU hMenu, WPARAM wID )
{
    CHECKITEM( IDM_FOG_LINEAR );
    CHECKITEM( IDM_FOG_EXP );
    CHECKITEM( IDM_FOG_EXP2 );
}

void RBSelectTriangleSize( HMENU hMenu, WPARAM wID )
{
    CHECKITEM( IDM_SIZE_3 );
    CHECKITEM( IDM_SIZE_10 );
    CHECKITEM( IDM_SIZE_25 );
    CHECKITEM( IDM_SIZE_50 );
    CHECKITEM( IDM_SIZE_100 );
    CHECKITEM( IDM_SIZE_1000 );
    CHECKITEM( IDM_SIZE_10000 );
}

void RBSelectZoom( HMENU hMenu, WPARAM wID )
{
    CHECKITEM( IDM_ZOOM_1x1 );
    CHECKITEM( IDM_ZOOM_1x2 );
    CHECKITEM( IDM_ZOOM_1x4 );
    CHECKITEM( IDM_ZOOM_1x8 );
}

void RBDoClearTest( HWND hWnd )
{
    glClear( GL_COLOR_BUFFER_BIT );
}

void RBDoHatTest( HWND hWnd )
{
    int i, j;

    glBegin( GL_TRIANGLES );
    for ( j = 0; j < s_elements_data.num_element_rows; j++ )
    {
        for ( i = 0; i < s_elements_data.verts_per_row-2; i++ )
        {
            if ( s_config.do_texture )
                glTexCoord2fv( s_elements_data.interleaved_array[s_elements_data.elements_array[j][i]].st );
            glColor3fv( s_elements_data.interleaved_array[s_elements_data.elements_array[j][i]].rgb );
            glVertex3fv( s_elements_data.interleaved_array[s_elements_data.elements_array[j][i]].xyz );

            if ( s_config.do_texture )
                glTexCoord2fv( s_elements_data.interleaved_array[s_elements_data.elements_array[j][i+1]].st );
            glColor3fv( s_elements_data.interleaved_array[s_elements_data.elements_array[j][i+1]].rgb );
            glVertex3fv( s_elements_data.interleaved_array[s_elements_data.elements_array[j][i+1]].xyz );

            if ( s_config.do_texture )
                glTexCoord2fv( s_elements_data.interleaved_array[s_elements_data.elements_array[j][i+2]].st );
            glColor3fv( s_elements_data.interleaved_array[s_elements_data.elements_array[j][i+2]].rgb );
            glVertex3fv( s_elements_data.interleaved_array[s_elements_data.elements_array[j][i+2]].xyz );
        }
    }
    glEnd();
}

void RBDoHatStripTest( HWND hWnd )
{
    int i, j;

    for ( j = 0; j < s_elements_data.num_element_rows; j++ )
    {
        glBegin( GL_TRIANGLE_STRIP );
        for ( i = 0; i < s_elements_data.verts_per_row; i++ )
        {
            if ( s_config.do_texture )
                glTexCoord2fv( s_elements_data.interleaved_array[s_elements_data.elements_array[j][i]].st );
            glColor3fv( s_elements_data.interleaved_array[s_elements_data.elements_array[j][i]].rgb );
            glVertex3fv( s_elements_data.interleaved_array[s_elements_data.elements_array[j][i]].xyz );
        }
        glEnd();
    }
}

void RBDoHatDrawElementsTest( HWND hWnd )
{
    int i;

    glInterleavedArrays( GL_T2F_C3F_V3F, 0, s_elements_data.interleaved_array );

    for ( i = 0; i < s_elements_data.num_element_rows; i++ )
    {
        glDrawElements( GL_TRIANGLE_STRIP, s_elements_data.verts_per_row, GL_UNSIGNED_INT, s_elements_data.elements_array[i] );
    }
}

void RBDoTest( HWND hWnd )
{
    int i;
    DWORD start, stop;
    unsigned long total = 0L;
    
    glColor3f( 1.0F, 0.0F, 0.0F );
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glFinish();

    for ( i = 0; i < s_config.iterations; i++ )
    {
        start = timeGetTime();
        
        if ( s_config.primitive == IDM_TRIANGLES )
        {
            RBDoHatTest( hWnd );
        }
        else if ( s_config.primitive == IDM_TRIANGLE_STRIP )
        {
            RBDoHatStripTest( hWnd );
        }
        else if ( s_config.primitive == IDM_DRAW_ELEMENTS )
        {
            RBDoHatDrawElementsTest( hWnd );
        }
        else
        {
            RBDoClearTest( hWnd );
        }
        
        glFinish();

        stop = timeGetTime();
        
        s_stats.area_filled        += ( float ) ( s_stats.num_screen_triangles * s_config.triangle_size );
        s_stats.triangles_rendered += s_stats.num_screen_triangles;

        total += ( stop - start );
    }
    
    s_stats.elapsed_time = total / 1000.0F;

    if ( s_config.primitive == IDM_CLEAR )
    {
        RECT r;
        float screen_size, total_rendered;
        
        GetClientRect( hWnd, &r );
        
        screen_size = ( float ) r.right * ( float ) r.bottom;
        total_rendered = screen_size * s_config.iterations;
        
        s_stats.mpixels_second = total_rendered / ( s_stats.elapsed_time * 1000000.0F );
    }
    else
    {
        s_stats.triangles_second = ( ( float ) s_stats.triangles_rendered ) / s_stats.elapsed_time;
        s_stats.mpixels_second   = ( ( float ) ( s_stats.area_filled ) ) / ( 1000000.0F * ( s_stats.elapsed_time ) ) ;
    }
}

void RBLogDialog( HINSTANCE hInstance )
{
    DialogBox( hInstance, MAKEINTRESOURCE( IDD_STATS_DIALOG ), 0, StatsDialogProc );
}

BOOL CALLBACK StatsDialogProc( HWND hDlg,
                              UINT uMsg,
                              WPARAM wParam,
                              LPARAM lParam )
{
    char buffer[4096];
    
    switch( uMsg )
    {
    case WM_INITDIALOG:
        {
            sprintf( buffer, "\n"
                "  Triangle size:\t\t%d\n"
                "\n"
                "  Triangles/second:\t%.0f\n"
                "  Mpixels/second:\t%f\n"
                "\n"
                "",
                s_config.triangle_size,
                s_stats.triangles_second, 
                s_stats.mpixels_second );
            SetDlgItemText( hDlg, IDC_STATS, buffer );
        }
        return TRUE;
    case WM_COMMAND:
        if ( wParam == IDOK )
        {
            EndDialog( hDlg, RB_OK );
            return TRUE;
        }
        break;
    }
    
    return FALSE;
}

void RBGenerateLog( const char *logfilename )
{
    time_t      t;
    const char *enable_string[] = { "disabled", "enabled" };
    const char *datestring;
    FILE       *fp = 0;
    
    const char *depthfunc[] =
    {
        "GL_NEVER",
            "GL_LESS",
            "GL_EQUAL",
            "GL_LEQUAL",
            "GL_GREATER",
            "GL_NOTEQUAL",
            "GL_GEQUAL",
            "GL_ALWAYS"
    };
    
    const char *drawbuffer[] =
    {
        "GL_FRONT",
            "GL_BACK"
    };
    
    const char *perspective[] =
    {
        "GL_FASTEST",
            "GL_NICEST"
    };
    
    const char *fogcurve;
    const char *texenv, *texfilter;
    
    switch ( s_config.fog_curve )
    {
    case GL_LINEAR:
        fogcurve = "GL_LINEAR";
        break;
    case GL_EXP:
        fogcurve = "GL_EXP";
        break;
    case GL_EXP2:
        fogcurve = "GL_EXP2";
        break;
    default:
        fogcurve = "(unknown)";
        break;
    };
    
    switch ( s_config.texture_environment )
    {
    case GL_REPLACE:
        texenv = "GL_REPLACE";
        break;
    case GL_DECAL:
        texenv = "GL_DECAL";
        break;
    case GL_MODULATE:
        texenv = "GL_MODULATE";
        break;
    case GL_ADD:
        texenv = "GL_ADD";
        break;
    default:
        texenv = "(unknown)";
        break;
    }

#ifdef NOTYET    
    switch ( s_config.texture_format )
    {
    case IDM_TEXTURE_CI:
        texfmt = "color-index";
        break;
    case IDM_TEXTURE_RGB:
        texfmt = "RGB";
        break;
    default:
        texfmt = "(unknown)";
        break;
    }
#endif
    
    switch ( s_config.texture_filter )
    {
    case IDM_TEXTURE_NEAREST:
        texfilter = "color-index";
        break;
    case IDM_TEXTURE_LINEAR:
        texfilter = "RGB";
        break;
    default:
        texfilter = "(unknown)";
        break;
    }
    
    if ( ( fp = fopen( logfilename, "a+" ) ) != 0 )
    {
        time( &t );
        datestring = ctime( &t );
        
        fprintf( fp, "\n/*--- %-20.20s ---------------------------------------------------------- */\n\n", datestring );
        fprintf( fp, "OEM:               %s\n", glGetString( GL_VENDOR ) );
        fprintf( fp, "Version:           %s\n", glGetString( GL_VERSION ) );
        fprintf( fp, "Renderer:          %s\n", glGetString( GL_RENDERER ) );
        fprintf( fp, "\n" );
        fprintf( fp, "Triangle size:     %d\n", s_config.triangle_size );
        fprintf( fp, "Dither:            %s\n", enable_string[s_config.do_dither] );
        fprintf( fp, "Draw buffer:       %s\n", drawbuffer[s_config.draw_buffer - GL_FRONT] );
        fprintf( fp, "\n" );
        fprintf( fp, "Depth buffer:      %s\n", enable_string[s_config.do_depth_test] );
        fprintf( fp, "Depth size:        %d-bit\n", s_config.depth_size );
        fprintf( fp, "Depth writes:      %s\n", enable_string[s_config.do_depth_writes] );
        fprintf( fp, "Depth test:        %s\n", depthfunc[s_config.depth_func - GL_NEVER] );
        fprintf( fp, "Depth clear:       %f\n", s_config.depth_clear );
        fprintf( fp, "\n" );
        fprintf( fp, "Fog:               %s\n", enable_string[s_config.do_fog] );
        fprintf( fp, "Cheap fog:         %s\n", enable_string[s_config.do_cheap_fog] );
        fprintf( fp, "Fog curve:         %s\n", fogcurve );
        fprintf( fp, "\n" );
        fprintf( fp, "Texturing:         %s\n", enable_string[s_config.do_texture] );
        fprintf( fp, "Env:               %s\n", texenv );
        fprintf( fp, "Width:             %d\n", s_config.texture_width );
        fprintf( fp, "Height:            %d\n", s_config.texture_height );
        fprintf( fp, "Orientation:       %d\n", s_config.texture_orientation );
        fprintf( fp, "Perspective:       %s\n", perspective[s_config.perspective_correction - GL_FASTEST] );
        fprintf( fp, "Filter:            %s\n", texfilter );
        fprintf( fp, "\n" );
        fprintf( fp, "Iterations:        %d\n",   s_config.iterations );
        fprintf( fp, "Triangles/second:  %.0f\n", s_stats.triangles_second );
        fprintf( fp, "Mpixels/second:    %f\n",   s_stats.mpixels_second );
        
        fclose( fp );
    }
}

void RBStoreConfigToDisk( void )
{
    FILE *fp;
    
    if ( ( fp = fopen( CFGNAME, "wb" ) ) != 0 )
    {
        fwrite( &s_config, sizeof( s_config ), 1, fp );
        fclose( fp );
    }
}

void RBCreateTexture( void )
{
    int num_components;
    int format;
    GLubyte *data;
    int      s, t;
    
    if ( s_config.rgb_mode )
    {
        num_components = 3;
        format         = GL_RGB;
        
        data = malloc( sizeof( unsigned char ) * 4 * s_config.texture_width * s_config.texture_height );

        if ( s_config.random_texture )
            srand( 0 );
        
        for ( t = 0; t < s_config.texture_height; t++ )
        {
            for ( s = 0; s < s_config.texture_width; s++ )
            {
                if ( s_config.random_texture )
                {
                    data[(t*s_config.texture_width+s)*3+0] = rand() * 255 / RAND_MAX;
                    data[(t*s_config.texture_width+s)*3+1] = rand() * 255 / RAND_MAX;
                    data[(t*s_config.texture_width+s)*3+2] = rand() * 255 / RAND_MAX;
                }
                else
                {
                    if ( ( s + t ) & 1 )
                    {
                        data[(t*s_config.texture_width+s)*3+0] = 0xFF;
                        data[(t*s_config.texture_width+s)*3+1] = 0xFF;
                        data[(t*s_config.texture_width+s)*3+2] = 0xFF;
                    }  
                    else
                    {
                        data[(t*s_config.texture_width+s)*3+0] = 0x00;
                        data[(t*s_config.texture_width+s)*3+1] = 0x00;
                        data[(t*s_config.texture_width+s)*3+2] = 0x00;
                    }
                }
            }
        }
        
    }
    else
    {
        num_components = GL_COLOR_INDEX8_EXT;
        format         = GL_COLOR_INDEX;
        
        data = malloc( sizeof( char ) * s_config.texture_width * s_config.texture_height );
        
        for ( t = 0; t < s_config.texture_height; t++ )
        {
            for ( s = 0; s < s_config.texture_width; s++ )
            {
                if ( s_config.random_texture )
                {
                    data[t*s_config.texture_width+s] = rand() * 255 / RAND_MAX;
                }
                else
                {
                    if ( ( s + t ) & 1 )
                    {
                        data[t*s_config.texture_width+s] = 0xFF;
                    }  
                    else
                    {
                        data[t*s_config.texture_width+s] = 0x00;
                    }
                }
            }
        }
    }
    
    glTexImage2D( GL_TEXTURE_2D,
        0,                           // base level
        num_components,              // number of color components in the texture
        s_config.texture_width,
        s_config.texture_height,
        0,                           // border
        format,                      // texture format
        GL_UNSIGNED_BYTE,            // texture type
        data );
    
    free( data );
}

void RbMessage( int type, const char *msg )
{
    switch ( type )
    {
    case RB_MSG_ERROR:
        MessageBox( 0, msg, "RB Error", MB_OK | MB_ICONERROR );
        break;
    case RB_MSG_WARNING:
        MessageBox( 0, msg, "RB Warning", MB_OK | MB_ICONWARNING );
        break;
    case RB_MSG_INFORMATION:
    default:
        MessageBox( 0, msg, "RB Information", MB_OK | MB_ICONINFORMATION );
        break;
    }
}

static void RBHelp( void )
{
   char superbuffer[1000]=
      "-?\t\tthis help screen\n"
      "-noprompt\t\tdon't prompt when done\n"
      "-everything\trun full test suite\n"
      "-checkerboard\tuse a checkerboard texture (not recommended)\n"
      "-lockarrays\t\tuse SGI lock arrays extension\n"
      "-width:val\t\tspecify the window size in pixels (default is 640)\n"
      "-height:val\t\tspecify the window height in pixels (default is 480)\n\n";

   MessageBox( 0, superbuffer, "RasterBench 1.00, Copyright (C) 1997 Silicon Graphics, Inc.", MB_OK | MB_ICONINFORMATION );
}

#define MARGIN 10

void RBBuildData( HWND hWnd )
{
#if PRINTELEMENTS
    FILE *fp = fopen( "debug.out", "wt" );
#endif
    float twidth, theight;
    RECT  rect;
    int   r, c;
    float z = -0.9F;
    float s_scale = 1.0F / ( s_config.ftexture_width * s_config.texture_zoom );
    float t_scale = 1.0F / ( s_config.ftexture_height * s_config.texture_zoom );

    /*
    ** get information on the client rectangle
    */
    GetClientRect( hWnd, &rect );
    twidth = ( float ) sqrt( 2*s_config.triangle_size );
    theight = twidth;

    /*
    ** compute vertex grid
    */
    s_elements_data.num_vertex_rows = ( int ) ( ( float ) ( rect.bottom  - ( 2.0F * MARGIN ) ) / ( theight * 2.0F ) );
    s_elements_data.num_cols        = ( int ) ( ( float ) ( rect.right   - ( 2.0F * MARGIN ) ) / twidth );

    s_elements_data.num_vertex_rows -= 1;
    s_elements_data.num_vertex_rows |= 1;

    s_elements_data.verts_per_row = s_elements_data.num_cols + 1 + s_elements_data.num_cols;

    s_elements_data.num_vertices = s_elements_data.num_vertex_rows * s_elements_data.verts_per_row;

    s_elements_data.interleaved_array = ( RBIVertex * ) malloc( sizeof( RBIVertex ) * s_elements_data.num_vertices );

    for ( r = 0; r < s_elements_data.num_vertex_rows; r++ )
    {
        RBIVertex *rowptr = &s_elements_data.interleaved_array[r*s_elements_data.verts_per_row];

        for ( c = 0; c < s_elements_data.num_cols+1; c++ )
        {
            rowptr->xyz[0] = MARGIN + c * twidth;
            rowptr->xyz[1] = MARGIN + r * theight * 2.0F;
            rowptr->xyz[2] = z;

            rowptr->st[0] = s_scale * rowptr->xyz[0];
            rowptr->st[1] = t_scale * rowptr->xyz[1];

            if ( c % 3 == 0 )
            {
                rowptr->rgb[0] = 1.0F;
                rowptr->rgb[1] = 0.0F;
                rowptr->rgb[2] = 0.0F;
            }
            else if ( c % 3 == 1 )
            {
                rowptr->rgb[0] = 0.0F;
                rowptr->rgb[1] = 1.0F;
                rowptr->rgb[2] = 0.0F;

            }
            else
            {
                rowptr->rgb[0] = 0.0F;
                rowptr->rgb[1] = 0.0F;
                rowptr->rgb[2] = 1.0F;
            }

            rowptr++;
        }

        for ( c = 0; c < s_elements_data.num_cols; c++ )
        {
            rowptr->xyz[0] = MARGIN + c * twidth  + twidth  * 0.5F;
            rowptr->xyz[1] = MARGIN + r * theight * 2.0F + theight;
            rowptr->xyz[2] = z;

            rowptr->st[0] = s_scale * rowptr->xyz[0];
            rowptr->st[1] = t_scale * rowptr->xyz[1];

            if ( c % 3 == 1 )
            {
                rowptr->rgb[0] = 1.0F;
                rowptr->rgb[1] = 0.0F;
                rowptr->rgb[2] = 0.0F;
            }
            else if ( c % 3 == 2 )
            {
                rowptr->rgb[0] = 0.0F;
                rowptr->rgb[1] = 1.0F;
                rowptr->rgb[2] = 0.0F;
            }
            else
            {
                rowptr->rgb[0] = 0.0F;
                rowptr->rgb[1] = 0.0F;
                rowptr->rgb[2] = 1.0F;
            }

            rowptr++;
        }
    }

    /*
    ** compute elements
    */
    s_elements_data.num_element_rows = s_elements_data.num_vertex_rows + s_elements_data.num_vertex_rows - 1;
    s_elements_data.elements_array = ( unsigned int ** ) malloc( sizeof( unsigned int * ) * s_elements_data.num_element_rows );
    for ( r = 0; r < s_elements_data.num_element_rows; r++ )
    {
#if PRINTELEMENTS
        fprintf( fp, "row %d\n", r );
#endif

        s_elements_data.elements_array[r] = ( unsigned int * ) malloc( sizeof( unsigned int ) * s_elements_data.verts_per_row );

        /*
        ** wide bottom row
        */
        if ( !( r & 1 ) )
        {
            for ( c = 0; c < s_elements_data.verts_per_row; c++ )
            {
                if ( !( c & 1 ) )
                {
                    s_elements_data.elements_array[r][c] = r * ( s_elements_data.verts_per_row ) + ( c - r * s_elements_data.verts_per_row ) /2;
                }
                else
                {
                    s_elements_data.elements_array[r][c] = s_elements_data.elements_array[r][c-1] + s_elements_data.num_cols + 1;
                }
            }
        }
        /*
        ** narrow bottom row
        */
        else
        {
            for ( c = 0; c < s_elements_data.verts_per_row; c++ )
            {
                if ( !( c & 1 ) )
                {
                    s_elements_data.elements_array[r][c] = r * ( s_elements_data.verts_per_row ) + ( c - r * s_elements_data.verts_per_row ) / 2 + s_elements_data.num_cols;
                }
                else
                {
                    s_elements_data.elements_array[r][c] = s_elements_data.elements_array[r][c-1] - s_elements_data.num_cols;
                }
            }
        }

#if PRINTELEMENTS
        for ( c = 0; c < s_elements_data.verts_per_row; c++ )
        {
            fprintf( fp, "%d: %.2f,%.2f,%.2f\n", 
                     s_elements_data.elements_array[r][c],
                     s_elements_data.interleaved_array[s_elements_data.elements_array[r][c]].xyz[0],
                     s_elements_data.interleaved_array[s_elements_data.elements_array[r][c]].xyz[1],
                     s_elements_data.interleaved_array[s_elements_data.elements_array[r][c]].xyz[2] );
        }
#endif
    }

    s_stats.num_screen_triangles = s_elements_data.num_element_rows * ( s_elements_data.num_cols * 2 - 1 );

#if PRINTELEMENTS
    fclose( fp );
#endif
}
