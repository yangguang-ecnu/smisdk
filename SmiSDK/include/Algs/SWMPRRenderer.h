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
// Filename:	SWMPRRenderer.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGSWMPRRenderer_H_
#define _PGSWMPRRenderer_H_
//////////////////////////////////////////////////////////////////////////


#include "DeviceRendererBase.h"
#include "VoxelToMPRRenderer.h"
#include "../Math/Point3D.h"
#include "../Core/Volume.h"
#include "../Core/Voxel3D.h"
#include "../Core/Timer.h"
#include "ImageFilterBase.h"

#define _USE_TEXTURE_CHKIMG 0
#define _DEBUG_DUMP_IMG 0



namespace PGAlgs
{
	using namespace PGCore;
	using namespace PGMath;

	template <class T, class U>
	class SWMPRRenderer : public DeviceRendererBase<T, U>
	{
	public:
		SWMPRRenderer();

		virtual bool UpdateCB(void	*iDataObject);

		virtual bool SetTransferFunctionLUT(PGCore::TransferFunctionLUT<unsigned char> *iLUT, const int iRendererIndex=0);

		virtual bool SetMaskTransferFunctionLUT(PGCore::TransferFunctionLUT<unsigned char> *iLUT, const int iRendererIndex=0);
		
		virtual bool SetSlicerPositionAndDirection(double iPosition, ePgSlicingDirection iDirection);
		
		//iCursor3D has xyz in [-0.5...0.5] range
		virtual bool SetCursorPosition(const PGMath::Point3D<float>& iCursor3D);
		
		virtual bool SetSkipFactor(int iSkipFactor);

		virtual int GetSkipFactor() const ;		

		bool SetSlicerPosition(double iPosition);
		
		virtual void Render();		

		bool Execute();

		virtual bool PrepareRenderer();

		virtual ~SWMPRRenderer(void);

		virtual void Clear() ;		

		bool UpdateRender(int iIndex=-1, bool iUpdateDisplay=0);

	protected:

		void display();

		virtual bool ImageSizeUpdated();
		
	private:
		
		int									m_renderCount;
		
#ifdef _USE_TEXTURE		
		std::vector<GLuint>					m_pTextureName;
#endif
		
		PGCore::Image<U>					m_checkImage;
		
		std::vector<PGAlgs::VoxelToMPRRenderer<T, U> *>	m_mprRenderer;

		virtual int		init(const int iVolumeIndex=0);

	};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "SWMPRRenderer.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGSWMPRRenderer_H_
