;;;;
;;;;	STRPROC.SC: miscellaneous STRing PROCedures
;;;;
;;;;	(These will be in memory if you use any of these procedures.)
;;;;

(module# STRPROC)


(procedure
	toupper
	tolower
	isspace
	isdigit
	isalpha
	isalnum
	isprint
	ispunct
	iscntrl
	isxdigit
	ishappy
	stricmp
	strCopies
	strFitTo
	SetRangeUpperLower
	FixVerb
)

(public
	toupper		0
	tolower		1
	isspace		2
	isdigit		3
	isalpha		4
	isalnum		5
	isprint		6
	ispunct		7
	iscntrl		8
	isxdigit		9
	ishappy		10
	stricmp		11
	strCopies	12
	strFitTo		13
	SetRangeUpperLower 14
	FixVerb		15	
)

;; Return uppercase alphabetical equivalent
(procedure (toupper c)
	(return (if (<= `a c `z) (- c 32) else c))
)

;; Return lowercase alphabetical equivalent
(procedure (tolower c)
	(return (if (<= `A c `Z) (+ c 32) else c))
)

;; White space or control?
(procedure (isspace c)
	(return
		(or
			(== c 32)
			(== c 13)
			(== c 10)
			(== c 9)
		)
	)
)

;; Is it a nasty control character?
(procedure (iscntrl c)
	(return (or (< c 32) (> c 127)))
)

;; Is a character printable?
(procedure (isprint c)
	(return (<= 32 c 126))
)

;; Is it a decimal digit
(procedure (isdigit c)
	(return (<= `0 c `9))
)

;; Is it an alphabetical char
(procedure (isalpha c)
	(return
		(or
			(<= `A c `Z)
			(<= `a c `z)
		)
	)
)

;; Alphanumeric?
(procedure (isalnum c)
	(return
		(or
			(isdigit c)
			(isalpha c)
		)
	)
)

;; Is it punctuation of some sort?
(procedure (ispunct c)
	(return
		(and
			(!= c 32)
			(isprint c)
			(not (isalnum c))
		)
	)
)

;; A hexidecimal digit?
(procedure (isxdigit c)
	(return
		(or
			(isdigit c)
			(<= `a c `f)
			(<= `A c `F)
		)
	)
)

;; Is it a happyface code?
(procedure (ishappy c)
	(return (<= 22 c 31))
)


;;
;; Perform a non-case sensitive comparison
;;	Also supports compToFrom style comparison
;;
(procedure (stricmp str1 str2 pStart pFinish &tmp c tBeg tEnd)
	(if (> argc 2)
		(= tBeg pStart)
 	else
		(= tBeg 0)
	)
	(if (> argc 3)
		(= tEnd pFinish)
 	else
		(= tEnd (str1 size:))
		(if (!= tEnd (str2 size:))
			(return FALSE)
		)
		(-- tEnd)
	)
	(for ((= c tBeg)) (<= c tEnd) ((++ c))
		(if (!= (toupper (str1 at: c)) (toupper (str2 at: c)))
			(return FALSE)
		)
	)
	(return TRUE)
)


(procedure (strCopies whatToCopy howMany &tmp numCopies str i)
;;
;;	Returns a new string of 'howMany' copies of the passed 'whatToCopy'. 
;;

		(= numCopies
			(if (< argc 2)
				1
			else
				howMany
			)
		)

;**		(= str (String newWith: (* numCopies (StrSize whatToCopy))))
		(= str (String newWith: (* numCopies (KString StrLen whatToCopy))))
		(for ((= i 0)) (< i numCopies) ((++ i))
			(str cat: whatToCopy)
		)
		(return str)
)

(procedure (strFitTo str extent fnt &tmp r maxWid maxHt targetFont sizer curChar lastChar)
;;
;;	Fit string to a single line of text, n pixels wide (n must be less than 320)
;;	Defaults to using userFont
;; Returns new string size
;;
	
	(= targetFont (if (> argc 2) fnt else userFont))
	(= r
		(if (extent isKindOf: Rectangle)
			extent
		else
			((Rectangle new:)
				sizeToStr: "w" targetFont extent,
				yourself:
			)
		)
	)
	(= maxWid (r width:))
	(= maxHt  (r height:))
;**	(if (not (IsObject extent))
;**		(r dispose:)
;**	)

	(= curChar (- (str size:) 1))
	(= sizer (Rectangle new:))
	(repeat
		(sizer sizeToStr: str targetFont	maxWid)
		(breakif
			(or
				(< curChar 0)
				(<= (sizer height:) maxHt)
			)
		)
		(str at: curChar 0)
		(-- curChar)
	)
	(sizer dispose:)
	(return curChar)
)

;; KDS. Will set a range of characters in a string to upper case.
;; Index string starts at 0.

;; Parameters (optional parms in <>) are: 
;;		aString	= string or string object to pass in
;;		<aOpt>		= Upper case FALSE (default) or lower case TRUE
;;		<aStart>		= Set start of range. If no finish given,
;;						  only 1 character is set.
;;		<aFinish>	= Set end of range. 

;; upper case examples:

;;	(= tStr (String with: "junk"))
;; (SetRangeUpperLower tStr) ;; returns: Junk

;;	(= tStr (String with: "junk"))
;; (SetRangeUpperLower tStr FALSE 1 3) ;; returns: jUNK

;; lower case examples:

;;	(= tStr (String with: "JUNK"))
;; (SetRangeUpperLower tStr TRUE) ;; returns: Junk

;;	(= tStr (String with: "JUNK"))
;; (SetRangeUpperLower tStr TRUE 1 3) ;; returns: Junk

(procedure (SetRangeUpperLower aString aOpt aStart aFinish &tmp i tBegin tEnd)
	
	;; if my optional argument is true, set the range to lower case
	;; otherwise default to upper case conversion.

	(if (and
			(> argc 1)
			(not (== aOpt 1))
		)
		(= aOpt 0)
	)

	(if (and
			(> argc 2)
			(>= (aString size:) aStart)
		)
		(= tBegin aStart)
 	else
		(= tBegin 0)
	)	

	(if (and
			(> argc 3)
			(>= (aString size:) aFinish)
		)
		(= tEnd aFinish)
 	else
		(= tEnd tBegin)
	)

	(for ((= i tBegin)) (<= i tEnd) ((++ i))
		(if aOpt
			(aString at: i (tolower (aString at: i)))
		else
			(aString at: i (toupper (aString at: i)))
		)
	)
	(return aString)
)

;; KDS fixes up verb text with strings ending in s

(procedure (FixVerb aString &tmp tStr)

	(if
		(or
 			(IsObject aString?)
			(== (MemType aString?) MemText)
		)
	
		(= aString (String with: aString))
		(= tStr (String with: (aString right: 2)))

		(if (and
				(KString StrCmp "ss" (StrData tStr) 2) 
				(== (aString at: (- (aString size:) 1)) `s)
			)			
			(tStr dispose:)
			(aString dispose:)
			(return "are")
		else
			(tStr dispose:)
			(aString dispose:)
			(return "is")
		)
	else
		(return "Unknown item is")
	)
)

