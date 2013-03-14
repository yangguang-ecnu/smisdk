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

#ifndef _PG_VOXEL3D_HPP_
#define _PG_VOXEL3D_HPP_
//////////////////////////////////////////////////////////////////////////

#include "Core/Voxel3D.h"

namespace PGCore 
{

	template <class T>
	void Voxel3D<T>::SetViewTransform(const PGMath::AffineTransform<float> & iTransform)
	{
		m_transformView = iTransform;

		m_transformStdToRaw = m_transformView;//m_transformStdToImg;
		m_transformStdToRaw.ConcatPre(&m_transformRegistration);//m_transformView);
		m_transformStdToRaw.ConcatPre(&m_transformStdToImg);//m_transformRegistration);

		m_transformRawToStd = m_transformStdToRaw;

		m_transformRawToStd.Invert();

		//m_transformRegistration = iTransform;	

		//m_transformStdToImg.ConcatPost(&iTransform);
		//update();
	}

	template <class T>
	void Voxel3D<T>::SetRegistrationTransform(const PGMath::AffineTransform<float> & iTransform)
	{
		m_transformRegistration = iTransform;	

		std::vector<float> dims;	
		{
			dims.push_back(m_dimensions.X());
			dims.push_back(m_dimensions.Y());
			dims.push_back(m_dimensions.Z());	
			std::sort(dims.begin(), dims.end());		
		}
		float invSc = 1.0f/dims[2];

		PGMath::Matrix4x4<float> regMat = m_transformRegistration.Matrix4x4();
		regMat[3]	= regMat[3]		*	invSc;
		regMat[7]	= regMat[7]		*	invSc;
		regMat[11]	= -regMat[11]	*	invSc;// Has to be negative. Check why

		m_transformRegistration = PGMath::AffineTransform<float>(regMat);
/*
		// scale to go into std space
		PGMath::DegreesOfFreedomBase<float> regDoFs;
		regDoFs.SetParameter(kPgDegreeOfFreedomScalingX, 1.0f/m_dimensions.X());
		regDoFs.SetParameter(kPgDegreeOfFreedomScalingY, 1.0f/m_dimensions.Y());
		regDoFs.SetParameter(kPgDegreeOfFreedomScalingZ, 1.0f/m_dimensions.Z());
		PGMath::AffineTransform<float> regTrans(PGMath::Point3D<float>(0.0f, 0.0f, 0.0f), 
					regDoFs);

		m_transformRegistration.ConcatPre(&regTrans);*/
		//m_transformRegistration.Invert();

		m_transformStdToRaw = m_transformView;//m_transformStdToImg;
		m_transformStdToRaw.ConcatPre(&m_transformRegistration);//m_transformView);
		m_transformStdToRaw.ConcatPre(&m_transformStdToImg);//m_transformRegistration);

		m_transformRawToStd = m_transformStdToRaw;
		m_transformRawToStd.Invert();

		//m_transformStdToImg.ConcatPost(&m_transformRegistration);
		//update();
	}

	template <class T>
	void Voxel3D<T>::SetInterpolationType(const ePgInterpolationType iInterpolationType)
	{
		m_interpolationType = iInterpolationType;//ePgInterpolationTypeNearestNeighbor;//iInterpolationType;
		updateInterpolationFunctionPtr();
		LOG1("<this> Interpolation method: %d", m_interpolationType);
	}
	
	template <class T>	
	const PGMath::AffineTransform<float>& Voxel3D<T>::GetTransformVoxelToImage() const 
	{ 
		// voxel to raw
		return m_transformStdToRaw; 
	};

	template <class T>	
	bool Voxel3D<T>::Update() 
	{ 
		return update(); 
	};


template <class T>
Voxel3D<T>::Voxel3D()
{	
	m_type = kPgDataObjectTypeIsotropicVoxel3D;
	//m_interpolationType = ePgInterpolationTypeTriLinear;//ePgInterpolationTypeTriLinear;//ePgInterpolationTypeNearestNeighbor;//ePgInterpolationTypeTriLinear;
	m_volumeAccessor = 0;
	m_minValue = 0.0f;
	m_maxValue = 1.0f;
	m_rangeInv = 1.0f;

	SetInterpolationType(ePgInterpolationTypeTriLinear);
	//updateInterpolationFunctionPtr();		
}

template <class T>
Voxel3D<T>::Voxel3D(const PGCore::MetaData< T > &iMetaData, 
				PGCore::Volume< T > *iVolumeAccessor)
{	
	m_type = kPgDataObjectTypeIsotropicVoxel3D;
	m_metaData = iMetaData;
	//m_interpolationType = ePgInterpolationTypeNearestNeighbor;//ePgInterpolationTypeNearestNeighbor;//ePgInterpolationTypeTriLinear;
	m_volumeAccessor = iVolumeAccessor;

	SetInterpolationType(ePgInterpolationTypeTriLinear);
	//updateInterpolationFunctionPtr();	
	update();

	/*
	float ival = 0.98f;
	PGCore::Timer			gTimer;
	gTimer.Reset();
	for (long i=0; i<100000; i++)
	{
		T oval = T_ROUND(ival);
	}	
	float es = gTimer.ElapsedSeconds();
	LOG1("Voxel3D: Time for normal round: %3.8f", es);		


	gTimer.Reset();
	for (long i=0; i<100000; i++)
	{
		T oval = ASM_ROUND(ival);
	}	
	es = gTimer.ElapsedSeconds();
	LOG1("Voxel3D: Time for asm round: %3.8f", es);	
	*/
}


template <class T>
Voxel3D<T>::~Voxel3D(void)
{	
	
}

template <class T>
bool Voxel3D<T>::SetValue(const float& iX,  const float& iY, const float& iZ, const T& iValue) // interpolate	
{
	//PGMath::Point3D<float> m_cachedPointImg;
	m_transformStdToRaw.Apply(PGMath::Point3D<float>(iX, iY, iZ), m_cachedPointImg);

	//GetLogger()->Log("%f %f %f: %d", m_cachedPointImg.X(), m_cachedPointImg.Y(), m_cachedPointImg.Z(), iValue);
	return m_volumeAccessor->SetValue(m_cachedPointImg.Y(), m_cachedPointImg.X(), m_cachedPointImg.Z(), iValue);
}

template <class T>
T Voxel3D<T>::GetValueNoBoundCheck(const float& iX,  const float& iY, const float& iZ) //const // interpolate
{
	//PGMath::Point3D<float> m_cachedPointImg;
	m_transformStdToImg.Apply(PGMath::Point3D<float>(iX, iY, iZ), m_cachedPointImg);
	return interpolateNoBoundCheck(m_cachedPointImg);
}

// iZ is -0.5f -> 0.5f
template <class T>
bool Voxel3D<T>::GetInterpolatedImage(const int& oSizeX, const int& oSizeY, const float& iZ, const int& iSkip, PGCore::Image<T>& ioImage) const // interpolate	
{
	return false;
	/*
	if (oSizeX*oSizeY ==0 || iSkip<=0)
	{
		return false;
	}

	long sX = oSizeX/iSkip, sY = oSizeY/iSkip;
	bool res = ioImage.SetDimensions(sY, sX);
	if (!res)
	{
		return false;
	}

	T* oBuf = (T *)ioImage.GetBuffer();
	if (!oBuf)
	{
		return false;
	}

	long oIndex=0;
	float m_start=-0.5f, m_end=0.5f, r=0, c=0;
	int rIndex=0, cIndex=0;
	float rStep = (float(iSkip))/(oSizeY-1), cStep = (float(iSkip))/(oSizeX-1);

	// create a plane for resampling
	// can you extract the whole image at once??
	for (r=m_start, rIndex=0; r<=(m_end) && rIndex<oSizeY; r+=rStep)
	{		 	
			for (c=m_start, cIndex=0; c<=(m_end) && cIndex<oSizeX; c+=cStep)
			{					
				oBuf[oIndex++] = GetValue(c, r, iZ);
				cIndex++;				
			}
			rIndex++;			
	}	

	return true;
	*/
}

template <class T>
int Voxel3D<T>::GetMaskValue(const float& iX,  const float& iY, const float& iZ) const // doesnt interpolate
{
	PGCore::BitVolume& maskVol = GetVolumeAccessor()->GetBitVolume();

	if (maskVol.IsInitialized())
	{
		bool mVal = maskVol.GetValue(m_cachedPointImg.Y(), m_cachedPointImg.X(), m_cachedPointImg.Z());
		return mVal ? 1 : 0;
	}

	return -1;
}

template <class T>
T Voxel3D<T>::GetValue(const float& iX,  const float& iY, const float& iZ) //const // interpolate
{
	//if (!m_volumeAccessor)
	{
	//	return 0;
	}

	//PGMath::Point3D<float> iPointView, iPointReg, m_cachedPointImg;
	//PGMath::Point3D<float> m_cachedPointImg;


	//check feetfirst estimation. that is the only doubtful thing now!!!

	m_transformStdToRaw.Apply(PGMath::Point3D<float>(iX, iY, iZ), m_cachedPointImg);
		
	//m_transformRegistration.Apply(PGMath::Point3D<float>(iX, iY, iZ), iPointReg);

	//m_transformView.Apply(iPointReg, iPointView);	

//	m_transformStdToImg.Apply(iPointReg, m_cachedPointImg);
	//m_transformStdToImg.Apply(PGMath::Point3D<float>(iX, iY, iZ), iPointView);
	//m_transformStdToImg.Apply(iPointView, m_cachedPointImg);

	

	

	// transform from std space to std space (transformed)
	/*if (!m_transformRegistration.Apply(iPointStd, iPointReg))
	{
		return 0;
	}

	if (!m_transformStdToImg.Apply(iPointReg, m_cachedPointImg))
	{
		return 0;
	}*/

	//PGMath::Point3D<float> m_cachedPointImgTest(128.0f + iX*256.0f, 
	//	128.0f + iY*256.0f, 48.0f + iZ*92.0f);

	/*if (m_cachedPointImg.X() == 62 && m_cachedPointImg.Y() == 62)
	{
		printf("Found one!");
	}*/

	// mask here
	//err

	// all this code should be in Voxel3D.. so a different LuT can be applied to the masked voxels

	T oVal = interpolate(m_cachedPointImg);

	/*
	PGCore::BitVolume& maskVol = GetVolumeAccessor()->GetBitVolume();

	if (maskVol.IsInitialized())
	{
		if (maskVol.GetValue(m_cachedPointImg.Y(), m_cachedPointImg.X(), m_cachedPointImg.Z())) 
			return oVal;
		else
			// return level value
			return (oVal-10) < 0 ? 0 : (oVal-10);
	}
	*/

	return oVal;

	/*
	PGMath::Point3D<float> iPointStd(iX, iY, iZ), iPointDicom, iPointIso, 
		iPointStdTransformed, iPointPat, iPointImage;

	// transform from std space to std space (transformed)
	if (!m_transformRegistration.Apply(iPointStd, iPointStdTransformed))
	{
		return 0;
	}

	// first transform from std space to patient space
	if (!m_transformStdToPat.Apply(iPointStdTransformed, iPointPat))
	{
		return 0;
	}

	// transform from patient space to iso space	
	if (!m_transformPatToIso.Apply(iPointPat, iPointIso))
	{
		return 0;
	}

	// first transform from iso space to dicom space
	if (!m_transformIsoToDcm.Apply(iPointIso, iPointDicom))
	{
		return 0;
	}		
	
	// transform from dicom space to image space	
	if (!m_transformDcmToImage.Apply(iPointDicom, iPointImage))
	{
		return 0;
	}
*/
	//return interpolate(iPointImage);
}


template <class T>
T Voxel3D<T>::GetLocalAverage(const float& iX,  const float& iY, const float& iZ) // const // interpolate
{
	m_transformStdToRaw.Apply(PGMath::Point3D<float>(iX, iY, iZ), m_cachedPointImg);

	PixelBase<T> pxVal;
	double localAvgVal = 0;
	int sx=(int)m_cachedPointImg.X(), sy=(int)m_cachedPointImg.Y(), sz=(int)m_cachedPointImg.Z();
	for (int z=-1; z<=1; z++)
	{
		for (int y=-1; y<=1; y++)
		{
			for (int x=-1; x<=1; x++)
			{
				T localVal = interpolateTrilinear(PGMath::Point3D<float>(sx+x, sy+y, sz+z));
				localAvgVal+= localVal;				
			}
		}
	}

	return T(localAvgVal/27);
}

template <class T>
T Voxel3D<T>::GetInterpolatedRawValue(const float& iX,  const float& iY, const float& iZ) // const // interpolate
{
	m_transformStdToRaw.Apply(PGMath::Point3D<float>(iX, iY, iZ), m_cachedPointImg);

	return interpolateTrilinear(m_cachedPointImg);
}

template <class T>
T Voxel3D<T>::GetLocalMaximumValue(const float& iX,  const float& iY, const float& iZ) // const // interpolate
{
	m_transformStdToRaw.Apply(PGMath::Point3D<float>(iX, iY, iZ), m_cachedPointImg);

	PixelBase<T> pxVal;
	T localMaximumVal = pxVal.GetMinValue();
	int sx=(int)m_cachedPointImg.X(), sy=(int)m_cachedPointImg.Y(), sz=(int)m_cachedPointImg.Z();
	for (int z=-1; z<=1; z++)
	{
		for (int y=-1; y<=1; y++)
		{
			for (int x=-1; x<=1; x++)
			{
				T localVal = getRawValue(sx+x, sy+y, sz+z);
				localMaximumVal = max(localMaximumVal, localVal);
			}
		}
	}

	return localMaximumVal;
}
	

template <class T>
T Voxel3D<T>::GetRawValue(const float& iX,  const float& iY, const float& iZ) // const // interpolate
{
	//PGMath::Point3D<float> iPointView, iPointReg, m_cachedPointImg;
	//PGMath::Point3D<float> m_cachedPointImg;

	m_transformStdToRaw.Apply(PGMath::Point3D<float>(iX, iY, iZ), m_cachedPointImg);
	//m_transformRegistration.Apply(PGMath::Point3D<float>(iX, iY, iZ), iPointReg);

	return getRawValue((int)m_cachedPointImg.X(), (int)m_cachedPointImg.Y(), (int)m_cachedPointImg.Z());	
}

template <class T>
bool Voxel3D<T>::GetStdToImgCoord(const float& iX,  const float& iY, const float& iZ,
								  float& oX,  float& oY, float& oZ) const
{	
	PGMath::Point3D<float> pointImg;

	// perform testing that all volumes provide same tranformed point
	// renderer's std to raw transform changes y's scaling component during initializeviewers (in full res)??

	bool rv = m_transformStdToRaw.Apply(PGMath::Point3D<float>(iX, iY, iZ), pointImg);
	
	//oX = int(pointImg.X()+0.5f); oY = int(pointImg.Y()+0.5f); oZ = int(pointImg.Z()+0.5f);
	
	oX = pointImg.X(); oY = pointImg.Y(); oZ = pointImg.Z();

#define _TEST_XFORMS_ 0
#if (_TEST_XFORMS_)
	PGMath::Point3D<float> stdPt(iX, iY, iZ);
	PGMath::Point3D<float> patPt;
	PGMath::Point3D<float> isoPt;
	PGMath::Point3D<float> dcmPt;	
	PGMath::Point3D<float> imgPt;	

	m_transformStdToPat.Apply(stdPt, patPt); // Normalize to bring to a unit space
	m_transformPatToIso.Apply(patPt, isoPt); // apply direction cosines
	m_transformIsoToDcm.Apply(isoPt, dcmPt); // move origin to volume's center 
    m_transformDcmToImage.Apply(dcmPt, imgPt); // apply pixel spacing & inversion of Z as needed	
#endif


	return rv;
}

template <class T>
bool Voxel3D<T>::GetStdToPatCoord(const float& iX,  const float& iY, const float& iZ,
								  float& oX,  float& oY, float& oZ) 
{
		
	PGMath::Point3D<float> stdPt(iX, iY, iZ);
	PGMath::Point3D<float> patPt;
	PGMath::Point3D<float> isoPt;
	PGMath::Point3D<float> dcmPt;	
	PGMath::Point3D<float> outPt;	

	m_transformStdToPat.Apply(stdPt, patPt); // Normalize to bring to a unit space
	//m_transformPatToIso.Apply(patPt, isoPt); // apply direction cosines // stay in patient space
	m_transformIsoToDcm.Apply(patPt, dcmPt); // move origin to volume's center    


	const std::vector< PGMath::Vector3D<float> >& imgPosPatient = m_metaData.GetImagePositionsPatient();
	int firstSliceIdx = 0;//invZFactor<0.0f ? imgPosPatient.size()-1 : 0;
	const PGMath::Vector3D<float>& imgPosPatientOrg = imgPosPatient[firstSliceIdx];	

	PGMath::Matrix4x4<float> matrixDcmToPat;	
	matrixDcmToPat.Identity();
	{
		// get first slice pos
		matrixDcmToPat.SetElement(0, 3, imgPosPatientOrg.X());
		matrixDcmToPat.SetElement(1, 3, imgPosPatientOrg.Y());
		matrixDcmToPat.SetElement(2, 3, imgPosPatientOrg.Z());
	}	

	PGMath::AffineTransform<float> transformDcmToPat(matrixDcmToPat);



	transformDcmToPat.Apply(dcmPt, outPt); // add slice co-ordinates

	oX = outPt.X(); oY = outPt.Y(); oZ = outPt.Z();

	return true;
}

template <class T>
bool Voxel3D<T>::GetPatToStdCoord(const float& iX,  const float& iY, const float& iZ,
								  float& oX,  float& oY, float& oZ) 
{
		
	PGMath::Point3D<float> stdPt;
	PGMath::Point3D<float> patPt;
	PGMath::Point3D<float> isoPt;
	PGMath::Point3D<float> dcmPt;	
	PGMath::Point3D<float> inPt(iX, iY, iZ);	

	const std::vector< PGMath::Vector3D<float> >& imgPosPatient = m_metaData.GetImagePositionsPatient();
	int firstSliceIdx = 0;//invZFactor<0.0f ? imgPosPatient.size()-1 : 0;
	const PGMath::Vector3D<float>& imgPosPatientOrg = imgPosPatient[firstSliceIdx];	

	PGMath::Matrix4x4<float> matrixPatToDcm;	
	matrixPatToDcm.Identity();
	{
		// get first slice pos
		matrixPatToDcm.SetElement(0, 3, imgPosPatientOrg.X());
		matrixPatToDcm.SetElement(1, 3, imgPosPatientOrg.Y());
		matrixPatToDcm.SetElement(2, 3, imgPosPatientOrg.Z());
	}	
	
	PGMath::AffineTransform<float> transformPatToDcm(matrixPatToDcm);
	transformPatToDcm.Invert();
	transformPatToDcm.Apply(inPt, dcmPt); // subtract slice co-ordinates

	/*
	PGMath::Point3D<float> stdPt(iX, iY, iZ);
	PGMath::Point3D<float> patPt;
	PGMath::Point3D<float> isoPt;
	PGMath::Point3D<float> dcmPt;	
	PGMath::Point3D<float> imgPt;	

	m_transformStdToPat.Apply(stdPt, patPt); // Normalize to bring to a unit space
	m_transformPatToIso.Apply(patPt, isoPt); // apply direction cosines
	m_transformIsoToDcm.Apply(isoPt, dcmPt); // move origin to volume's center 
    m_transformDcmToImage.Apply(dcmPt, imgPt); // apply pixel spacing & inversion of Z as needed	
	*/


	m_transformDcmToIso.Apply(dcmPt, isoPt); // move origin to volume's center    
	m_transformIsoToPat.Apply(isoPt, patPt); // apply direction cosines
	m_transformPatToStd.Apply(patPt, stdPt); //Normalize to bring to a unit space

	oX = stdPt.X(); oY = stdPt.Y(); oZ = stdPt.Z();

	return true;
}

template <class T>
bool Voxel3D<T>::GetImgToPatCoord(const float& iX,  const float& iY, const float& iZ,
								  float& oX,  float& oY, float& oZ) 

{	
	// img -> std -> pat
	float ptStd[3]={0, 0, 0};
	bool rv = GetImgToStdCoord(iX, iY, iZ, ptStd[0], ptStd[1], ptStd[2]); 
	if (!rv) return false;


	rv = GetStdToPatCoord(ptStd[0], ptStd[1], ptStd[2], oX, oY, oZ);

	return rv;

	
	/*
	PGMath::Point3D<float> stdPt(iX, iY, iZ);
	PGMath::Point3D<float> patPt;
	PGMath::Point3D<float> isoPt;
	PGMath::Point3D<float> dcmPt;	
	PGMath::Point3D<float> imgPt;	

	m_transformStdToPat.Apply(stdPt, patPt); // Normalize to bring to a unit space
	m_transformPatToIso.Apply(patPt, isoPt); // apply direction cosines
	m_transformIsoToDcm.Apply(isoPt, dcmPt); // move origin to volume's center 
    m_transformDcmToImage.Apply(dcmPt, imgPt); // apply pixel spacing & inversion of Z as needed	
	*/

	PGMath::Point3D<float> pointStd, pointPat;

	// perform testing that all volumes provide same tranformed point
	// renderer's std to raw transform changes y's scaling component during initializeviewers (in full res)??
	
	bool imagesFollowScanDirection = m_metaData.GetImagesAlongScanDirection();
	float invZFactor = 1.0f;
	if (!imagesFollowScanDirection)
	{
		// Example: if axial images are head first, they increase in opposite direction to 
		// DICOM patient space's z-axis		
		invZFactor = -1.0f; 
	}
	const std::vector< PGMath::Vector3D<float> >& imgPosPatient = m_metaData.GetImagePositionsPatient();
	int firstSliceIdx = 0;//invZFactor<0.0f ? imgPosPatient.size()-1 : 0;
	const PGMath::Vector3D<float>& imgPosPatientOrg = imgPosPatient[firstSliceIdx];	

	PGMath::Matrix4x4<float> matrixImageToPat;	
	matrixImageToPat.Identity();
	{
		// get first slice pos
		matrixImageToPat.SetElement(0, 3, imgPosPatientOrg.X());
		matrixImageToPat.SetElement(1, 3, imgPosPatientOrg.Y());
		matrixImageToPat.SetElement(2, 3, imgPosPatientOrg.Z());
	}	

	PGMath::AffineTransform<float> transformImageToPat(matrixImageToPat);

	rv = m_transformImageToDcm.Apply(PGMath::Point3D<float>(iX, iY, iZ), pointStd);
	rv |= transformImageToPat.Apply(pointStd, pointPat);
	
	//oX = int(pointImg.X()+0.5f); oY = int(pointImg.Y()+0.5f); oZ = int(pointImg.Z()+0.5f);
	
	oX = pointPat.X(); oY = pointPat.Y(); oZ = pointPat.Z();

	return rv;
}

template <class T>
bool Voxel3D<T>::GetPatToImgCoord(const float& iX,  const float& iY, const float& iZ,
								  float& oX,  float& oY, float& oZ) 
{		
	// pat -> std -> img
	float ptStd[3]={0, 0, 0};
	bool rv = GetPatToStdCoord(iX, iY, iZ, ptStd[0], ptStd[1], ptStd[2]); 
	if (!rv) return false;


	rv = GetStdToImgCoord(ptStd[0], ptStd[1], ptStd[2], oX, oY, oZ);

	return rv;





	PGMath::Point3D<float> pointStd, pointImg;

	float invZFactor = 1.0f;
	bool imagesFollowScanDirection = m_metaData.GetImagesAlongScanDirection();
	if (!imagesFollowScanDirection)
	{
		// Example: if axial images are head first, they increase in opposite direction to 
		// DICOM patient space's z-axis		
		invZFactor = -1.0f; 
	}
	const std::vector< PGMath::Vector3D<float> >& imgPosPatient = m_metaData.GetImagePositionsPatient();
	int firstSliceIdx = 0;//invZFactor<0.0f ? imgPosPatient.size()-1 : 0;
	const PGMath::Vector3D<float>& imgPosPatientOrg = imgPosPatient[firstSliceIdx];	

	PGMath::Matrix4x4<float> matrixPatToImage;	
	matrixPatToImage.Identity();
	{
		// get first slice pos
		matrixPatToImage.SetElement(0, 3, -imgPosPatientOrg.X());
		matrixPatToImage.SetElement(1, 3, -imgPosPatientOrg.Y());
		matrixPatToImage.SetElement(2, 3, -imgPosPatientOrg.Z());
	}	

	PGMath::AffineTransform<float> transformPatToImage(matrixPatToImage);

	// perform testing that all volumes provide same tranformed point
	// renderer's std to raw transform changes y's scaling component during initializeviewers (in full res)??
	
	rv = transformPatToImage.Apply(PGMath::Point3D<float>(iX, iY, iZ), pointStd);
	rv |=	m_transformDcmToImage.Apply(pointStd, pointImg);	
	
	//oX = int(pointImg.X()+0.5f); oY = int(pointImg.Y()+0.5f); oZ = int(pointImg.Z()+0.5f);
	
	oX = pointImg.X(); oY = pointImg.Y(); oZ = pointImg.Z();

	return rv;
}


template <class T>
bool Voxel3D<T>::GetImgToStdCoord(const float& iX,  const float& iY, const float& iZ,
								  float& oX,  float& oY, float& oZ) const
{	
	PGMath::Point3D<float> pointImg;

	// perform testing that all volumes provide same tranformed point
	// renderer's std to raw transform changes y's scaling component during initializeviewers (in full res)??
	
	bool rv = m_transformRawToStd.Apply(PGMath::Point3D<float>(iX, iY, iZ), pointImg);
	
	//oX = int(pointImg.X()+0.5f); oY = int(pointImg.Y()+0.5f); oZ = int(pointImg.Z()+0.5f);
	
	oX = pointImg.X(); oY = pointImg.Y(); oZ = pointImg.Z();

	return rv;
}


template <class T>
T Voxel3D<T>::GetNormValue(const float& iX,  const float& iY, const float& iZ) // const // interpolate
{
	T rawValue = GetRawValue(iX, iY, iZ);

	float huVal = m_metaData.GetSlope() * float(rawValue) + m_metaData.GetIntercept();
	
	return T(huVal);
}


template <class T>
Voxel3D<T> &Voxel3D<T>::operator=(const Voxel3D<T> &iVoxel3DObject) 
{	
	m_metaData = iVoxel3DObject.GetMetaData();
	m_volumeAccessor = iVoxel3DObject.GetVolumeAccessor();
	return *this;
}

template <class T>
const PGCore::MetaData< T > & Voxel3D<T>::GetMetaData() const
{
	return m_metaData;
}

template <class T>
const PGMath::Point3D<float> & Voxel3D<T>::GetDimensions() const
{
	return m_dimensions;
}

template <class T>
PGCore::Volume< T > * Voxel3D<T>::GetVolumeAccessor() const
{
	return m_volumeAccessor;
}


// private
template <class T>
bool Voxel3D<T>::update()
{
	if (!m_volumeAccessor)
	{
		return false;
	}

	if (!m_volumeAccessor->GetDataRange(&m_minValue, &m_maxValue))
	{
		m_minValue = 0.0f;
		m_maxValue = 1.0f;
		m_rangeInv = 1.0f;
		return false;
	}

	m_rangeInv = float(PG_LUT_SIZE)/(float)(m_maxValue-m_minValue);

	if (!updateTransforms())
	{
		return false;
	}
	return true;
}

template <class T>
bool Voxel3D<T>::updateTransformImgToDcm()
{
	// TimgToMM = Tscale(x, y, z)

	//check not only feet first, but for all thhree orientations, check whether slice count increases along the
	//	resp. axis direction. if not, invert.

	float invZFactor = 1.0f; 
	//bool isAxialScanFlag = m_metaData.GetAxialScanFlag();
	/*if (isAxialScanFlag)
	{
		bool feetFirstFlag = m_metaData.GetImagesAlongScanDirection();

		if (!feetFirstFlag)
		{
			// if axial images are head first, they increase in opposite direction to 
			// DICOM patient space's z-axis		
			invZFactor = -1.0f; 
		}
	} */

	bool imagesFollowScanDirection = m_metaData.GetImagesAlongScanDirection();
	if (!imagesFollowScanDirection)
	{
		// Example: if axial images are head first, they increase in opposite direction to 
		// DICOM patient space's z-axis		
		invZFactor = -1.0f; 
	}


	// Tscale	
	PGMath::Vector3D<float> spacings = m_metaData.GetSpacing();	
	if (spacings.Length()==0.0f)
	{
		return false;
	}

	
	//const std::vector< PGMath::Vector3D<float> >& imgPosPatient = m_metaData.GetImagePositionsPatient();
	//int firstSliceIdx = invZFactor ? imgPosPatient.size()-1 : 0;
	//const PGMath::Vector3D<float>& imgPosPatientOrg = imgPosPatient[firstSliceIdx];	

	PGMath::Matrix4x4<float> m_matrixImageToMMScale;	
	m_matrixImageToMMScale.Identity();
	{
		m_matrixImageToMMScale.SetElement(0, 0, spacings.X()); 
		m_matrixImageToMMScale.SetElement(1, 1, spacings.Y()); 
		m_matrixImageToMMScale.SetElement(2, 2, invZFactor*spacings.Z()); 

		// get first slice pos
		//m_matrixImageToMMScale.SetElement(0, 3, imgPosPatientOrg.X());
		//m_matrixImageToMMScale.SetElement(1, 3, imgPosPatientOrg.Y());
		//m_matrixImageToMMScale.SetElement(2, 3, imgPosPatientOrg.Z());
	}	
	
	m_transformImageToDcm = PGMath::AffineTransform<float>(m_matrixImageToMMScale);

	m_transformDcmToImage = m_transformImageToDcm;
	m_transformDcmToImage.Invert();	

	return true;
}



template <class T>
bool Voxel3D<T>::updateTransformIsoToPat()
{
	// TimgToDcm = TtransToOrg-1 * Trotate * TtransToOrg 	

	// TtranslateFeetFirst
	// invert Z here because the volume is filled head first, not feet first
	//float spacingZ = m_metaData.GetImagesAlongScanDirection() ?  : 1.0f-spacings.Z();

	//is this the cause?? for a sagittal MRA scan, the Y co-ordinate doesnt translate properly.

	// Trotate	
	PGMath::Vector3D<float> imgX(1.f, 0.f, 0.f), imgY(0.f, 1.f, 0.f), imgZ(0.f, 0.f, 1.f); 		
	const std::vector<PGMath::Vector3D< float > > & imgXs = m_metaData.GetImageOrientationsPatientX();		
	const std::vector<PGMath::Vector3D< float > > & imgYs = m_metaData.GetImageOrientationsPatientY();	
	if (!imgXs.empty() && !imgYs.empty()) {
		imgX = imgXs[0];
		//imgX = PGMath::Vector3D<float>(-imgXs[0].X(), -imgXs[0].Y(), -imgXs[0].Z());		

		//imgY = PGMath::Vector3D<float>(-imgYs[0].X(), -imgYs[0].Y(), -imgYs[0].Z());		
		imgY = imgYs[0];	// ?? invert since Y in std space is opposite from Y in DICOM space

		imgZ = imgX ^ imgY;	// normal to image					
	}
	
	/*
	x'   | 1 0  0 |   | x |    | x*1 + y*0 + z*0  |
	y' = | 0 0 -1 | * | y | =  | x*0 + y*0 + z*-1 |
	z'	 | 0 1  0 |   | z |    | x*0 + y*1 + z*0  |

	Take a coronal image
	Take its orientation vectors to be (1, 0, 0) & (0, 0, -1) in DCM patient space
	Take a point on its
	  
	
	PGMath::Point3D<float>	  iDimensionsDMM;	
	m_transformImageToDcm.Apply(m_dimensionsImage, iDimensionsDMM);
	PGMath::Matrix4x4<float> m_matrixMMToMMorg;	
	{
		m_matrixMMToMMorg.SetElement(0, 3, -iDimensionsDMM.X()/2.0f);
		m_matrixMMToMMorg.SetElement(1, 3, -iDimensionsDMM.Y()/2.0f);
		m_matrixMMToMMorg.SetElement(2, 3, -iDimensionsDMM.Z()/2.0f);
	}*/


	PGMath::Matrix4x4<float> m_matrixIsoToPatientRotate;	
	{
		m_matrixIsoToPatientRotate.SetElement(0, 0, imgX.X()); 
		m_matrixIsoToPatientRotate.SetElement(0, 1, imgX.Y()); 
		m_matrixIsoToPatientRotate.SetElement(0, 2, imgX.Z()); 

		m_matrixIsoToPatientRotate.SetElement(1, 0, imgY.X()); 
		m_matrixIsoToPatientRotate.SetElement(1, 1, imgY.Y()); 
		m_matrixIsoToPatientRotate.SetElement(1, 2, imgY.Z()); 

		m_matrixIsoToPatientRotate.SetElement(2, 0, imgZ.X()); 
		m_matrixIsoToPatientRotate.SetElement(2, 1, imgZ.Y()); 
		m_matrixIsoToPatientRotate.SetElement(2, 2, imgZ.Z()); 
	}
	m_matrixIsoToPatientRotate.Invert(m_matrixIsoToPatientRotate); // needed as the rotation shd be from img to dcm
	
	/*
	PGMath::Matrix4x4<float> m_matrixMMOrgToMM;	
	{
		m_matrixMMOrgToMM.SetElement(0, 3, iDimensionsDMM.X()/2.0f);
		m_matrixMMOrgToMM.SetElement(1, 3, iDimensionsDMM.Y()/2.0f);
		m_matrixMMOrgToMM.SetElement(2, 3, iDimensionsDMM.Z()/2.0f);
	}

	PGMath::AffineTransform<float> m_transformMMToMMorg = PGMath::AffineTransform<float>(m_matrixMMToMMorg);
	PGMath::AffineTransform<float> m_transformMMOrgToMM = PGMath::AffineTransform<float>(m_matrixMMOrgToMM);
	PGMath::AffineTransform<float> m_transformMMToDicomRotate = PGMath::AffineTransform<float>(m_matrixMMToDicomRotate);

	//m_matrixMMToDicom.Multiply(m_matrixMMToMMorg, m_matrixMMToDicom);
	//m_matrixMMToDicom.Multiply(m_matrixMMToDicomRotate, m_matrixMMToDicom);
	//m_matrixMMToDicom.Multiply(m_matrixMMOrgToMM, m_matrixMMToDicom);

	m_transformIsoToPat = m_transformMMToMMorg;	
	m_transformIsoToPat.ConcatPost(&m_transformMMToDicomRotate);
	m_transformIsoToPat.ConcatPost(&m_transformMMOrgToMM);
	*/

	m_transformIsoToPat = PGMath::AffineTransform<float>(m_matrixIsoToPatientRotate);

	m_transformPatToIso = m_transformIsoToPat;
	m_transformPatToIso.Invert();	

	return true;
}

template <class T>
bool Voxel3D<T>::updateTransformDcmToIso()
{
	// TDcmToIso = Ttrans(-x/2, -y/2, -z/2)

	// Ttrans
	PGMath::DegreesOfFreedomBase<float> transDoF;	
	PGMath::Vector3D<float> spacings = m_metaData.GetSpacing();
	
	PGMath::Point3D<float>	  iDimensionsDMM;
	PGMath::Point3D<float>    iDimensionsImage = PGMath::Point3D<float>(m_dimensionsImage.X()-1, 
		m_dimensionsImage.Y()-1, m_dimensionsImage.Z()-1);

	m_transformImageToDcm.Apply(iDimensionsImage, iDimensionsDMM);

	transDoF.SetParameter(PGMath::kPgDegreeOfFreedomTranslationX, -iDimensionsDMM.X()/2); 
	transDoF.SetParameter(PGMath::kPgDegreeOfFreedomTranslationY, -iDimensionsDMM.Y()/2); 
	transDoF.SetParameter(PGMath::kPgDegreeOfFreedomTranslationZ, -iDimensionsDMM.Z()/2); 		

	m_transformDcmToIso = PGMath::AffineTransform<float>(		
		PGMath::Point3D<float>(0, 0, 0),
		transDoF);
	
	m_transformIsoToDcm = m_transformDcmToIso;

	return m_transformIsoToDcm.Invert();
}


template <class T>
bool Voxel3D<T>::updateTransformPatToStd()
{
	std::vector<float> dims;	
	{
		dims.push_back(m_dimensions.X());
		dims.push_back(m_dimensions.Y());
		dims.push_back(m_dimensions.Z());	
		std::sort(dims.begin(), dims.end());		
	}

	// create std to dicom space transform
	// deduce largest mm dimension	
	float scaleToStd = 1.0f;
	{	
		scaleToStd = 1.0f/dims[2];
		printf("\nDimensions: %lf %lf %lf", dims[0], dims[1], dims[2]);
	}
		
	PGMath::DegreesOfFreedomBase<float> isoToStdDoF;		
	isoToStdDoF.SetParameter(PGMath::kPgDegreeOfFreedomScalingX, scaleToStd); 
	isoToStdDoF.SetParameter(PGMath::kPgDegreeOfFreedomScalingY, scaleToStd); 
	isoToStdDoF.SetParameter(PGMath::kPgDegreeOfFreedomScalingZ, scaleToStd); 

	m_transformPatToStd = PGMath::AffineTransform<float>(
		PGMath::Point3D<float>(0.0f, 0.0f, 0.0f),
		isoToStdDoF);

	m_transformStdToPat = m_transformPatToStd;

	return m_transformStdToPat.Invert();
}


template <class T>
bool Voxel3D<T>::updateTransforms()
{
	if (!updateDimensionsImage())
	{
		return false;
	}

	if (!updateTransformImgToDcm())
	{
		return false;
	}	

	if (!updateTransformDcmToIso())
	{
		return false;
	}	

	if (!updateTransformIsoToPat())
	{
		return false;
	}

	if (!updateDimensionsMM())
	{
		return false;
	}

	if (!updateTransformPatToStd())
	{
		return false;
	}	

	
	//m_transformStdToImg = m_transformRegistration;
	m_transformStdToImg = m_transformStdToPat;			  // Normalize to bring to a unit space
	m_transformStdToImg.ConcatPre(&m_transformPatToIso);  // apply direction cosines
	m_transformStdToImg.ConcatPre(&m_transformIsoToDcm);  // move origin to volume's center 
	m_transformStdToImg.ConcatPre(&m_transformDcmToImage);// apply pixel spacing & inversion of Z as needed

	m_transformStdToRaw = m_transformStdToImg;

	m_transformRawToStd = m_transformStdToRaw;
	m_transformRawToStd.Invert();

	m_transformImgToStd = m_transformStdToImg;

	return m_transformImgToStd.Invert();
}

template <class T>
bool Voxel3D<T>::updateDimensionsMM()
{
	if (!m_volumeAccessor)
	{
		m_dimensions = PGMath::Point3D<float>(0, 0, 0);		
		return false;
	}

	// get raw dims
	PGMath::Point3D<float> iDimensionsD;
	m_transformImageToDcm.Apply(m_dimensionsImage, iDimensionsD);	
	m_transformIsoToPat.Apply(iDimensionsD, iDimensionsD);

	m_dimensions = PGMath::Point3D<float>(fabs(iDimensionsD.X()), 
		fabs(iDimensionsD.Y()), fabs(iDimensionsD.Z()));	
	

	return true;
}


template <class T>
bool Voxel3D<T>::updateDimensionsImage()
{
	if (!m_volumeAccessor)
	{		
		m_dimensionsImage = PGMath::Point3D<float>(0, 0, 0);
		return false;
	}

	// get raw dims
	PGMath::Point3D<long> iDimensions;
	m_volumeAccessor->GetDimensions(iDimensions);
	PGMath::Point3D<float> iDimensionsD(iDimensions.X(), iDimensions.Y(), iDimensions.Z());

	m_dimensionsImage = iDimensionsD;//%iSpacings;

	return true;
}

template <class T>
bool Voxel3D<T>::updateInterpolationFunctionPtr()
{
	switch (m_interpolationType)
	{
	case PGCore::ePgInterpolationTypeNearestNeighbor:
			m_interpolationFnPtr = &Voxel3D<T>::interpolateNearestNeighbor;
			break;
		
	case PGCore::ePgInterpolationTypeTriLinear:
			m_interpolationFnPtr = &Voxel3D<T>::interpolateTrilinear;
			break;

		default:
			m_interpolationFnPtr = &Voxel3D<T>::interpolateNearestNeighbor;
			break;
	}

	return true;
}



template <class T>
T Voxel3D<T>::normalize(const T& iValue) const
{
	float dValue = ((float)(iValue-m_minValue) * m_rangeInv);///(float)(m_maxValue-m_minValue));
	return (T)(0.5 + dValue);//floor(dValue);
}

template <class T>
T Voxel3D<T>::interpolateNoBoundCheck(const PGMath::Point3D<float> & iPoint) const
{
	T iValue = interpolateTrilinearNoBoundCheck(iPoint);		
	return iValue<=m_minValue ? (T)0 : normalize(iValue);
}


template <class T>
T Voxel3D<T>::interpolate(const PGMath::Point3D<float> & iPoint) const
{
	/*if (iPoint.X() > m_dimensions.X() || iPoint.Y() > m_dimensions.Y()-1 || iPoint.Z() > m_dimensions.Z()-1
		|| iPoint.X() < 0 || iPoint.Y() < 0 || iPoint.Z() < 0)
	{
		return T(0);
	}*/
	// add interpolation here
	// X : iPoint.X() <columns>
	// Y : iPoint.Y() <rows>	

	//	fetch and normalize to 0...255 scale

	//int iPointZ = (int)(iPoint.Z());//m_metaData.GetImagesAlongScanDirection() ? 
		//(int)(iPoint.Z()) : m_dimensionsImage.Z()-1-(int)(iPoint.Z());

	T iValue = (this->*m_interpolationFnPtr)(iPoint);
	
	//try
	{
		//iValue = m_volumeAccessor->GetValue((int)(iPoint.Y()), (int)(iPoint.X()),  (int)(iPoint.Z()));

	} 
	//catch (...)
	{
		//LOG0("Voxel3D<T>::interpolate error: z index out of bound");
	}

	//iValue

	////if (iValue<=m_minValue)		return (T)0;

	//iValue = normalize(iValue);
	
	/*if (iValue>4095 || iValue<0)
	{
		iValue=iValue;
	}*/

	return iValue<=m_minValue ? (T)0 : normalize(iValue);
}

template <class T>
T Voxel3D<T>::getRawValue(int iX, int iY, int iZ) const
{
	return m_volumeAccessor->GetValue(iY, iX, iZ);
}

template <class T>
T Voxel3D<T>::interpolateTrilinearNoBoundCheck(const PGMath::Point3D<float> & iPoint) const
{	
	//V000 (1 - x) (1 - y) (1 - z) +	
	//V100 x (1 - y) (1 - z) + 
	//V010 (1 - x) y (1 - z) + 
	//V001 (1 - x) (1 - y) z +
	//V101 x (1 - y) z + 
	//V011 (1 - x) y z + 
	//V110 x y (1 - z) + 
	//V111 x y z 		
	
	int x0 = (int)(iPoint.X()), y0 = (int)(iPoint.Y()),	z0 = (int)(iPoint.Z());
	float wx = iPoint.X()-x0, wy = iPoint.Y()-y0, wz = iPoint.Z()-z0;
	T ival=0;

	float nwx = (1.0f-wx), nwy = (1.0f-wy), nwz = (1.0f-wz);
	

	float nwx_nwy = nwx*nwy, nwx_wy = nwx*wy, wx_nwy = wx*nwy, wx_wy = wx*wy;

//#define _USE_OPT
#ifdef _USE_OPT

	

	T eightNeighbors[8];
	m_volumeAccessor->GetEightNeighborValues(x0, y0, z0, eightNeighbors);	

	
	ival	 = nwx_nwy*nwz		*	eightNeighbors[0];	
	ival	+= nwx_nwy*wz		*	eightNeighbors[1];	// +x, y, +z
	ival	+= nwx_wy *nwz		*	eightNeighbors[2];	    // x, +y, z
	ival	+= nwx_wy *wz		*   eightNeighbors[3];	// 	
	ival	+= wx_nwy *nwz		*   eightNeighbors[4];
	ival	+= wx_nwy *wz		*   eightNeighbors[5];
	ival	+= wx_wy  *nwz		*   eightNeighbors[6];
	ival	+= wx_wy  *wz		*   eightNeighbors[7];
#else
	
	ival	 = nwx_nwy*nwz		*	(float)m_volumeAccessor->GetValueNoBoundCheck(y0, x0, z0);	// x, y, z
	ival	+= nwx_nwy*wz		*	(float)m_volumeAccessor->GetValueNoBoundCheck(y0, x0, z0+1);	// x, y, z+
	ival	+= wx_nwy *nwz		*	(float)m_volumeAccessor->GetValueNoBoundCheck(y0, x0+1, z0);	// x+, y, z
	ival	+= wx_nwy *wz		*   (float)m_volumeAccessor->GetValueNoBoundCheck(y0, x0+1, z0+1);	// x+, y, z+	
	ival	+= nwx_wy *nwz		*   (float)m_volumeAccessor->GetValueNoBoundCheck(y0+1, x0, z0);   // x, y+, z
	ival	+= nwx_wy *wz		*   (float)m_volumeAccessor->GetValueNoBoundCheck(y0+1, x0, z0+1);   // x, y+, z+
	ival	+= wx_wy  *nwz		*   (float)m_volumeAccessor->GetValueNoBoundCheck(y0+1, x0+1, z0);   // x+, y+, z
	ival	+= wx_wy  *wz		*   (float)m_volumeAccessor->GetValueNoBoundCheck(y0+1, x0+1, z0+1);   // x+, y+, z+
/*
	ival	 = nwx*nwy*nwz		*	m_volumeAccessor->GetValueNoBoundCheck(y0, x0, z0);	
	ival	+= nwx*nwy*wz		*	m_volumeAccessor->GetValueNoBoundCheck(y0+1, x0, z0+1);	// +x, y, +z
	ival	+= nwx*(wy)*nwz		*	m_volumeAccessor->GetValueNoBoundCheck(y0, x0+1, z0);	    // x, +y, z
	ival	+= nwx*(wy)*(wz)	*   m_volumeAccessor->GetValueNoBoundCheck(y0, x0+1, z0+1);	// 	
	ival	+= (wx)*nwy*nwz		*   m_volumeAccessor->GetValueNoBoundCheck(y0+1, x0, z0);
	ival	+= (wx)*nwy*(wz)	*   m_volumeAccessor->GetValueNoBoundCheck(y0+1, x0, z0+1);
	ival	+= (wx)*(wy)*nwz	*   m_volumeAccessor->GetValueNoBoundCheck(y0+1, x0+1, z0);
	ival	+= (wx)*(wy)*(wz)	*   m_volumeAccessor->GetValueNoBoundCheck(y0+1, x0+1, z0+1);*/
#endif

	//return (T)(ival+0.5f) > (T)(ival)  ?  (T)(ival+0.5f) : (T)(ival) ;;
	return T_ROUND(ival);//ASM_ROUND(ival);;//T_ROUND(ival);//(T)(ival+0.5f) > (T)(ival)  ?  (T)(ival+0.5f) : (T)(ival) ;;

	// add interpolation here
	// X : iPoint.X() <columns>
	// Y : iPoint.Y() <rows>	

	//	fetch and normalize to 0...255 scale

	//T iValue = (this->*m_interpolationFnPtr)(iPoint);
	
	//try
	{
		//T iValue = m_volumeAccessor->GetValue((int)(iPoint.Y()), (int)(iPoint.X()),  (int)(iPoint.Z()));
	} 
	//catch (...)
	{
		//LOG0("Voxel3D<T>::interpolate error: z index out of bound");
	}

	////if (iValue<=m_minValue)
	//	return (T)0;

	////iValue = normalize(iValue);
	
	//return iValue;
}


template <class T>
T Voxel3D<T>::interpolateTrilinear(const PGMath::Point3D<float> & iPoint) const
{	
	//V000 (1 - x) (1 - y) (1 - z) +	
	//V100 x (1 - y) (1 - z) + 
	//V010 (1 - x) y (1 - z) + 
	//V001 (1 - x) (1 - y) z +
	//V101 x (1 - y) z + 
	//V011 (1 - x) y z + 
	//V110 x y (1 - z) + 
	//V111 x y z 		
	
	int x0 = (int)(iPoint.X()), y0 = (int)(iPoint.Y()),	z0 = (int)(iPoint.Z());
	float wx = iPoint.X()-x0, wy = iPoint.Y()-y0, wz = iPoint.Z()-z0;
	float ival=0;

	float nwx = (1.0f-wx), nwy = (1.0f-wy), nwz = (1.0f-wz);
	

#define _USE_OPT
#ifdef _USE_OPT
	float nwx_nwy = nwx*nwy, nwx_wy = nwx*wy, wx_nwy = wx*nwy, wx_wy = wx*wy;

	T eightNeighbors[8];// = {0, 0, 0, 0, 0, 0, 0, 0};
	bool res = m_volumeAccessor->GetEightNeighborValues(y0, x0, z0, eightNeighbors);	
	if (!res)
	{
		return m_minValue;
	}
	
	ival	 = nwx_nwy*nwz		*	(float)eightNeighbors[0];	// x, y, z
	ival	+= nwx_nwy*wz		*	(float)eightNeighbors[1];	// x, y, z+
	ival	+= wx_nwy *nwz		*	(float)eightNeighbors[2];	// x+, y, z
	ival	+= wx_nwy *wz		*   (float)eightNeighbors[3];	// x+, y, z+	
	ival	+= nwx_wy *nwz		*   (float)eightNeighbors[4];   // x, y+, z
	ival	+= nwx_wy *wz		*   (float)eightNeighbors[5];   // x, y+, z+
	ival	+= wx_wy  *nwz		*   (float)eightNeighbors[6];   // x+, y+, z
	ival	+= wx_wy  *wz		*   (float)eightNeighbors[7];   // x+, y+, z+
#else
	
	ival	 = nwx*nwy*nwz		*	m_volumeAccessor->GetValue(y0, x0, z0);	
	ival	+= nwx*nwy*wz		*	m_volumeAccessor->GetValue(y0+1, x0, z0+1);	// +x, y, +z
	ival	+= nwx*(wy)*nwz		*	m_volumeAccessor->GetValue(y0, x0+1, z0);	    // x, +y, z
	ival	+= nwx*(wy)*(wz)	*   m_volumeAccessor->GetValue(y0, x0+1, z0+1);	// 	
	ival	+= (wx)*nwy*nwz		*   m_volumeAccessor->GetValue(y0+1, x0, z0);
	ival	+= (wx)*nwy*(wz)	*   m_volumeAccessor->GetValue(y0+1, x0, z0+1);
	ival	+= (wx)*(wy)*nwz	*   m_volumeAccessor->GetValue(y0+1, x0+1, z0);
	ival	+= (wx)*(wy)*(wz)	*   m_volumeAccessor->GetValue(y0+1, x0+1, z0+1);
#endif

	T oValue = T_ROUND(ival);

	/*if (oValue<255 && oValue>0)
	{
		oValue=oValue;
		LOG1("Value not max/min: %d", oValue);
	}*/
	//return (T)(ival+0.5f) > (T)(ival)  ?  (T)(ival+0.5f) : (T)(ival) ;;
	return oValue;//ASM_ROUND(ival);;//T_ROUND(ival);//(T)(ival+0.5f) > (T)(ival)  ?  (T)(ival+0.5f) : (T)(ival) ;;

	// add interpolation here
	// X : iPoint.X() <columns>
	// Y : iPoint.Y() <rows>	

	//	fetch and normalize to 0...255 scale

	//T iValue = (this->*m_interpolationFnPtr)(iPoint);
	
	//try
	{
		//T iValue = m_volumeAccessor->GetValue((int)(iPoint.Y()), (int)(iPoint.X()),  (int)(iPoint.Z()));
	} 
	//catch (...)
	{
		//LOG0("Voxel3D<T>::interpolate error: z index out of bound");
	}

	////if (iValue<=m_minValue)
	//	return (T)0;

	////iValue = normalize(iValue);
	
	//return iValue;
}


template <class T>
T Voxel3D<T>::interpolateNearestNeighbor(const PGMath::Point3D<float> & iPoint) const
{
	// add interpolation here
	// X : iPoint.X() <columns>
	// Y : iPoint.Y() <rows>	

	//	fetch and normalize to 0...255 scale

	//int iPointZ = (int)(iPoint.Z());//m_metaData.GetImagesAlongScanDirection() ? 
		//(int)(iPoint.Z()) : m_dimensionsImage.Z()-1-(int)(iPoint.Z());

	T iValue = getRawValue((int)(iPoint.X()), (int)(iPoint.Y()),  (int)(iPoint.Z()));

	
	//catch (...)
	{
		//LOG0("Voxel3D<T>::interpolate error: z index out of bound");
	}

	//if (iValue<=m_minValue)
	//	return (T)0;

	//iValue = normalize(iValue);
	
	/*if (iValue>4095 || iValue<0)
	{
		iValue=iValue;
	}*/

	return iValue;
}

#ifdef _PG_GENERATE_SDK_LIBS_
	template class Voxel3D<short>; 
	template class Voxel3D<char>;
	template class Voxel3D<float>;
	template class Voxel3D<unsigned short>; 
	template class Voxel3D<unsigned char>; 
#endif
	
};

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PG_VOXEL3D_HPP_
