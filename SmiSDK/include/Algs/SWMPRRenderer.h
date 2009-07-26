// -*- C++ -*-
// © [2006-2016] Science.Medical.Imaging Group [(unpublished)] /**/Rights Reserved.
//
// UNPUBLISHED -- Rights reserved under the copyright laws of the United
// States.   Use of a copyright notice is precautionary only and does not
// imply publication or disclosure.
//
// THE CONTENT OF THIS WORK CONTAINS TECHNICAL AND INTELLECTUAL PROPERTY OF
// SCIENCE.MEDICAL.IMAGING (groups.google.com/group/medicalimagingscience). 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND SCIENCE.MEDICAL.IMAGING GROUP
// MAKES NO WARRANTY REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY
// SPECIFIC USE. ENTIRE RISK TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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

		void Clear() ;		

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
