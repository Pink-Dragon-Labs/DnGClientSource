;;;;
;;;; PACKDATA.SC: Packed data array 
;;;;
;;;; Lets you put all kinds of data into an array, and then
;;;; retrieve it later. You must know how it has been put in
;;;; to get it out.
;;;;
;;;;	Example:
;;;;		(= packData (PackedData new:))
;;;;		(= byteArr  (PackedData with: 10 20 30))
;;;;		(packData putWord: 5)
;;;;	   (packData putArray: byteArr)
;;;;		(packData putByte: 1 2 3)
;;;;
;;;;		(= a   (packData getWord:))
;;;;		(= arr (packData getArray:))
;;;;		(= a   (packData getByte:))
;;;;

(module# PACKDATA)


(define PACKDATA_TYPE	255)

(class PackedData of ByteArray
	(properties
		int	getIndex: 0
		int	putIndex: 0
		int	bitPutIndex: 0
		int	bitGetIndex: 0
		int	pIsBigEndian	0
	)
	(methods
		getByte
		getWord
		getLong
		getArray
		getBits
		getString

		putByte
		putWord
		putLong
		putArray
		putBits
		putString

		peek
	)
	(method (new)
		(= getIndex (= putIndex (= bitPutIndex (= bitGetIndex 0))))
		(super new: &rest)
	)
	(method (dispose)
		(= getIndex (= putIndex (= bitPutIndex (= bitGetIndex 0))))
		(super dispose: &rest)
	)

	;;
	;;
	(method (getByte)
		(return (KPackData GetByte self))
	)
	;;
	;;
	(method (getWord startByte)
		(return (KPackData GetWord self))
	)

	;;
	;;
	(method (getLong aLongInt &tmp ret tLongInt tLowWord tHighWord)
		(if argc
			(= tLongInt aLongInt)

			(if (not tLongInt)
				(= tLongInt (LongInt new:))
			)
		else
			(= tLongInt (LongInt new:))
		)

		(KPackData GetLong self tLongInt)

		(return tLongInt)
	)
	;;
	;;
	(method (getArray arrType &tmp ret len arrayType i)
		(= len (self getLong:))

		(if (or (len less: 0) (< getIndex 0))
			(len dispose:)
			(return NULLID)
		)

		(= arrayType (self getLong:))
		
		(if (and argc (not (arrayType equal: arrType)))
			(Printf "Error in PackData getArray: got type=%d, wanted type=%d"
					arrayType arrType
			)
		)

		(= ret
			(switch (arrayType lowWord?)
				(WORDARRAY
					(IntArray new: (len lowWord?))
				)
				(IDARRAY
					(IDArray new: (len lowWord?))
				)
				(BYTEARRAY
					(ByteArray new: (len lowWord?))
				)
				(STRARRAY
					(String new: (len lowWord?))
				)
				(else
					(PackedData new: (len lowWord?))
				)
			)
		)
		(switch (arrayType lowWord?)
			(WORDARRAY
				(for ((= i 0)) (len greater: i) ((++ i))
					(ret at: i (self getWord:))
				)
			)
			(IDARRAY
				(for ((= i 0)) (len greater: i) ((++ i))
					(ret at: i (self getWord:))
				)
			)
			(STRARRAY
				(for ((= i 0)) (len greater: i) ((++ i))
					(ret at: i (self getByte:))
				)
				(ret at: (len lowWord?) 0)
			)
			(else
				(ret copyToFrom: 0 self getIndex? (len lowWord?))
				(+= getIndex (len lowWord?))
			)
		)

		(len dispose:)
		(arrayType dispose:)

		(return ret)
	)

	(method (getString aLen &tmp tLen tRet tIndex tByte tObj)
		(if argc
			(= tLen aLen)
		else
			(= tLen 0)
		)

		(= tObj (Clone String))
		(tObj data: (KPackData PackGetString self tLen))

		(return tObj)
	)

	(method (putString aString aLen &tmp tLen tRet tIndex)
		(if (> argc 1)
			(= tLen aLen)
		else
			(= tLen (aString size:))
		)

		(= tIndex 0)

		(self putWord: tLen)

		(while tLen
			(self putByte: (aString at: tIndex)) 

			(-- tLen)
			(++ tIndex)
		)
	)

	(method (getBits aBits &tmp tRetVal tBitIndex tVal)
		(return (KPackData GetBits self aBits))
	)

	;;
	;;
	(method (putByte args &tmp i)
		(if (>= putIndex 0)
			(for ((= i 0)) (< i argc) ((++ i))
				(KPackData PutByte self [args i])
			)
		)
	)
	;;
	;;
	(method (putWord args &tmp i)
		(if (>= putIndex 0)
			(for ((= i 0)) (< i argc) ((++ i))
				(KPackData PutWord self [args i])
			)
		)
	)
	;;
	;;
	(method (putLong args &tmp i)
		(if (>= putIndex 0)
			(for ((= i 0)) (< i argc) ((++ i))
				(if (== [args i] -1)
					(self putWord: -1 -1)
				else
					(KPackData PutLong self [args i])
				)
			)
		)
	)
	;;
	;;
	(method (putArray array arrSize &tmp len arrayType i)
		(= len 
			(LongInt new:
				(if (> argc 1)
					arrSize
				else
					(array size:)
				)
			)
		)

		(if (or (len less: 0) (< putIndex 0))
			(len dispose:)
			(return FALSE)
		)
		
		(= arrayType 
			(LongInt new:
				(if (array isKindOf: PackedData)
					PACKDATA_TYPE
				else
					(array type?)
				)
			)
		)

		(self
			putLong:	len,
			putLong:	arrayType,
		)

		(switch (arrayType lowWord?)
			(WORDARRAY
				(for ((= i 0)) (len greater: i) ((++ i))
					(self putWord: (array at: i))
				)
			)
			(IDARRAY
				(for ((= i 0)) (len greater: i) ((++ i))
					(self putWord: (array at: i))
				)
			)
			(STRARRAY
				(for ((= i 0)) (len greater: i) ((++ i))
					(self putByte: (array at: i))
				)
			)
			(else
				(self copyToFrom: putIndex array 0 (len lowWord?))
				(+= putIndex (len lowWord?))
			)
		)

		(len dispose:)
		(arrayType dispose:)

		(return TRUE)
	)
	;;
	;;
	(method (putBits aValue aBits &tmp tBitIndex tByte)
		(KPackData PutBits self aValue aBits)
	)

	;;
	;;	Allow you to peek at the next byte, word, long, array
	;;	without getting it.
	;;
	(method (peek what &tmp i beforeIndex ret)
	 	(= beforeIndex getIndex)

		;;; do the given method
		(= ret (Eval self what:))

		;;; player must dispose of array if do getArray

		(= getIndex beforeIndex)

		(return ret)
	)
)

