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
// Filename:	Volume.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:38:35 AM
//

#ifndef _PG_VOLUME_H_
#define _PG_VOLUME_H_
//////////////////////////////////////////////////////////////////////////

#define _PG_MIN_OCTREE_CUBE_SIZE_ 16

#include "../Math/Vector3D.h"
#include "PixelBase.h"
#include "Image.h"
#include "BitVolume.h"
#include "BaseDataObject.h"

namespace PGCore 
{

template <class T>
class Volume : public BaseDataObject
{
public:
	Volume();

	Volume(const PGMath::Point3D<float>& iOrigin);

public:
	virtual ~Volume(void);

	Volume<T>& operator=(const Volume<T>& iVolumeObject);

	bool Clear();

	inline int GetSize() const ;

	inline void GetDimensions(PGMath::Point3D<long> &oDimensions) const;

	bool GetDataRange(T *oMin, T* oMax);

	bool GetMoments(T* oMean, T* oVariance) const;

	const std::vector<Image <T > > &GetImages() const;

	bool AddImage(const Image<T>& iImageObject);
	const Image<T>& GetImage(int iImageIndex) const;

	T GetValue(const int iRow, const int iColumn, const int iImageIndex) const;
	T GetValueFromOctree(const int iRow, const int iColumn, const int iImageIndex) const;

	bool SetValue(const int iRow, const int iColumn, const int iImageIndex, const T& iValue);
	T GetValueNoBoundCheck(const int iRow, const int iColumn, const int iImageIndex) const;
	T GetAvgValue(const int iRow, const int iColumn, const int iImageIndex) const;

	// BitVolume operations
	// iImageIndex = 0 or 1
	bool InitializeBitVolume(const int iImageIndex=0);
	BitVolume& GetBitVolume(const int iImageIndex=0);
	bool ResetBitVolume(const int iImageIndex=0);
	bool DilateBitVolume(const int iImageIndex=0, const int iVoxels=1);
	bool ErodeBitVolume(const int iImageIndex=0, const int iVoxels=1);
	bool InvertBitVolume(const int iImageIndex=0);

	bool FinalizeMask(); // OR temp mask with main mask

	bool GetEightNeighborValues(const int iRow, const int iColumn, const int iImageIndex, 
			T ioEightNeighbors[8]) const;	

	bool Normalize(Volume<T> &oVolume) const;

	void  SetOrigin(const PGMath::Point3D<float>& iOrigin);
	const PGMath::Point3D<float>& GetOrigin() const;

	bool Reset(const T& iValue);

	bool CreateOctree(); 

	bool CreateOctreeForBlock(); 

private:
	std::vector<Image<T > > m_images;
	int						m_rows, m_columns;


	int						m_LowestBlockSize;

	// spatial reference
	PGMath::Point3D<float>	m_origin;		

	BitVolume				m_maskVolume[2]; // 0: working volume, 1: temp volume

	bool clearOctreeBlocks();
};
	
};
#ifndef _PG_GENERATE_SDK_LIBS_
	#include "Volume.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_VOLUME_H_
