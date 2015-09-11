xcopy ..\build\bin\Release\* ..\install\ /i /e
xcopy "NeuroTitan v1 Seed\*" "..\install\NeuroTitan v1\" /i /e
windeployqt -core -gui -widgets ..\install\Risiko.exe
pause