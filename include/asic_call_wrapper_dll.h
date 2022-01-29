#ifdef _WIN32

#ifndef ASIC_CALL_WRAPPER_H
	#define ASIC_CALL_WRAPPER_H
	
    #include <windows.h>

    #define C_ICEHANDLE_SIZE 524288

	typedef struct{
		char dummy[C_ICEHANDLE_SIZE];
	} IceHandleClass;

    #ifdef DLL_EXPORT
        // Windows DLL Export
        #define DECLDIR __declspec(dllexport)
        #include "asic_call.h"
    #else
        // Windows DLL Import
        #define DECLDIR __declspec(dllimport)
    #endif
    
#ifdef __cplusplus
extern "C" {
#endif
        DECLDIR int asic_call_parse(char * InString, char * OutString);
        DECLDIR BOOL WINAPI DllMain (HANDLE hDll, DWORD dwReason, LPVOID lpReserved);
#ifdef __cplusplus
    }
#endif
    
#endif

#endif
