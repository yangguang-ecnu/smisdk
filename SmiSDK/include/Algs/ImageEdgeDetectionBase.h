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
