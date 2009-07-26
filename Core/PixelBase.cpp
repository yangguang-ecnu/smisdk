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
// Filename:	PixelBase.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:01:26 PM
//

#ifndef _PGPixelBase_HPP_
#define _PGPixelBase_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Core/PixelBase.h"


using namespace PGMath;

namespace PGCore
{

template <class T>
inline PixelBase<T>::PixelBase()
{	
	m_type = kPgDataObjectTypePixel;
	m_value = GetMinValue();
}

template <class T>
inline PixelBase<T>::PixelBase(const PixelBase<T>& iPixel)
{
	*this = iPixel;
}

template <class T>
inline PixelBase<T>::~PixelBase(void)
{
}


template <class T>
inline PixelBase<T> &PixelBase<T>::operator=(const PixelBase<T> &iPixel) 
{	
	m_value = iPixel.GetValue();
	m_type = kPgDataObjectTypePixel;
	return *this;
}

template <>
inline const char PixelBase<char>::GetMinValue() const
{
	return CHAR_MIN;
}

/*
template <>
inline const PgUChar_RGBAPixel PixelBase<PgUChar_RGBAPixel>::GetMinValue() const
{
	return PgChar_RGBAPixel(0, 0, 0, 0);
}*/


template <>
inline const PgChar_RGBAPixel PixelBase<PgChar_RGBAPixel>::GetMinValue() const
{
	return PgChar_RGBAPixel(CHAR_MIN, CHAR_MIN, CHAR_MIN, CHAR_MIN);
}

template <>
inline const PGMath::Point3D<unsigned char> PixelBase<PGMath::Point3D<unsigned char> >::GetMinValue() const
{
	return PGMath::Point3D<unsigned char>(PG_UCHAR_MIN, PG_UCHAR_MIN, PG_UCHAR_MIN, PG_UCHAR_MIN);
}

template <>
inline const PGMath::Point3D<unsigned short> PixelBase<PGMath::Point3D<unsigned short> >::GetMinValue() const
{
	return PGMath::Point3D<unsigned short>(PG_USHORT_MIN, PG_USHORT_MIN, PG_USHORT_MIN, PG_USHORT_MIN);
}

template <>
inline const unsigned char PixelBase<unsigned char>::GetMinValue() const
{
	return PG_UCHAR_MIN;
}

template <>
inline const short PixelBase<short>::GetMinValue() const
{
	return PG_SHORT_MIN;
}


template <>
inline const unsigned short PixelBase<unsigned short>::GetMinValue() const
{
	return PG_USHORT_MIN;
}



template <>
inline const PGMath::Point3D<char> PixelBase<PGMath::Point3D<char> >::GetMaxValue() const
{
	return PGMath::Point3D<char>(PG_CHAR_MAX, PG_CHAR_MAX, PG_CHAR_MAX, PG_CHAR_MAX);
}

template <>
inline const PGMath::Point3D<unsigned char> PixelBase<PGMath::Point3D<unsigned char> >::GetMaxValue() const
{
	return PGMath::Point3D<unsigned char>(PG_UCHAR_MAX, PG_UCHAR_MAX, PG_UCHAR_MAX, PG_UCHAR_MAX);
}

template <>
inline const PGMath::Point3D<unsigned short> PixelBase<PGMath::Point3D<unsigned short> >::GetMaxValue() const
{
	return PGMath::Point3D<unsigned short>(PG_USHORT_MAX, PG_USHORT_MAX, PG_USHORT_MAX, PG_USHORT_MAX);
}

template <>
inline const char PixelBase<char>::GetMaxValue() const
{
	return PG_CHAR_MAX;
}

template <>
inline const unsigned char PixelBase<unsigned char>::GetMaxValue() const
{
	return PG_UCHAR_MAX;
}

template <>
inline const short PixelBase<short>::GetMaxValue() const
{
	return PG_SHORT_MAX;
}


template <>
inline const unsigned short PixelBase<unsigned short>::GetMaxValue() const
{
	return PG_USHORT_MAX;
}


template <>
inline const double PixelBase<double>::GetMaxValue() const
{
	return (double)PG_SHORT_MAX;
}


template <>
inline const double PixelBase<double>::GetMinValue() const
{
	return (double)PG_SHORT_MIN;
}


template <>
inline const float PixelBase<float>::GetMaxValue() const
{
	return (double)PG_SHORT_MAX;
}


template <>
inline const float PixelBase<float>::GetMinValue() const
{
	return (float)PG_SHORT_MIN;
}

#ifdef _PG_GENERATE_SDK_LIBS_
	template class PixelBase<double>;
	template class PixelBase<short>; 
	template class PixelBase<unsigned short>; 
	template class PixelBase<char>; 
	template class PixelBase<unsigned char>; 
	template class PixelBase<float>; 
	template class PixelBase<PGMath::Point3D<char>>; 
	template class PixelBase<PGMath::Point3D<unsigned char>>; 
	template class PixelBase<PGMath::Point3D<unsigned short>>; 
#endif

};
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGPixelBase_HPP_
