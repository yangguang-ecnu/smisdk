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
// Filename:	Timer.h
// Author:		Prashant Chopra
// Created:		Monday, December 25, 2006 at 7:21:44 PM
//

#if !defined(AFX_TIMER_H_INCLUDED_)
#define AFX_TIMER_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <time.h>
#include <sys/timeb.h>

namespace PGCore
{

class Timer 
{
public:	
	Timer();	

	virtual ~Timer();		
	
	void Reset();

	double ElapsedSeconds();	

private:	
	
	SYSTEMTIME	m_systemStartTime, m_systemEndTime;
};

};
#endif // !defined(AFX_TIMER_H_INCLUDED_)