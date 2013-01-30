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

	bool GetGradient(const int iRow, const int iColumn, const int iImageIndex, 
								   PGMath::Vector3D<float>& oGradient) const;

	// BitVolume operations
	// iImageIndex = 0 or 1
	bool InitializeBitVolume(const int iImageIndex=0);
	BitVolume& GetBitVolume(const int iImageIndex=0);
	bool ResetBitVolume(const int iImageIndex=0);
	bool DilateBitVolume(const int iImageIndex=0, const int iVoxels=1);
	bool ErodeBitVolume(const int iImageIndex=0, const int iVoxels=1);
	bool InvertBitVolume(const int iImageIndex=0);

	bool FinalizeMask(); // OR temp mask with main mask

	// pointcloud ops
	std::vector<PGMath::Point3D<float> >& GetPointCloud(); // in CT space

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

	std::vector<PGMath::Point3D<float> >			m_pointCloud; // in CT space

	bool clearOctreeBlocks();
};
	
};
#ifndef _PG_GENERATE_SDK_LIBS_
	#include "Volume.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_VOLUME_H_
