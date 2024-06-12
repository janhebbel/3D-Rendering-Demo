@echo off

if not exist build mkdir build
pushd build

gcc -DUNICODE -D_CRT_SECURE_NO_WARNINGS -Wno-unused-parameter -Wall -Wextra -pedantic -D_DEBUG -O0 -g -o chadge_debug ../code/win32_main.c 
gcc -DUNICODE -D_CRT_SECURE_NO_WARNINGS -w -fdata-sections -ffunction-sections -Wl,--gc-sections -DNDEBUG -O3 -g0 -s -o chadge_release ../code/win32_main.c

popd
