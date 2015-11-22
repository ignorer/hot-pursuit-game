mkdir ..\Release
mkdir ..\Release\Resources
xcopy ..\..\Externals\FreeGLUT\bin\freeglut.dll ..\Release
xcopy ..\..\Externals\StrategyDll.dll ..\Release
xcopy ..\..\Resources ..\Release\Resources /d /e /y
