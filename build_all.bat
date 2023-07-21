echo [INFO] ----------Building external libraries...
pushd bat

echo [INFO] ----------Building glfw...
call build_glfw.bat
popd

echo [INFO] ----------Building project...
mkdir build
pushd build
del /f /s /q * >nul
cmake ..
popd
pause