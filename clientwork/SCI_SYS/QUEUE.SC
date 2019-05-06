(module# QUEUE)

(class Queue kindof List
	(methods
		put
		get
		peek
	)

	(method (dispose)
		(self release:)
		(super dispose: &rest)
	)

	(method (put)
		(self add: &rest)
	)

	(method (get &tmp tRetVal)
		(= tRetVal (self at: 0))
		(self delete: tRetVal)

		(return tRetVal)
	)

	(method (peek)
		(return (self at: 0))
	)
)
