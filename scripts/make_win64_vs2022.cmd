REM 
REM Build intransive dice binaries with Visual Studio 2022 and cmake
REM

pushd ..
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=amd64 -host_arch=amd64
if not exist build_win64 mkdir build_win64
pushd build_win64
cmake -G "Visual Studio 17 2022" ..
if %ERRORLEVEL% neq 0 ( @echo ERROR building cmake files & @pause )
cmake --build . --clean-first --config Debug
if %ERRORLEVEL% neq 0 ( @echo ERROR building intransive dice debug binaries & @pause )
cmake --build . --clean-first --config Release
if %ERRORLEVEL% neq 0 ( @echo ERROR building intransive dice release binaries & @pause )
start "intransive_dice.sln" intransive_dice.sln
popd

popd
@timeout /t 10
