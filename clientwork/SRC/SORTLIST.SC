(module# SORTED_LIST)

(define ASCENDING 1)
(define DESCENDING -1)

;A list class that sorts the objects added

(class SortedList of List
  (properties
     int pSortSelector 0          ;The selector to sort elements by
     int pSortDirection ASCENDING ;1 for ascending -1 descending
  )
  (methods
     findSlot
  )
	(method (add item &tmp obj n node)
		(if (not elements)
			(= elements (KList LNew))
		)
		

		;Loop over the arguments, adding each to the Collection.
		(for	((= n 0)) (< n argc) ((++ n))
			(if (not (self isDuplicate: [item n]))
				(if (= obj (self findSlot: [item n]))
              (self addBefore: obj [item n])
           else
              (self addToEnd: [item n])
           )
			)
		)
		(return self)
	)
	(method (findSlot item &tmp obj node ret tSelector i tSize)
		(= ret 0)
     (if (not (= tSelector pSortSelector))
        (return NULL)
     )
		(if elements
			(= tSize size)

			(for ((= i 0)) (< i tSize) ((++ i))
				(= obj (KList LAt elements i))

				(cond 
					((not (obj respondsTo: tSelector))
                 NULL
              )
              ((and (== pSortDirection  ASCENDING)
                    (>= (obj tSelector?) (item tSelector?))
               )
                 (= ret obj)
                 (break)
              )
              ((and (== pSortDirection  DESCENDING)
                    (<= (obj tSelector?) (item tSelector?))
               )
                 (= ret obj)
                 (break)
              )
				)
			)
		)
     (return ret)
	)
)
