// -*- C++ -*-
// � [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved, see below.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use
// this file except in compliance with the License. You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// More details about the license can be found here: http://www.apache.org/licenses
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is 
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
// See the License for the specific language governing permissions and limitations under the License.
//
// Filename:	GLRendererBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGGLRendererBase_H_
#define _PGGLRendererBase_H_
//////////////////////////////////////////////////////////////////////////
//#define _USE_GLEW "_USE_GLEW"
#define _PG_GLSL_VR_ 1
#define _PG_MAX_GL_TX_SIZE_ 1560
#define _PG_MAX_GL_TX_XY_SIZE_ 256
#define _PG_GLUT_WIN_SZ_ 800

#include "DeviceRendererBase.h"
#include "VoxelToMPRRenderer.h"
#include "Math/Point3D.h"
#include "Core/Volume.h"
#include "Core/Voxel3D.h"
#include "Core/Timer.h"
#include "ImageFilterBase.h"
#include <windows.h>
#include <wingdi.h>

#ifdef _USE_GLEW	
	#include <GL/glew.h>
#else	
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
	#include <GL/glprocs.h> // OpenGL {1.2, 1.3, 1.4, 1.5} and extension functions
#endif


static void *PgStaticRenderer = 0;
// static vars to enable glut handling
static int  gSparseMode=0, gLeftDown=0, gMiddleDown=0, gRightDown=0, gXDown=0, gYDown=0, gXUp=0, gYUp=0, gXTemp=0, gYTemp=0;
static float gRotationIncrement=5.0f, gTranslationIncrement=0.0001f, gSlicerPos=0.0f;
static PGMath::Vector3D<float> gRotationStatic(0, 0, 0), gTranslationStatic(0, 0, 0);



namespace PGAlgs
{
	using namespace PGCore;
	using namespace PGMath;

	
	
	
	template <class T, class U>
	class GLRendererBase : public DeviceRendererBase<T, U>
	{
	public:	

		GLRendererBase();
		
		virtual ~GLRendererBase(void);
			
		// no need to call this method if glut initilization is desired
		virtual bool SetContext(HDC iContext);

		// Kilo Bytes
		long GetMaximum3DTextureMemory(); // returns amount of 3D texture memory available

		// Kilo Bytes
		long GetAvailableGPUMemory(); 

		virtual bool PrepareRenderer(); 
	
		virtual bool UpdateRender(int iIndex=-1, bool iUpdateDisplay=0);
		
		virtual void Render();

		virtual void Clear();

		virtual const PGMath::AffineTransform<float>&	GetViewTransform() { return PGMath::AffineTransform<float>(); };
		
		virtual bool SetViewTransform(const PGMath::AffineTransform<float>&	iTransform) { return false; } 

		virtual bool SetSlicerPosition(double iPosition); // MPR plane position. VR: cut plane position

		bool SetTranslationUnit(const float& iTransX, const float& iTransY, const float& iTransZ, int iIndex=-1);		
		
		bool SetRotationAngles(const float& iRotX, const float& iRotY, const float& iRotZ, int iIndex=-1);
		
		virtual bool SetSkipFactor(int iSkipFactor) { m_skipFactor = iSkipFactor; return true; };

		virtual int  GetSkipFactor() const { return m_skipFactor; };

		virtual bool SetSuperSamplingFactors(const PGMath::Point3D<float>& iSamplingFactors) { return false; };

		virtual bool UpdateLUTParams(const int iLowIncr, const int iHighIncr, const int iLowGradIncr, const int iHighGradIncr) { LOG0("GLRendererBase::UpdateLUTParams"); return true; };

	protected:				
		int						m_skipFactor;
		int						m_max3DTextureSize;//KB
		long					m_remaining3DTextureSize;//KB		

		PGMath::Vector3D<float>	gTranslation[2];
		PGMath::Vector3D<float>	gRotation[2];
		
		bool							    m_externalContext;
		bool								m_glutInitialized;
		bool								m_glutLoopInitialized;		
		HDC									m_hdContext;
		HGLRC								m_glContext;
		
		GLenum								m_imageFormat, m_imageType;

		virtual void display() = 0;
		virtual bool ImageSizeUpdated() = 0;		
		virtual int	init(const int iVolumeIndex=0) = 0;	

		virtual int	initGLthread(const int iVolumeIndex=0);
		bool initializeOpenGL(HDC hdc);
		int	 choosePixelFormatEx(HDC hdc,int *p_bpp,int *p_depth,int *p_dbl,int *p_acc);
		int logLastGlError();
		
		// methods for glut initialization and usage when external drawing context isnt passed
		static void StaticRender(void);
		static bool InitGlutWindow(int argc, char *argv[], const int iSx=256, const int iSy=256);		
		static bool StartGlutRenderLoop(void *iRenderer);
		
		static void ReshapeCB(int iNewWidth, int iNewHeight);
		static void MouseClickCB(int button, int state, int x, int y );
		static void MouseMotionCB(int x, int y);	
		static void KeyPressCB(unsigned char key, int x, int y);
	};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "GLRendererBase.hpp"
#endif


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGGLRendererBase_H_
