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
