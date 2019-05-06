castoff
i:

cd\sys\sci\swat\sierrawh
if not "%1"=="o" goto sierrawh
del *.obj
:sierrawh
wmake
copy sierrawh.exe s:\games\sci\system\interps\swat

cd\sys\sci\swat\sciwh
if not "%1"=="o" goto sciwh
del *.obj
:sciwh
wmake
copy sciwh.exe s:\games\sci\system\interps\swat

cd\sys\sci\swat
caston
