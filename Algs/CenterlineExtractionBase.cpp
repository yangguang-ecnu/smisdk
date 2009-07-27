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
// Filename:	CenterlineExtractionBase.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGCenterlineExtractionBase_HPP_
#define _PGCenterlineExtractionBase_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/CenterlineExtractionBase.h"

namespace PGAlgs
{

	using namespace PGAlgs;

	static const char kPgNeighborIncr[26][3] = 
		{
			// Z=-1
			{-1, -1, -1 },
			{ -1,  0, -1 },
			{ -1,  1, -1 },
			{  0,  1, -1 },
			{  1,  1, -1 },
			{  1,  0, -1 },
			{  1, -1, -1 },
			{  0, -1, -1 },
			{  0,  0, -1 },

			// Z=+1
			{ -1, -1, 1 },
			{ -1,  0, 1 },
			{ -1,  1, 1 },
			{  0,  1, 1 },
			{  1,  1, 1 },
			{  1,  0, 1 },
			{  1, -1, 1 },
			{  0, -1, 1 },

			// Z = 0
			{ -1, -1, 0 },
			{ -1,  0, 0 },
			{ -1,  1, 0 },
			{  0,  1, 0 },
			{  1,  1, 0 },
			{  1,  0, 0 },
			{  1, -1, 0 },
			{  0, -1, 0 },
			{  0,  0, 1 }
		};

	template <class T, class U>
	CenterlineExtractionBase<T, U>::CenterlineExtractionBase(void) 
	{
		//m_pIOVolume = 0;
		m_pIVolume = 0;
		m_count = 0;
		m_window = 0.1f;
		m_lowValue = 0;
		m_highValue = 255;
		m_totalCount = 1;
		m_autoDilationCount = 1;
		m_gradientHigh = 60;
		m_maxLoopCount = 500;
		m_neighborTh = 3;
		m_lastDirection = PGMath::Vector3D<float>(0, 0, 0);
		m_autoAdjustConditions = false;
	};

	// -0.5 to 0.5
	template <class T, class U>
	bool CenterlineExtractionBase<T, U>::SetSeeds(const std::vector< PGMath::Point3D< float > > &iSeeds)
	{
		m_pSeeds = iSeeds;
		return m_pSeeds.size()>0;
	};

	template <class T, class U>
	bool CenterlineExtractionBase<T, U>::SetGradientHigh(const int& iGradient)
	{
		m_gradientHigh = iGradient;
		return true;
	}

	template <class T, class U>
	bool CenterlineExtractionBase<T, U>::SetAutoAdjustConditions(const bool iFlag)
	{
		m_autoAdjustConditions = iFlag;
		return true;
	}

	// 0.0 to 1.0
	template <class T, class U>
	bool CenterlineExtractionBase<T, U>::SetWindow(const float& iWindow)
	{	
		if (iWindow<0.0f || iWindow >1.0f)
		{
			return false;
		}

		m_window = iWindow;
		return true;
	};

	template <class T, class U>
	bool CenterlineExtractionBase<T, U>::SetAutoDilationCount(const int& iCount)
	{
		m_autoDilationCount = iCount;
		return true;
	};

	template <class T, class U>
	CenterlineExtractionBase<T, U>::~CenterlineExtractionBase(void) {  };

	template <class T, class U>
	bool CenterlineExtractionBase<T, U>::transformSeedsVoxelToImg()
	{
		if (m_pIVolume->GetVolumeAccessor()==NULL)
		{
			return false;
		}

		// transform seeds into volume space
		PGMath::AffineTransform<float> voxelToImage = m_pIVolume->GetTransformVoxelToImage();
		
		const PGCore::MetaData< T >& oMetaData = m_pIVolume->GetMetaData();
		

		for (int i=0; i<m_pSeeds.size(); i++)
		{
			PGMath::Point3D< float > seed = m_pSeeds[i];
			voxelToImage.Apply(seed, m_pSeeds[i]);		

			GetLogger()->Log("Seed[%d]: [%f %f %f] -> [%f %f %f]", i, seed.X(), seed.Y(), seed.Z(), 
					m_pSeeds[i].X(), m_pSeeds[i].Y(), m_pSeeds[i].Z());				
			
		}		
		
		return true;
	};

	// in image co-ordinates
	template <class T, class U>
	bool CenterlineExtractionBase<T, U>::setVoxel(const PGMath::Point3D<int> &iVoxel, const bool iValue)
	{
		//if (m_pIOVolume->GetVolumeAccessor()==NULL)
		{
		//	return false;
		}

		GetLogger()->Log("Marking %d %d %d [%d]", iVoxel.X(), iVoxel.Y(), iVoxel.Z(), iValue? 1 : 0);
		
		return m_pIVolume->GetVolumeAccessor()->GetBitVolume(1).SetValue(iVoxel.Y(), iVoxel.X(), iVoxel.Z(), iValue);		
		
	};


	template <class T, class U>
	bool CenterlineExtractionBase<T, U>::autoAdjustConditions(const PGMath::Point3D<int>& iSeed, const eCLineRetCode iReason)
	{
		return true;

		//int gradHigh = m_gradientHigh;
		//return gradHigh;

		/*
		// sample in a 3x3x3 cube, compute avg gradient
		PGMath::Vector3D<float> gradientValue;
		m_pIVolume->GetVolumeAccessor()->GetGradient(iVoxel.X(), iVoxel.Y(), iVoxel.Z(), gradientValue);
		float gVal = gradientValue.Length();
		*/
	}

	template <class T, class U>
	float CenterlineExtractionBase<T, U>::estimateRadiusAlongVector(const PGMath::Point3D<int> &iVoxel,
			const PGMath::Vector3D<float>& iDirection, PGMath::Point3D<int>& oRadialPoint)
	{
		float incr = 2.0f;
		float radius = 0.0f;
		int iCount=0;
		
		PGMath::Vector3D<float> direction = iDirection;
		direction.Normalize();
		PGMath::Point3D<float> nextPt(iVoxel.X(), iVoxel.Y(), iVoxel.Z());
		PGMath::Point3D<int> nextPtI(iVoxel);
		while (conditionCheck(nextPtI)==CLineRetCodeOk && iCount<1000)
		{			
			PGMath::Vector3D<float> incrVec = direction*incr;
			nextPt = PGMath::Point3D<float>(nextPt.X() + incrVec.X(),
					nextPt.Y() + incrVec.Y(), nextPt.Z() + incrVec.Z());
			nextPtI = PGMath::Point3D<int>(nextPt.X(), nextPt.Y(), nextPt.Z());
			iCount++;
		}

		oRadialPoint = nextPtI;
		radius = (oRadialPoint - iVoxel).Length();

		return radius;
	}


	/*
		for any point, the next direction is roughly determined by least gradient direction out of 
		26 directions. Then it is adjusted based on local profile in a plane perpendicular to the initial plane.
	*/
	template <class T, class U>
	eCLineRetCode CenterlineExtractionBase<T, U>::sampleCrossSection(const PGMath::Point3D<int> &iVoxel, 
		const PGMath::Vector3D<float> &iCurrentDirection, 
		PGMath::Point3D<int> &oAdjustedVoxel		
		)
	{
		// find plane perpendicular to iCurrentDirection
		//	find least component of iCurrentDirection
		PGMath::Vector3D<float> currentDirection = iCurrentDirection;
		currentDirection.Normalize();

		const float axes[3][3] = {
			{1, 0, 0},
			{0, 1, 0},
			{0, 0, 1}
		};
		/*
		int leastAxisIdx = 0;
		if (currentDirection.X() < currentDirection.Y())
		{
			if (currentDirection.X() < currentDirection.Z())
			{
				leastAxisIdx = 0;
			} else 
			{
				leastAxisIdx = 2;
			}
		} else if (currentDirection.Y() < currentDirection.Z())
		{
			leastAxisIdx = 1;
		} else 
		{
			leastAxisIdx = 2;
		}

		PGMath::Vector3D<float> planeVectorOne(axes[leastAxisIdx][0], axes[leastAxisIdx][1], axes[leastAxisIdx][2]);
		PGMath::Vector3D<float> planeVectorTwo = planeVectorOne^currentDirection;

		planeVectorTwo.Normalize();

		// find out the other primary axis
		float minDot = 9999;
		for (int i=0; i<3; i++)
		{
			float dot = planeVectorTwo * PGMath::Vector3D<float>(axes[i][0], axes[i][1], axes[i][2]);
			minDot = (dot < minDot) ? dot : minDot;			
		}

		float theta = acos(minDot);
*/
		// in the plane, shoot rays around iVoxel in 8 directions, store radii
		float pi = 22.0f/7.0f;
		float da = pi/4.0f, angle = 0;
		float radii[8];
		PGMath::Point3D<int> perimeter[8];

		for (int a=0; a<8; a++, angle+=da)
		{
			float x = cos(angle), y = sin(angle);

			// shoot ray from center in this direction to estimate radius
			radii[a] = estimateRadiusAlongVector(iVoxel, PGMath::Vector3D<float>(x, y, 0),
					perimeter[a]);
		}

		// oAdjustedVoxel becomes CG of the sampled disc
		oAdjustedVoxel = PGMath::Point3D<int>(0, 0, 0);
		float oneEigth = 1.0f/8.0f;
		for (int a=0; a<8; a++)
		{
			PGMath::Point3D<int> incrVec = perimeter[a]*oneEigth;
			oAdjustedVoxel = oAdjustedVoxel + incrVec;
		}

		return CLineRetCodeOk;
	}


	// 0: out of range, 1: ok, -1: stack error
	//template <class T, class U>
	//eCLineRetCode CenterlineExtractionBase<T, U>::visitVoxel(const PGMath::Point3D<int> &iVoxel)
	
	template <class T, class U>
	eCLineRetCode CenterlineExtractionBase<T, U>::directionCheck(const PGMath::Point3D<int> &iVoxel, 
		PGMath::Vector3D<float> &oNextDirection, PGMath::Point3D<int> &oAdjustedVoxel)
	{
		// shoot rays perpendicular to the current direction, approximate a disc
		// find disc's CG, //do hessian analysis here if for small vessels
		// find direction with least resistance, move dT,
		// mark as next seed


		// compute gradient along 26 neighbors
		T cValue = m_pIVolume->GetVolumeAccessor()->GetValue(iVoxel.Y(), iVoxel.X(), iVoxel.Z());
		int gradValue, minGradVal = 9999, minGradIndex = 0;
		float maxDot=0.1f;
		PGMath::Vector3D<float> nextDir;

		for (int i=0; i<26; i++)
		{
			nextDir = PGMath::Vector3D<float>(kPgNeighborIncr[i][0], 
					kPgNeighborIncr[i][1], 
					kPgNeighborIncr[i][2]);
			nextDir.Normalize();

			PGMath::Vector3D<float> nextLocation(iVoxel.X()+kPgNeighborIncr[i][0], 
					iVoxel.Y()+kPgNeighborIncr[i][1], 
					iVoxel.Z()+kPgNeighborIncr[i][2]);

			bool maskValue = m_pIVolume->GetVolumeAccessor()->GetBitVolume(1).GetValue(
				nextLocation.Y(), nextLocation.X(), nextLocation.Z()
				);
			if (!maskValue)
			{
				T nextValue = 
					m_pIVolume->GetVolumeAccessor()->GetValue(
						nextLocation.Y(), nextLocation.X(), nextLocation.Z()
						);
				gradValue = int(abs(nextValue-cValue));

				if (minGradVal>gradValue)
				{
					int dot = m_lastDirection*nextDir;
					if (dot>maxDot)
					{
						minGradVal = gradValue;
						minGradIndex = i;
						maxDot = dot;
					}
				}
			}
		}
   
		eCLineRetCode retCode = CLineRetCodeGradient;
		if (minGradIndex>-1)
		{
			oNextDirection = PGMath::Vector3D<float>(kPgNeighborIncr[minGradIndex][0], 
							kPgNeighborIncr[minGradIndex][1], 
							kPgNeighborIncr[minGradIndex][2]);

			m_lastDirection = nextDir;
			retCode = sampleCrossSection(iVoxel, nextDir, oAdjustedVoxel);
		}

		return retCode;
	}	


	template <class T, class U>
	eCLineRetCode CenterlineExtractionBase<T, U>::conditionCheck(const PGMath::Point3D<int> &iVoxel)
	{
		if (//m_count>10000 || 
			iVoxel.X() < 1 || iVoxel.X() > m_volumeDimensions.X()-2 ||
			iVoxel.Y() < 1 || iVoxel.Y() > m_volumeDimensions.Y()-2 ||
			iVoxel.Z() < 1 || iVoxel.Z() > m_volumeDimensions.Z()-2
			)
			return CLineRetCodeOutOfRange;

		bool maskValue = m_pIVolume->GetVolumeAccessor()->GetBitVolume(1).GetValue(iVoxel.Y(), iVoxel.X(), iVoxel.Z());

		if (maskValue)
		{
			return CLineRetCodeAlreadyMarked;
		}

		//return CLineRetCodeOk;


		//T iOValue = m_pIVolume->GetVolumeAccessor()->GetValue(iVoxel.X(), iVoxel.Y(), iVoxel.Z());
		T iValue = m_pIVolume->GetVolumeAccessor()->GetAvgValue(iVoxel.X(), iVoxel.Y(), iVoxel.Z());

		//GetLogger()->Log("visitVoxel[%d]: Considering %d %d %d: M:%d V: %d", m_count, iVoxel.X(), iVoxel.Y(), iVoxel.Z(),
		//	maskValue, iValue);

		//if (fabs((float)(iValue-m_seedValue)) > 10) 
		if (iValue<m_lowValue || iValue>m_highValue)
		{
			//setVoxel(iVoxel, 5);
			return CLineRetCodeValue;
		}

/*
		PGMath::Vector3D<float> gradientValue;
		m_pIVolume->GetVolumeAccessor()->GetGradient(iVoxel.X(), iVoxel.Y(), iVoxel.Z(), gradientValue);
		//if (gradientValue.X() > 50 || gradientValue.Y() > 50 || gradientValue.Z() > 50)
		float gVal = gradientValue.Length();
		if (gVal > m_gradientHigh)
		{
			//GetLogger()->Log("visitVoxel[%d]: rejecting %3.4f / 50", m_count, gVal);
			return CLineRetCodeGradient;
		}
		*/

		return CLineRetCodeOk;
	}

	template <class T, class U>
	eCLineRetCode CenterlineExtractionBase<T, U>::marchVoxel(const PGMath::Point3D<int> &iVoxel)
	{			
		eCLineRetCode	checkCode = CLineRetCodeOk;

		checkCode = conditionCheck(iVoxel);
		if (checkCode!=CLineRetCodeOk)
		{
			return  checkCode;
		}
		
		
		// get next direction
		PGMath::Vector3D<float> nextDirection;
		PGMath::Point3D<int> oAdjustedVoxel;
		checkCode = directionCheck(iVoxel, nextDirection, oAdjustedVoxel);
		
		setVoxel(oAdjustedVoxel, true);
		m_count++;

		if (checkCode!=CLineRetCodeOk)
		{
			return checkCode;
		}
		
		bool rv = m_stack.Push(			
			PGMath::Point3D<int>(
			iVoxel.X()+nextDirection.X(), 
			iVoxel.Y()+nextDirection.Y(), 
			iVoxel.Z()+nextDirection.Z()
			)
			);
		if (!rv) 
		{
			return CLineRetCodeStackError;
		}

		return CLineRetCodeOk;
	};
	

#ifdef _PG_GENERATE_SDK_LIBS_
	template class CenterlineExtractionBase<short, short>; 
#endif


};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGCenterlineExtractionBase_HPP_
