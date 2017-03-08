#define UNICODE
#include <windows.h>
#include <stdio.h>
#define INPUT_BUFFER_SIZE 1024
INT wmain(INT argc, WCHAR **argv) {
   INT iBytesRead, iBytesWritten = 0;
   WCHAR wsInputBuffer[INPUT_BUFFER_SIZE];
   HANDLE hTempFile;
   WCHAR szTempDirectoryPath[MAX_PATH];
   WCHAR szTempFileName[MAX_PATH];
   // Get %TEMP% env variable
   GetTempPath(MAX_PATH, &szTempDirectoryPath);
   wprintf(L"%s\n",szTempDirectoryPath);
   // Create unique temp bat file
   GetTempFileName(szTempDirectoryPath, L"vimexec", 0, szTempFileName);
   wprintf(L"%s\n",szTempFileName);
   // Write temp file
   hTempFile = CreateFile((LPTSTR) szTempFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
   do{
       ReadFile(GetStdHandle(STD_INPUT_HANDLE), wsInputBuffer, INPUT_BUFFER_SIZE, &iBytesRead, NULL);
       WriteFile(hTempFile, wsInputBuffer, iBytesRead, &iBytesWritten, NULL);
   } while(iBytesRead);
   CloseHandle(hTempFile);
   // Start vim with that unique file
   ShellExecute(NULL, L"open",L"vim", szTempFileName, NULL, SW_SHOWDEFAULT);
   // Execute bat file when vim exits
}
