// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
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
// Filename:	VoxelPlaneIntersector.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGVoxelPlaneIntersector_H_
#define _PGVoxelPlaneIntersector_H_
//////////////////////////////////////////////////////////////////////////


#include "RendererBase.h"
#include "../Core/Volume.h"
#include "../Core/Voxel3D.h"
#include "../Math/AffineTransform.h"
#include "../Core/TransferFunctionLUT.h"

namespace PGAlgs
{
	using namespace std;
	using namespace PGCore;

	typedef enum {
		kPgSlicingDirectionUnknown = -1,
		kPgSlicingDirectionCoronal = 0,
		kPgSlicingDirectionSagital = 1,
		kPgSlicingDirectionTransverse = 2
	} ePgSlicingDirection;

	template <class T, class U>
	class VoxelPlaneIntersector : public RendererBase<T, U>
	{
	public:
		VoxelPlaneIntersector(void) 
		{

			m_oImageObject = 0;
			m_pSlicerPosition = 0;
			m_pSlicerPositionLast=-100;
			m_inVolume.clear();
			m_outBuf = 0;
			mLuT.clear();
			mMaskLuT.clear();
			m_LuTBuf.clear();
			m_MaskLuTBuf.clear();//.resize(iMultiVolumeCount);// = 0;//mLuT.GetBuffer();
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

		virtual bool UpdateCB(void	*iDataObject)
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

		const ePgSlicingDirection GetSlicingDirection() const
		{
			return m_slicingDirection;
		}

		bool SetSkipFactor(int iSkipFactor)
		{
			m_skipFactor = iSkipFactor;
			return true;
		}

		int GetSkipFactor() const 
		{
			return m_skipFactor;
		}

		const PGMath::AffineTransform<float>&	GetViewTransform() const {  return m_pTransformStdReslice; }
		
		bool SetViewTransform(const PGMath::AffineTransform<float>&	iTransform) 
		{ 
			m_pTransformStdReslice = iTransform; 
			
			bool res = false;
			for (int i=0; i<m_inVolume.size(); i++)
			{
				m_inVolume[i]->SetViewTransform(m_pTransformStdReslice);
				res = true;			
			}

			return res; 
		}


		bool SetSlicingDirection(ePgSlicingDirection iDirection)
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

		bool SetSlicerPosition(double iPosition)
		{
			m_pSlicerPosition = iPosition;
			return true;
		}

		bool SetBlendWeight(const float iWeight, const int iRendererIndex=0) 
		{ 
			if (iRendererIndex<0 || iRendererIndex>m_blendWeight.size()-1)
				return false;
			m_blendWeight[iRendererIndex] = iWeight;		
			return true;
		}

		float GetBlendWeight(const int iRendererIndex) const 
		{ 
			if (iRendererIndex<0 || iRendererIndex>m_blendWeight.size()-1)
				return false;
			return m_blendWeight[iRendererIndex];	
		}

		bool SetTransferFunctionLUT(PGCore::TransferFunctionLUT<U> *iLUT, const int iVolumeIndex=-1)
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

		bool SetMaskTransferFunctionLUT(PGCore::TransferFunctionLUT<U> *iLUT, const int iVolumeIndex=-1)
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

		bool Execute();

		virtual bool PrepareRenderer();

		bool Render();

		void Clear() 
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

		virtual ~VoxelPlaneIntersector(void) { Clear();  };


	protected:


		virtual bool ImageSizeUpdated();

	private:
		std::vector<float>				m_blendWeight;
		std::vector<PGCore::Voxel3D<T>*> m_inVolume;
		
		PGCore::Image<PGMath::Point3D<U>> *m_oImageObject;	
		PGMath::Point3D<U>				*m_outBuf;
		ePgSlicingDirection				m_slicingDirection;
		PGMath::AffineTransform<float>	m_pTransformStdReslice;
		PGCore::TransferFunctionLUT<U>  m_defaultLUT;
		std::vector<PGCore::TransferFunctionLUT<U> *> mLuT, mMaskLuT;
		std::vector<Point3D<U> *>		m_LuTBuf, m_MaskLuTBuf;

		double							m_pSlicerPosition, m_pSlicerPositionLast;
		int								m_skipFactor;
		float							m_invRows, m_invColumns;
		float							m_start, m_end;
		bool							updateSlicingTransform();
		bool							renderWithMask();
		bool							renderWithoutMask();
	};

};

#include "VoxelPlaneIntersector.hpp"

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGVoxelPlaneIntersector_H_
