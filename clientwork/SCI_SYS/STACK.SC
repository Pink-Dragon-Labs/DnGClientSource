(module# STACK)

;; Class Stack: This is a general LIFO stack class. You can push and pop
;; valuse onto and off of it.
;;

(class Stack kindof List
	(properties
		int	top	0			;; this is the top of the stack
	)
		
	(methods 
		push
		pop
	)

	(method (push val &tmp i)
		(for ((= i 0)) (< i argc) ((++ i))
			(self addToFront: [val i])
		)

		(= top [val i])
	)

	(method (pop &tmp val)
		(if (self size:)
			(= val top)
			(= top (self at: 1))

			(self delete: val)
		else
			(= val 0)
			(= top 0)
		)

		(return val)
	)
)


