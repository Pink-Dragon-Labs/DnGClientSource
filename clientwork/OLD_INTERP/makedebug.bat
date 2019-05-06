@echo off

rem makes debug interpreter

cd win95

del *.obj

rem these guys won't assemle as part of the make process, but they will
rem if called individually like this

ml /DWINDOWS /c /Cp /Zm /Zp4 /DDEBUG /DNETWORK ..\celasm.asm
ml /DWINDOWS /c /Cp /Zm /Zp4 /DDEBUG /DNETWORK ..\dcomp.asm
ml /DWINDOWS /c /Cp /Zm /Zp4 /DDEBUG /DNETWORK ..\pmachine.asm

wmake

cd ..
