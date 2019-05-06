;;;;
;;;;	FILE.SC
;;;;
;;;;	The File class allows you to open and write to a file on disk. 
;;;;	This is useful for logging user input for which you have no
;;;;	response in the development or beta-test phase, writing utilities
;;;;	which allow you to position Actors on a picture and then write
;;;;	out the coordinates, etc.
;;;;
;;;;	Classes:
;;;;		File

(module#	FILE)

(class File kindof Object
	(properties
		id  handle	0				;private -- the OS's handle for the open file
	)

	(methods
		open						;open/create file
		readString				;read a string from the file
		writeString				;write a string to the file
		write						;write some data to the file
		read						;read some data from the file
		seek						;set file position
		close						;close file
		delete					;delete the file
		rename					;rename the file
		getByte					;get one byte from file
		putByte					;write one byte to file
		getWord					;get one word from file
		putWord					;write one word to file
	)

	(method (open mode)
		;; Open the file.  'mode' is the mode in which to open the file:
		;;		mode =
		;;			fAppend		Appends to end of file.  Default if 'mode'
		;;							is not specified.
		;;			fRead			Positions at start of file.
		;;			fTrunc		Truncates the file to zero length when opened.
		;;
		;;	open:returns the File ID if file is opened successfully,NULL otherwise.

		(= handle
			(switch argc
				(0
					(FileIO FileOpen (StrData name) fAppend)
				)
				(1
				 	(FileIO FileOpen (StrData name) mode)
				)
				(else
					0
				)
			)
		)
		(if (== handle -1)
			(= handle 0)
		)
		(return (if handle self else NULL))
	)

	(method (write arr len &tmp i)
		;; Write len bytes to the file from arr.

		;Open the file if it is not presently open.
		(if (not handle)
			(self open:)
		)

		(return
			(if handle (FileIO FileWrite handle (ArrayData arr) len) else 0)
		)
	)

	(method (writeString str &tmp i)
		;; Write the string pointed to by 'str' to the file.  Multiple strings
		;; may be sent in one call.

		;Open the file if it is not presently open.
		(if (not handle)
			(self open:)
		)

		;Multiple writes accepted.
		(if handle
			(for ((= i 0)) (< i argc) ((++ i))
				(if (not (FileIO FileFPuts handle (StrData [str i])))
					(return FALSE)
				)
			)
		)
		(return TRUE)
	)

	(method (read arr len)
		;; Read len bytes from a file into arr

		(if (!= argc 2)
			(return 0)
		)

		;Open the file if it is not presently open.
		(if (not handle)
			(self open:fRead)
		)

		(return
			(if handle (FileIO FileRead handle (ArrayData arr) len) else 0)
		)
	)

	(method (readString str len &tmp strLen)
		;; Read a line (of maximum length len) from a file.

		(if (!= argc 2)
			(return 0)
		)

		;Open the file if it is not presently open.
		(if (not handle)
			(self open: fRead)
		)
		(= strLen (Min len (KArray ArraySize (StrData str))))

		(return
			(if handle (FileIO FileFGets (StrData str) strLen handle) else	NULL)
		)
	)
	
	(method (getByte)
		(return (FileIO FileGetByte handle))
	)

	(method (putByte b &tmp i)
		(for ((= i 0)) (< i argc) ((++ i))
			(FileIO FilePutByte handle [b i])
		)
	)

	(method (getWord)
		(return (FileIO FileGetWord handle))
	)

	(method (putWord w &tmp i)
		(for ((= i 0)) (< i argc) ((++ i))
			(FileIO FilePutWord handle [w i])
		)
	)

	(method (rename newName &tmp rc)
		; return 0 if successful
		(= rc (FileIO FileRename name newName))

		; DEBUG! What to do here?
		(if (not rc)
			(= name newName)
		)
		(return rc)
	)

	(method (seek offset mode &tmp theMode)
		; Change the position in the file where the next read or write will
		;	occur.  The new position is specified by means of the mode and offset.
		;	if the mode is,
		;		fileSeekBeg		the offset is relative to the beginning of the
		;							file (default)
		;		fileSeekCur		the offset is relative to the current position
		;		fileSeekEnd		the offset is relative to the end of the file
		;
		;	the offset can be negative (for fileSeekCur and fileSeekEnd modes)
		; the new file position is returned

		(= theMode (if (>= argc 2) mode else fileSeekBeg))

		;Open the file if it is not presently open.
		(if (not handle)
			(self open:fRead)
		)

		(return (if handle (FileIO FileSeek handle offset theMode) else 0))
	)

	(method (close)
		;; Close the file.  This makes sure that all writes which were made
		;; actually go to the disk file.

		(if handle
			(FileIO FileClose handle)
			(= handle 0)
		)
	)
	
	(method (delete)
		(if handle
			(self close:)
		)
		(return (FileIO FileUnlink name))
	)

	(method (dispose)
		(self close:)
		(super dispose:)
	)
)
