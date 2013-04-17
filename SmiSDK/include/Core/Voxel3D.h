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
// Filename:	Voxel3D.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 12:38:35 AM
//

#ifndef _PG_VOXEL3D_H_
#define _PG_VOXEL3D_H_
//////////////////////////////////////////////////////////////////////////



#include "BaseDataObject.h"
#include "Volume.h"
#include "MetaData.h"


#include "Math/Point3D.h"
#include "Math/AffineTransform.h"

namespace PGCore 
{

typedef enum {
	ePgInterpolationTypeUnknown = -1,
	ePgInterpolationTypeNearestNeighbor = 0,
	ePgInterpolationTypeTriLinear = 1	
} ePgInterpolationType;

// *************** Spaces ************
// STD space
// Units	:	Arbitrary
// +X		:	Patient right to left
// +Y		:	Patient ant to post
// +Z		:	Patient feet to head
// Origin	:	Center of the mm Volume 
// Span		:	-0.5f ... +0.5f, over all of the MM volume 

// DICOM patint space
// Units	:	MM
// +X		:	Patient right to left
// +Y		:	Patient ant to post
// +Z		:	Patient feet to head
// Origin	:	Center of the mm Volume 
// Span		:	All of the MM volume

template <class T>
class Voxel3D : public BaseDataObject
{
public:
	Voxel3D();
	Voxel3D(const PGCore::MetaData< T > &iMetaData, 
				PGCore::Volume< T > *iVolumeAccessor);
	Voxel3D<T>& operator=(const Voxel3D<T>& iVoxel3DObject);	

	// X, Y, Z: -0.5 ... +0.5
	T GetValue(const float& iX,  const float& iY, const float& iZ) ;//const; // interpolate	
	bool SetValue(const float& iX,  const float& iY, const float& iZ, const T& iValue); // interpolate	

	int GetMaskValue(const float& iX,  const float& iY, const float& iZ) const; // does not interpolate	

	T GetLocalAverage(const float& iX,  const float& iY, const float& iZ); // const // interpolate

	T GetLocalMaximumValue(const float& iX,  const float& iY, const float& iZ);

	T GetInterpolatedRawValue(const float& iX,  const float& iY, const float& iZ) ;//const; // interpolate	

	T GetRawValue(const float& iX,  const float& iY, const float& iZ) ;//const; // interpolate	

	T GetNormValue(const float& iX,  const float& iY, const float& iZ) ;//const; // interpolate	

	T GetValueNoBoundCheck(const float& iX,  const float& iY, const float& iZ);// const; // interpolate 

	bool GetStdToPatCoord(const float& iX,  const float& iY, const float& iZ,
								  float& oX,  float& oY, float& oZ);	

	bool GetPatToStdCoord(const float& iX,  const float& iY, const float& iZ,
								  float& oX,  float& oY, float& oZ);	

	bool GetStdToImgCoord(const float& iX,  const float& iY, const float& iZ,
								  float& oX,  float& oY, float& oZ) const;	

	bool GetImgToStdCoord(const float& iX,  const float& iY, const float& iZ,
								  float& oX,  float& oY, float& oZ) const;

	bool GetImgToPatCoord(const float& iX,  const float& iY, const float& iZ,
								  float& oX,  float& oY, float& oZ);

	bool GetPatToImgCoord(const float& iX,  const float& iY, const float& iZ,
								  float& oX,  float& oY, float& oZ);

	const PGMath::Vector3D<float>& GetImageSetOrigin(); 

	bool GetInterpolatedImage(const int& oSizeX, const int& oSizeY, const float& iZ, const int& iSkip, PGCore::Image<T>& ioImage) const; // interpolate	

	virtual ~Voxel3D(void);
	
	const PGCore::MetaData< T > &GetMetaData() const; 
	PGCore::Volume< T > *GetVolumeAccessor() const;
	const PGMath::Point3D<float> &GetDimensions() const;

	void  SetViewTransform(const PGMath::AffineTransform<float> & iTransform);

	void  SetRegistrationTransform(const PGMath::AffineTransform<float> & iTransform);

	void  SetInterpolationType(const ePgInterpolationType iInterpolationType);
	
	const PGMath::AffineTransform<float>& GetTransformVoxelToImage() const;

	bool Update();

private:	
	
	PGCore::MetaData< T >			m_metaData;
	mutable PGCore::Volume< T > *	m_volumeAccessor;
	PGMath::Point3D<float>			m_dimensionsImage; // in pixels		
	ePgInterpolationType			m_interpolationType;
	PGMath::Point3D<float>			m_dimensions; // in mm			
	T								m_minValue, m_maxValue;
	//float							m_rangeInv;

	PGMath::Point3D<float>			m_cachedPointImg;

	PGMath::AffineTransform<float>	m_transformImgToIso_Scale;		// apply scaling
	PGMath::AffineTransform<float>	m_transformIsoToImg_Scale;	

	PGMath::AffineTransform<float>	m_transformImgToIso_InvertZ;		// apply Z flip if needed, add the first slice's co-ordinates
	PGMath::AffineTransform<float>	m_transformIsoToImg_InvertZ;	

	PGMath::AffineTransform<float>	m_transformRegistration;	// registration etc.
	PGMath::AffineTransform<float>	m_transformView;			// View

	PGMath::AffineTransform<float>	m_transformStdToPat_Translate;	    // isotropic space is the  space: -x/2 ... +x/2 etc
	PGMath::AffineTransform<float>	m_transformPatToStd_Translate;	    // isotropic space is the  space: -x/2 ... +x/2 etc

	PGMath::AffineTransform<float>	m_transformPatToIso_Rotate;		// apply any rotation to orient patient in dcm space
	PGMath::AffineTransform<float>	m_transformIsoToPat_Rotate;	

	PGMath::AffineTransform<float>	m_transformStdToPat_Normalize;	    // std space is the view space: -0.5 ... +0.5
	PGMath::AffineTransform<float>	m_transformPatToStd_Normalize;	    // std space is the view space: -0.5 ... +0.5	

	// all transforms concatenated
	PGMath::AffineTransform<float>	m_transformStdToImg;	    // std space is the view space: -0.5 ... +0.5
	PGMath::AffineTransform<float>	m_transformImgToStd;	    // std space is the view space: -0.5 ... +0.5
	
	PGMath::AffineTransform<float>	m_transformStdToRaw;	    // std space is the view space: -0.5 ... +0.5
	PGMath::AffineTransform<float>	m_transformRawToStd;	    // std space is the view space: -0.5 ... +0.5
	

	typedef T (Voxel3D<T>::*InterpolateFuncPtr)(const PGMath::Point3D<float> & iPoint) const;

	InterpolateFuncPtr m_interpolationFnPtr;

	bool	update();
	bool	updateDimensionsImage();
	bool	updateDimensionsMM();
	bool	updateTransforms();
	bool	updateTransformImgToDcm();	
	bool	updateTransformDcmToIso();
	bool	updateTransformIsoToPat();
	bool	updateTransformPatToStd();
	bool	updateInterpolationFunctionPtr();

	T		getRawValue(int iX, int iY, int iZ) const;
	T		interpolate(const PGMath::Point3D<float> & iPoint) const;
	T		interpolateNoBoundCheck(const PGMath::Point3D<float> & iPoint) const;
	T		interpolateNearestNeighbor(const PGMath::Point3D<float> & iPoint) const;
	T		interpolateTrilinear(const PGMath::Point3D<float> & iPoint) const;
	T		interpolateTrilinearNoBoundCheck(const PGMath::Point3D<float> & iPoint) const;

	//T		normalize(const T& iValue) const;
};


};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "Voxel3D.hpp"
#endif
//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_VOXEL3D_H_
