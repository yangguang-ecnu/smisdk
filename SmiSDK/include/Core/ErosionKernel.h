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
// Filename:	ErosionKernel.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_ErosionKernel_H_
#define _PG_ErosionKernel_H_
//////////////////////////////////////////////////////////////////////////

#include "Kernel.h"

// http://www.cee.hw.ac.uk/hipr/html/Erode.html
// http://www.cee.hw.ac.uk/hipr/html/erode.html


/*
		// Example
		PGCore::ErosionKernel<char, T> erosionKernel(1);
		{			
			PGAlgs::ImageBinarizer<T, char> binarizer;
			binarizer.SetInput(static_cast<PGCore::Image < T > *>(&nextImage));
			binarizer.SetThresholdRange(900, 1024);
			binarizer.Execute();
	
			binarizer.GetOutput(static_cast<PGCore::BitImage *>(&bImage));
			bImage.UnPack(nextImage);

			char fileName[256] = {0};
			_snprintf(fileName, 255, "C:\\Data\\Dump\\OriginalImage_%04d.pgm", i); fileName[255] = '\0';
			PGAlgs::DumpImageAsPGM(nextImage, std::string(fileName));
			PGCore::Image<T> sImage;
			erosionKernel.Convolve(nextImage, sImage);	
			_snprintf(fileName, 255, "C:\\Data\\Dump\\DilatedImage_%04d.pgm", i); fileName[255] = '\0';
			PGAlgs::DumpImageAsPGM(sImage, std::string(fileName));
		}
*/
namespace PGCore
{

	template <class T, class U>
	class ErosionKernel : public Kernel<T, U>
	{
	public:
		ErosionKernel(const int iRadius=1);		

		~ErosionKernel(void);

		ErosionKernel(const ErosionKernel<T, U>& iKernelObject);	

		ErosionKernel<T, U>& operator=(const ErosionKernel<T, U>& iKernelObject);

		const int& GetRadius() const { return m_radius; }

	protected: 	
	
		virtual bool Initialize_(); // derived and overridden as required


	private:
		int		m_radius;

		virtual bool convolve(const BitImage& iImage, BitImage& oImage) const;

		virtual bool convolve(const Image<T>& iImage, Image<U>& oImage) const;

		bool drawCircle(int iXc, int iYc, int iRadius, float* ioBuf);

	};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "ErosionKernel.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_ErosionKernel_H_