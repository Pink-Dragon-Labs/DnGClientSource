castoff
i:

cd\sys\sci\swat\scih
if not "%1"=="o" goto scih
del *.obj
:scih
wmake
copy scih.exe s:\games\sci\system\interps\swat

cd\sys\sci\swat\sierrah
if not "%1"=="o" goto sierrah
del *.obj
:sierrah
wmake
copy sierrah.exe s:\games\sci\system\interps\swat

cd\sys\sci\swat\sciwh
if not "%1"=="o" goto sciwh
del *.obj
:sciwh
wmake
copy sciwh.exe s:\games\sci\system\interps\swat

cd\sys\sci\swat\sierrawh
if not "%1"=="o" goto sierrawh
del *.obj
:sierrawh
wmake
copy sierrawh.exe s:\games\sci\system\interps\swat

cd\sys\sci\swat
caston
