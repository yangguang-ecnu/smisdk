// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
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
// Filename:	ImageCompressorBase.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageCompressorBase_CPP_
#define _PGImageCompressorBase_CPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/ImageCompressorBase.h"

namespace PGAlgs
{

	
template <class T, class U>
ImageCompressorBase<T, U>::ImageCompressorBase()
{ 
	m_oImageObject = 0; 	
	SetCompressionFlag(true);
};


template <class T, class U>
ImageCompressorBase<T, U>::~ImageCompressorBase(void)
{ 
	if (m_oImageObject) 
	{
		delete m_oImageObject;
		m_oImageObject = 0;
	};
}

template <class T, class U>
void ImageCompressorBase<T, U>::SetCompressionFlag(bool iForward)
{
	m_Forward = iForward;	
}

#ifdef _PG_GENERATE_SDK_LIBS_
	template class ImageCompressorBase<short, short>; 
	template class ImageCompressorBase<unsigned char, unsigned char>;
	template class ImageCompressorBase<PGMath::Point3D<unsigned char>, PGMath::Point3D<unsigned char>>;	
	template class ImageCompressorBase<PGMath::Point3D<unsigned char>, unsigned char>;	
	template class ImageCompressorBase<unsigned char, PGMath::Point3D<unsigned char>>;	
#endif


};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageCompressorBase_CPP_
