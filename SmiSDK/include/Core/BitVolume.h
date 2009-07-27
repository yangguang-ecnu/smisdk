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
// Filename:	BitVolume.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:38:35 AM
//

#ifndef _PG_BitVolume_H_
#define _PG_BitVolume_H_
//////////////////////////////////////////////////////////////////////////

#include "Math/Vector3D.h"
#include "PixelBase.h"
#include "BitImage.h"
#include "BaseDataObject.h"

namespace PGCore 
{

class BitVolume : public BaseDataObject
{
public:
	BitVolume();

public:
	virtual ~BitVolume(void);

	BitVolume& operator=(const BitVolume& iBitVolumeObject);
	
	BitVolume& operator|=(const BitVolume& iBitVolumeObject);

	bool Clear();

	bool Invert(); 

	int GetSize() const;

	bool SetDimensions(const PGMath::Vector3D<long> &iDimensions);

	void GetDimensions(PGMath::Point3D<long> &oDimensions) const;

	const std::vector<BitImage> &GetImages() const;	

	bool AddImage(const BitImage& iImageObject);

	bool SetImage(int iImageIndex, const BitImage& iImageObject);

	const BitImage& GetImage(int iImageIndex) const;	

	bool GetValue(const int iRow, const int iColumn, const int iImageIndex) const;

	bool SetValue(const int iRow, const int iColumn, const int iImageIndex, const bool iValue);
	
	bool IsInitialized() const;

	bool Reset(const bool iValue);

protected:
	bool					m_initialized;


private:	
	std::vector<BitImage>   m_BitImages;
	int						m_rows, m_columns;

};
	
};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "BitVolume.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_BitVolume_H_
