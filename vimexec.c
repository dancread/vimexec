#include <windows.h>
#include <stdio.h>
#define INPUT_BUFFER_SIZE 1024
#define VIM_EXE "gvim "
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
    // Build command path
    lstrcpy(szFullCommand, VIM_EXE);
    lstrcat(szFullCommand, szTempBatchFileName);
    system(szFullCommand);
    // Execute .bat file
    system(szTempBatchFileName);
    ExitProcess(0);
}
