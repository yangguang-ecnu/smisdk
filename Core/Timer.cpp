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
