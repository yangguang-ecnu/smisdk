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
// Filename:	ImageGaussianDerivative.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//
// Comment:		http://blogs.mathworks.com/steve/2006/11/28/separable-convolution-part-2/

#ifndef _PGImageGaussianDerivative_HPP_
#define _PGImageGaussianDerivative_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/ImageGaussianDerivative.h"
#include "Core/GaussianKernel.h"
#include "Core/DiffOfGaussianDerivativeKernel.h"


namespace PGAlgs
{

template <class T, class U>
ImageGaussianDerivative<T, U>::ImageGaussianDerivative(double iSigma/*=1.0f*/) 
{ 
	m_oImageObject = 0; 
	m_sigma = iSigma;
};

template <class T, class U>
ImageGaussianDerivative<T, U>::~ImageGaussianDerivative(void) 
{ 
	if (m_oImageObject) 
	{
		delete m_oImageObject;
		m_oImageObject = 0;
	}
};

template <class T, class U>
bool ImageGaussianDerivative<T, U>::Execute() {
	
	if (!m_iDataObject) {
		LOG0("ImageGaussianDerivative: Error: no input");
		return false;
	}
	
	// get first image
	PGCore::Image<T> *inImage = (static_cast<PGCore::Image < T > *>(m_iDataObject));
	if (!inImage) {
		LOG0("ImageGaussianDerivative: Error: failure to fetch input image");
		return false;
	}

	long iRows=0, iColumns=0;
	inImage->GetDimensions(iRows, iColumns);
	if (iRows*iColumns <=0) {
		LOG0("ImageGaussianDerivative: Error: invalid image size");
		return false;
	}

	const T* inBuf = inImage->GetBuffer();
	if (!inBuf) 
	{
		LOG0("ImageGaussianDerivative: Error: invalid image");
		return false;
	}

	if (m_oImageObject == NULL)
		m_oImageObject = new PGCore::Image<U>(iRows, iColumns);
	U* outBuf = m_oImageObject->GetBuffer();
	if (!outBuf) 
	{
		LOG0("ImageGaussianDerivative: Error: invalid out image");
		return false;
	}

	/* 
	1. Compute the Gaussian and the Gaussian derivative kernels.
	2. Smooth the image with the Gaussian in the y direction and convolve the image with
	the Gaussian derivative in the x direction. The result is the gradient in the x direction.
	3. Smooth the image with the Gaussian in the x direction and convolve the image with
	the Gaussian derivative in the y direction. The result is the gradient in the y direction.
	4. Compute the magnitude and direction of the gradient.
	*/

	// 1
	double dSigma = m_sigma;
	PGCore::GaussianKernel<double, double> gaussian(dSigma);		
	PGCore::DerivativeOfGaussianKernel<double, double> derivOfGaussian(dSigma);		
	PGCore::Image<double> oImageDerivG(iRows, iColumns), oImageDerivGX(iRows, iColumns), oImageDerivGY(iRows, iColumns);

	// 2
	{
		gaussian.Convolve1D_Y(*inImage, oImageDerivG);
		derivOfGaussian.Convolve1D_X(oImageDerivG, oImageDerivGX);
	}
	// 3
	{
		gaussian.Convolve1D_X(*inImage, oImageDerivG);
		derivOfGaussian.Convolve1D_Y(oImageDerivG, oImageDerivGY);
	}

	const T* inBufX = oImageDerivGX.GetBuffer();
	if (!inBufX)
	{
		return false;
	}

	const T* inBufY = oImageDerivGY.GetBuffer();
	if (!inBufY)
	{
		return false;
	}

	// 4
	{				
		long imgIter = 0;				
		while (imgIter< iRows*iColumns) 
		{
			double inValX = *(inBufX+imgIter);
			double inValY = *(inBufY+imgIter);
			double inVal = fabs(inValX) + fabs(inValY);
			U outVal = (U)((inVal));
			*(outBuf+imgIter) = outVal;			
			imgIter++;

		}		
	} 	

	
	m_oDataObject = (static_cast<PGCore::BaseDataObject *>(m_oImageObject));

	return true;
}

#ifdef _PG_GENERATE_SDK_LIBS_
	template class ImageGaussianDerivative<double, double>; 
	//template class ImageGaussianDerivative<short, double>; 
	//template class ImageGaussianDerivative<unsigned char, double>;
#endif

};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageGaussianDerivative_HPP_
