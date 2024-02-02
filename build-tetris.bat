
@echo off
set BAT_BASE_PATH=%~dp0
set PRJ_BASE_PATH=%BAT_BASE_PATH%
set BIN_PATH=%PRJ_BASE_PATH%\bin\
set SRC_PATH=%PRJ_BASE_PATH%\src\
set INC_PATH=%PRJ_BASE_PATH%\includes\

md %BIN_PATH%
clang.exe -o %BIN_PATH%\Tetris.exe %SRC_PATH%\*.c %SRC_PATH%\scenes\*.c %SRC_PATH%\SinglyLinkedLists\singlylinkedlist.c -l %BAT_BASE_PATH%\raylibdll.lib -I %INC_PATH%

echo Build completed!
