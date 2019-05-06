(module# SYSCLASS)

(class Object kindof RootObj)
(class Collection kindof Object
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
)
(class List kindof Collection)

(class WDragger kindof Object
	(properties
		id		pObj 0
	)
)

