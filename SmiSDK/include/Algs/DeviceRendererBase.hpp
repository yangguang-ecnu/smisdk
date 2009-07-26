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
// Filename:	DeviceRendererBase.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGDeviceRendererBase_HPP_
#define _PGDeviceRendererBase_HPP_
//////////////////////////////////////////////////////////////////////////

#include "DeviceRendererBase.h"

namespace PGAlgs
{
	using namespace PGCore;
	using namespace PGMath;
	
	

	template <class T, class U>
	bool DeviceRendererBase<T, U>::SetBlendWeight(const float iWeight, const int iRendererIndex=0) 
	{ 
		if (iRendererIndex<0 || iRendererIndex>m_blendRatio.size()-1)
		{
			return false;
		}
		//return 
		//m_mprRenderer[iRendererIndex]->SetBlendWeight(iWeight, iRendererIndex);
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
};




//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGDeviceRendererBase_HPP_
