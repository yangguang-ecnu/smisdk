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
