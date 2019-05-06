@echo off

rem makes non-debug interpreter

cd win95v

del *.obj

rem these guys won't assemle as part of the make process, but they will
rem if called individually like this

ml /DWINDOWS /c /Cp /Zm /Zp4 /DNETWORK ..\celasm.asm
ml /DWINDOWS /c /Cp /Zm /Zp4 /DNETWORK ..\dcomp.asm
ml /DWINDOWS /c /Cp /Zm /Zp4 /DNETWORK ..\pmachine.asm

wmake

cd ..
