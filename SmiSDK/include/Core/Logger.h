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
// Filename:	Logger.h
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 6:09:17 PM
//

// Logger.h: interface for the Logger class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PG_Logger_H 
#define _PG_Logger_H "_PG_Logger_H"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4244)
#pragma warning(disable:4996)
#pragma warning(disable:4793)
#pragma warning(disable:4018)
#pragma warning(disable:4305)
#pragma warning(disable:4244)
#pragma warning(disable:4172)
#pragma warning(disable:4965)
#pragma warning(disable:4800)

#include <windows.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <sstream>
#include "Core.h"
#include "Timer.h"

#define STRING_LENGTH 512
static int		kPgMaxLogFileSizeInKB=100;
static long	int	kPgTotalBytesAllocated=0;
static long	int	kPgPeakBytesAllocated=0;
static std::vector<int> loggerId;
static const char* kPgDefaultPath = "C:\\";

class PGMemInfo
{
public:
	PGMemInfo();

	PGMemInfo(
		bool	iFreed,
		int		iPointerToBlock,
		const long&	iNumberOfBytes);
	
	~PGMemInfo();
	
	bool	freed;
	int		pointerToBlock;
	long	numberOfBytes;
};

static std::vector<int> pointerIndex;
static std::map<int, PGMemInfo> memInfo;    
static bool beginLogTxtDone=false;

namespace PGCore
{
	class Logger 
	{
	public:	
		Logger(const char* iFileName=0);	

		virtual ~Logger();		

		int  Log(char *fmt, ...);

		void Close();

		bool Rotate();

		bool SetLogFile(const char* iFileName);

		const char* GetLogFile() { return m_filename.c_str();}

	protected:

		void MemoryDumpText();

	private:	

		std::string	m_filename;
		FILE *m_fptr;
		CRITICAL_SECTION  m_cs;

		SYSTEMTIME	m_systemTime;
		char tmpbuf_date[STRING_LENGTH];
		char tmpbuf_time[STRING_LENGTH]; 

		int	 m_openedBraceCount;
		std::vector<Timer> m_timers;

		void getSystemTime(char* oDate, char* oTime);
		int	 getCurrentFileSizeInKB();	
		void beginLogText();
		void endLogText();
		bool open();
		void close();


	};
};

#endif // !defined(AFX_Logger_H__9B651563_FA0B_4911_8024_918D60EB56AD__INCLUDED_)