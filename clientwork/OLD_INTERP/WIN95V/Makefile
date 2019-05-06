include = ..\;c:\watcom\h;c:\watcom\h\nt

!ifndef SIERRAW
target	= sciw
debugAsmOpts = /DNETWORK
debugOpt = /dHR /dNW 
bannerOpt=
!else
target	= sierraw
debugAsmOpts = /DNETWORK
debugOpt = /dHR /dNW
bannerOpt= -Q
!endif

#debugObjs = &
#	hotkey.obj &

objects = 
debugObjs = &
	hotkey.obj &
	arglist.obj &
	array.obj &
	asm2cpp.obj &
	bufferd.obj &
	bufferw.obj &
	breakpt.obj &
	celasm.obj &
	celobj.obj &
	celobj1.obj &
	celobj2.obj &
	celobja.obj &
	config.obj &
	criterr.obj &
	criterrw.obj &
	cursorw.obj &
	dcomp.obj &
	debug.obj &
	debugdsp.obj &
	debugmnu.obj &
	dialog.obj &
	dos.obj &
	drawlist.obj &
	event.obj &
	eventw.obj &
	except.obj &
	font.obj &
	fonti.obj &
	graph.obj &
	graphmgr.obj &
	graphmw.obj &
	info.obj &
	integ.obj &
	intrptw.obj &
	kbdmgr.obj &
	kbdmgrw.obj &
	kbdw.obj &
	kernel.obj &
	language.obj &
	line.obj &
	list.obj &
	longint.obj &
	lzss.obj &
	magnify.obj &
	math.obj &
	memid.obj &
	memmgr.obj &
	memmgre.obj &
	memmgrw.obj &
	memtype.obj &
	message.obj &
	motion.obj &
	mouse.obj &
	moview.obj &
	msg.obj &
	msgw.obj &
	networkw.obj &
	new.obj &
	newroom.obj &
	object.obj &
	packdata.obj &
	palette.obj &
	palmgri.obj &
	palmgrt.obj &
	palmgrw.obj &
	picture.obj &
	plane.obj &
	platfrmw.obj &
	plist.obj &
	pmachine.obj &
	polygon.obj &
	polyfuse.obj &
	ratio.obj &
	rect.obj &
	rectlist.obj &
	remap.obj &
	resource.obj &
	restype.obj &
	sci.obj &
	screen.obj &
	script.obj &
	sendstac.obj &
	shiftpal.obj &
	shwstyle.obj &
 	si.obj &
	silist.obj &
	sound.obj &
	stampver.obj &
	string.obj &
	table.obj &
	textid.obj &
	time.obj &
	timew.obj &
	vocab.obj &
	windll.obj &
	window.obj &
	winfuncs.obj &
 	mousew.obj &
	sciw.obj &
	sciwin.obj &
	winthunk.obj

dllObjs = &
	scidll.obj &
	midiw.obj &
	winsnd.obj

#	*wlink name $(target) system nt_win option stack=40k &

#		debug codeview file {$(debugObjs)} library mss32.lib library dinput.lib
$(target).exe : $(debugObjs) $(objects) 
	*wlink name $(target) system nt_win  &
		debug codeview file {$(debugObjs)} option map library mss32.lib 
	cvpack $(target)
#	dbg2map $(target).exe
# 	msym $(target).map
	wrc /bt=nt /d=_M_IX86 sciw.rc sciw.exe
#   wbind $(target) -R -30 $(target).res

$(target).res : $(target).rc sciw.ico
    wrc -r -30 $(target).rc

scidll.dll : $(dllObjs)
	*wlink &
		system win386 &
		option map &
		option stack=12K &
		option maxdata=8K	&
		option mindata=4K &
		file {$(dllObjs)}
	wbind scidll -d -n
