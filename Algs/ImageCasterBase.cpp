// -*- C++ -*-
// � [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved, see below.
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
// Filename:	ImageCasterBase.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//
//////////////////////////////////////////////////////////////////////////


#include "Algs/ImageCasterBase.h"

namespace PGAlgs
{

template <class T, class U>
ImageCasterBase<T, U>::ImageCasterBase(void) 
{

};

template <class T, class U>
bool ImageCasterBase<T, U>::GetOutput(PGCore::BaseDataObject *oDataObject)  
{
	PGCore::Image<U> *tempImgPtr	= static_cast<PGCore::Image < U > *>(m_oDataObject);
	PGCore::Image<U> *tempOutImgPtr = static_cast<PGCore::Image<U> *>(oDataObject);
	*tempOutImgPtr = *tempImgPtr;
	return true;
}	

template <class T, class U>
ImageCasterBase<T, U>::~ImageCasterBase(void) 
{

};


#ifdef _PG_GENERATE_SDK_LIBS_
	template class ImageCasterBase<short, short>; 
	template class ImageCasterBase<unsigned char, unsigned char>; 	
	template class ImageCasterBase<float, short>; 
	template class ImageCasterBase<short, float>; 
	template class ImageCasterBase<float, unsigned char>; 
	template class ImageCasterBase<unsigned char, float>; 
	template class ImageCasterBase<PGMath::Point3D<unsigned char>, PGMath::Point3D<unsigned char>>; 	
#endif
};




//////////////////////////////////////////////////////////////////////////
// EOF

