cd includes/OpenGLAPI
call build.bat
cd ../../ParticleAPI
call build.bat
cd ../../..
cmake -G "MinGW Makefiles" -B build
xcopy /I /Y Shaders "build/Shaders"
cd .\build\
make
