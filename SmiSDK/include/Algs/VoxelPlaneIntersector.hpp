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
// Filename:	VoxelPlaneIntersector->hpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//E

#ifndef _PGVoxelPlaneIntersector_HPP_
#define _PGVoxelPlaneIntersector_HPP_
//////////////////////////////////////////////////////////////////////////


#include "VoxelPlaneIntersector.h"

namespace PGAlgs
{
	using namespace std;
	using namespace PGCore;	

	template <class T, class U>
	bool VoxelPlaneIntersector<T, U>::ImageSizeUpdated()
	{
		// reallocate img buffer here
		// ok query 		
			m_invRows=1.0f/(m_rows-1);
			m_invColumns = 1.0f/(m_columns-1);

			if (m_oImageObject)
			{
				delete m_oImageObject;
				m_oImageObject = 0;
			}

			

			{
				
				m_oImageObject = new PGCore::Image<PGMath::Point3D<U>>(m_rows, m_columns);
				if (!m_oImageObject) return false;

				m_outBuf = m_oImageObject->GetBuffer();	

				if (!m_outBuf) return false;

			}


			return true;
	}


	template <class T, class U>
	bool VoxelPlaneIntersector<T, U>::PrepareRenderer() 
	{
		//Clear();
		if (!m_imageSizeChanged && m_pSlicerPosition==m_pSlicerPositionLast)
		{
			return true;
		}

		if (!m_iDataObject || (m_rows*m_columns)<=0) {
			LOG0("VoxelPlaneIntersector: Error: no input");
			return false;
		}		

		m_invRows=1.0f/(m_rows-1);
		m_invColumns = 1.0f/(m_columns-1);

		PGCore::Voxel3D < T > *inVolume = (dynamic_cast<PGCore::Voxel3D < T > *>(m_iDataObject));
		if (!inVolume) {
			LOG0("VoxelPlaneIntersector:initVolumes Error: failure to fetch input volume");
			return false;
		}

		int iMultiVolumeCount = 1;		

		// get the input volume		
		//if (!m_imageSizeChanged)
		if (m_inVolume.size() != iMultiVolumeCount)
		{
			mLuT.resize(iMultiVolumeCount);
			mMaskLuT.resize(iMultiVolumeCount);
			m_LuTBuf.resize(iMultiVolumeCount);// = 0;//mLuT.GetBuffer();
			m_MaskLuTBuf.resize(iMultiVolumeCount);// = 0;//mLuT.GetBuffer();
			m_blendWeight.resize(iMultiVolumeCount);

			PGMath::Point3D<float> mmDims;
			mmDims = inVolume->GetDimensions();
			if (mmDims.Z()<=0) {
				LOG0("VoxelPlaneIntersector: Error: empty input volume");
				return false;
			}	

			m_inVolume.push_back(inVolume);

			mLuT[0] = 0;//.resize(iMultiVolumeCount);
			mMaskLuT[0] = 0;
			m_LuTBuf[0] = 0;//.resize(iMultiVolumeCount);// = 0;//mLuT.GetBuffer();
			m_MaskLuTBuf[0] = 0;//.resize(iMultiVolumeCount);// = 0;//mLuT.GetBuffer();
			m_blendWeight[0] = 1.0f/(float)(iMultiVolumeCount);
			SetTransferFunctionLUT(&m_defaultLUT);
		}	
		

		// ok query 
		//int iRows = (int)(mmDims.Y()), iColumns = (int)(mmDims.X());

		if (m_oImageObject==0)
		{
			m_oImageObject = new PGCore::Image<PGMath::Point3D<U>>(m_rows, m_columns);
		}
		m_outBuf = m_oImageObject->GetBuffer();
		if (!m_outBuf) 
		{
			LOG0("VoxelPlaneIntersector: Error: invalid out image");
			return false;
		}

		//LOG0("{ VoxelPlaneIntersector: Begin render.");
		// fetches a slice at z=0.0, x and y as in dicom space	
		//long imgIter = 0;	
		//int maxDim = max(m_rows, m_columns);

		if (m_imageSizeChanged) m_imageSizeChanged= false;

		return true;
	}


	template <class T, class U>
	bool VoxelPlaneIntersector<T, U>::Render()
	{	
		bool maskInit = m_inVolume[0]->GetVolumeAccessor()->GetBitVolume().IsInitialized();
		return maskInit ? renderWithMask() : renderWithoutMask();
	}


	template <class T, class U>
	bool VoxelPlaneIntersector<T, U>::Execute() 
	{	

		if (!PrepareRenderer())
		{
			LOG0("VoxelPlaneIntersector: Error: PrepareRender failed");
			return false;
		}

		return Render();
	}

	template <class T, class U>
	bool VoxelPlaneIntersector<T, U>::renderWithMask()
	{
		//LOG0("{ VoxelPlaneIntersector: Begin renderWithMask.");
		//if (!m_outBuf) return false;

		//int iter=0;

		//LOG0("\t{ VoxelPlaneIntersector: Begin render loops.");
		//U *lBuf = mLuT.GetBuffer();
		PGMath::Point3D<U>* oBuf = m_outBuf;
		//assert(oBuf);
		//assert(m_LuTBuf);
		//PGCore::PixelBase<Point3D<char> > tPixel;

		//memset(oBuf, 0, sizeof(U)*m_rows*m_columns);
		PixelBase<U> uPixel;
		U uMaxVal = uPixel.GetMaxValue();

		PGMath::Point3D<U> bValue=PGMath::Point3D<U>(0, 0, 0);
		float r=0, c=0, wt=1.0f/(float)mLuT.size();
		int i=0;
		//int offset=0, ri=0, ci=0;
		//int riBegin=(int)(m_start+0.5f)*(float)(m_rows), ciBegin=(int)(m_start+0.5f)*(float)(m_columns);
		//for (r=m_start, ri=riBegin; r<m_end; r+=m_invRows, ri++)
		
		// bound check can be avoided?
		
		//int m_skipFactor = 8;
		long oBufOffset = 0;

		//errrrrrrrrrrrrrrrr
		float rStep=m_invRows*m_skipFactor, cStep=m_invColumns*m_skipFactor;
		//float m_startRows = (m_rows%2) ? m_start : m_start+rStep;
		//float m_startCols = (m_columns%2) ? m_start : m_start+cStep;

		int rIndex=0, cIndex=0;
		

#define _TEST_INTERP 0
		//for (r=m_startRows; r<=(m_end); r+=rStep)
		//for (r=m_start, rIndex=0; r<(m_end) && rIndex<m_rows; r+=rStep)//m_invRows*m_skipFactor)
		for (r=m_start, rIndex=0; r<=(m_end) && rIndex<m_rows; r+=rStep)//m_invRows*m_skipFactor)
		{		 	
			//for (c=m_startCols; c<=(m_end); c+=cStep)				
			//for (c=m_start, cIndex=0; c<(m_end) && cIndex<m_columns; c+=cStep)//m_invColumns*m_skipFactor)				
			for (c=m_start, cIndex=0; c<=(m_end) && cIndex<m_columns; c+=cStep)//m_invColumns*m_skipFactor)				
			{	
				//0.005882382
				//if (r == ((126.0f)/255.0f - 0.5f) && c == ((126.0f)/255.0f - 0.5f))
				//if (fabs(r) - 1.5f*rStep < 0.001f && fabs(c) - 2.5f*cStep < 0.001f) // 130, 126
				/*if (fabs(r) - 2.5f*rStep < 0.001f && fabs(c) - 2.5f*cStep < 0.001f) // 130, 125
				{
					float curPosR = (r+0.5f)*255.0f;
					float curPosC = (c+0.5f)*255.0f;
					printf("Found one!");
				}*/

				bValue=U(0);
				for (i=0; i<mLuT.size(); i++)
				{
					T	inVal			=	m_inVolume[i]->GetValue(c, r, m_pSlicerPosition);
					

#if (_TEST_INTERP==0)
					PGMath::Point3D<U> iVal(0), iMaskVal(0);
					//make a function ptr based on the status of mask
					//add progress
					
					int inMaskVal = 0;
					//float blendVal = 1.0f;
					{	
						iVal = m_LuTBuf[i][inVal];						

						inMaskVal		=	m_inVolume[i]->GetMaskValue(c, r, m_pSlicerPosition);
						if (inMaskVal)
						{
							iMaskVal = m_MaskLuTBuf[i][inVal];	
							iVal = iMaskVal*m_blendWeight[i] + iVal*(1.0f-m_blendWeight[i]);
						}
						
						

						//blendVal = inMaskVal ? (1.0f-m_blendWeight[0]) : m_blendWeight[0];
					} 
	
					bValue = bValue + PGMath::Point3D<U>(iVal.X(), iVal.Y(), iVal.Z(), uMaxVal);//unsigned char(255.0f*blendVal+0.5f));
#else					
					bValue = bValue + PGMath::Point3D<U>(inVal, inVal, inVal, 255);//iVal.W());//.0f*m_blendWeight[i]);//iVal.W()*m_blendWeight[i]);
#endif
					//bValue = bValue + PGMath::Point3D<U>(m_inVolume[i]->GetValue(c, r, m_pSlicerPosition));//*m_blendWeight[i];
					//*(oBuf++) = m_LuTBuf[i][m_inVolume[i]->GetValue(c, r, m_pSlicerPosition)];				
				}
				for (int k=0; k<m_skipFactor && cIndex<=(m_columns-1); k++)
				{
					*(oBuf+oBufOffset) = bValue;//U(bValue.X(), bValue.Y(), bValue.Z(), uMaxVal);
					oBufOffset++;
					cIndex++;
				}
			}
			rIndex++;
			// copy m_skipFactor-1 rows
			for (int k=0; k<m_skipFactor-1 && rIndex<=(m_rows-1); k++)
			{
				memcpy(oBuf+oBufOffset, oBuf+(oBufOffset-m_columns), m_columns*sizeof(PGMath::Point3D<U>));
				oBufOffset += (m_columns);//*(m_skipFactor-1);
				rIndex++;
			}
		}			

		//LOG0("\t} VoxelPlaneIntersector: End render loops.");
		

		m_pSlicerPositionLast = m_pSlicerPosition;

		//LOG0("\t{ VoxelPlaneIntersector: Begin result assignment.");
		m_oDataObject = (dynamic_cast<PGCore::BaseDataObject *>(m_oImageObject));
		//LOG0("\t} VoxelPlaneIntersector: End result assignment.");

		//LOG0("} VoxelPlaneIntersector: End renderWithMask.");

		return true;

	}


	template <class T, class U>
	bool VoxelPlaneIntersector<T, U>::renderWithoutMask()
	{
		//LOG1("{ VoxelPlaneIntersector: Begin renderWithoutMask Skip (%d).", m_skipFactor);
		//if (!m_outBuf) return false;

		//int iter=0;

		//LOG0("\t{ VoxelPlaneIntersector: Begin render loops.");
		//U *lBuf = mLuT.GetBuffer();
		PGMath::Point3D<U>* oBuf = m_outBuf;
		//assert(oBuf);
		//assert(m_LuTBuf);
		//PGCore::PixelBase<Point3D<char> > tPixel;

		//memset(oBuf, 0, sizeof(U)*m_rows*m_columns);
		PixelBase<U> uPixel;
		U uMaxVal = uPixel.GetMaxValue();

		PGMath::Point3D<U> bValue(0, 0, 0);
		float r=0, c=0, wt=1.0f/(float)mLuT.size();
		int i=0;
		//int offset=0, ri=0, ci=0;
		//int riBegin=(int)(m_start+0.5f)*(float)(m_rows), ciBegin=(int)(m_start+0.5f)*(float)(m_columns);
		//for (r=m_start, ri=riBegin; r<m_end; r+=m_invRows, ri++)
		
		// bound check can be avoided?
		
		//int m_skipFactor = 8;
		long oBufOffset = 0, oBufOffsetLimit = m_rows*m_columns - 1;

		//errrrrrrrrrrrrrrrr
		float rStep=m_invRows*m_skipFactor, cStep=m_invColumns*m_skipFactor;
		//float m_startRows = (m_rows%2) ? m_start : m_start+rStep;
		//float m_startCols = (m_columns%2) ? m_start : m_start+cStep;

		int rIndex=0, cIndex=0;
		
		if (0)
		{
			for (int i=0; i<m_rows; i++)
			{
				for (int j=0; j<m_columns; j++)	
				{
					PGMath::Point3D<U> iVal = m_LuTBuf[0][i];
					PGMath::Point3D<U> bValue(iVal.X(), iVal.Y(), iVal.Z(), uMaxVal);
					*(oBuf+i*m_columns + j) = bValue;
				}
			}

			//m_oDataObject = (dynamic_cast<PGCore::BaseDataObject *>(m_oImageObject));

			return true;
		}
		

#define _TEST_INTERP 0
		for (r=m_start, rIndex=0; r<=(m_end) && rIndex<m_rows && oBufOffset<oBufOffsetLimit; r+=rStep)
		//for (r=m_end, rIndex=0; r>=(m_start) && rIndex>-1 && oBufOffset<oBufOffsetLimit; r-=rStep)
		{		
			
			for (c=m_start, cIndex=0; c<=(m_end) && cIndex<m_columns && oBufOffset<oBufOffsetLimit; c+=cStep)
			{	
				bValue=U(0);
				for (i=0; i<mLuT.size(); i++)
				{
					T	inVal			=	m_inVolume[i]->GetValue(c, r, m_pSlicerPosition);
					

#if (_TEST_INTERP==0)
					PGMath::Point3D<U> iVal(0, 0, 0);
					//make a function ptr based on the status of mask
					//add progress
					
					iVal = m_LuTBuf[i][inVal];						
					

					bValue = bValue + PGMath::Point3D<U>(iVal.X(), iVal.Y(), iVal.Z(), uMaxVal);
#else					
					bValue = bValue + PGMath::Point3D<U>(inVal, inVal, inVal, uMaxVal);//iVal.W());//.0f*m_blendWeight[i]);//iVal.W()*m_blendWeight[i]);
#endif
					//bValue = bValue + PGMath::Point3D<U>(m_inVolume[i]->GetValue(c, r, m_pSlicerPosition));//*m_blendWeight[i];
					//*(oBuf++) = m_LuTBuf[i][m_inVolume[i]->GetValue(c, r, m_pSlicerPosition)];				
				}
				for (int k=0; k<m_skipFactor && cIndex<=(m_columns-1); k++)
				{
					*(oBuf+oBufOffset) = bValue;//U(bValue.X(), bValue.Y(), bValue.Z(), uMaxVal);
					oBufOffset++;
					
					if (oBufOffset>oBufOffsetLimit) 
					{
						//LOG2("\t(0)Image offset is off bounds: %d > %d", oBufOffset, oBufOffsetLimit);
						break;
					}
					cIndex++;
				}
			}
			rIndex++;
			// copy m_skipFactor-1 rows
			for (int k=0; k<m_skipFactor-1 && rIndex<=(m_rows-1); k++)
			{
				memcpy(oBuf+oBufOffset, oBuf+(oBufOffset-m_columns), m_columns*sizeof(PGMath::Point3D<U>));
				oBufOffset += (m_columns);//*(m_skipFactor-1);
				if (oBufOffset>oBufOffsetLimit) 
				{
					//LOG2("\t(1)Image offset is off bounds: %d > %d", oBufOffset, oBufOffsetLimit);
					break;
				}
				rIndex++;
			}
		}			

		//LOG0("\t} VoxelPlaneIntersector: End render loops.");
		

		m_pSlicerPositionLast = m_pSlicerPosition;

		//LOG0("\t{ VoxelPlaneIntersector: Begin result assignment.");
		m_oDataObject = (dynamic_cast<PGCore::BaseDataObject *>(m_oImageObject));
		//LOG0("\t} VoxelPlaneIntersector: End result assignment.");

		//LOG2("} VoxelPlaneIntersector: End renderWithoutMask written until %d / %d.", oBufOffset, oBufOffsetLimit);

		return true;


	}


	template <class T, class U>
	bool VoxelPlaneIntersector<T, U>::updateSlicingTransform()
	{

		PGMath::DegreesOfFreedomBase<float> resliceDoF;

		switch(m_slicingDirection)
		{
		case kPgSlicingDirectionCoronal:
			{				
				resliceDoF.SetParameter(PGMath::kPgDegreeOfFreedomRotationX, 90.0f); // was 90							
			}
			break;

		case kPgSlicingDirectionSagital:
			{	
				resliceDoF.SetParameter(PGMath::kPgDegreeOfFreedomScalingX, -1.0f);
				resliceDoF.SetParameter(PGMath::kPgDegreeOfFreedomRotationY, 90.0f); //was 90
				resliceDoF.SetParameter(PGMath::kPgDegreeOfFreedomRotationZ, -90.0f);				
			}
			break;

		case kPgSlicingDirectionTransverse:			
			{					
			}
			break;

		default:
			return false;
		}	

		m_pTransformStdReslice = PGMath::AffineTransform<float>(		
			PGMath::Point3D<float>(0.0f, 0.0f, 0.0f), 
			resliceDoF);

		return true;
	}

};


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGVoxelPlaneIntersector_HPP_
