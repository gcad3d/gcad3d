// Mircosoft - VC and Msys2
//
//
#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>


// set export " __declspec(dllexport)" or ""
#if defined _MSC_VER || __MINGW64__
#define export __declspec(dllexport)
#else
#define export
#endif


// // set import " __declspec(dllimport)" or "extern"
// #if defined _MSC_VER || __MINGW64__
// #define import __declspec(dllimport)
// #else
// #define import extern
// #endif


// eof
