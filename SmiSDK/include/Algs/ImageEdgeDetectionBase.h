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
// Filename:	ImageEdgeDetectionBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageEdgeDetectionBase_H_
#define _PGImageEdgeDetectionBase_H_
//////////////////////////////////////////////////////////////////////////


#include "ImageFilterBase.h"
#include "Core/GaussianKernel.h"
#include "Core/DiffOfGaussianDerivativeKernel.h"


namespace PGAlgs
{

	template <class T, class U>
	class ImageEdgeDetectionBase : public ImageFilterBase<T, U>
	{
	public:
		ImageEdgeDetectionBase(double iSigma=1.0f);		

		// [0...1]
		void SetHysterisisThresholds(const float& iLowTh, const float& iHighTh); 

		bool Execute();

	public:	
		virtual ~ImageEdgeDetectionBase(void);

	protected:

		bool detectEdges(float iSigma, /* Standard deviation of the gaussian kernel. */
			float iTlow,  /* Fraction of the high threshold in hysteresis. */
			float iThigh,
			PGCore::Image<T> *iInImage,
			PGCore::Image<U> *oEdgeImage
			);


	private:
		PGCore::Image<U>	*m_oImageObject;
		double				m_sigma;
		float				m_loTh, m_hiTh;

		enum ePgEdgeType
		{
			kPgNOEDGE=0,
			kPgPOSSIBLE_EDGE=100,
			kPgEDGE=200
		};

		bool nonMaximumSuppression(T *mag, T *gradx, T *grady, int nrows, int ncols, U *result);

		bool applyHysteresis(T *mag, T *nms, int rows, int cols, float tlow, float thigh, U*edge);

		bool followEdges(T* edgemapptr, T *edgemagptr, short lowval, int cols);
	};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "ImageEdgeDetectionBase.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageEdgeDetectionBase_H_
