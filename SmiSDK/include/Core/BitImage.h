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