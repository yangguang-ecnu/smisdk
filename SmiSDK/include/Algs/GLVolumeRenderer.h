// -*- C++ -*-
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved, see below.
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
// Filename:	GLVolumeRenderer->h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGGLVolumeRenderer_H_
#define _PGGLVolumeRenderer_H_
//////////////////////////////////////////////////////////////////////////


#include "GLRendererBase.h"
#include "../Core/Volume.h"
#include "../Core/Timer.h"


#define _TEST_SCENE 0
//#define _HIGH_BW "_HIGH_BW"
//#define _USE_CG
#ifdef _USE_CG
// to use fragment shading
#include <Cg/cgGL.h>

#pragma comment( lib, "cg.lib" )							// Search For Cg.lib While Linking
#pragma comment( lib, "cggl.lib" )							// Search For CgGL.lib While Linking
#endif


namespace PGAlgs
{

#ifdef _USE_CG
	typedef struct {
		// ** Nvidia CG Stuff
		CGprogram fragmentProgram;
		CGprogram fragmentSlicerProgram;
		CGcontext shaderContext;
		CGprofile fragmentProfile;
		CGparameter cgTex3d;
		CGparameter cgTexColormap;
		//CGparameter cgSlicerTexColormap;
	} CgInfo;
	PGAlgs::CgInfo					gCgInfo;
#endif

	const int				kPgFramesToMeasure=16;	
	const float				kPgMinThNoiseInMask[3] = 
	{
		0.70f, // CT
		0.25f, // MR
		0.25f // PET
	};

	
template <class T>
class ThreadedUpdateVRParams : public AsyncProcessParams
{
public: 
	ThreadedUpdateVRParams<T>() 
	{
		m_hdContext = (HDC)0;
		m_pIVolume = 0;
		m_MaskLUT = 0;
		m_LuT = 0;
		m_blendRatio = 0.5f;
		m_rendererIndex = -1;
		m_updateDisplay = true;
		m_rows=256;
		m_columns=256;
	};

	HDC														 m_hdContext;
	PGCore::Voxel3D<T>										 *m_pIVolume;
	PGCore::TransferFunctionLUT<PG_RENDERING_OUT_TYPE>		 *m_MaskLUT;
	PGCore::TransferFunctionLUT<PG_RENDERING_OUT_TYPE>		 *m_LuT;
	float													  m_blendRatio;
	int														  m_rendererIndex;
	bool													  m_updateDisplay;
	unsigned int											  m_rows, m_columns;	
};

	template <class T, class U>
	class GLVolumeRenderer : public GLRendererBase<T, U>
	{
	public:
		GLVolumeRenderer(void);

		virtual bool SetInput(PGCore::BaseDataObject *iDataObject);

		virtual bool UpdateCB(void	*iDataObject);

		virtual bool SetTransferFunctionLUT(PGCore::TransferFunctionLUT<U> *iLUT, const int iRendererIndex=0);
		
		virtual bool SetMaskTransferFunctionLUT(PGCore::TransferFunctionLUT<U> *iLUT, const int iRendererIndex=0);

		//iCursor3D has xyz in [-0.5...0.5] range
		virtual bool SetCursorPosition(const PGMath::Point3D<float>& iCursor3D);

		// xyz in [-0.5...0.5] range
		virtual bool SetSlicerPosition(double iPosition);
		
		virtual bool SetSuperSamplingFactors(const PGMath::Point3D<float>& iSamplingFactors) { gSuperSampligFactorZ = iSamplingFactors.Z(); return true; }

		virtual bool Execute();

		virtual bool UpdateRender(int iIndex=-1, bool iUpdateDisplay=0) = 0;
		
		virtual bool PrepareRenderer() = 0;

		virtual ~GLVolumeRenderer(void); 		

		virtual const PGMath::AffineTransform<float>&	GetViewTransform();
		
		virtual bool SetViewTransform(const PGMath::AffineTransform<float>&	iTransform);

		virtual bool SetContext(HDC iContext);

		
	protected:

		virtual void display() = 0;

		virtual bool ImageSizeUpdated();

	
		bool							m_ready;	
		float							m_coronalCutPlanePosition;
		PGMath::Point3D<float>			m_cursor3D;

		PGCore::Logger					m_threadLogger;
		ThreadedUpdateVRParams<T>		m_updateVRParams;

		std::vector<PGCore::Voxel3D<T> *>	m_voxel3DList;
		std::vector<PGCore::Volume< T > *>	m_volumeAccessor;
		std::vector<PGMath::Point3D<long> >	m_voxelDims;
		std::vector<PGCore::TransferFunctionLUT<U> *> m_MaskLUT;
		std::vector<PGCore::TransferFunctionLUT<U> *> m_LuT;		
		
		unsigned int			gLeftDown, gMiddleDown, gRightDown;
		int						gXDown, gXUp, gYDown, gYUp, gXTemp, gYTemp;
		float					gCameraNear, gCameraFar, gCameraEyeZ;
		float					gDzFull, gDzSparse, gRotate;
		unsigned int			gLowerBound, gLowerEndNoiseBound;
		float					gSuperSampligFactorZ;
		
		
		PGMath::Vector3D<float>	gMinVec, gMaxVec, gCameraCenter;
		
		float					gXMin, gXMax, gYMin, gYMax, gZMin, gZMax; 
		
		std::vector<PGMath::Point3D<long> >   gVolumeScope;
		
		unsigned int			gFrameCount;
		PGCore::Timer			gTimer;

		virtual void	renderSingleVolume(const int iVolumeIndex=0)=0;
		virtual int		initGLthread()=0; 
		virtual int		init(const int iVolumeIndex=0)=0;

		void	renderVolume();
		void	renderAxisPlanes();	
		void	getGLViewParams (
			PGMath::Vector3D< float > &oMinVec,
			PGMath::Vector3D< float > &oMaxVec,							
			PGMath::Vector3D< float > &oCameraCenter,
			float &oCameraEye,
			float &oCameraNear,
			float &oCameraFar, const int iVolumeIndex=0
			);
		
		void clampToNextHigherNumber(PGMath::Point3D<long>& ioDim);		
	};
};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "GLVolumeRenderer.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGGLVolumeRenderer_H_
