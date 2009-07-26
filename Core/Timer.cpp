// -*- C++ -*- http://groups.google.com/group/medicalimagingscience
// © [2006-2018] Prashant Chopra [pixel.to.life@gmail.com] /**/Rights Reserved under GPL.
//
// This program is a free software: you may redistribute and/or modify it under the
// terms of the GNU General Public License (GPL) as published by the Free Software
// Foundation, version 3 or greater, as long as the result is kept free and opensource.
//
// A copy of the GNU General Public License can be found at http://www.gnu.org/licenses/.
// 
/// THIS SOFTWARE IS MADE AVAILABLE 'AS IS' AND THE AUTHOR MAKES NO WARRANTY
// REGARDING ITS PERFORMANCE, OR ITS FITNESS FOR ANY SPECIFIC USE. ENTIRE RISK
// TO ITS QUALITY AND PERFORMANCE IS WITH THE USER.
//
// Filename:	Timer.cpp
// Author:		Prashant Chopra
// Created:		Monday, December 25, 2006 at 7:21:44 PM
//

#include "Core/Timer.h"


namespace PGCore
{
//////////////////////////////////////////////////////////////////////////

Timer::Timer() 
{
	Reset();	
}

//////////////////////////////////////////////////////////////////////////

Timer::~Timer() 
{
	
}

//////////////////////////////////////////////////////////////////////////

void Timer::Reset()
{
	GetLocalTime(&m_systemStartTime);
	
	//GetLocalTime(&m_systemTime);
	//time (&m_start);
}

//////////////////////////////////////////////////////////////////////////

double Timer::ElapsedSeconds()
{

	//time_t pEnd;
	//time (&pEnd);
	//return difftime (pEnd, m_start);
	GetLocalTime(&m_systemEndTime);
	double tStart = 60.0f*(m_systemStartTime.wMinute)
		+ (m_systemStartTime.wSecond)
		+ 0.001* (double)(m_systemStartTime.wMilliseconds);

	double tEnd = 60.0f*(m_systemEndTime.wMinute)
		+ (m_systemEndTime.wSecond)
		+ 0.001* (double)(m_systemEndTime.wMilliseconds);
	
	return tEnd - tStart;
}

};
