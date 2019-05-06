;;---------------------------------------------------------------------------
;;
;; LONGINT.SC
;; (c) Sierra On-Line, Inc, 1995
;;
;; Author: Stephen Nichols
;;
;; Description:
;;
;;		This file contains a class that represents 32-bit long integers.
;;		The class relies on the LongInt kernel call implicitly.
;;
;; Usage:
;;	
;;		The LongInt class is used like any other class.  You must make a
;; 	clone of it before using it.  Each mathematical operation (add, sub
;;		mul and div) has two methods of use.  The first allows you to pass
;;		just a word value or both a lowWord/highWord combination.  The second
;;		method handles manipulating LongInt's with LongInt's (i.e. adding
;;		two LongInt's).
;;
;;	Example:
;;	
;;		;; create a new LongInt object with the value $abcdef
;;		(= tLongA (LongInt new: $AB $CDEF))
;;
;;		;; add the word value 666 to it
;;		(tLongA add: 666)
;;
;;		;; subtract the hi/lo word value $bcdef from it
;;		(tLongA sub: $b $cdef)
;;
;;		;; add the LongInt to itself
;;		(tLongA addTo: tLongA)
;;
;; Code Map:
;;
;; 	Classes: 
;;			LongInt
;;
;; 	Procedures: 
;;			none
;;
;;		Instances:
;;			none
;;
;; History:
;;
;;		11/12/1995: Stephen Nichols
;;						
;;			Initial revision.
;;
;;---------------------------------------------------------------------------

(module#	LONGINT)

(class LongInt of Object
	(properties
		int	lowWord	0
		int	hiWord	0
	)

	(methods
		set
		setLong
		add
		addLong
		sub
		subLong
		mul
		mulLong
		div
		divLong

		bitAND
		bitANDLong
		bitOR
		bitORLong
		bitXOR
		bitXORLong
		bitNOT
		bitSHL
		bitSHR

		setBit
		resetBit
		toggleBit
		getBit

		swap
		negate
		abs

		equal
		equalLong
		less
		lessLong
		lessEqual
		lessEqualLong
		greater
		greaterLong
		greaterEqual
		greaterEqualLong

		asStr
		fromStr
	)

	(method (new &tmp tClone)
		(= tClone (super new:))

		(if argc
			(tClone set: &rest)
		else
			(tClone set: 0)
		)

		(return tClone)
	)

	(method (set)
		(return (KLong LongSet self &rest))
	)

	(method (setLong aLong)
		(return (KLong LongSet self (aLong lowWord?) (aLong hiWord?)))
	)

	;; This method is called to add either a word or hi/lo word combination
	;; to this LongInt.
	;;
	(method (add)
		(return (KLong LongAdd self &rest))
	)

	;; This method is called to add a LongInt to this LongInt.
	;;
	(method (addLong aLong)
		(return (KLong LongAdd self (aLong lowWord?) (aLong hiWord?)))
	)

	;; This method is called to subtract either a word or hi/lo word
	;; combination from this LongInt.
	;;
	(method (sub)
		(return (KLong LongSub self &rest))
	)

	;; This method is called to subtract a LongInt from this LongInt.
	;;
	(method (subLong aLong)
		(return (KLong LongSub self (aLong lowWord?) (aLong hiWord?)))
	)

	;; This method is called to multiply this LongInt by either a word or
	;; hi/lo word combination.
	;;
	(method (mul)
		(return (KLong LongMul self &rest))
	)

	;; This method is called to multiply this LongInt by another LongInt
	;;
	(method (mulLong aLong)
		(return (KLong LongMul self (aLong lowWord?) (aLong hiWord?)))
	)

	;; This method is called to divide this LongInt by either a word or
	;; hi/lo word combination
	;;
	(method (div)
		(return (KLong LongDiv self &rest))
	)

	;; This method is called to divide this LongInt by another LongInt.
	;;
	(method (divLong aLong)
		(return (KLong LongDiv self (aLong lowWord?) (aLong hiWord?)))
	)

	;; This method is called to perform a bitwise AND on this LongInt
	;; with a hi/lo word combination.  Single arguments are not allowed.
	;;
	(method (bitAND)
		(return (KLong LongAnd self &rest))
	)

	;; This method is called to perform a bitwise AND on this LongInt 
	;; with another LongInt.
	;;
	(method (bitANDLong aLong)
		(return (KLong LongAnd self (aLong lowWord?) (aLong hiWord?)))
	)

	;; This method is called to perform a bitwise OR on this LongInt
	;; with a hi/lo word combination.  Single arguments are not allowed.
	;;
	(method (bitOR)
		(return (KLong LongOr self &rest))
	)

	;; This method is called to perform a bitwise OR on this LongInt 
	;; with another LongInt.
	;;
	(method (bitORLong aLong)
		(return (KLong LongOr self (aLong lowWord?) (aLong hiWord?)))
	)

	;; This method is called to perform a bitwise XOR on this LongInt
	;; with a hi/lo word combination.  Single arguments are not allowed.
	;;
	(method (bitXOR)
		(return (KLong LongXor self &rest))
	)

	;; This method is called to perform a bitwise XOR on this LongInt 
	;; with another LongInt.
	;;
	(method (bitXORLong aLong)
		(return (KLong LongXor self (aLong lowWord?) (aLong hiWord?)))
	)

	;; This method is called to perform a bitwise complement on this LongInt.
	;;
	(method (bitNOT)
		(return (KLong LongNot self))
	)

	;; This method is called to set a particular bit of this LongInt.
	;;
	(method (setBit aWhatBit)
		(return (KLong LongSetBit self aWhatBit))
	)

	;; This method is called to reset a particular bit of this LongInt.
	;;
	(method (resetBit aWhatBit)
		(return (KLong LongResetBit self aWhatBit))
	)

	;; This method is called to toggle a particular bit of this LongInt on
	;; and off.  The new state of the bit is returned.
	;;
	(method (toggleBit aWhatBit)
		(return (KLong LongToggleBit self aWhatBit))
	)

	;; This method is called to get the value of a bit of this LongInt.
	;;
	(method (getBit aWhatBit)
		(return (KLong LongGetBit self aWhatBit))
	)

	;; This method is called to perform a bitwise shift of this LongInt
	;; n places to the left.  The sign of this long is ignored for
	;; purposes of the shift.
	;;
	(method (bitSHL aHowFar)
		(return (KLong LongShiftLeft self aHowFar))
	)

	;; This method is called to perform a bitwise shift of this LongInt
	;; n places to the right.  The sign of this long is ignored for
	;; purposes of the shift.
	;;
	(method (bitSHR aHowFar)
		(return (KLong LongShiftRight self aHowFar))
	)

	;; This method is called to swap the hi/lo bytes of this LongInt
	(method (swap)
		(return (KLong LongSwap self))
	)

	;; This method is called to negate this LongInt.
	;;
	(method (negate)
		(return (KLong LongNeg self))
	)

	;; This method is called to calculate the absolute value of this 
	;; LongInt and modify it's lo/hi word.
	;;
	(method (abs)
		(return (KLong LongAbs self))
	)

	;; This method is called to compare this LongInt to either a word
	;; or hi/lo word combination.  If the LongInt is equal to the
	;; passed value, return TRUE.
	;;
	(method (equal)
		(return (KLong LongEqual self &rest))
	)

	;; This method is called to compare this LongInt to another LongInt.
	;; If the two LongInt's are equal, return TRUE.
	;;
	(method (equalLong aLong)
		(return (KLong LongEqual self (aLong lowWord?) (aLong hiWord?)))
	)

	;; This method is called to compare this LongInt to either a word
	;; or hi/lo word combination.  If the LongInt is less than the
	;; passed value, return TRUE.
	;;
	(method (less)
		(return (KLong LongLess self &rest))
	)

	;; This method is called to compare this LongInt to another LongInt.
	;; If this LongInt is less than the passed LongInt, return TRUE.
	;;
	(method (lessLong aLong)
		(return (KLong LongLess self (aLong lowWord?) (aLong hiWord?)))
	)

	;; This method is called to compare this LongInt to either a word
	;; or hi/lo word combination.  If the LongInt is less than or equal to 
	;; the passed value, return TRUE.
	;;
	(method (lessEqual)
		(return (KLong LongLessEqual self &rest))
	)

	;; This method is called to compare this LongInt to another LongInt.
	;; If this LongInt is less than or equal to the passed LongInt, 
	;; return TRUE.
	;;
	(method (lessEqualLong aLong)
		(return (KLong LongLessEqual self (aLong lowWord?) (aLong hiWord?)))
	)

	;; This method is called to compare this LongInt to either a word
	;; or hi/lo word combination.  If the LongInt is greater than the
	;; passed value, return TRUE.
	;;
	(method (greater)
		(return (KLong LongGreater self &rest))
	)

	;; This method is called to compare this LongInt to another LongInt.
	;; If this LongInt is greater than the passed LongInt, return TRUE.
	;;
	(method (greaterLong aLong)
		(return (KLong LongGreater self (aLong lowWord?) (aLong hiWord?)))
	)

	;; This method is called to compare this LongInt to either a word
	;; or hi/lo word combination.  If the LongInt is greater than or equal to 
	;; the passed value, return TRUE.
	;;
	(method (greaterEqual)
		(return (KLong LongGreaterEqual self &rest))
	)

	;; This method is called to compare this LongInt to another LongInt.
	;; If this LongInt is greater than or equal to the passed LongInt, 
	;; return TRUE.
	;;
	(method (greaterEqualLong aLong)
		(return (KLong LongGreaterEqual self (aLong lowWord?) (aLong hiWord?)))
	)

	;; This method is called to create and return a String object that 
	;; contains the ASCII representation of this LongInt.  The aSigned
	;; flag tells the kernel whether to consider the LongInt as signed
	;; or not.
	;;
	(method (asStr aSigned &tmp tStr)
		(= tStr (String new:))
		(KLong LongAsStr self (tStr data?) (if argc aSigned else 1))
		(return tStr)
	)

	;; This method is called to convert an ASCII string to it's LongInt
	;; representation.  
	;;
	(method (fromStr aStr)
		(return (KLong LongFromStr self (StrData aStr)))
	)
)


