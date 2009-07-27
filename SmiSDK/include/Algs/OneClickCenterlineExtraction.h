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
// Filename:	OneClickCenterlineExtraction.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGOneClickCenterlineExtraction_H_
#define _PGOneClickCenterlineExtraction_H_
//////////////////////////////////////////////////////////////////////////


#include "CenterlineExtractionBase.h"

// implement hessian analysis based centerline extraction

namespace PGAlgs
{

	template <class T, class U>
	class OneClickCenterlineExtraction : public CenterlineExtractionBase<T, U>
	{
	public:

		OneClickCenterlineExtraction(void);		

		virtual bool Execute();

		virtual eCLineRetCode conditionCheck(const PGMath::Point3D<int> &iVoxel);
	

	public:	
		virtual ~OneClickCenterlineExtraction(void);

	protected:
		virtual bool autoAdjustConditions(const PGMath::Point3D<int>& iSeed, const eCLineRetCode iReason);

	};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "OneClickCenterlineExtraction.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGOneClickCenterlineExtraction_H_
