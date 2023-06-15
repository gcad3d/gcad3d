// for using dynamic-dlls (../ut/os_dll_uix.c ../ut/os_dll_ms.c)

#define DLL_UNLOAD               0      // unload
#define DLL_LOAD_only            1      // dlopen RTLD_LAZY
#define DLL_LOAD_all             2      // dlopen RTLD_LAZY|RTLD_GLOBAL
#define DLL_CONNECT              3      // connect function
#define DLL_EXEC                 4      // execute function

#define DLL_LOAD_EXEC           16      // load+startFunc+unload
#define DLL_LOAD_EXEC_UNLOAD    17      // load+startFunc+unload

// eof
