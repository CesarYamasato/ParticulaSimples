cmake -G "MinGW Makefiles" -B build
cd .\build\
make
copy /B ".\libOpenGLAPI.dll" "../../../build"
copy /B ".\libOpenGLAPI.dll.a" "../../../build"
EXIT /B
