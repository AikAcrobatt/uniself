#pragma once


#ifdef UNS_DLL_C_MQL5_STDCALL_EXPORT
#define DLL_EXPORT(type) extern "C" __declspec(dllexport) type __stdcall
#endif

#ifdef UNS_DLL_C_MQL5_CDECL_EXPORT
#define DLL_EXPORT(type) extern "C" __declspec(dllexport) type __cdecl
#endif

#ifdef UNS_DLL_MQL5_STDCALL_EXPORT
#define DLL_EXPORT(type) __declspec(dllexport) type __stdcall
#endif

#ifdef UNS_DLL_MQL5_CDECL_EXPORT
#define DLL_EXPORT(type) __declspec(dllexport) type __cdecl
#endif