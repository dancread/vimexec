#include <windows.h>
#include <stdio.h>
#define INPUT_BUFFER_SIZE 1024
#define VIM_EXE "vim "
#define MSG_FILE_HEADER ":: Everything below this comment block will be executed from a .bat file.\r\n:: If you want to cancel the commands, delete them all!\r\n"
INT main() {
    INT iBytesRead, iBytesWritten = 0;
    CHAR wsInputBuffer[INPUT_BUFFER_SIZE];
    HANDLE hTempFile;
    CHAR szTempDirectoryPath[MAX_PATH];
    CHAR szTempFileName[MAX_PATH];
    CHAR szTempBatchFileName[MAX_PATH];
    CHAR szFullCommand[MAX_PATH+sizeof(VIM_EXE)];
    // If no piped input, then exit
    if(GetFileType(GetStdHandle(STD_INPUT_HANDLE))!= FILE_TYPE_PIPE){
        ExitProcess(1);
    }
    // Get %TEMP% env variable
    GetTempPath(MAX_PATH, &szTempDirectoryPath);
    // Create unique temp bat file
    GetTempFileName(szTempDirectoryPath, "edx", 0, szTempFileName);
    // Change it to a *.bat file
    lstrcpyn(szTempBatchFileName, szTempFileName, MAX_PATH);
    szTempBatchFileName[lstrlen(szTempBatchFileName)-3]='\0';
    lstrcat(szTempBatchFileName,"bat");
    MoveFile(szTempFileName, szTempBatchFileName );
    printf("%s\n",szTempFileName);
    printf("%s\n",szTempBatchFileName);
    // Write temp file
    hTempFile = CreateFile((LPTSTR) szTempBatchFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    // Write file header
    WriteFile(hTempFile, MSG_FILE_HEADER, lstrlen(MSG_FILE_HEADER), &iBytesWritten, NULL);
    // Read stdin
    do{
       ReadFile(GetStdHandle(STD_INPUT_HANDLE), wsInputBuffer, INPUT_BUFFER_SIZE, &iBytesRead, NULL);
       WriteFile(hTempFile, wsInputBuffer, iBytesRead, &iBytesWritten, NULL);
    } while(iBytesRead);
    CloseHandle(hTempFile);
    // Start vim with that unique file
    SetStdHandle(STD_INPUT_HANDLE, CreateFileW(L"CONIN$", GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL));
    ShellExecute(GetConsoleWindow(), "open",VIM_EXE, szTempBatchFileName, NULL, SW_SHOWDEFAULT);
    // Execute .bat file
    ShellExecute(GetConsoleWindow(), "open", szTempBatchFileName, NULL, NULL, SW_SHOWDEFAULT);
    ExitProcess(0);
}
