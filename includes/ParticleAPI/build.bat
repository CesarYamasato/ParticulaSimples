cmake -G "MinGW Makefiles" -B build
cd .\build\
make
copy /B ".\libParticleAPI.dll" "../../../build"
copy /B ".\libParticleAPI.dll.a " "../../../build"
EXIT /B
