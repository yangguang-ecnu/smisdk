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
// Filename:	DilationKernel.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_DilationKernel_H_
#define _PG_DilationKernel_H_
//////////////////////////////////////////////////////////////////////////

#include "Kernel.h"

// http://www.cee.hw.ac.uk/hipr/html/dilate.html
// http://www.cee.hw.ac.uk/hipr/html/erode.html

/*
		// Example
		PGCore::DilationKernel<T, T> dilationKernel(1);
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
			dilationKernel.Convolve(nextImage, sImage);	
			_snprintf(fileName, 255, "C:\\Data\\Dump\\DilatedImage_%04d.pgm", i); fileName[255] = '\0';
			PGAlgs::DumpImageAsPGM(sImage, std::string(fileName));
		}
*/

namespace PGCore
{

	template <class T, class U>
	class DilationKernel : public Kernel<T, U>
	{
	public:
		DilationKernel(const int iRadius=1);		

		~DilationKernel(void);

		DilationKernel(const DilationKernel<T, U>& iKernelObject);	

		DilationKernel<T, U>& operator=(const DilationKernel<T, U>& iKernelObject);

		const int& GetRadius() const { return m_radius; }

	protected: 	
	
		virtual bool Initialize_(); // derived and overridden as required


	private:
		int		m_radius;

		virtual bool convolve(const BitImage& iImage, BitImage& oImage) const;

		virtual bool convolve(const Image<T>& iImage, Image<U>& oImage) const;

		bool drawCircle(int iXc, int iYc, int iRadius, float * ioBuf);

	};
};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "DilationKernel.hpp"
#endif
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_DilationKernel_H_