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
// Filename:	Kernel.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_Kernel_H_
#define _PG_Kernel_H_
//////////////////////////////////////////////////////////////////////////

#include "ObjectBase.h"
#include "BaseDataObject.h"
#include "Image.h"
#include "BitImage.h"

namespace PGCore
{
	template <class T, class U>
	class Kernel : public ObjectBase, public BaseDataObject
	{

	public:

		Kernel();	

		virtual ~Kernel(void);

		Kernel<T, U>& operator-=(const Kernel<T, U>& iKernel);

		virtual bool Convolve(const BitImage& iImage, BitImage& oImage) const;

		virtual bool Convolve(const Image<T>& iImage, Image<U>& oImage) const;

		virtual bool Convolve1D_X(const Image<T>& iImage, Image<U>& oImage) const;

		virtual bool Convolve1D_Y(const Image<T>& iImage, Image<U>& oImage) const;

		float* GetBuffer() { return m_buffer; };

		bool Clear();

		bool Reset(const float iValue=0);

		bool SetValue(const unsigned int &iRow, const unsigned int &iCol, const float iValue);

		const float GetValue(const unsigned int &iRow, const unsigned int &iCol) const;

		float* GetBuffer() const;

		virtual bool	Initialize_() = 0;

		bool SetDimension(const unsigned int &iDimension);

		const unsigned int & GetDimension() const;

		bool IsSeparable() const { return m_separable; };

		bool Dump() const;

	protected: 
		bool			m_separable;
		unsigned int	m_dimension;		

		float*			m_buffer;		// make it 2D for non-separable and non-symmetric kernels
		
	private:

		

		virtual bool convolve(const BitImage& iImage, BitImage& oImage) const;

		virtual bool convolve(const Image<T>& iImage, Image<U>& oImage) const;

		virtual bool convolveSeparable(const Image<T>& iImage, Image<U>& oImage) const;

		bool convolveX(const Image<T>& iImage, Image<U>& oImage) const;

		bool convolveY(const Image<T>& iImage, Image<U>& oImage) const;

	};

};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "Kernel.hpp"
#endif


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_Kernel_H_

