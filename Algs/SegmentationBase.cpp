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
// Filename:	SegmentationBase.hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGSegmentationBASE_HPP_
#define _PGSegmentationBASE_HPP_
//////////////////////////////////////////////////////////////////////////


#include "Algs/SegmentationBase.h"

#define kPgStDevSpreadFac 2.5
#define kPgStDevGradientFac 0.01
#define kPgSHORT_MAX 32767


namespace PGAlgs
{

	using namespace PGAlgs;

	template <class T>
	PGAlgs::ThreadedSegmentationParams<T>::ThreadedSegmentationParams() 
	{
		m_pSeeds.clear();	
		m_autoDilationCount = 0;
		m_autoAdjustConditions = false;
		m_addSeedCode = SegAddSeedCodeSeed;
		m_pIVolume = 0;
		m_window = 0.1f;
		m_gradientHigh = 75;
		m_maxLoopCount = 32767;
		m_neighborTh = 3;
		m_stdSpreadValue = kPgStDevSpreadFac;
		m_stdSpreadGradient = kPgStDevGradientFac;
	};

	template <class T, class U>
	SegmentationBase<T, U>::SegmentationBase(void) 
	{
		//m_pIOVolume = 0;
		m_pIVolume = 0;
		m_count = 0;
		m_window = 0.1f;
		m_lowValue = 0;
		m_highValue = 255;
		m_totalCount = 1;
		m_autoDilationCount = 1;
		m_addSeedCode = SegAddSeedCodeSeed;
		m_gradientHigh = 60;
		m_maxLoopCount = 32;
		m_neighborTh = 3;
		m_autoAdjustConditions = false;
		m_stdSpreadValue = kPgStDevSpreadFac;
		m_stdSpreadGradient = kPgStDevGradientFac;
		m_undoCounter = 0;
	};

	// -0.5 to 0.5
	template <class T, class U>
	bool SegmentationBase<T, U>::SetSeeds(const std::vector< PGMath::Point3D< float > > &iSeeds)
	{
		m_pSeeds = iSeeds;
		return m_pSeeds.size()>0;
	};

	template <class T, class U>
	bool SegmentationBase<T, U>::SetGradientHigh(const int& iGradient)
	{
		m_gradientHigh = iGradient;
		return true;
	}

	template <class T, class U>
	bool SegmentationBase<T, U>::SetAutoAdjustConditions(const bool iFlag)
	{
		m_autoAdjustConditions = iFlag;
		return true;
	}

	template <class T, class U>
	bool SegmentationBase<T, U>::SetSeedMode(const eSegAddSeedCode iMode)
	{
		m_addSeedCode = iMode;
		return true;
	}

	template <class T, class U>
	bool SegmentationBase<T, U>::SetStdSpreadValue(const float iStdSpreadValue)
	{
		m_stdSpreadValue = iStdSpreadValue;
		return true;
	}

    template <class T, class U>
	bool SegmentationBase<T, U>::SetStdSpreadGradient(const float iStdSpreadGradient)
	{
		m_stdSpreadGradient = iStdSpreadGradient;
		return true;
	}

	// 0.0 to 1.0
	template <class T, class U>
	bool SegmentationBase<T, U>::SetWindow(const float& iWindow)
	{	
		if (iWindow<0.0f || iWindow >1.0f)
		{
			return false;
		}

		m_window = iWindow;
		return true;
	};

	template <class T, class U>
	bool SegmentationBase<T, U>::SetAutoDilationCount(const int& iCount)
	{
		m_autoDilationCount = iCount;
		return true;
	};

	template <class T, class U>
	SegmentationBase<T, U>::~SegmentationBase(void) {  };

	template <class T, class U>
	bool SegmentationBase<T, U>::transformSeedsVoxelToImg()
	{
		if (m_pIVolume->GetVolumeAccessor()==NULL)
		{
			return false;
		}

		// transform seeds into volume space
		
		

		//PGMath::AffineTransform<float> voxelToImage = m_pIVolume->GetTransformVoxelToImage();
		
		//const PGCore::MetaData< T >& oMetaData = m_pIVolume->GetMetaData();
		//bool feetFirst = oMetaiPointSet.GetAxialScanFeetFirstFlag();
		//bool axialScan = oMetaiPointSet.GetAxialScanFlag();

		for (int i=0; i<m_pSeeds.size(); i++)
		{
			PGMath::Point3D< float > seed = m_pSeeds[i];
			float ox=0, oy=0, oz=0;

			bool rv = m_pIVolume->GetStdToImgCoord(seed.X(), seed.Y(), seed.Z(), ox, oy, oz);
			if (!rv) return false;

			//voxelToImage.Apply(seed, m_pSeeds[i]);
			m_pSeeds[i] = PGMath::Point3D<float>(ox, oy, oz);

			GetLogger()->Log("Seed[%d]: [%f %f %f] -> [%f %f %f]", i, seed.X(), seed.Y(), seed.Z(), 
					m_pSeeds[i].X(), m_pSeeds[i].Y(), m_pSeeds[i].Z());

			// shouldnt have to do this
			/*if (feetFirst) 
			{
				m_pSeeds[i] = PGMath::Point3D<float>(m_pSeeds[i].X(), m_pSeeds[i].Y(), m_volumeDimensions.Z()-1-m_pSeeds[i].Z());
				GetLogger()->Log("\tFlippedZSeed[%d]: [%f %f %f]", i, m_pSeeds[i].X(), m_pSeeds[i].Y(), m_pSeeds[i].Z());
			}*/	
			
		}		
		
		return true;
	};

	template <class T, class U>
	bool SegmentationBase<T, U>::UndoLastStep()
	{
		if (m_undoCounter==1) // already done one undo
		{
			return false;
		}

		m_undoCounter++;

		bool rv = m_pIVolume->GetVolumeAccessor()->SubtractMask(1);

		rv |= this->PostExecute();		

		return rv;	
	}

	
	template <class T, class U>
	bool SegmentationBase<T, U>::RedoLastStep()
	{
		if (m_undoCounter==0) // nothing to redo
		{
			return false;
		}

		m_undoCounter--;

		bool rv = m_pIVolume->GetVolumeAccessor()->AddMask(1);

		rv |= this->PostExecute();		

		return rv;
	};

	// in image co-ordinates
	template <class T, class U>
	bool SegmentationBase<T, U>::setVoxel(const PGMath::Point3D<int> &iVoxel, const bool iValue)
	{
		//if (m_pIOVolume->GetVolumeAccessor()==NULL)
		{
		//	return false;
		}

		//GetLogger()->Log("Marking %d %d %d", iVoxel.X(), iVoxel.Y(), iVoxel.Z());		
		return m_pIVolume->GetVolumeAccessor()->GetBitVolume(1).SetValue(iVoxel.Y(), iVoxel.X(), iVoxel.Z(), iValue);						
	};


	template <class T, class U>
	bool SegmentationBase<T, U>::autoAdjustConditions(const PGMath::Point3D<int>& iSeed, const eSegRetCode iReason)
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

	// 0: out of range, 1: ok, -1: stack error
	//template <class T, class U>
	//eSegRetCode SegmentationBase<T, U>::visitVoxel(const PGMath::Point3D<int> &iVoxel)
	
	template <class T, class U>
	eSegRetCode SegmentationBase<T, U>::conditionCheck(const PGMath::Point3D<int> &iVoxel)
	{
		if (//m_count>10000 || 
			iVoxel.X() < 1 || iVoxel.X() > m_volumeDimensions.X()-2 ||
			iVoxel.Y() < 1 || iVoxel.Y() > m_volumeDimensions.Y()-2 ||
			iVoxel.Z() < 1 || iVoxel.Z() > m_volumeDimensions.Z()-2
			)
			return SegRetCodeOutOfRange;

		bool maskValue = m_pIVolume->GetVolumeAccessor()->GetBitVolume(1).GetValue(iVoxel.Y(), iVoxel.X(), iVoxel.Z());

		if (maskValue)
		{
			return SegRetCodeAlreadyMarked;
		}

		//T iValue = m_pIVolume->GetVolumeAccessor()->GetAvgValue(iVoxel.Y(), iVoxel.X(), iVoxel.Z());
		T iValue = m_pIVolume->GetVolumeAccessor()->GetValue(iVoxel.Y(), iVoxel.X(), iVoxel.Z());

		//GetLogger()->Log("visitVoxel[%d]: Considering %d %d %d: M:%d V: %d", m_count, iVoxel.X(), iVoxel.Y(), iVoxel.Z(),
		//	maskValue, iValue);

		//if (fabs((float)(iValue-m_seedValue)) > 10) 
		if (iValue<m_lowValue || iValue>m_highValue)
		{
			//setVoxel(iVoxel, 5);
			return SegRetCodeValue;
		}

		// Prashant C, 8/4/08: revisit this		
		if (0)
		{
			PGMath::Vector3D<float> gradientValue;
			m_pIVolume->GetVolumeAccessor()->GetGradient(iVoxel.Y(), iVoxel.X(), iVoxel.Z(), gradientValue);
			//if (gradientValue.X() > 50 || gradientValue.Y() > 50 || gradientValue.Z() > 50)
			float gVal = gradientValue.Length();
			if (gVal > m_gradientHigh)
			{
				//GetLogger()->Log("visitVoxel[%d]: rejecting %3.4f / 50", m_count, gVal);
				return SegRetCodeGradient;
			}
		}
		
		
		return SegRetCodeOk;
	}

	template <class T, class U>
	void SegmentationBase<T, U>::dumpSeedsOnImages(const std::vector<PGMath::Point3D< float > >& iSeeds)
	{
		int crossSize = 8;
		for (int i=0; i<iSeeds.size(); i++)
		{
			const PGMath::Point3D< float >& iPoint = iSeeds[i];
			PGCore::Image<T> iImage = m_pIVolume->GetVolumeAccessor()->GetImage(iPoint.Z());
			PGCore::PixelBase<T> tPixel;
			T minVal = tPixel.GetMaxValue(), maxVal = tPixel.GetMinValue();
			iImage.GetDataRange(&minVal, &maxVal);

			for (int j=-crossSize/2; j<=crossSize/2; j++)
			{
				if (j!=0) iImage.SetValue(int(iPoint.Y()+j+0.5f), int(iPoint.X()+0.5f), maxVal+100);
			}
			
			for (int i=-crossSize/2; i<=crossSize/2; i++)
			{
				if (i!=0) iImage.SetValue(int(iPoint.Y()+0.5f), int(iPoint.X()+i+0.5f), maxVal+100);
			}
			

			iImage.Update();
			
			char imgPath[512] = {0};
			_snprintf(imgPath, 511, "C:/Tmp/Dump/SeedImage_%03.2f_%03.2f_%03.2f.pgm", iPoint.X(), iPoint.Y(), iPoint.Z());
			imgPath[511] = '\0';	
	
			DumpImageAsPGM(iImage, imgPath);		

		}

	}

	template <class T, class U>
	bool SegmentationBase<T, U>::analyzeSeedPoint(const PGMath::Point3D<int>& iSeed, T& oMinValue, T& oMaxValue, 
		T& oMeanValue, float& oStdDev, float& oSNR)
	{
		PGCore::Image<T> iImage = m_pIVolume->GetVolumeAccessor()->GetImage(int(iSeed.Z()+0.5f));

		PGAlgs::ImageStatisticsFilter<T, U> statFilter;
		
		statFilter.SetInput(static_cast<PGCore::BaseDataObject *>(&iImage));
		statFilter.SetSeedPoint(PGMath::Point3D<int>(int(iSeed.X()+0.5f), 
									int(iSeed.Y()+0.5f), int(iSeed.Z()+0.5f)));
		statFilter.SetWindowSize(2);

		bool rv = statFilter.Execute();
		if (!rv) return false;

		statFilter.GetRange(oMinValue, oMaxValue);
		statFilter.GetMoments(oMeanValue, oStdDev, oSNR);
		
		return true;
	}

	template <class T, class U>
	void SegmentationBase<T, U>::commitIncrMask()
	{
		PGMath::Point3D<int> nextVoxel; 

		while (0)//m_incrStack.Size())
		{
			//m_incrStack.Pop(nextVoxel);
			setVoxel(nextVoxel, true);
		}
	}
	
	template <class T, class U>
	void SegmentationBase<T, U>::uncommitIncrMask()
	{
		PGMath::Point3D<int> nextVoxel; 

		while (0)//m_incrStack.Size())
		{
			//m_incrStack.Pop(nextVoxel);
			setVoxel(nextVoxel, false);
		}
	}

		
	template <class T, class U>
	eSegRetCode SegmentationBase<T, U>::visitVoxel(const PGMath::Point3D<int> &iVoxel)
	{			
		eSegRetCode	checkCode = conditionCheck(iVoxel);

		if (checkCode!=SegRetCodeOk)
		{
			return  checkCode;
		}
		
		
		setVoxel(iVoxel, true);
		//m_incrStack.Push(iVoxel);
		m_count++;
		//ProcessBase::StatusCallBackFn(100.0f*( (double)m_count/(double)m_totalCount ), std::string("Segmenting..."), 0, 0);
		
		bool rv = m_stack.Push(PGMath::Point3D<int>(iVoxel.X(), iVoxel.Y(), iVoxel.Z()+1));
		if (!rv) return SegRetCodeStackError;

		rv = m_stack.Push(PGMath::Point3D<int>(iVoxel.X(), iVoxel.Y(), iVoxel.Z()-1));
		if (!rv) return SegRetCodeStackError;

		rv = m_stack.Push(PGMath::Point3D<int>(iVoxel.X(), iVoxel.Y()+1, iVoxel.Z()));
		if (!rv) return SegRetCodeStackError;

		rv = m_stack.Push(PGMath::Point3D<int>(iVoxel.X(), iVoxel.Y()-1, iVoxel.Z()));
		if (!rv) return SegRetCodeStackError;

		rv = m_stack.Push(PGMath::Point3D<int>(iVoxel.X()+1, iVoxel.Y(), iVoxel.Z()));
		if (!rv) return SegRetCodeStackError;

		rv = m_stack.Push(PGMath::Point3D<int>(iVoxel.X()-1, iVoxel.Y(), iVoxel.Z()));
		if (!rv) return SegRetCodeStackError;
		

		return SegRetCodeOk;
	};


	// Taubin's method
	// http://people.cas.uab.edu/~mosya/cl/CircleFitByTaubin.cpp	
	template <class T, class U>
	bool SegmentationBase<T, U>::fitCircle (const PGMath::PointSet& iPointSet, PGCore::Circle& ioCircle)
	{	
		int i,iter,IterMAX=99;

		float Xi,Yi,Zi;
		float Mz,Mxy,Mxx,Myy,Mxz,Myz,Mzz,Cov_xy,Var_z;
		float A0,A1,A2,A22,A3,A33;
		float Dy,xnew,x,ynew,y;
		float DET,Xcenter,Ycenter;

		PGMath::Point3D<float> pMean = iPointSet.ComputeMeans();   // Compute x- and y- sample means (via a function in the class "data")

		//     computing moments 
		//             (note: all moments will be normed, i.e. divided by the number of points)

		Mxx=Myy=Mxy=Mxz=Myz=Mzz=0.;

		for (i=0; i<iPointSet.Size(); i++)
		{
			Xi = iPointSet.Element(i).X() - pMean.X();   //  centered x-coordinates
			Yi = iPointSet.Element(i).Y() - pMean.Y();   //  centered y-coordinates
			Zi = Xi*Xi + Yi*Yi;

			Mxy += Xi*Yi;
			Mxx += Xi*Xi;
			Myy += Yi*Yi;
			Mxz += Xi*Zi;
			Myz += Yi*Zi;
			Mzz += Zi*Zi;
		}
		Mxx /= iPointSet.Size();
		Myy /= iPointSet.Size();
		Mxy /= iPointSet.Size();
		Mxz /= iPointSet.Size();
		Myz /= iPointSet.Size();
		Mzz /= iPointSet.Size();

		//    computing the coefficients of the characteristic polynomial

		Mz = Mxx + Myy;
		Cov_xy = Mxx*Myy - Mxy*Mxy;
		Var_z = Mzz - Mz*Mz;
		A3 = 4*Mz;
		A2 = -3*Mz*Mz - Mzz;
		A1 = Var_z*Mz + 4*Cov_xy*Mz - Mxz*Mxz - Myz*Myz;
		A0 = Mxz*(Mxz*Myy - Myz*Mxy) + Myz*(Myz*Mxx - Mxz*Mxy) - Var_z*Cov_xy;
		A22 = A2 + A2;
		A33 = A3 + A3 + A3;

		//    finding the root of the characteristic polynomial
		//    using Newton's method starting at x=0  
		//     (it is guaranteed to converge to the right root)

		for (x=0.,y=A0,iter=0; iter<IterMAX; iter++)  // usually, 4-6 iterations are enough
		{
			Dy = A1 + x*(A22 + A33*x);
			if (Dy<=kPgFineEpsilon) break;

			xnew = x - y/Dy;

			if ((xnew == x)) break;

			ynew = A0 + xnew*(A1 + xnew*(A2 + xnew*A3));
			if (abs(ynew)>=abs(y))  break;

			x = xnew;  y = ynew;
		}

		//    computing the circle parameters

		DET = x*x - x*Mz + Cov_xy;
		Xcenter = (Mxz*(Myy - x) - Myz*Mxy)/DET/2.0;
		Ycenter = (Myz*(Mxx - x) - Mxz*Mxy)/DET/2.0;

		float circlea = Xcenter + pMean.X();
		float circleb = Ycenter + pMean.Y();
		float circler = sqrt(Xcenter*Xcenter + Ycenter*Ycenter + Mz);
		float circles = computeSigma(iPointSet, ioCircle);
		float circlei = 0;
		float circlej = iter;  //  return the number of iterations, too

		ioCircle = Circle(circlea, circleb, circler);//, circles, circlei, circlej);

		return true;
	}

	//****************** Sigma ************************************
	//
	//   estimate of Sigma = square root of RSS divided by N
	template <class T, class U>
	float SegmentationBase<T, U>::computeSigma(const PGMath::PointSet& iPointSet, PGCore::Circle& ioCircle)
	{
		if (iPointSet.Size()<=0) return 0.0f;

		float sum = 0.,dx,dy;

		for (int i=0; i<iPointSet.Size(); i++)
		{
			dx = iPointSet.Element(i).X() - ioCircle.a();
			dy = iPointSet.Element(i).Y() - ioCircle.b();
			sum += sqrt(sqrt(dx*dx+dy*dy) - ioCircle.r());
		}

		return sqrt(sum/iPointSet.Size());
	}


		
#ifdef _PG_GENERATE_SDK_LIBS_
	template class SegmentationBase<short, short>; 
	template class SegmentationBase<unsigned char, unsigned char>;
#endif



};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGSegmentationBASE_HPP_
