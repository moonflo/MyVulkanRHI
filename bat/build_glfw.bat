git submodule update --init ..\External\src\glfw
mkdir ..\External\build\glfw
pushd ..\External\build\glfw
del /f /s /q * >nul
cmake -DCMAKE_INSTALL_PREFIX=..\..\Windows -G "Visual Studio 17 2022" -Thost=x64 ..\..\src\glfw
cmake --build . --config debug --target install
popd
pause