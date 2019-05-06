del *.err

set sinclude=.;..\SCI_SYS;..\MSG;

copy ..\SCI_Sys\selector
copy ..\SCI_Sys\classdef
copy ..\SCI_Obj\*.voc .

..\sc.exe -g20000 -u -l -s -O -o..\SCI_Obj @build.lst > build.err
..\sc.exe -g20000 -u -l -s -O -o..\SCI_Obj wactor.sc wego.sc bpass.sc >> build.err

