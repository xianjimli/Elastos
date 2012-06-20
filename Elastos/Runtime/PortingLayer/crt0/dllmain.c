
#if defined(_win32) || defined(_wince)
  #define WINAPI __stdcall
#else
  #define WINAPI
#endif

typedef int BOOL;
typedef void* HINSTANCE;
typedef unsigned int DWORD;
typedef void* LPVOID;

BOOL WINAPI DllMain(
        HINSTANCE hinstDLL,
        DWORD dwReason,
        LPVOID lpvReserved)
{
    return 1;
}
