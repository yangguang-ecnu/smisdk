// -*- C++ -*-
// � [2006-2016] Science.Medical.Imaging Group [(unpublished)] /**/Rights Reserved.
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

