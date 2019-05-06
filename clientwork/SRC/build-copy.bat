del *.err

call ..\tools\setsys

copy ..\SCI_Sys\selector
copy ..\SCI_Sys\classdef
copy ..\SCI_Obj\*.voc .

..\tools\sc.exe -g1600 -u -l -s -O -o..\SCI_Obj @build.lst > build.err
..\tools\sc.exe -g1500 -u -l -s -O -o..\SCI_Obj atplist.sc lists.sc >> build.err
..\tools\sc.exe -g1500 -u -l -s -O -o..\SCI_Obj wactor.sc wego.sc bpass.sc >> build.err

