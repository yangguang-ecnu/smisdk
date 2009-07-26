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
// Filename:	DrawHelper.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_DrawHelper_H_
#define _PG_DrawHelper_H_
//////////////////////////////////////////////////////////////////////////

#include "Image.h"
#include "BitImage.h"


namespace PGCore
{
	namespace DrawHelper
	{
		// works only on square images for now
		template <class T>
		static bool DrawCircleOverlaid(int iXc, int iYc, int iRadius, PGCore::Image<T> & ioImage, const float iValue=-1/*, bool iClean=false*/);

		template <class T>
		static bool DrawBox(int iXLeftTop, int iYLeftTop, int iXRightBottom, int iYRightBottom, PGCore::Image<T> & ioImage, const T iValue=-1/*, bool iClean=false*/);
		
		template <class T>
		static bool DrawCircle(int iXc, int iYc, int iRadius, PGCore::Image<T> & ioImage, const T iValue=-1/*, bool iClean=false*/);		
	};
};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_DrawHelper_H_