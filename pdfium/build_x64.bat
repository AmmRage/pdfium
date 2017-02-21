call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x64 
msbuild build\all.sln /p:Configuration=Release /p:Platform=x64 
