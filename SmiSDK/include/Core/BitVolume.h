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
