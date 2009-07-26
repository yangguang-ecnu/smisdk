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