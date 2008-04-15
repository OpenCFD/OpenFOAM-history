#ifndef _INCLUDE_H_
	#define _INCLUDE_H_
	
	#include <windows.h>
	#include <string>
	#define MICODCM_API

	#define WIN32_LEAN_AND_MEAN

	#pragma once

	#if defined(_DEBUG) && !defined(DCM_TRACE)
		#define DCM_TRACE
	#endif	
    #include "trace.h"
	
#endif
