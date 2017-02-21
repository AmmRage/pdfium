call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86 
msbuild build\all.sln /p:Configuration=Release /p:Platform=Win32 
