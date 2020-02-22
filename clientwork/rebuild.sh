
rm -rf SCI_Obj
./buildall.bat
cp SCI_Obj/*.{hep,scr} scripts
cp SCI_Obj/*.voc vocabs

cd ../../DnGMakevols
rm -f out/*
sh MakeAll.bat
cd -

cp resources/* Interp/Debug
cp resources/* Interp/Release
