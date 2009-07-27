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
// Filename:	GLMPRRenderer.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGGLMPRRenderer_H_
#define _PGGLMPRRenderer_H_
//////////////////////////////////////////////////////////////////////////


#include "Algs/GLRendererBase.h"
#include "Algs/VoxelToMPRRenderer.h"
#include "Algs/ImageFilterBase.h"
#include "Math/Point3D.h"
#include "Core/Volume.h"
#include "Core/Voxel3D.h"
#include "Core/Timer.h"


//if disabled, works fine even after getting context
#define _USE_TEXTURE
#define _USE_TEXTURE_CHKIMG 0
#define _ZOOM_RECENTER 1
//#define _DEBUG_DUMP_IMG 1

static const char *outdataPath = "C:/Data/";

#ifdef _DEBUG_DUMP_IMG
	static int gDumpedImage=0;
#endif


namespace PGAlgs
{
	using namespace PGCore;
	using namespace PGMath;

	template <class T, class U>
	class GLMPRRenderer : public GLRendererBase<T, U>
	{
	public:

		GLMPRRenderer(); 
		
		virtual bool UpdateCB(void	*iDataObject);		
		
		virtual bool SetTransferFunctionLUT(PGCore::TransferFunctionLUT<U> *iLUT, const int iRendererIndex=0);
		virtual bool SetMaskTransferFunctionLUT(PGCore::TransferFunctionLUT<U> *iLUT, const int iRendererIndex=0);
		
		virtual bool SetSlicerPosition(double iPosition);

		virtual bool SetSlicerDirection(ePgSlicingDirection iDirection);
		
		const PGMath::AffineTransform<float>&	GetViewTransform() const;
		
		bool SetViewTransform(const PGMath::AffineTransform<float>&	iTransform);
		
		//iCursor3D has xyz in [-0.5...0.5] range
		virtual bool SetCursorPosition(const PGMath::Point3D<float>& iCursor3D);

		
		virtual bool SetSkipFactor(int iSkipFactor);

		virtual int GetSkipFactor() const ;
		
		bool Execute();

		virtual bool PrepareRenderer();

		virtual ~GLMPRRenderer(void) { Clear(); };

		void Clear() ;
		

		virtual bool UpdateRender(int iIndex=-1, bool iUpdateDisplay=0);

		virtual bool SetBlendWeight(const float iWeight, const int iRendererIndex=0);
		

public:


	protected:
		void display();

		virtual bool ImageSizeUpdated();

		bool LoadImage(int iBeginIndex, int iEndIndex);
		
	private:
		
		int									m_renderCount;
		
#ifdef _USE_TEXTURE
		std::vector<GLuint>					m_pTextureName;
#endif
		
		PGCore::Image<U>					m_checkImage;

		GLfloat								m_colorXAxis[4], m_colorYAxis[4];
	
		std::vector<PGAlgs::VoxelToMPRRenderer<T, U> *>	m_mprRenderer;
		
		void	getGLViewParams (
			PGMath::Vector3D< float > &oMinVec,
			PGMath::Vector3D< float > &oMaxVec,							
			PGMath::Vector3D< float > &oCameraCenter,
			float &oCameraEye,
			float &oCameraNear,
			float &oCameraFar
			);

		virtual int		initGLthread(const int iVolumeIndex=0);
		virtual int		init(const int iVolumeIndex=0);

	};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "GLMPRRenderer.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGGLMPRRenderer_H_
