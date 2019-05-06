;;;;
;;;;	FILESEL.SC: File Selector
;;;;
;;;;
;;;;	   A File Selector is a scrolling list of files and directories.
;;;;  This class adds several different buttons to the dialog so all
;;;;  control is done from this item. It is being added to which include:
;;;;		HOME   		- Goto Home directory
;;;;		OK		 		- Select current file
;;;;		CANCEL 		- Cancel file selection and return -1
;;;;		UP_DIR 		- Go up a directory
;;;;		FILE_MASK 	- File mask to use in displaying files
;;;;
;;;;		When you use the file selector class, the curItem property will 
;;;;	contain the string of the file that was selected or that was typed in.
;;;;	The file selector will clean up itself and clear out all file names
;;;;	except what is in curItem, so clean up the curItem object after you
;;;;	have processed it.
;;;;
;;;;
;;;;	Classes:
;;;;		FileSelector
;;;;

(module# FILESEL)


(procedure
	SizeDirectoryStr
	StripPath
	StripFileName
	ReplaceFileName
	GoDirectory
	FlattenDirectory
)

(enum
	CANCEL_BTN
	OK_BTN
	FILE_MASK_BTN
	GO_UP_DIR_BTN
	HOME_BTN
)

;; Directory attributes (used by FSDirFirst, FSDirNext)
(define FA_STOP		FALSE)	;; Returned on error or no more files found

(define FA_NORMAL		$00)		;; Normal file, no attributes 
(define FA_RDONLY		$01)		;; Read only attribute 
(define FA_HIDDEN		$02)		;; Hidden file 
(define FA_SYSTEM		$04)		;; System file 
(define FA_LABEL		$08)		;; Volume label 
(define FA_DIREC		$10)		;; Directory 
(define FA_ARCH		$20)		;; Archive 
(define FILENAME_TEXT_SIZE		18)

(define BTN_WIDTH		64)
(define BTN_HEIGHT	12)
(define HALF_BTN_WIDTH (- (/ BTN_WIDTH 2) MARGIN))

(class FileSelector of Selector
	(properties
		int itemsPerPage: 8
		int itemWidth:		64 
		id  searchStr:		0	;; search string
		id	 homeStr: 		0	;; home directory string
		id  curDirStr:    0	;; current directory string
		id  curFileStr:   0	;; current directory+file string

		id  directoryI:	0  ;; current directory text field
		id	 fileNameI:		0	;; current fileName editable field
		id  homeBtn:		0	;; Home button
		id	 okBtn:			0	;; OK button
		id	 cancelBtn: 	0	;; CANCEL button
		id  fileMaskBtn:  0	;; File Mask button
		id	 upDirBtn:		0	;; Up directory button
		int allowDoubleClick: TRUE	;; default for FileSelector is TRUE
	)
	(methods
		readDirectory
		sort
	)

	(method (init &tmp r verMargin)
		(if curFileStr ;; if not been initted
			(return)
		)
		;; Handle parameter defaulting
		(if (not searchStr)
			(= searchStr "*.*")
		)
		(if (not font)
			(= font smallFont)
		)
		(= r (Rectangle new:))
		(r sizeToStr: "M" font -1)
		(= itemWidth (* (r width:) 11))
		(r dispose:)

		;;; First figure out default path
		(if (not homeStr)
			(= homeStr (String new: 128))
			(FileIO FileGetCWD (StrData homeStr))
		)

		;;; Then add the passed in path
		(if (!= (homeStr at: (- (homeStr size:) 1)) `\\)
			(homeStr at: (homeStr size:) `\\ 0)
		)
		(= curDirStr  (String copy: homeStr))
		(= curFileStr (String copy: searchStr))

		(self	readDirectory: curDirStr curFileStr)

		((= directoryI (TextItem new:))
			text: 	"",
			font: 	smallFont,
			width: 	120,
			height: 	12,
			fore:  	fore,
			back:   	back,
			moveTo: 	left top,
			handler: self,
		)
		(directoryI text: (SizeDirectoryStr directoryI curDirStr))

		((= fileNameI (EditItem new:))
 			text: 	curFileStr,
			font: 	smallFont,
			max:  	30,
			;width: 	100,
			;height: 	10,
			moveTo: 	left (+ (directoryI bottom?) MARGIN),
			setSize:,
			handler: self,
		)
		(fileNameI state: (| (fileNameI state?) dOverwrite))
		(self setSize:)
		(self heighten: MARGIN)

		(super init: &rest) 

		(= verMargin (+ MARGIN 2 (systemButton borderSize?)))

		((= upDirBtn (systemButton new:))
			text: "..\\",
			helpStr:	"___Select to go one directory up towards the root.",
			font:	   smallFont,
			height:  BTN_HEIGHT,
			width:   HALF_BTN_WIDTH,
			value:   GO_UP_DIR_BTN,
			state:   dActive,
			handler: self,
		)
		(upDirBtn
			moveTo:  (+ (upArrow right?) verMargin) (+ (scrollBox top?) (upDirBtn borderSize?)),
		)
		((= fileMaskBtn (systemButton new:))
			text: 	"*.*",
			helpStr:	"___Select to change the search mask.",
			font:	   smallFont,
			height:  BTN_HEIGHT,
			width:   HALF_BTN_WIDTH,
			value:   FILE_MASK_BTN,
			moveTo:  (+ (upDirBtn right?) (upDirBtn borderSize?) MARGIN) (upDirBtn top?),
			state:   dActive,
			handler: self,
		)
		((= homeBtn (systemButton new:))
			text: "Home",
			helpStr:	"___Select to go back to your original home directory.",
			font:	   smallFont,
			height:  BTN_HEIGHT,
			width:   BTN_WIDTH,
			value:   HOME_BTN,
			moveTo: (upDirBtn left?) (+ (upDirBtn bottom?) verMargin),
			state:   dActive,
			handler: self,
		)
		((= okBtn (systemButton new:))
			text: "_OK_",
			helpStr:	"___Select to use this selection.",
			font:	   smallFont,
			height:  BTN_HEIGHT,
			width:   BTN_WIDTH,
			value:   OK_BTN,
			moveTo: (upDirBtn left?) (+ (homeBtn bottom?) verMargin),
			state:  (| dActive dExit),
			handler: self,
		)
		((= cancelBtn (systemButton new:))
			text: "Cancel",
			helpStr:	"___Select to abort this operation.",
			font:	   smallFont,
			height:  BTN_HEIGHT,
			width:   BTN_WIDTH,
			value:   CANCEL_BTN,
			moveTo: (okBtn left?) (+ (okBtn bottom?) verMargin),
			state:  (| dActive dExit),
			handler: self,
		)
		(directoryI
			width: (- (cancelBtn right?) (+ left MARGIN))
		)
		(fileNameI
			width: (directoryI width:)
		)

		(self setSize:)

		(cond
			((== client (curRoom pScreen?))
				;;; Let's init: what is specific to this class (the init does draw:)
				(directoryI   init: client)
				(fileNameI    init: client)
				(homeBtn		  init: client)
				(okBtn		  init: client)
				(cancelBtn	  init: client)
				(fileMaskBtn  init: client)
				(scrollBox    init: client)
			)
			(client
				;;; Lets add these items to the dialog.
				(client
					addBefore: self fileNameI,
					add: 		  directoryI upDirBtn fileMaskBtn homeBtn okBtn cancelBtn
				)
			)
		)

		;;; clear out curItem
		(if (and
				curItem ;;(IsObject curItem)
				elements
				(not (elements contains: curItem))
			)
			(curItem dispose:)
		)
		(= curItem 0)
	)
	(method (dispose)
		(if homeStr
			(homeStr dispose)
			(= homeStr 0)
		)
		(if curFileStr
			(curFileStr dispose)
			(= curFileStr 0)
		)
		(if curDirStr
			(curDirStr dispose)
			(= curDirStr 0)
		)

	 	(if elements
			(elements dispose:)
			(= elements 0)
		)

	 	(super dispose:)

		;;; Zero out all id properties
		(= directoryI (= fileNameI (= homeBtn (= okBtn (= cancelBtn (= fileMaskBtn (= upDirBtn 0)))))))

	)

	(method (moveTo h v isMoveBy &tmp dx dy)
		(if directoryI
			(= dx (- h left))
			(= dy (- v top))
			(if (< argc 3)
				(if directoryI		(directoryI  moveBy: dx dy))
				(if fileNameI		(fileNameI	 moveBy: dx dy))
				(if upDirBtn		(upDirBtn 	 moveBy: dx dy))
				(if fileMaskBtn	(fileMaskBtn moveBy: dx dy))
				(if homeBtn			(homeBtn		 moveBy: dx dy))
				(if okBtn			(okBtn		 moveBy: dx dy))
				(if cancelBtn		(cancelBtn	 moveBy: dx dy))
			)
		)

		(super moveTo: h v)
	)
	(method (setSize &tmp r)
		((= r (Rectangle new:))
			copy: self
		)
		(if directoryI	;;; topmost
			(r union: directoryI)
		)
		(if fileNameI	;;; sub-topmost (necessary)
			(r union: fileNameI)
		)
		(if scrollBox		;;; leftmost
			(r union: scrollBox)
		)
		(if downArrow		;;; downmost
			(r union: downArrow)
		)
		(if cancelBtn		;;; rightmost & downmost
			(r union: cancelBtn)
		)
		(self copy: r)
		(r dispose:)

	)
	(method (draw)
		(if directoryI		(directoryI   draw:))
		(if fileNameI		(fileNameI    draw:))
		(if homeBtn			(homeBtn		  draw:))
		(if okBtn			(okBtn		  draw:))
		(if cancelBtn		(cancelBtn	  draw:))
		(if fileMaskBtn	(fileMaskBtn  draw:))
		(if upDirBtn		(upDirBtn	  draw:))

		(super draw:)
	)

	(method (selectItem &tmp prevCurItem)
		(= prevCurItem curItem)

		(super selectItem: &rest)

		(if (and
				(!= curItem prevCurItem)
				(< argc 2)
			)
			(curFileStr copy: (curItem text?))
			(if fileNameI
				(fileNameI
					text: curFileStr,
					cursor: (curFileStr size:),
					draw:,
				)
			)
		)
	)

	;;;
	;;; Sort subDirs to top and all files after.
	;;;
	(method (sort fnStr &tmp seq tNewObj tObj)
		((= tNewObj (SelectorText new:))
			pSelector: self,
		 	init: fnStr,
		)
		(if (elements isEmpty:)
			(elements add: tNewObj)
		else
			(= seq (Sequence new: elements))
			(while
				(and
					(= tObj (seq next:))
					(or
						(!= ((tObj text?) test: FA_DIREC) (fnStr test: FA_DIREC))
						(< (fnStr compare: (tObj text?)) 0)
					)
 				)
				0
			)
			(seq dispose:)
			(if tObj
				(elements addBefore: tObj tNewObj)
			else
				(elements add: tNewObj)
			)
		)
	)

	;;;
	;;;	Read the current directory of files. move subDirs to top as we go
	;;;
	(method (readDirectory path mask &tmp curStr tPath dirList fileList i)
		(if elements
			(elements 
				eachElementDo: #dispose,
				release:
			)
		)
		(= elements (Set new:))
		(= tPath (String copy: path))
		
		;;; Get all the directories first
		(= curStr 	(FileNameStr new: 13))
		(= dirList  (Set new:))
		(= fileList (Set new:))

		(ReplaceFileName tPath "*.*")
		(if (FileIO FileFindFirst (tPath data?) (StrData curStr) FA_DIREC)
			(repeat
				(if (curStr test: FA_DIREC)
					(if
						(or
							(== (curStr compare: ".")  STR_EQUAL)
							(== (curStr compare: "..") STR_EQUAL)
						)
						(curStr dispose:)
					else
						(dirList add: curStr)
					)
					(= curStr (FileNameStr new: 13))
				)
				(breakif (not (FileIO FileFindNext (StrData curStr))))
			)
		)
		(if (> argc 1)
			(ReplaceFileName tPath mask)
		)
		(if (FileIO FileFindFirst (tPath data?) (curStr data?) FA_NORMAL)
			(repeat
				(if (not (curStr test: FA_NORMAL))
					(fileList add: curStr)

					(= curStr (FileNameStr new: 13))
				)
				(breakif (not (FileIO FileFindNext (StrData curStr))))
			)
		)
		(tPath dispose:)
		(curStr dispose:)

		(for ((= i 0)) (< i (dirList size:)) ((++ i))
			((= curStr (FileNameStr format: "<%s>" (dirList at: i)))
				state: ((dirList at: i) state?)
			)
			(self sort: curStr)
			((dirList at: i) dispose:)
		)
		(dirList release:, dispose:)

		(for ((= i 0)) (< i (fileList size:)) ((++ i))
			(self sort: (fileList at: i))
		)
		(fileList  release:, dispose:)

	)

	(method (update newFileStr &tmp str sz oldBits)
		(if argc
			(curFileStr copy: newFileStr)
			(self	readDirectory: curDirStr curFileStr)
			(= start   0)
			(= curItem 0)
			(= sz (- (elements size:) itemsPerPage))
			
			(scrollBar
				bottomValue: (if (< sz 0) 0 else sz),
;**				setThumbSize: (Max 6 (/ (* itemsPerPage (scrollBar height:)) (Max itemsPerPage (elements size:)))),
				init:,
				draw:,
			)
		)
		(if directoryI
			(= str (SizeDirectoryStr directoryI curDirStr))
			(if (directoryI text?)
				((directoryI text?) dispose:)
			)
			(directoryI
				back: cyan,
				fore: black,
			)
			(directoryI
				text: (String with: str),
				draw:,
			)
;**			(curDirStr copy: str)
			(str dispose:)
		)
		(if fileNameI
			(fileNameI
				text: curFileStr,
				cursor: (curFileStr size:),
				draw:,
			)
		)
		(super update:)
	)

	(method (handleEvent event &tmp ret whichElem fileStr which prevCurItem tObj)
		(= ret FALSE)
		(cond
			;;; Handle hitting ENTER on fileNameI field
			((and
				fileNameI
				(& (fileNameI state?) dSelected)
				(== (event type?) keyDown)
				(== (event message?) ENTER)
				)
				(= fileStr (fileNameI text?))
				(if (or
						(!= (fileStr indexOf: `*) -1)
						(!= (fileStr indexOf: `?) -1)
					)
					(self update: fileStr)
					(event claimed: TRUE)
				else
					(event claimed: TRUE)
					;;; Make the okBtn as selected
					(client selectItem: okBtn)
				)
			)
			((or
				(event claimed?)
				(not (& state dActive))
				)
				0
			)
			;;; Handle selecting an item in the scrollBox
			((and
				(& (event type?) mouseDown)
				(scrollBox onMe: event)
				)
				(event claimed: TRUE)
				(= ret FALSE)
				(= which 
					(/ 
						(- (event y?) (scrollBox top?))
						(/ (scrollBox height:) itemsPerPage)
					)
				)
				(if (< (= whichElem (+ which start)) (elements size:))
					(= tObj (elements at: whichElem))
					((= fileStr (FileNameStr copy: (tObj text?)))
						state: ((tObj text?) state?)
					)
					;;; if clicked on a directory name, then change to that directory
					(if (fileStr test: FA_DIREC)
						(fileStr translate: "<" "")
						(fileStr translate: ">" "")
						(GoDirectory curDirStr fileStr)
						(self update: curFileStr)
					else
						;;; if do double click on a file, then act like clicked
						;;; on that file and then the OK button.
						(if (and
								allowDoubleClick
								(== curItem (elements at: whichElem))
								(< (- (GetTime) doubleClickTime) 20)
							)
							(= value (okBtn value?))
							;;; kludgy way to fake an event.
							;;; we change the event so will click on OK button.
							;;; this assumes OKBtn is after this FileSelector.
							(client selectItem: okBtn)
							(event
								type: keyDown,
								message: ENTER,
								claimed: FALSE
							)
						else
							(= doubleClickTime (GetTime))
							(self selectItem: whichElem)
							(= ret FALSE)
						)
					)
					(fileStr dispose:)
					(FrameOut)
				)
			)
			;;; Handle pressing up and down arrows when in scroll box
			((and
				(& state dSelected)
				(== (event type?) direction)
				)
				(= prevCurItem curItem)
				(super handleEvent: event)
				(switch (event message?)
					(dirN
						(if (!= prevCurItem curItem)
							(curFileStr 
								copy: (if curItem (curItem text?) else searchStr),
							)
							(self update:)
						)
					)
					(dirS
						(if (!= prevCurItem curItem)
							(curFileStr 
								copy: (if curItem (curItem text?) else searchStr),
							)
							(self update:)
						)
					)
				)
			)
			((super handleEvent: event)
				0
			)
		)
		(return ret)
	)
	(method (doit whichItem action &tmp str str2)
		(if (and (> argc 1) (== action #doit))
			(switch whichItem
				(fileNameI 
					0
				)
				(homeBtn 
					(curDirStr copy: homeStr)
					(self update: curFileStr)
				)
				(fileMaskBtn 
					(= str  (String with: "*.*"))
					(= str2 (String with: (fileMaskBtn text?)))
					(if (== (str2 compare: str) STR_EQUAL)
						(curFileStr copy: str)
						(fileMaskBtn text: searchStr)
					else
						(curFileStr copy: searchStr)
						(fileMaskBtn text: "*.*")
					)
					(str  dispose:)
					(str2 dispose:)
					(fileMaskBtn draw:)
					(self update: curFileStr)
				)
				(upDirBtn 
					(FlattenDirectory curDirStr)
					(self update: curFileStr)
				)
				(okBtn 
					(homeStr copy: curDirStr)
					(= value (okBtn value?))

					;;; If editted and hit enter then update curItem to curFileStr
					((= curItem (SelectorText new:))
						init: (String with: curFileStr)
					)
				)
				(cancelBtn 
					(= value (cancelBtn value?))
					(= curItem 0)
				)
				(else
					(super doit: whichItem action)
				)
			)
		else
			(= value curItem)
		)
		(return TRUE)
	)
)

(class FileNameStr of String
	(properties
		id 	name:			0
		int	state:		0
	)
	(methods
		set
		clear
		test
	)
	;; Set a state property
	(method (set bits)
		(return (|= state bits))
	)
	;; Clear a state property
	(method (clear bits)
		(return (&= state (~ bits)))
	)
	(method (test bits)
		(return (& state bits))
	)
	(method (compare fnStr &tmp c ch ch2)
		(if fnStr ;;(IsObject fnStr)
			(= c 0)
			(while (== (= ch (fnStr at: c)) (= ch2 (self at: c)))
				(++ c)
			)
			(return (- ch ch2))
		else
			(return (super compare: fnStr))
		)
	)
)

;;;
;;;
;;;
(procedure (SizeDirectoryStr item dirStr &tmp r str str1 changed itemWid itemFont fontHt)
	(= str (String with: dirStr))
	(= r (Rectangle new:))
	(= itemFont (item font?))

	;;; If cannot fit string in given width, then change string so
	;;; a "..." is before the directory string.
	(= str1 (String with: "..."))
	(r sizeToStr: str1 itemFont -1)
	(str1 dispose:)
	(= itemWid (- (item width:) (r width:)))
	(= changed FALSE)
	(= fontHt (Font FGetPointSize itemFont))

	;;; go through deleting characters until string fits on one line
	(repeat
		(r sizeToStr: str itemFont itemWid)

		(if (<= (r height:) fontHt)
			(break)
		else
			(= changed TRUE)
			(= str1 str)
			(= str (str1 subStr: 1 (- (str1 size:) 1)))
			(str1 dispose:)
		)
	)

	(r dispose:)

	(if changed
		(= str1 str)
		(= str (String format: "...%s" str1))
		(str1 dispose:)
	)
	(return str)
)

;;
;; Strip path off a file name
;;
(procedure (StripPath str &tmp i)
	(cond
		((> (= i (str indexOf: `\\ TRUE)) -1)
			(str move: 0 (++ i) -1)
		)
		((> (= i (str indexOf: `: TRUE)) -1)
			(str move: 0 (++ i) -1)
		)
	)
	(return str)
)

;;
;; Strip name from a file path
;;
(procedure (StripFileName str &tmp i)
	(or
		(> (= i (str indexOf: `\\ TRUE)) -1)
		(> (= i (str indexOf: `: TRUE)) -1)
	)
	(str at: (++ i) 0)
	(return str)
)

;; Replace a file name
(procedure (ReplaceFileName	path newName &tmp i)
	(cond
		((not (path size:))
			(path
				at: 0 `\\ 0,
				cat: newName,
			)
		)
		((or
			(> (= i (path indexOf: `\\ TRUE)) -1)
			(> (= i (path indexOf: `: TRUE)) -1)
		 )
			(path
				at: (+ i 1) 0,
				cat: newName,
			)
		)
		((FileIO FileValidPath path)
			(if (!= (path at: (- (path size:) 1)) `\\)
				(path at: (path size:) `\\ 0)
			)
			(path cat: newName)
		)
		(else
			(path copy: newName)
		)
	)
)

;;
;; Add a directory into path
;;
(procedure (GoDirectory path newDir &tmp nStr)
	(= nStr (String copy: path))
	(StripPath nStr)
	(StripFileName path)
	(path
		cat: newDir,
		cat: "\\",
		cat: nStr,
	)
	(nStr dispose:)
)
;;
;; Delete a directory from a path
;;
(procedure (FlattenDirectory path &tmp nStr i)
	(= nStr (String copy: path))
	(StripPath nStr)
	(StripFileName path)
	(if (not (path size:))
		(path
			at:	0 `\\ 0,
		)
	)
	(= i
		(path
			at: (- (path size:) 1) 0,	 ;; put 0 at end of path
			indexOf: `\\ TRUE				 ;; find first \ fromRight
		)
	)
	(cond
		((< i 1)
			(if (== (path at: 1) `:)
				(path at: 2 `\\)
			else
				(path at: 0 `\\ 0)
			)
		)
		((and (== i 2) (== (path at: 1) `:))
			(path at: (++ i) 0)
		)
		(else
			(path at: (++ i) 0)
		)
	)
	(path
		cat: nStr
	)
)




