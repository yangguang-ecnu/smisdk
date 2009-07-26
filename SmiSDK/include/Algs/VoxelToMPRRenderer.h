// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
//
// Filename:	VoxelToMPRRenderer.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 5:11:33 PM
//

#ifndef _PGVoxelToMPRRenderer_H_
#define _PGVoxelToMPRRenderer_H_
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
	class VoxelToMPRRenderer : public RendererBase<T, U>
	{
	public:
		VoxelToMPRRenderer(void);

		virtual bool UpdateCB(void	*iDataObject);
	
		const ePgSlicingDirection GetSlicingDirection() const;		

		bool SetSkipFactor(int iSkipFactor);		

		int GetSkipFactor() const;
		
		const PGMath::AffineTransform<float>&	GetViewTransform() const;
		
		bool SetViewTransform(const PGMath::AffineTransform<float>&	iTransform);

		bool SetSlicingDirection(ePgSlicingDirection iDirection);

		bool SetSlicerPosition(double iPosition);

		bool SetBlendWeight(const float iWeight, const int iRendererIndex=0);

		float GetBlendWeight(const int iRendererIndex) const;

		bool SetTransferFunctionLUT(PGCore::TransferFunctionLUT<U> *iLUT, const int iVolumeIndex=-1);		

		bool SetMaskTransferFunctionLUT(PGCore::TransferFunctionLUT<U> *iLUT, const int iVolumeIndex=-1);		

		bool Execute();

		virtual bool PrepareRenderer();

		bool Render();

		void Clear();

		virtual ~VoxelToMPRRenderer(void);


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

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "VoxelToMPRRenderer.hpp"
#endif

//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGVoxelToMPRRenderer_H_
