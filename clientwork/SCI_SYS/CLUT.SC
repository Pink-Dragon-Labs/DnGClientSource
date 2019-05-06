(module# CLUT)

(class Clut of ByteArray
	(methods
		map
	)
	(method (new &tmp clone)
		(= clone (super new: 256))
		(clone init:)
		(if argc
			(clone map: &rest)
		)
		(return clone)
	)
	(method (init &tmp i)
		(if (not data)
			(= data (self callKernel: ArrayNew 256 type))
		)
		(for ((= i 0))(< i 256)((++ i))
			(self at: i i)
		)
	)
	(method (map c &tmp i)
		(for	((= i 0))
				(< i argc)
				((+= i 2))

			(self at: [c i] [c (+ i 1)])
		)
	)
)
