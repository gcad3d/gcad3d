// Mircosoft - VC and Msys2
//
//


// set export " __declspec(dllexport)" or ""
#if defined _MSC_VER || __MINGW64__
// MSVC and MSYS
#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#define export __declspec(dllexport)

#else
// Unix (invalidate "export"
#define export
#endif


// // set import " __declspec(dllimport)" or "extern"
// #if defined _MSC_VER || __MINGW64__
// #define import __declspec(dllimport)
// #else
// #define import extern
// #endif


// eof
