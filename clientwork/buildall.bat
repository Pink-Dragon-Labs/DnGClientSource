if not exist .\SCI_obj mkdir SCI_Obj
cd SCI_SYS
call build.bat
cd ..\SRC
call build.bat
cd ..
