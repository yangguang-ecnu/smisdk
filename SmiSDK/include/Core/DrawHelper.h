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