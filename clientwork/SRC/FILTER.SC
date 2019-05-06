WARNING THIS IS DEAD CODE!!!!

;*************************** PROFANITY FILTER ***************************
;*
;*	Desc: A string is passed to the prof Filter and it replaces all the
;*			profane words from the string	with *s. 
;*       It reads in all its profane word vocabulary from the file named
;*			"filter.sh".
;*
;*       IF "filter.sh" IS CHANGED. The following things needs change
;*			
;*			- HEIGHT 		if a row is added.
;*			- WIDTH 			is a word that is added is bigger than any of 
;*								the existing ones. 
;*			- MAXLETTERS   if a new letter is added to InitCmpArr
;*			- InitCmpArray if a profane word is added starting with a 
;*								new letter
;*							   other tha the existing ones.
;*			- wordCountArr	The corresponding index of InitCmpArray contains
;*								the number of words that starts with the same letter
;*								as in the InitCmpArray.  
;*								This then obviuosly has to reflect the correct number
;*								of words.
;*
;**************************************************************************

(module# FILTER)

(include "wobject.sh")

(procedure
	Filter
	BuildArray
	FilterName
)

(public 
	Filter		0
	BuildArray	1
	FilterName	2
)

;**************************************************************************
;* 
;* Filter sensors any profanity that is contained in the string passed to it
;* The proc matches the first letter of the string with the first letter of 
;* of the probable profane words. 
;* If it matches it goes on to match the sec letter with the sec letter and 
;* so on.  If the match is complete the profane word is sensored by inserting
;* `* in the palce of the word.
;*
;**************************************************************************

(define MAX_BAD 26)

(local
	lBadTable	=	[
		"asswipe"
		"asshole"
		"bitch"
		"blowjob"
		"cock"
		"cunt"
		"clit"
		"chink"
		"dickhead"
		"dickwad"
		"fag"
		"fuck"
		"fvck"
		"gook"
		"homo"
		"kunt"
		"nigger"
		"penis"
		"pussy"
		"phuck"
		"schlong"
		"slut"
		"shit"
		"sphincter"
		"vagina"
		"whore"
	]

	lReplaceTable	=	[
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
		"****"
	]
)

(procedure (Filter aString &tmp i tStr tStr2 tFound tChar tCapStorage)	

	(= tStr 0)
	(= tStr2 0)

	(= tFound FALSE)

	(= tStr (String with: aString))
	(= tCapStorage (Set new:))

	(for ((= i 0)) (< i (tStr size:)) ((++ i))
		(= tChar (tStr at: i))
		(if (and 
				(>= tChar `A)
				(<= tChar `Z)
			)				
			(tCapStorage add: i)
		)
	)

	(KString StrLwr (StrData tStr))

	(= tStr2 (String with: tStr))

	(for ((= i 0)) (< i MAX_BAD) ((++ i))

		(KString StrTrn (StrData tStr) [lBadTable i] [lReplaceTable i] (StrData tStr2))

		(if (KString StrCmp (StrData tStr) (StrData tStr2) (tStr size:))
			(= tStr (String with: tStr2)) 
			(= tFound TRUE)
		)
	)
	
	(if tFound
		
		(= aString (strCopies tStr2))

		(for ((= i 0)) (< i (aString size:)) ((++ i))
			(if (tCapStorage contains: i)
				(aString at: i (toupper (aString at: i)))
			)
		)
	)

	(tStr dispose:)
	(tStr2 dispose:)
	(tCapStorage dispose:)

	(return aString)
)

(procedure (BuildArray)
	(return)
)

