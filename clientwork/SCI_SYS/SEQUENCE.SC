;;;
;;; SEQUENCE.SC: Sequences -- a quick way through lists!
;;;
;;; The Sequence class allows iteration over a Collection in a controlled
;;; manner.  You should use it when you desire to access the members of a
;;; Collection rather than stepping through the collection yourself.
;;; Its much quicker than going by the at: method and easier and more 
;;; readable than using Kernel calls.
;;; ---------------------------------------------------------------------
;;; Example of Usage:
;;;		(= seq (Sequence new: theList))
;;;		(while (= obj (seq next:)
;;;			;; process obj
;;;		)
;;;		(seq dispose:)
;;;	OR
;;;		(= seq (Sequence new: theList))
;;;		(for ((= obj (seq begin:)) obj (= obj (seq next:))
;;;			;; process obj
;;;		)
;;;		(seq dispose:)
;;;	

(module# SEQUENCE)


(define 	AT_START	0)
(define	AT_END	-1)

(class Sequence kindof Object
	(properties
		id		data		0	;; data elements for list
		id		nextNode 0	;; its really current node, but used as in Collection/List/Set
	)

	(methods
		next			;; get the next node in the sequence
		prev			;; get the previous node in the sequence
		begin			;; return beginning node; start sequence here
		end			;; return end node; end sequence here
		set			;; set sequence pointer to passed object
		value			;; return value of current node
	)

	(method (new list &tmp newObj)
		((= newObj (super new:))
			data:     list,
		)
		(return newObj)
	)

	(method (next &tmp tNode)
		(+= nextNode 1)
		(= tNode nextNode)

		(if (< tNode (data size:))
			(return (data at: tNode))
		else
			(return 0)
		)
	)

	(method (prev &tmp tNode)
		(switch nextNode
			(AT_END
				(= nextNode (- (data size:) 1))
				(= tNode nextNode)
 			)
			(else
				(-= nextNode 1)
				(= tNode nextNode)
			)
		)

		(if (>= tNode 0)
			(return (data at: tNode))
		else
			(return 0)
		)
	)

	(method (begin)
		(= nextNode AT_START)
	)

	(method (end)
		(= nextNode AT_END)
	)

	(method (set obj &tmp seqObj)
		;;Set the sequence pointer to the passed object.

		(self begin:)
		(while
			(and
				(= seqObj (self next:))
				(!= obj seqObj)
			)
		)
		(return (== obj seqObj))
	)

	(method (value)
		(if (and (< nextNode (data size:))
					(>= nextNode 0)
			 )
			(return (data at: nextNode))
		else
			(return 0)
		)
	)
)

