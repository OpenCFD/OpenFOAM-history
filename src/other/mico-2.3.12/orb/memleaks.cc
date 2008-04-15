/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (C) 2003 Sorin Mustaca
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

/**
 * This file was added by Sorin.Mustaca@objectsecurity.com
 * Its purpose is to create only in WINDOWS a dump file with memory leaks in the running program.
 * The file is called mico_memory_leaks.log and is located in the working directory of the user.
 * Warning: if the program is killed there will be no log file.
 */

#if defined(_WIN32) && defined(_DEBUG)

#include <windows.h>
#include <direct.h>
#include <string>
#include <crtdbg.h>
	
	class MemoryDebug
	{
		HANDLE mhndl ;
	public:
		MemoryDebug()
		{

			char path[256];

			_getcwd(path,255);
			std::string mlogpath(path) ;

			mlogpath +="/mico_memory_leasks.log";
			
			mhndl = CreateFile( mlogpath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE,
								NULL, CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL , NULL );
			
			if( mhndl == INVALID_HANDLE_VALUE ) 
			{
				
				mhndl = 0;
			}
			
			
			int	dbgFlag = 0;
			
			// tighten control of heap allocations
			dbgFlag |= _CRTDBG_ALLOC_MEM_DF;
			//	dbgFlag &= ~_CRTDBG_ALLOC_MEM_DF;
			
			// check whole heap at every malloc/free. VERY SLOW - never use!
			//dbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;
			dbgFlag &= ~_CRTDBG_CHECK_ALWAYS_DF;
			
			// check CRT internal allocations 
			dbgFlag |= _CRTDBG_CHECK_CRT_DF;
			//dbgFlag &= ~_CRTDBG_CHECK_CRT_DF;
			
			// fill freed blocks with 0xDD 
			dbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
			//dbgFlag &= ~_CRTDBG_DELAY_FREE_MEM_DF;
			
			// call _CrtDumpMemoryLeaks at program exit 
			dbgFlag |= _CRTDBG_LEAK_CHECK_DF;
			//dbgFlag &= ~_CRTDBG_LEAK_CHECK_DF;
			
			_CrtSetDbgFlag( dbgFlag );
			
			/*warnings*/
			_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG );
			_CrtSetReportFile( _CRT_WARN, mhndl );
			/*errors*/
			_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG );
			_CrtSetReportFile( _CRT_ERROR, mhndl );
			/*asserts*/
			_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_WNDW );
			_CrtSetReportFile( _CRT_ASSERT, mhndl );
			
			_CrtDumpMemoryLeaks();
		}

		~MemoryDebug()
		{
			_CrtDumpMemoryLeaks();
			
			CloseHandle(mhndl);
		}
	};
	
	static MemoryDebug mem;
#endif
