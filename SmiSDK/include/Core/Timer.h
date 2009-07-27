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