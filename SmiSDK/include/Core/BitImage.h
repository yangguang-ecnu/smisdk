// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// � [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
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
// Filename:	BitImage.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 11:24:50 PM
//
#ifndef _PG_BitImage_H_
#define _PG_BitImage_H_
//////////////////////////////////////////////////////////////////////////

#include "ObjectBase.h"
#include "BaseDataObject.h"
#include "Image.h"
#include "Functional.h"
#include "PixelBase.h"

namespace PGCore
{


class BitImage : public ObjectBase, public BaseDataObject
{
public:

	BitImage(long iRows=0, long iColumns=0);	

	BitImage(const BitImage& iBitImageObject);
	
	BitImage& operator=(const BitImage& iBitImageObject);

	BitImage& operator|=(const BitImage& iBitImageObject); // OR

	virtual ~BitImage(void);
	
	bool Clear();

	bool Reset(bool iValue);

	bool SetValue(long iRow, long iColumn, const bool iValue);
	
	const bool GetValue(long iRow, long iColumn) const;
	
	bool SetDimensions(long &iRows, long &iColumns);

	bool GetDimensions(long &iRows, long &iColumns) const;

	long GetDataSize() const { return m_image.GetDataSize(); }

	char* GetBuffer() const { return m_image.GetBuffer(); }
	
	bool Invert(); 

	template <class T>
	bool UnPack(PGCore::Image<T>& oImage);

	
protected: 		
	Image<char>	m_image;

private:
	long		m_rows, m_columns;

};


template <class T>
inline bool BitImage::UnPack(PGCore::Image<T>& oImage) 
{
	oImage.SetDimensions(m_rows, m_columns);

	oImage.Reset(0);
	T maxVal = T(255);

	for (int iR=0; iR<m_rows; iR++)
	{
		for (int iC=0; iC<m_columns; iC++)
		{
			if (GetValue(iR, iC))
			{
				oImage.SetValue(iR, iC, maxVal);
			}
		}
	}

	return true;
}


};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "BitImage.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_BitImage_H_