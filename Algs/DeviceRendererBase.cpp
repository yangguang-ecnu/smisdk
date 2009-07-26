// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
/// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
//
// Filename:	DeviceRendererBase.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGDeviceRendererBase_HPP_
#define _PGDeviceRendererBase_HPP_
//////////////////////////////////////////////////////////////////////////

#include "Algs/DeviceRendererBase.h"

namespace PGAlgs
{
	using namespace PGCore;
	using namespace PGMath;
	
	template <class T, class U>
	DeviceRendererBase<T, U>::DeviceRendererBase() 
	{				
		m_oImageBuf = 0;
		m_oImageObject = 0;
		m_rows = 0;
		m_columns = 0;
		m_screenX = 0; m_screenLastX =0;
		m_screenY = 0; m_screenLastY =0;
		m_imagesDumped = 0;

		m_zoomFactor = 1.0f;
		m_zFac = 0.0f;
		m_centerOnCursor = 0;			
	};

	template <class T, class U>
	bool DeviceRendererBase<T, U>::SetBlendWeight(const float iWeight, const int iRendererIndex=0) 
	{ 
		if (iRendererIndex<0 || iRendererIndex>m_blendRatio.size()-1)
		{
			return false;
		}
		//return m_mprRenderer[0]->SetBlendWeight(iWeight, iRendererIndex);
		m_blendRatio[iRendererIndex] = iWeight;
		//m_blendRatio[(iRendererIndex+1)%2] = 1.0f-iWeight;
		return true;
	}

	template <class T, class U>
	float DeviceRendererBase<T, U>::GetBlendWeight(const int iRendererIndex) const 
	{ 
		if (iRendererIndex<0 || iRendererIndex>m_blendRatio.size()-1)
		{
			return false;
		}
		return m_blendRatio[iRendererIndex];//m_mprRenderer[0]->SetBlendWeight(iRendererIndex); 
	}

	template <class T, class U>
	bool DeviceRendererBase<T, U>::DumpImage(const std::string& iFolderPath, const int iRendererIndex/*=0*/)
	{
#if 1
		if (!UpdateRender(iRendererIndex, 0)) return false;

		char imgPath[512] = {0};
		if (!iFolderPath.empty())
		{
			_snprintf(imgPath, 511, "%s/RenderedImage_%05d_%02d_%03d.ppm", iFolderPath.c_str(), this, iRendererIndex, m_imagesDumped);
		} else
		{
			_snprintf(imgPath, 511, "C:/Tmp/Dump/RenderedImage_%05d_%02d_%03d.ppm", this, iRendererIndex, m_imagesDumped);
		}
		imgPath[511] = '\0';	

		//PGCore::Image<U> cImage = *m_oImageObject;
		//cImage.FlipVertical();

		DumpImageAsPPM(*m_oImageObject, imgPath);		

		m_imagesDumped++;

		return true;
#endif

		return false;
	}

	template <class T, class U>
	bool DeviceRendererBase<T, U>::SetFlagCenterOnCursor(int iCenterOnCursor)
	{
		m_centerOnCursor=iCenterOnCursor;
		return true;
	}

	template <class T, class U>
	int DeviceRendererBase<T, U>::GetFlagCenterOnCursor() const
	{
		return m_centerOnCursor;
	}


	template <class T, class U>
	bool DeviceRendererBase<T, U>::SetZoomFactor(float iZoomFactor)
	{
		if (iZoomFactor==0.0f) return false;

		m_zoomFactor = iZoomFactor;

		m_zFac = (1.0f - 1.0f/m_zoomFactor) * 0.5f;
		return true;
	}

	template <class T, class U>
	int DeviceRendererBase<T, U>::GetZoomFactor() const 
	{
		return m_zoomFactor;
	}

	template <class T, class U>
	void  DeviceRendererBase<T, U>::Clear() 
	{ 
		// remember to call child's clear before calling this
		if (m_oImageObject) 
		{ 
			delete m_oImageObject; 
			m_oImageObject=0; 
		}		
		
	}

#ifdef _PG_GENERATE_SDK_LIBS_
	template class DeviceRendererBase<PG_RENDERING_IN_TYPE, PG_RENDERING_OUT_TYPE>; 	
#endif

};




//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGDeviceRendererBase_HPP_
