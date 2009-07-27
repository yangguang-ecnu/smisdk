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
