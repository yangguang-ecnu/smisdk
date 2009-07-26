// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
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