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
// Filename:	VoxelToMPRRenderer.cpp
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//
//////////////////////////////////////////////////////////////////////////


#include "Algs/VoxelToMPRRenderer.h"

namespace PGAlgs
{
	using namespace std;
	using namespace PGCore;	


	template <class T, class U>
	VoxelToMPRRenderer<T, U>::VoxelToMPRRenderer(void) 
	{

		m_oImageObject = 0;
		m_pSlicerPosition = 0;
		m_pSlicerPositionLast=-100;
		m_inVolume.clear();
		m_outBuf = 0;
		mLuT.clear();
		mMaskLuT.clear();
		m_LuTBuf.clear();
		m_MaskLuTBuf.clear();
		m_blendWeight.clear();
		m_invRows = 1.0f;
		m_invColumns = 1.0f;
		m_start = -0.5f;
		m_end = 0.5f;
		m_skipFactor = 1;
		m_defaultLUT = PGCore::TransferFunctionLUT<U>(kPgLUTTypeGrayScale);
		SetTransferFunctionLUT(&m_defaultLUT);
		SetSlicingDirection(kPgSlicingDirectionCoronal);
	};


	template <class T, class U>
	bool VoxelToMPRRenderer<T, U>::UpdateCB(void	*iDataObject)
	{
		bool res = false;
		int successCount = 0;
		if (iDataObject)
		{
			std::vector< float > *iVector = (static_cast<std::vector<float> *>(iDataObject));

			PGMath::DegreesOfFreedomBase<float> activeDoF(*iVector); 

			PGMath::AffineTransform<float> transformResult(PGMath::Point3D<float>(0.0f, 0.0f, 0.0f), 
				activeDoF);

			for (int i=0; i<m_inVolume.size(); i++)
			{
				m_inVolume[i]->SetRegistrationTransform(transformResult);
				successCount++;
			}

		}

		return (successCount==m_inVolume.size());
	}



	template <class T, class U>
	const ePgSlicingDirection  VoxelToMPRRenderer<T, U>::GetSlicingDirection() const
	{
		return m_slicingDirection;
	}

	template <class T, class U>
	bool VoxelToMPRRenderer<T, U>::SetSkipFactor(int iSkipFactor)
	{
		m_skipFactor = iSkipFactor;
		return true;
	}

	template <class T, class U>
	int VoxelToMPRRenderer<T, U>::GetSkipFactor() const 
	{
		return m_skipFactor;
	}


	template <class T, class U>
	const PGMath::AffineTransform<float>&VoxelToMPRRenderer<T, U>::GetViewTransform() const {  return m_pTransformStdReslice; }


	template <class T, class U>
	bool VoxelToMPRRenderer<T, U>::SetViewTransform(const PGMath::AffineTransform<float>&	iTransform) 
	{ 
		m_pTransformStdReslice = iTransform; 

		bool res = false;
		for (int i=0; i<m_inVolume.size(); i++)
		{
			if (m_inVolume[i]!=NULL)
			{
				m_inVolume[i]->SetViewTransform(m_pTransformStdReslice);
				res = true;			
			}
		}

		return res; 
	}



	template <class T, class U>
	bool VoxelToMPRRenderer<T, U>::SetSlicingDirection(ePgSlicingDirection iDirection)
	{
		m_slicingDirection = iDirection;		
		updateSlicingTransform();

		bool res = false;
		for (int i=0; i<m_inVolume.size(); i++)
		{
			m_inVolume[i]->SetViewTransform(m_pTransformStdReslice);
			res = true;			
		}

		return res;
	}


	template <class T, class U>
	bool VoxelToMPRRenderer<T, U>::SetSlicerPosition(double iPosition)
	{
		m_pSlicerPosition = iPosition;
		return true;
	}


	template <class T, class U>
	bool VoxelToMPRRenderer<T, U>::SetBlendWeight(const float iWeight, const int iRendererIndex=0) 
	{ 
		if (iRendererIndex<0 || iRendererIndex>m_blendWeight.size()-1)
			return false;
		m_blendWeight[iRendererIndex] = iWeight;		
		return true;
	}

	template <class T, class U>
	float VoxelToMPRRenderer<T, U>::GetBlendWeight(const int iRendererIndex) const 
	{ 
		if (iRendererIndex<0 || iRendererIndex>m_blendWeight.size()-1)
			return false;
		return m_blendWeight[iRendererIndex];	
	}

	template <class T, class U>
	bool VoxelToMPRRenderer<T, U>::SetTransferFunctionLUT(PGCore::TransferFunctionLUT<U> *iLUT, const int iVolumeIndex=-1)
	{
		if (!iLUT || m_inVolume.empty() 
			|| m_inVolume.size()!=mLuT.size() || m_LuTBuf.size()!=mLuT.size())
			return false;

		int indexBegin= (iVolumeIndex==-1) ? 0 : iVolumeIndex;
		int indexEnd= (iVolumeIndex==-1) ?  m_inVolume.size()-1: iVolumeIndex;
		int okCount=0;

#define _USE_3D_LUT
#ifdef _USE_3D_LUT
		for (int i=indexBegin; i<=indexEnd; i++)
		{
			mLuT[i] = iLUT;
			m_LuTBuf[i] = mLuT[i]->GetBuffer();
			okCount+= (m_LuTBuf[i]!=0);
		}

#else
		(okCount=(indexEnd-indexBegin+1));
#endif
		return (okCount==(indexEnd-indexBegin+1));
	}

	template <class T, class U>
	bool VoxelToMPRRenderer<T, U>::SetMaskTransferFunctionLUT(PGCore::TransferFunctionLUT<U> *iLUT, const int iVolumeIndex=-1)
	{
		if (!iLUT || m_inVolume.empty() 
			|| m_inVolume.size()!=mMaskLuT.size() || m_MaskLuTBuf.size()!=mMaskLuT.size())
			return false;

		int indexBegin= (iVolumeIndex==-1) ? 0 : iVolumeIndex;
		int indexEnd= (iVolumeIndex==-1) ?  m_inVolume.size()-1: iVolumeIndex;
		int okCount=0;

#define _USE_3D_LUT
#ifdef _USE_3D_LUT
		for (int i=indexBegin; i<=indexEnd; i++)
		{
			//use different vector for mask LUT
			mMaskLuT[i] = iLUT;
			m_MaskLuTBuf[i] = mMaskLuT[i]->GetBuffer();
			okCount+= (m_LuTBuf[i]!=0);
		}

#else
		(okCount=(indexEnd-indexBegin+1));
#endif
		return (okCount==(indexEnd-indexBegin+1));
	}

	template <class T, class U>
	void VoxelToMPRRenderer<T, U>::Clear() 
	{ 
		if (m_oImageObject) 
		{ 
			delete m_oImageObject; 
			m_oImageObject=0; 
		}
		for (int i=0; i<m_inVolume.size(); i++)
			m_inVolume[i] = 0;
		m_inVolume.clear();
		m_blendWeight.clear();
		mLuT.clear();
		mMaskLuT.clear();
		m_LuTBuf.clear();
		m_MaskLuTBuf.clear();
	}

	template <class T, class U>
	VoxelToMPRRenderer<T, U>::~VoxelToMPRRenderer(void)
	{ 
		Clear();  
	};

	template <class T, class U>
	bool VoxelToMPRRenderer<T, U>::ImageSizeUpdated()
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
	bool VoxelToMPRRenderer<T, U>::PrepareRenderer() 
	{
		//Clear();
		if (!m_imageSizeChanged && m_pSlicerPosition==m_pSlicerPositionLast)
		{
			return true;
		}

		if (!m_iDataObject || (m_rows*m_columns)<=0) {
			LOG0("VoxelToMPRRenderer: Error: no input");
			return false;
		}		

		m_invRows=1.0f/(m_rows-1);
		m_invColumns = 1.0f/(m_columns-1);

		PGCore::Voxel3D < T > *inVolume = (dynamic_cast<PGCore::Voxel3D < T > *>(m_iDataObject));
		if (!inVolume) {
			LOG0("VoxelToMPRRenderer:initVolumes Error: failure to fetch input volume");
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
				LOG0("VoxelToMPRRenderer: Error: empty input volume");
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
			LOG0("VoxelToMPRRenderer: Error: invalid out image");
			return false;
		}

		//LOG0("{ VoxelToMPRRenderer: Begin render.");
		// fetches a slice at z=0.0, x and y as in dicom space	
		//long imgIter = 0;	
		//int maxDim = max(m_rows, m_columns);

		if (m_imageSizeChanged) m_imageSizeChanged= false;

		return true;
	}


	template <class T, class U>
	bool VoxelToMPRRenderer<T, U>::Render()
	{	
		bool maskInit = m_inVolume[0]->GetVolumeAccessor()->GetBitVolume().IsInitialized();
		return maskInit ? renderWithMask() : renderWithoutMask();
	}


	template <class T, class U>
	bool VoxelToMPRRenderer<T, U>::Execute() 
	{	

		if (!PrepareRenderer())
		{
			LOG0("VoxelToMPRRenderer: Error: PrepareRender failed");
			return false;
		}

		return Render();
	}

	template <class T, class U>
	bool VoxelToMPRRenderer<T, U>::renderWithMask()
	{
		//LOG0("{ VoxelToMPRRenderer: Begin renderWithMask.");
		//if (!m_outBuf) return false;

		//int iter=0;

		//LOG0("\t{ VoxelToMPRRenderer: Begin render loops.");
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
					PGMath::Point3D<U> iVal(0);
					//make a function ptr based on the status of mask
					//add progress

					int inMaskVal = 0;
					//float blendVal = 1.0f;
					{		
						inMaskVal		=	m_inVolume[i]->GetMaskValue(c, r, m_pSlicerPosition);
						if (inMaskVal)
							iVal = m_MaskLuTBuf[i][inVal];						
						else
							iVal = m_LuTBuf[i][inVal];						

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

		//LOG0("\t} VoxelToMPRRenderer: End render loops.");


		m_pSlicerPositionLast = m_pSlicerPosition;

		//LOG0("\t{ VoxelToMPRRenderer: Begin result assignment.");
		m_oDataObject = (dynamic_cast<PGCore::BaseDataObject *>(m_oImageObject));
		//LOG0("\t} VoxelToMPRRenderer: End result assignment.");

		//LOG0("} VoxelToMPRRenderer: End renderWithMask.");

		return true;

	}


	template <class T, class U>
	bool VoxelToMPRRenderer<T, U>::renderWithoutMask()
	{
		//LOG1("{ VoxelToMPRRenderer: Begin renderWithoutMask Skip (%d).", m_skipFactor);
		//if (!m_outBuf) return false;

		//int iter=0;

		//LOG0("\t{ VoxelToMPRRenderer: Begin render loops.");
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

		//LOG0("\t} VoxelToMPRRenderer: End render loops.");


		m_pSlicerPositionLast = m_pSlicerPosition;

		//LOG0("\t{ VoxelToMPRRenderer: Begin result assignment.");
		m_oDataObject = (dynamic_cast<PGCore::BaseDataObject *>(m_oImageObject));
		//LOG0("\t} VoxelToMPRRenderer: End result assignment.");

		//LOG2("} VoxelToMPRRenderer: End renderWithoutMask written until %d / %d.", oBufOffset, oBufOffsetLimit);

		return true;


	}


	template <class T, class U>
	bool VoxelToMPRRenderer<T, U>::updateSlicingTransform()
	{

		//the trouble is here: this should only perform rotations and no scaling or inversion of axes

		//optimize this!

		PGMath::DegreesOfFreedomBase<float> resliceDoF;

		//fix here...then test with sagittal MR too

		switch(m_slicingDirection)
		{
		case kPgSlicingDirectionCoronal:
			{				
				resliceDoF.SetParameter(PGMath::kPgDegreeOfFreedomRotationX, 90.0f); 			
			}
			break;

		case kPgSlicingDirectionSagital:
			{	
				resliceDoF.SetParameter(PGMath::kPgDegreeOfFreedomScalingX, -1.0f);
				resliceDoF.SetParameter(PGMath::kPgDegreeOfFreedomRotationY, 90.0f); 		
				resliceDoF.SetParameter(PGMath::kPgDegreeOfFreedomRotationZ, -90.0f); 		

				//resliceDoF.SetParameter(PGMath::kPgDegreeOfFreedomRotationY, 90.0f); 
				//resliceDoF.SetParameter(PGMath::kPgDegreeOfFreedomRotationX, 90.0f); 				
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

#ifdef _PG_GENERATE_SDK_LIBS_
	template class VoxelToMPRRenderer<PG_RENDERING_IN_TYPE, PG_RENDERING_OUT_TYPE>; 		
#endif

};


//////////////////////////////////////////////////////////////////////////
// EOF

