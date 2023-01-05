/// Demonstrate token impersonation on a thread.

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <lmcons.h>
#include <processthreadsapi.h>
#include <stdio.h>
#include <tchar.h>

DWORD WINAPI ThreadFn(LPVOID lpParam)
{
    TCHAR username[UNLEN + 1];
    DWORD len = UNLEN + 1;
    if (GetUserName(username, &len) == 0)
    {
        printf("GetUserName error: %d\n", GetLastError());
        return 1;
    }

    _tprintf(_T("%s\n"), username);

    return 0;
}

int main(void)
{
    HANDLE hThread = CreateThread(NULL, 0, ThreadFn, NULL, CREATE_SUSPENDED, NULL);
    if (hThread == NULL)
    {
        printf("CreateThread error: %d\n", GetLastError());
        return 1;
    }

    HANDLE hToken = NULL;
    if (!LogonUser(
            TEXT("user"), TEXT("."), TEXT("user"),
            /*dwLogonType=*/LOGON32_LOGON_NETWORK, // `LogonUser` will return an impersonation token
                                                   // as required by `SetThreadToken` when
                                                   // `dwLogonType` is `LOGON32_LOGON_NETWORK`.
            LOGON32_PROVIDER_DEFAULT, &hToken))
    {
        printf("LogonUserW error: %d\n", GetLastError());
        return 1;
    }

    if (!SetThreadToken(&hThread, hToken))
    {
        printf("SetThreadToken error: %d\n", GetLastError());
        return 1;
    }

    if (ResumeThread(hThread) == -1)
    {
        printf("ResumeThread error: %d\n", GetLastError());
        return 1;
    }

    if (WaitForSingleObject(hThread, INFINITE) == WAIT_FAILED)
    {
        printf("WaitForSingleObject error: %d\n", GetLastError());
        return 1;
    }

    (void)CloseHandle(hThread);

    return 0;
}
