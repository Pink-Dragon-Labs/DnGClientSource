;;;;
;;;;	COLLECT.SC: Collection classes.
;;;;
;;;;
;;;;	Classes:
;;;;		Collection
;;;;			List
;;;;				Set

(module# COLLECT)


(class Collection kindof Object
	;;; The Collection class provides the ability to manipulate
	;;; collections of objects.  Objects which belong to a Collection
	;;; are said to be elements or members of it.  The Collection class
	;;; has no particular order defined for its elements, so it should
	;;; not be used for situations in which the objects should be
	;;; ordered -- use a List instead.
	
	(properties
		name 		"Collect"
		elements	0			;pointer to a Kernal List (KList) containing elements
		size 		0		 	;the number of elements in the list
		nextNode	0			;next node for use in eachElementDo, etc.
	)
	(methods
		doit
		add					;add elements
		delete				;delete elements
		eachElementDo		;invoke a method in each element
		firstTrue			;return first element satisfying a condition
		firstEqual			;return first element with a property equal to a value
		allTrue				;return TRUE if all elements satisfy a condition
		contains				;return TRUE if an object is in the Collection
		isEmpty				;return TRUE if the list is empty (size == 0)
		first					;return first KNode of the Collection
		next					;return next KNode of the Collection
		release				;delete elements from list to deallocate all nodes
		isDuplicate			;return object ID if already in Collection, else FALSE
	)

	(method (new)
		(return
			((super new:)
				elements: (KList LNew &rest),
				size:		 0,
				yourself:
			)
		)
	)

	(method (doit)
		(self eachElementDo: #doit: &rest)
	)

	(method (add item &tmp obj n node)
		;If the Collection does not have a KList, create one for it.
		(if (not elements)
			(= elements (KList LNew))
		)
		
		;Loop over the arguments, adding each to the Collection.
		(for	((= n 0))
				(< n argc)
				((++ n))
			(if (not (self isDuplicate: [item n]))
				(KList LAddToEnd elements [item n])
				(++ size)
			)
		)
		(return self)
	)

	(method (delete item &tmp n)
		;Loop over arguments, deleting each from the Collection.
		(for ((= n 0)) (< n argc) ((++ n))
			(if (KList LDeleteKey elements [item n])
				(-- size)
			)
		)
		(return self)
	)

	(method (dispose)
		(if elements
			(self eachElementDo: #dispose:)
			(KList LDispose elements)
		)
		(= elements NULL)
		(= size 0)
		(super dispose:)
	)

	(method (first)
		(return (KList LAt elements 0))
	)

	(method (next node)
		(return (KList LNextNode node))
	)

	(method (isEmpty)
		(return (or (== elements NULL) (KList LEmpty elements)))
	)

	(method (contains anObject)
		(return (!= (KList LIndexOf elements anObject) -1))
	)

	(method (eachElementDo action &tmp node obj tRetVal)
		(= tRetVal (KList LEachElementDo elements action &rest))
		(= size (KList LSize elements))
		(return tRetVal)
	)

	(method (firstTrue action &tmp node obj tRetVal)
		(= tRetVal (KList LFirstTrue elements action &rest))
		(= size (KList LSize elements))
		(return tRetVal)
	)

	(method (firstEqual property value &tmp obj node ret i)
	;; Check each element of the Collection to see if its property
	;; 'prop' equals 'value'.  Return the object ID of the first
	;; element satisfying this.
		
		(= ret 0)
		(if elements
			(for ((= i 0)) (< i size) ((++ i))
				(= obj (KList LAt elements i))

				(if (not (IsObject obj))
					(continue)
				)

				(if 
					(and
						(obj respondsTo: property)
						(== (obj property) value)
					)
					(= ret obj)
					(break)
				)
			)
		)
		(return ret)
	)
	
	(method (allTrue action &tmp node obj tRetVal)
		(= tRetVal (KList LAllTrue elements action &rest))
		(= size (KList LSize elements))
		(return tRetVal)
	)

	(method (release &tmp node)
		;
		; Delete all elements from collection to deallocate list nodes
		(if 
			(and
				elements
				(KList LSize elements)
			)

			(KList LDispose elements)
			(= elements (KList LNew))
		)

		(= size 0)
	)

	(method (isDuplicate obj)
		;
		; Return FALSE (i.e. obj not already in list), since for class Collection
		;	and List we don't care about duplicates.  For class Set, this method
		;	will be over-ridden to determine if obj is already in the Set.

		(return FALSE)
	)
)



(class List kindof Collection
	;;; A List is a Collection which has a specified order to its
	;;; elements.  You will probably use the Set class (below) more
	;;; than List, as Set automatically weeds out duplicates.
	
	(methods
		at					;return the object at a given position in the List
		last				;return the last KNode of the List
		prev				;return previous KNode
		addToFront		;add object to front of List
		addToEnd			;add object to end of List
		addAfter			;add object after an element of List
		addBefore		;add object before another object in List
		indexOf			;return index of an object in a List
		union				;union lists with self and return as a new list
		intersection	;new list of what is in self and not in other lists
		duplicate		;duplicate each element in self return as new list
		replace			;replace element at posn n with element(s)
	)

	(method (at n &tmp node)
		(return (KList LAt elements n))
	)

	(method (last)
		(return (KList LLastNode elements))
	)

	(method (prev node)
		(return (KList LPrevNode node))
	)
	
	(method (addToFront obj &tmp n)
		(if (not elements)
			(= elements (KList LNew))
		)
		
		(for	((= n (- argc 1)))
				(<= 0 n)
				((-- n))
			(if (not (self isDuplicate: [obj n]))
				(KList LAddToFront elements [obj n])
				(++ size)
			)
		)
		(return self)
	)

	(method (addToEnd obj &tmp n)
		(if (not elements)
			(= elements (KList LNew))
		)
		
		(for	((= n 0))
				(< n argc)
				((++ n))
			(if (not (self isDuplicate: [obj n]))
				(KList LAddToEnd elements [obj n])
				(++ size)
			)
		)
		(return self)
	)

	(method (addAfter el obj &tmp n num insertNode)
		(-- argc)
		(for ((= n 0)) (< n argc) ((++ n))
			(if (not (self isDuplicate: [obj n]))
				(KList LAddAfter elements el [obj n])
				(++ size)
			)
		)
		(return self)
	)

	(method (addBefore el obj &tmp n num insertNode)
		(-- argc)
		(for ((= n 0)) (< n argc) ((++ n))
			(if (not (self isDuplicate: [obj n]))
				(KList LAddBefore elements el [obj n])
				(++ size)
			)
		)
		(return self)
	)

	(method (indexOf obj &tmp n node)
		(return (KList LIndexOf elements obj))
	)

	(method (union parms &tmp newList i curListSize whichParm)
	;;
	;; Union the List(s) passed to this method with self and return 
	;; the result as a new List. Duplicates will accumulate in the result
	;; unless self is a Set instead of a List.
	;;
		(= parms parms)
		(= newList (self duplicate:))

		(for ((= whichParm 0)) (< whichParm argc) ((++ whichParm))

			(= curListSize ([parms whichParm] size:))
			(for ((= i 0)) (< i curListSize) ((++ i))
				(newList add: ([parms whichParm] at: i))
			)
		)
		(return newList)
	)

	(method (intersection parms &tmp newList i curListSize curNode whichParm)
	;;
	;; Return a new list of what is unique in self that is not in any of
	;; the passed lists.
	;;
		(= parms parms)
		(= newList (self duplicate:))

		(for ((= whichParm 0)) (< whichParm argc) ((++ whichParm))

			(= curListSize ([parms whichParm] size:))
			(for ((= i 0)) (< i curListSize) ((++ i))	

				(= curNode ([parms whichParm] at: i))
				(if (newList contains: curNode)
					(newList delete: curNode)
				)
			)
		)
		(return newList)
	)

	(method (duplicate &tmp dupList i mySize)
	;;
	;;	Make a duplicate list (not a clone) of self. 
	;;
		(= mySize (self size:))
		(= dupList (self new:))

		(for ((= i 0)) (< i mySize) ((++ i))
			(dupList add: (self at: i))
		)
		(return dupList)	
	)

	(method (replace oldElem newElem)
	;;
	;;	Replace one element with one or more others
	;;
		(self addAfter: oldElem newElem &rest)
		(self delete: oldElem)
	)
)



(class Set kindof List
	;;; A Set is a kind of List which does not contain duplicate
	;;; elements: adding an object to a Set which already contains the
	;;; object does not change the Set.  This eliminates the
	;;; possibility of a whole class of errors based on multiple
	;;; occurances of an object in a collection.  Most system
	;;; collections (the cast, etc.) are Sets.
	
	(method (isDuplicate obj)
		(return (self contains: obj))
	)
)


