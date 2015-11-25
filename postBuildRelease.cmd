mkdir ..\Release
mkdir ..\Release\Resources
xcopy ..\..\Externals\FreeGLUT\bin\freeglut.dll ..\Release /d
xcopy ..\..\Externals\StrategyDll.dll ..\Release /d
xcopy ..\..\Resources ..\Release\Resources /d /e /y
