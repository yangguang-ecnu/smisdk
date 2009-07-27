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
// Filename:	Logger.cpp
// Author:		Prashant Chopra
// Created:		Saturday, November 04, 2006 at 6:09:17 PM
//
//
//////////////////////////////////////////////////////////////////////
#include "Core/Logger.h"

PGMemInfo::PGMemInfo()
{	
	freed = false;
	pointerToBlock = 0;
	numberOfBytes = 0;

};

PGMemInfo::PGMemInfo(
					 bool	iFreed,
					 int		iPointerToBlock,
					 const long&	iNumberOfBytes)
{
	freed = iFreed;
	pointerToBlock = iPointerToBlock;
	numberOfBytes = iNumberOfBytes;
};

PGMemInfo::~PGMemInfo()
{
	pointerToBlock = 0;
	freed = true;
	numberOfBytes = 0;
};


namespace PGCore
{
	Logger::Logger(const char* iFileName /*=0*/) 
	{ 
		m_fptr = 0;
		
		/* Initialize the critical section -- This must be done before locking */
		InitializeCriticalSection(&m_cs);

		EnterCriticalSection(&m_cs);
		m_openedBraceCount = 0;
		m_timers.clear();
		//loggerId.push_back((int)this);
		LeaveCriticalSection(&m_cs);
		
		

		bool rv = false;
		std::string logPathStr;
		if (iFileName==0)
		{
			char szPath[MAX_PATH]; 
			DWORD dwBufferSize = MAX_PATH;
			GetTempPath( dwBufferSize, szPath );	

			std::string pStr = std::string(szPath) + kPgApplicationName + "\\";

			// create directory
			if (GetFileAttributes(pStr.c_str()) == INVALID_FILE_ATTRIBUTES)
			{
				rv = CreateDirectory(pStr.c_str(), NULL);
				if (!rv)
				{
					pStr = std::string(kPgDefaultPath);
				}			
			} else
			{
				rv = true;
			}

			logPathStr = pStr + "\\" + kPgLoggerFileName;			
		} else
		{
			logPathStr = iFileName;
			rv = true;
		}		

		SetLogFile(logPathStr.c_str());	

		Rotate();

		if (!rv)
		{
			Log("Failed to create specified directory %s. Using %s", iFileName ? iFileName : "NULL", kPgDefaultPath);
		}

		//Log("<%d>: Called constructor...\n", this);

		EnterCriticalSection(&m_cs);
		m_openedBraceCount = 0;
		loggerId.push_back((int)this);
		LeaveCriticalSection(&m_cs);
	}

	//////////////////////////////////////////////////////////////////////////

	Logger::~Logger() 
	{
		//Log("<%d>: Called destructor...\n", this);
		

		if (m_openedBraceCount>0)
		{
			Log("**** WARNING: %d open braces exist!", m_openedBraceCount);
		}

		if (!loggerId.empty())
		{
			if ((int)this == loggerId[0])
			{
				endLogText();
				close();	
			}
		}		

		/* Release system object when all finished -- usually at the end of the cleanup code */
		DeleteCriticalSection(&m_cs);
	}


	//////////////////////////////////////////////////////////////////////////

	bool Logger::Rotate()
	{
		if (m_filename.empty())
			return false;				

		int currentFileSize = getCurrentFileSizeInKB();

		if (currentFileSize<kPgMaxLogFileSizeInKB)
		{
			return false;
		}

		endLogText();
		close();

		std::string bckFileName = m_filename;		
		try {
			// backup old file

			//char sysCmd[MAX_PATH] = {0};
			char bckFileNameC[MAX_PATH] = {0};			
			{
#ifdef _LOG_TIME_DATE_BAK
				int pDate[3] = {0, 0, 0}, pTime[3] = {0, 0, 0};
				_strdate( tmpbuf_date ); _strtime( tmpbuf_time );			
				sscanf(tmpbuf_date, "%02d/%02d/%02d", &(pDate[0]), &(pDate[1]), &(pDate[2])); 
				sscanf(tmpbuf_time, "%02d:%02d:%02d", &(pTime[0]), &(pTime[1]), &(pTime[2])); 
				_snprintf(bckFileNameC, MAX_PATH-1, "%s_%02d.%02d.%02d_%02d.%02d.%02d.bak", m_filename.c_str(), 
					(pDate[0]), (pDate[1]), (pDate[2]),
					(pTime[0]), (pTime[1]), (pTime[2]));				
#else
				_snprintf(bckFileNameC, MAX_PATH-1, "%s.bak", m_filename.c_str());				
#endif
				bckFileNameC[MAX_PATH-1] = '\0';
			}


			bckFileName = bckFileNameC;		
			//::MoveFile(m_filename.c_str(), bckFileName.c_str());
			//_snprintf(sysCmd, MAX_PATH-1, "move %s %s", m_filename.c_str(), bckFileName.c_str());
			//sysCmd[MAX_PATH-1] = '\0';
			//system(sysCmd);

			DWORD lErr = 0;
			int rVal = ::DeleteFile(bckFileName.c_str());
			if (rVal==0)
			{
				DWORD lErr = GetLastError();
				printf("Error: failed to delete bck log file");
			}

			rVal = ::MoveFile(m_filename.c_str(), bckFileName.c_str());
			if (rVal==0)
			{
				DWORD lErr = GetLastError();
				printf("Error: failed to move log file");
			}

		} 
		catch (...)
		{
			Log("Error: Cannot backup old file from %s to %s.", m_filename.c_str(), bckFileName.c_str());
		}

		return SetLogFile(m_filename.c_str());				
	}

	void Logger::Close()
	{
		return close();
	}

	bool Logger::SetLogFile(const char* iFileName)
	{
		if (!iFileName) 
		{
			return false;
		}

		//endLogText();
		//close();

		m_filename = iFileName;

		if (!open()) 
		{
			return false;
		}

		//beginLogTxtDone = false;

		beginLogText();
		return true;
	}
	//////////////////////////////////////////////////////////////////////////

	void Logger::getSystemTime(char* oDate, char* oTime)
	{
		if (!oDate || !oTime)
		{
			return;
		}

		GetLocalTime(&m_systemTime);

		try
		{
			sprintf(oDate, "%02d.%02d.%04d",  
				m_systemTime.wMonth,	m_systemTime.wDay, m_systemTime.wYear);		
			sprintf(oTime, "%02d:%02d:%02d:%03d",  
				m_systemTime.wHour, m_systemTime.wMinute, m_systemTime.wSecond, m_systemTime.wMilliseconds);   
		}	
		catch (...)
		{

		}
		return;
	}



	int Logger::Log(char *fmt, ...)
	{
		if (!m_fptr) return false;

		/* Enter the critical section -- other threads are locked out */
		EnterCriticalSection(&m_cs);

		/* Do some thread-safe processing! */



		va_list ap;                                /* special type for variable    */
		char format[STRING_LENGTH];                /* argument lists               */
		int count = 0;
		int i, j;                                  /* Need all these to store      */
		char c;                                    /* values below in switch       */
		double d;
		unsigned u;
		char *s;
		void *v;

		getSystemTime(tmpbuf_date, tmpbuf_time);
		fprintf(m_fptr, "\n[%d][%s | %s] ",  this, tmpbuf_date, tmpbuf_time);    /* log date/time */
		fprintf(stderr, "\n[%d][%s | %s] ",  this, tmpbuf_date, tmpbuf_time);    /* log date/time */

		va_start(ap, fmt);                         /* must be called before work   */
		double timeElapsed = -1.0f;
		while (*fmt) 
		{
			for (j = 0; fmt[j] && fmt[j] != '%'; j++)
				format[j] = fmt[j];                    /* not a format string          */
			if (j) 
			{
				format[j] = '\0';
			
				// check if the first character is a left brace '{'				
				if (format[0]=='}' && m_openedBraceCount>0)
				{
					if (m_openedBraceCount<m_timers.size())
					{
						timeElapsed = m_timers[m_openedBraceCount].ElapsedSeconds();
						m_timers.pop_back();
					}
					m_openedBraceCount--;					
					
				}

				char format_indented[STRING_LENGTH];
				int szStr = (j+1);
				szStr = szStr <= (STRING_LENGTH-m_openedBraceCount) ? (szStr) :  (STRING_LENGTH-m_openedBraceCount);

				// first copy the tabs
				int tcount=0;
				while (tcount<m_openedBraceCount)
				{
					format_indented[tcount] = '\t';
					//_snprintf(format_indented+tcount, 1, "\t");
					tcount++;
				}

				_snprintf(format_indented+m_openedBraceCount, j+1, "%s", format);							

				count += fprintf(m_fptr, format_indented);    /* log it verbatim              */
				fprintf(stderr, format_indented);			 /* log it verbatim              */
				fmt += j;	

				if (format[0]=='{' && m_openedBraceCount<STRING_LENGTH)
				{
					m_openedBraceCount++;
					m_timers.push_back(Timer());
				} 

			} else 
			{
				for (j = 0; !isalpha(fmt[j]); j++) 
				{   /* find end of format specifier */
					format[j] = fmt[j];
					if (j && fmt[j] == '%')              /* special case printing '%'    */
						break;
				}
				format[j] = fmt[j];                    /* finish writing specifier     */
				format[j + 1] = '\0';                  /* don't forget NULL terminator */
				fmt += j + 1;

				switch (format[j]) 
				{                   /* cases for all specifiers     */
				case 'd':
				case 'i':                              /* many use identical actions   */
					i = va_arg(ap, int);                 /* process the argument         */
					count += fprintf(m_fptr, format, i); /* and log it                 */
					fprintf(stderr, format, i); /* and log it                 */
					break;
				case 'o':
				case 'x':
				case 'X':
				case 'u':
					u = va_arg(ap, unsigned);
					count += fprintf(m_fptr, format, u);
					fprintf(stderr, format, u);
					break;
				case 'c':
					c = (char) va_arg(ap, int);          /* must cast!                   */
					count += fprintf(m_fptr, format, c);
					break;
				case 's':
					s = va_arg(ap, char *);
					count += fprintf(m_fptr, format, s);
					fprintf(stderr, format, s);
					break;
				case 'f':
				case 'e':
				case 'E':
				case 'g':
				case 'G':
					d = va_arg(ap, double);
					count += fprintf(m_fptr, format, d);
					fprintf(stderr, format, d);
					break;
				case 'p':
					v = va_arg(ap, void *);
					count += fprintf(m_fptr, format, v);
					fprintf(stderr, format, v);
					break;
				case 'n':
					count += fprintf(m_fptr, "%d", count);
					fprintf(stderr, "%d", count);
					break;
				case '%':
					count += fprintf(m_fptr, "%%");
					fprintf(stderr, "%%");
					break;
				default:
					fprintf(stderr, "Invalid format specifier in log().\n");
				}
			}		 	
		}

		if (timeElapsed>0)
		{
			char format_indented[STRING_LENGTH];
			_snprintf(format_indented, STRING_LENGTH, " : %3.7f seconds ", timeElapsed);		
			int sEnd = m_openedBraceCount+24;
			sEnd = sEnd < STRING_LENGTH ? sEnd : (STRING_LENGTH-1);
			format_indented[sEnd] = '\0';

			count += fprintf(m_fptr, format_indented);    /* log it verbatim              */
			fprintf(stderr, format_indented);			 /* log it verbatim              */
		}

		va_end(ap);                                /* clean up                     */

		fflush(m_fptr);
		fflush(stderr);

		/* Leave the critical section -- other threads can now EnterCriticalSection() */
		LeaveCriticalSection(&m_cs);

		return count;
	}
	//////////////////////////////////////////////////////////////////////////


	void Logger::beginLogText() 
	{
		if (beginLogTxtDone) return;

		EnterCriticalSection(&m_cs);
		if (m_fptr) 
		{
			fprintf(m_fptr, "\n\n");
			fprintf(m_fptr, "**********************************************************************\n");
			fprintf(m_fptr, "**********************************************************************\n");	
			fprintf(m_fptr, "**********************************************************************\n");
#ifdef _DEBUG
			fprintf(m_fptr, "**************	Engine Execution log: DEBUG\n");		
#else
			fprintf(m_fptr, "**************	Engine Execution log: RELEASE\n");		
#endif
			//_strdate( tmpbuf_date ); _strtime( tmpbuf_time );	
			getSystemTime(tmpbuf_date, tmpbuf_time);
			fprintf(m_fptr, "**************	Log start date: %s\n", tmpbuf_date);
			fprintf(m_fptr, "**************	Log start time: %s\n", tmpbuf_time);	
			fprintf(m_fptr, "**********************************************************************\n");
			fprintf(m_fptr, "**********************************************************************\n");	
		}
		LeaveCriticalSection(&m_cs);

		fprintf(stderr, "\n\n");
		fprintf(stderr, "**********************************************************************\n");
		fprintf(stderr, "**********************************************************************\n");	
		fprintf(stderr, "**********************************************************************\n");
#ifdef _DEBUG
		fprintf(stderr, "**************	Engine Execution log: DEBUG\n");		
#else
		fprintf(stderr, "**************	Engine Execution log: RELEASE\n");		
#endif
		//_strdate( tmpbuf_date ); _strtime( tmpbuf_time );	
		getSystemTime(tmpbuf_date, tmpbuf_time);
		fprintf(stderr, "**************	Log start date: %s\n", tmpbuf_date);
		fprintf(stderr, "**************	Log start time: %s\n", tmpbuf_time);	
		fprintf(stderr, "**********************************************************************\n");
		fprintf(stderr, "**********************************************************************\n");

		EnterCriticalSection(&m_cs);
		beginLogTxtDone = true;
		LeaveCriticalSection(&m_cs);
	}

	//////////////////////////////////////////////////////////////////////////


	void Logger::MemoryDumpText()
	{
#if (_TRACK_MEMORY)
		//EnterCriticalSection(&m_cs);
		for (int i=0; i<pointerIndex.size(); i++)
		{
			PGMemInfo mInfo = memInfo[pointerIndex[i]];
			if (!mInfo.freed)
			{
				fprintf(m_fptr, "***  Unfreed Memory Chunk (%d): %d B\n", mInfo.pointerToBlock, mInfo.numberOfBytes);			
			}

		}
		//LeaveCriticalSection(&m_cs);

#endif
		//pointerIndex.clear();
		//memInfo.clear();
	}

	void Logger::endLogText()
	{
		//	if (endLogTxtDone) return;



		EnterCriticalSection(&m_cs);
		if (m_fptr) {
			MemoryDumpText();
			fprintf(m_fptr, "\n\n\n\n");
			fprintf(m_fptr, "**********************************************************************\n");
			fprintf(m_fptr, "**********************************************************************\n");		
			fprintf(m_fptr, "**************	Execution log\n");		
			//_strdate( tmpbuf_date ); _strtime( tmpbuf_time );	
			fprintf(m_fptr, "**************	Remaining unfreed Memory: %d B [Peak: %d KB]\n", kPgTotalBytesAllocated, kPgPeakBytesAllocated/1024);			
			getSystemTime(tmpbuf_date, tmpbuf_time);
			fprintf(m_fptr, "**************	Log end date: %s\n", tmpbuf_date);
			fprintf(m_fptr, "**************	Log end time: %s\n", tmpbuf_time);			
			fprintf(m_fptr, "**********************************************************************\n");
			fprintf(m_fptr, "**********************************************************************\n");		
		}
		LeaveCriticalSection(&m_cs);
		fprintf(stderr, "\n\n\n\n");
		fprintf(stderr, "**********************************************************************\n");
		fprintf(stderr, "**********************************************************************\n");		
		fprintf(stderr, "**************	Execution log\n");		
		//_strdate( tmpbuf_date ); _strtime( tmpbuf_time );	
		fprintf(stderr, "************** Remaining unfreed Memory: %d B [Peak: %d KB]\n", kPgTotalBytesAllocated, kPgPeakBytesAllocated/1024);			
		getSystemTime(tmpbuf_date, tmpbuf_time);
		fprintf(stderr, "**************	Log end date: %s\n", tmpbuf_date);
		fprintf(stderr, "**************	Log end time: %s\n", tmpbuf_time);		
		fprintf(stderr, "**********************************************************************\n");
		fprintf(stderr, "**********************************************************************\n");

		//EnterCriticalSection(&m_cs);
		//endLogTxtDone = true;
		//LeaveCriticalSection(&m_cs);
	}

	void Logger::close()
	{	
		if (m_fptr) 
		{
			fclose(m_fptr);
			m_fptr=0;
		}
	}

	bool Logger::open()
	{
		if (m_filename.empty()) 
		{
			return false;
		}	

		m_fptr = fopen(m_filename.c_str(), "a");
		if (!m_fptr) {
			return false;
		}

		return true;
	}


	int	 Logger::getCurrentFileSizeInKB()
	{
		//return kPgMaxLogFileSizeInKB+1;

		int returnSize=-1;

		close();


		HANDLE hFile;
		LPCSTR filename = ((LPCSTR)(m_filename.c_str()));
		if ((hFile = CreateFile (filename,
			GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_READONLY,(HANDLE)NULL)) == (HANDLE)(-1))
		{
			printf("\nFailed to open log file.");
			SetLogFile(m_filename.c_str());	
			return returnSize;
		}		

		DWORD fileSizeInBytes;
		fileSizeInBytes = GetFileSize(hFile, 0);	

		if (fileSizeInBytes == 0xFFFFFFFF)
		{
			printf("\nFailed to get file size.");
			CloseHandle(hFile);
			SetLogFile(m_filename.c_str());	
			return returnSize;
		}

		CloseHandle(hFile);	
		returnSize = fileSizeInBytes/1024;	

		open();	

		return returnSize;	
	}




};


