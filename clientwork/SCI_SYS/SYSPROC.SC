;;;;
;;;;	SYSPROC.SC: System general procedures
;;;;
;;;;	Some general-purpose simple procedures.
;;;;

(module# SYSPROC)


(procedure 
	sign
	umod
	Min
	Max
	OneOf
	Eval
	AltKeyLookup
	CountBits
)

(public
	sign				0
	umod				1
	Min				2
	Max				3
	OneOf				4
	Eval				5
	AltKeyLookup 	6
	CountBits		7
)

(procedure (sign x)
	;return +1 if arg is positive, -1 if negative, 0 if 0
	(return 
		(if (< x 0) -1 else (> x 0))
	)
)

(procedure (umod n modN)
	(-= n (* modN (/ n modN)))
	(if (< n 0) (+= n modN))
	(return n)
)

(procedure (Min nums &tmp otherMin)
	;;n-ary Min by P. Ghenis
	(if (or (== argc 1) (< nums (= otherMin (Min &rest))))
		nums
	else 
		otherMin
	)
)

(procedure (Max nums &tmp otherMax)
	;;n-ary Max by P. Ghenis
	(if (or 
			(== argc 1) 
			(> nums (= otherMax (Max &rest)))
		)
		nums
	else
		otherMax
	)
)

(procedure (OneOf what things &tmp i)
	;
	; Determine if what is one of things

	(for
		(	(= i 0)	)
		(< i (- argc 1))
		(	(++ i)	)
		
		(if (== what [things i]) (return (or what TRUE)))
	)
	(return FALSE)
)

(procedure (Eval obj sel)
	;
	; Way of delaying binding until runtime
	(if 
		(and
			(IsObject obj)
			(obj respondsTo: sel)
		)

		(return (obj sel &rest))
	else
		(return 0)
	)
)

(procedure (AltKeyLookup what &tmp i ret)
	(= ret 0)
	(if (not altKeyTable)
 		(= altKeyTable 
			(ByteArray with:
			 	30 48 46 32 18 33 34 35 23 36 
				37 38 50 49 24 25 16 19 31 20 
				22 47 17 45 21 44
			)
		)
	)
	(= i 0)

	;;; If pass in 'a'-'z' then return alt-key equivalent
	(if (< what 255)
		(= ret (<< (altKeyTable at: (- (toupper what) `A)) 8))
	;;; if pass in alt-'a' throught alt-'z' then return alpha('a'-'z')
	else
		(if (!= (= i (altKeyTable indexOf: (>> what 8))) -1)
			(= ret (+ `A i))
		)
	)
	(return ret)
)

;; this function counts the number of set bits in the passed value
;;
(procedure (CountBits aWhatVal &tmp i tRetVal)
	(= tRetVal 0)

	(for ((= i 0)) (< i 16) ((++ i))
		(+= tRetVal (> (& aWhatVal (<< 1 i)) 0))
	)

	(return tRetVal)
)

