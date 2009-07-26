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
// Filename:	ImageFourierTransform.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageFourierTransform_H_
#define _PGImageFourierTransform_H_
//////////////////////////////////////////////////////////////////////////


#include "ImageFilterBase.h"
#include "Core/GaussianKernel.h"
#include "Core/DiffOfGaussianDerivativeKernel.h"


namespace PGAlgs
{

	template <class T, class U>
	class ImageFourierTransform : public ImageFilterBase<T, U>
	{
	public:
		ImageFourierTransform();

		void SetInversionFlag(bool iInverseTransform);
		
		bool GetInversionFlag() const;

		bool GetFourierImages(PGCore::Image<U> &oImageReal, PGCore::Image<U> &oImageImaginary) const;
		
		bool SetFourierImages(const PGCore::Image<U> &iImageReal, const PGCore::Image<U> &iImageImaginary);

		bool Execute();

		virtual ~ImageFourierTransform(void);

	protected:
		bool m_inverseTransform;

		bool performFFT2D(PGCore::Image<T> *iInImageReal, PGCore::Image<T> *iInImageImaginary, PGCore::Image<U> *oImageReal,	PGCore::Image<U> *oImageImaginary);
		bool performFFT2DInv(PGCore::Image<U> *iInImageReal, PGCore::Image<U> *iInImageImaginary, PGCore::Image<T> *oImageReal,	PGCore::Image<T> *oImageImaginary);

		bool performFFT1D(int iElements, U *ioBufferReal, U *ioBufferImaginary);

		bool rootSquaredSum(const PGCore::Image<U> &iImage1, const PGCore::Image<U> &iImage2, PGCore::Image<U> &oImage); 
		bool rootSquaredSumInv(const PGCore::Image<T> &iImage1, const PGCore::Image<T> &iImage2, PGCore::Image<U> &oImage); 



	private:
		PGCore::Image<U>	*m_oImageObject;
		PGCore::Image<U>	m_ImageReal, m_ImageImaginary;

	};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "ImageFourierTransform.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageFourierTransform_H_
