//-----------------------------------------------------------------------------
// Project     : SDK Core
// Version     : 1.0
//
// Category    : Common Base Classes
// Filename    : public.sdk/source/main/dllmain.cpp
// Created by  : Steinberg, 01/2004
// Description : Windows DLL Entry
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2020, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include <windows.h>

#include "pluginterfaces/base/ftypes.h"

#if defined(_MSC_VER) && defined(DEVELOPMENT)
	#include <crtdbg.h>
#endif

#ifdef UNICODE
	#define tstrrchr wcsrchr
#else
	#define tstrrchr strrchr
#endif

HINSTANCE ghInst = nullptr;
void* moduleHandle = nullptr;
#define VST_MAX_PATH 2048
Steinberg::tchar gPath[VST_MAX_PATH] = { 0 };

#define DllExport __declspec (dllexport)

// Implemented by plugin
extern bool InitModule();
extern bool DeinitModule();

extern "C" {
	static int moduleCounter{ 0 }; // Counting for InitDll/ExitDll pairs

	// Called from host (possibly more than once) right after loading dll
	bool DllExport InitDll() {
		if (++moduleCounter == 1) {
			return InitModule();
		}
		return true;
	}

	// Called from host (possibly more than once) right before loading dll
	bool DllExport ExitDll() {
		if (--moduleCounter == 0) {
			return DeinitModule();
		}
		else if (moduleCounter < 0) {
			return false;
		}
		return true;
	}
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID /*lpvReserved*/) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		#if defined(_MSC_VER) && defined(DEVELOPMENT)
			_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
			_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
			_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
			int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
			_CrtSetDbgFlag(flag | _CRTDBG_LEAK_CHECK_DF);
		#endif

		moduleHandle = ghInst = hInst;

		// Get path of component
		if (GetModuleFileName(ghInst, gPath, MAX_PATH) > 0) {
			Steinberg::tchar* backslash = tstrrchr(gPath, TEXT('\\'));
			if (backslash) {
				gPath[backslash - gPath + 1] = 0;
			}
		}
	}
	return TRUE;
}
