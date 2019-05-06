@echo off


del ..\sci_obj\*.hep
del ..\sci_obj\*.scr

del classdef
copy selector.new selector

echo.                       > build.err
echo Building system       >> build.err
echo.                      >> build.err

set sinclude=.
..\sc.exe -u -O -s -o..\SCI_Obj @build.lst     >> build.err
..\sc.exe -u -O -s -o..\SCI_Obj @netbuild.lst  >> build.err

del 777.*

:End

