
;*********************************************************************
;*	Desc: This module is for text files on the client side. 				
;*																							
;*			Contains: openPage, openFile, changePage																				
;*																							
;*********************************************************************

(module# READABLE_BOOK)

(include wobject.sh)
(include rlmrmmgr.sh)

(define	PAGE	450)
(define	MAXWORDS 70)
(define	EOL	13)
(define	NUMCHARS 350)

(local
	fileCursor = 0
	lABook = 0
	lReturnText = FALSE
	lDone = FALSE
)

(class PageObject of File
	(methods 
		setScript
	)
	(properties
		id	script	0
	)

	(method (doit)
		(super doit: &rest)
		(if script
			(script doit:)
		)
	)

	(method (setScript newScript)
		(if script
			(script dispose:)
		)
		
		(if newScript
			(newScript init: self &rest)	;init sets our script property!
		)

	)
)

(class Book of PageObject
	(methods 
		openPage
		openFile
		changePage
	)
	(properties
		int	pPageNum		0		;the current page num
		int	pMaxPage		0		;max number of Pages
		id		pText			NULL	;text to be displayed 
		int	pIsFile		FALSE	;
		int	pBookName	0
	)

	(method (init)
		(super init:&rest)

		(= pText (String new:))

		(if (not pPageNum)
			(= pPageNum 1)
		)
	)

	(method (openPage aWhatFile aNumOfPage &tmp tStr) 
		(if pIsFile 
			(= pMaxPage aNumOfPage)
			(return (self openFile: aWhatFile))
		)
	)

	;********************************************************************;
	;*																						  *;	
	;* This method initially opens the file and creates a text item 	  *;
	;*																						  *;	
	;* which is then displayed														  *;	
	;*																						  *;	
	;********************************************************************;

	(method (openFile aWhatName &tmp tItem tStr done wordCount lastChar)
																  
		;This where we assign the name of the file
		(= name aWhatName)
		(= lastChar TRUE)

		(if (== (self open: fRead) NULL)
			(Printf "Error opening file")
		 else
			;If it comes here then file was opened successfully

			(= pText (String new: PAGE))					;Read 500 or whatever PAGE 
																	;is defined to be bytes into the string
			(= tStr (String new:))
			(= done FALSE)
			(= wordCount 0)

			(while (not done)
				(if (== (self read: tStr 1) NULL) 
					(break)	;Reached EOF
				)

				(pText cat: tStr)
				(++ fileCursor)
				(self seek: fileCursor fileSeekBeg)

				(cond 
					((== (tStr at: 0) 32) 
						(++ wordCount)
						
						(if (>= wordCount MAXWORDS)
							(= done TRUE)
						)
						(= lastChar NULL)
					)
					((== (tStr at: 0) EOL)
						(if (== lastChar EOL)
							(+= wordCount 6)
						)
						(= lastChar EOL)
					)
				)

			)
		)

		(return pText)	
	)

	;********************************************************************;
	;*																						  *;	
	;* reading from file to perform page up and page down and 			  *;
	;*																						  *;	
	;* display the appropriate text on screen 							 	  *;
	;*																						  *;	
	;********************************************************************;

	(method (changePage newPage &tmp done wordCount tStr tStr2 destReached lastChar) 

		(= tStr (String new:))
		(= tStr2 (String new:))
		(= done FALSE)
		(= wordCount 0)
		(= fileCursor -1)
		(= destReached FALSE)
		(= lastChar NULL)


		(cond 
			((<= newPage 1)
				(= pPageNum: 1)
				(= fileCursor 0)
				(= destReached TRUE)
			)
			((>= newPage pMaxPage)
				(= pPageNum pMaxPage)
				(= fileCursor (* (- pMaxPage 1) NUMCHARS))
			)
			(else
				(= pPageNum newPage)
				(= fileCursor (* (- newPage 1) NUMCHARS))
			)
		)

		(while (not destReached)
			(++ fileCursor)
			(self seek: fileCursor fileSeekBeg)
			(if (== (self read: tStr 1) NULL) 
				;reached EOF
				(break)
			)
			(cond 
				((or (== (tStr at: 0) 32) (== (tStr at: 0) EOL))
					(= destReached TRUE)
				)
			)
		)

		(while (not done)
			(self seek: fileCursor fileSeekBeg)
			(if (== (self read: tStr 1) NULL) 
				;reached EOF
				(break)
			)
			(cond 
				((== (tStr at: 0) 32) 
					(++ wordCount)
					
					(if (>= wordCount MAXWORDS)
						(= done TRUE)
					)
					(= lastChar NULL)
				)
				((== (tStr at: 0) EOL)
					(if (== lastChar EOL)
						(+= wordCount 6)
					)
					(= lastChar EOL)
				)
			)
			(++ fileCursor)
			(tStr2 cat: tStr)
		)

		(= pText tStr2)
		(return pText)
	)

)

(instance sBookScript of Script
	(method (changeState newState)
		(switchto (= state newState)
			(
				(theGame handsOff:)
				(= lABook (BookPage new:))
				(server getBookInfo: ego lABook self)
			)
			(
				(server getBookPage: ego 1 lABook self)
			)
			(
				(client pMaxPage: (lABook pNumPages?))
				(client pText: (lABook pText?))

				(= lDone TRUE)
				(theGame handsOn:)
				(self dispose:)
			)
		)
	)
)

(class BookPage of Object
	(properties
		id		pText		0
		int	pNumPages	0
	)
)



