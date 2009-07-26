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
// Filename:	DeviceRendererBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGDeviceRendererBase_H_
#define _PGDeviceRendererBase_H_
//////////////////////////////////////////////////////////////////////////
//#define _USE_GLEW "_USE_GLEW"

#include "RendererBase.h"
#include "VoxelToMPRRenderer.h"
#include "Math/Point3D.h"
#include "Core/Volume.h"
#include "Core/Voxel3D.h"
#include "Core/Timer.h"
#include "ImageFilterBase.h"



#include <windows.h>


namespace PGAlgs
{
	using namespace PGCore;
	using namespace PGMath;

//#if (_DEBUG_DUMP_IMG)
	//static int						gDumpedImage=0;
//#endif

	template <class T, class U>
	class DeviceRendererBase : public RendererBase<T, U>
	{
	public:
		DeviceRendererBase();		

		virtual ~DeviceRendererBase(void) { Clear(); };

		virtual bool UpdateCB(void	*iDataObject) = 0;

		virtual bool SetTransferFunctionLUT(PGCore::TransferFunctionLUT<U> *iLUT, const int iRendererIndex=0) = 0;

		//iCursor3D has xyz in [-0.5...0.5] range
		virtual bool SetCursorPosition(const PGMath::Point3D<float>& iCursor3D) = 0;		

		virtual bool SetSkipFactor(int iSkipFactor) = 0;		

		virtual int  GetSkipFactor() const = 0;

		virtual bool Execute() = 0;

		virtual bool UpdateRender(int iIndex=-1, bool iUpdateDisplay=0) = 0;				

		virtual bool SetBlendWeight(const float iWeight, const int iRendererIndex=0);

		float GetBlendWeight(const int iRendererIndex) const;

		bool SetFlagCenterOnCursor(int iCenterOnCursor);

		int GetFlagCenterOnCursor() const;

		bool SetZoomFactor(float iZoomFactor);

		int GetZoomFactor() const;

		bool DumpImage(const std::string& iFolderPath, const int iRendererIndex=0);

		virtual bool PrepareRenderer()=0;

		virtual void Render()=0;

		virtual void Clear();



	protected:

		virtual void display() = 0;
		virtual bool ImageSizeUpdated() = 0;

		
		
		int									m_centerOnCursor;
		float								m_zoomFactor;
		float								m_zFac;
		PGCore::Image<PGMath::Point3D<U>>   *m_oImageObject, m_tempImageObject;	
		PGMath::Point3D<U>					*m_oImageBuf;

		std::vector<float>					m_blendRatio;


		PGMath::Point3D< long >				m_voxelDims;


		float								m_screenX, m_screenY;
		float								m_screenLastX, m_screenLastY;

		int									m_imagesDumped;

		virtual int		init(const int iVolumeIndex=0) = 0;

	};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "DeviceRendererBase.hpp"
#endif


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGDeviceRendererBase_H_
