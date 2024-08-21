@echo off

if not exist build mkdir build
pushd build

set defines=/D"UNICODE" /D"_CRT_SECURE_NO_WARNINGS"
set compile_flags=/WX /W4 /FC /external:anglebrackets /external:W0 /diagnostics:column /wd4100
set link_flags=/nologo /incremental:no /subsystem:windows gdi32.lib opengl32.lib user32.lib

REM /P preprocesses a file

cl /Zi /Od %compile_flags% %defines% /D"_DEBUG" /Fe"chadge_debug" ../code/win32_main.c /link /debug %link_flags%
REM cl /O2 %compile_flags% %defines% /D"NDEBUG" /Fe"chadge_release" ../code/win32_main.c /link %link_flags%

popd
