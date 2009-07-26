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
// Filename:	ImageBinarizer.h
// Author:		Prashant Chopra
// Created:		Sunday, November 05, 2006 at 6:50:29 PM
//

#ifndef _PGImageBinarizer_H_
#define _PGImageBinarizer_H_
//////////////////////////////////////////////////////////////////////////


#include "ProcessBase.h"
#include "../Core/BitImage.h"

namespace PGAlgs
{

template <class T, class U>
class ImageBinarizer : public ProcessBase
{
public:
	ImageBinarizer(void);

	bool Execute();

	void SetThresholdRange(const T& iLowTh, const T& iHighTh);
	bool GetThresholdRange(T& oLowTh, T& oHighTh) const;

	bool GetOutput(PGCore::BaseDataObject *oDataObject);

public:	
	virtual ~ImageBinarizer(void);

protected:

	

private:
	PGCore::BitImage	*m_oImageObject;

	T					m_lowTh, m_highTh;
};


};

#ifndef _PG_GENERATE_SDK_LIBS_
	#include "ImageBinarizer.hpp"
#endif


//////////////////////////////////////////////////////////////////////////
// EOF
#endif	// _PGImageBinarizer_H_
