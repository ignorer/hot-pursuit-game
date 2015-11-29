mkdir ..\Debug
mkdir ..\Debug\Resources
xcopy ..\..\Externals\FreeGLUT\bin\freeglut.dll ..\Debug /d
xcopy ..\..\Resources ..\Debug\Resources /d /e /y
