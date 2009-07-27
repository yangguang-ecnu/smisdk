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
// Filename:	ImageAnisotropicDiffuser.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageAnisotropicDiffuser_H_
#define _PGImageAnisotropicDiffuser_H_
//////////////////////////////////////////////////////////////////////////


#include "ImageCasterBase.h"

namespace PGAlgs
{

template <class T, class U>
class ImageAnisotropicDiffuser : public ImageCasterBase<T, U>
{
public:
	ImageAnisotropicDiffuser() { m_oImageObject = 0; };

	bool Execute();

public:	
	virtual ~ImageAnisotropicDiffuser(void) { if (m_oImageObject) delete m_oImageObject;};

protected:

	bool diffuseAniso(const PGCore::Image<T> &iImage, PGCore::Image<U> &oImage);

private:
	PGCore::Image<U>	*m_oImageObject;

	
};


template <class T, class U>
bool ImageAnisotropicDiffuser<T, U>::Execute()
{
	// swap endians here
	if (sizeof(T) != sizeof(U) || sizeof(T)!=sizeof(short)) {
		LOG2("ImageAnisotropicDiffuser: Error swapping endians. Types different sizes: %d - %d", sizeof(T), sizeof(U));
		return false;
	}

	if (!m_iDataObject) {
		LOG0("ImageAnisotropicDiffuser: Error: no input");
		return false;
	}
	
	// also check base image types here

	// get first image
	PGCore::Image<T> *inImage = (static_cast<PGCore::Image < T > *>(m_iDataObject));
	if (!inImage) {
		LOG0("ImageAnisotropicDiffuser: Error: failure to fetch input image");
		return false;
	}

	long iRows=0, iColumns=0;
	inImage->GetDimensions(iRows, iColumns);
	if (iRows*iColumns <=0) 
	{
		LOG0("ImageAnisotropicDiffuser: Error: invalid image size");
		return false;
	}

	m_oImageObject = new PGCore::Image<U>(iRows, iColumns);
	
	if (!diffuseAniso(*inImage, *m_oImageObject))
	{
		LOG0("ImageAnisotropicDiffuser: Error: Failed to diffuse");
		return false;
	}
	
	m_oDataObject = (static_cast<PGCore::BaseDataObject *>(m_oImageObject));

	return true;
}



template <class T, class U>
bool ImageAnisotropicDiffuser<T, U>::diffuseAniso(const PGCore::Image<T> &iImage, PGCore::Image<U> &oImage)
{
	const T* inBuf = iImage.GetBuffer();
	if (!inBuf) 
	{
		LOG0("ImageAnisotropicDiffuser: Error: invalid image");
		return false;
	}

	long iRows=0, iColumns=0;
	iImage.GetDimensions(iRows, iColumns);
	if (iRows*iColumns <=0) 
	{
		LOG0("ImageAnisotropicDiffuser: Error: invalid image size");
		return false;
	}


	U* outBuf = oImage.GetBuffer();
	if (!outBuf) 
	{
		LOG0("ImageAnisotropicDiffuser: Error: invalid out image");
		return false;
	}

	{				
		long imgIter = 0;				
		while (imgIter< iRows*iColumns) 
		{
			//errrr

			//T inVal = *(inBuf+imgIter);
			//U outVal = diffuseAniso(inVal);				
			//*(outBuf+imgIter) = outVal;			
			imgIter++;

		}		
	} 	

	return true;
}


};



//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageAnisotropicDiffuser_H_
