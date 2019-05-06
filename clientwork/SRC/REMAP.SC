(module# REMAP)

(class RemapObj of Object
	(properties
		int	pColor		-1
		int	pPercent		0
		int	pCount		1
	)

	(method (init aColor aPercent)
		(= pColor aColor)
		(= pPercent aPercent)

		(RemapColors RemapByPct aColor aPercent)
	)

	(method (dispose)
		(RemapColors RemapOff pColor)
		(super dispose:)
	)
)

(class Remaps of List
	(method (add aColor aPercent &tmp tObj)
		(= tObj (self firstEqual: #pColor aColor))

		(if tObj
			(if (!= aPercent (tObj pPercent?))
				(Fatal "Remaps::add ( %d, %d ): color overwrite" aColor aPercent)
			)

			(tObj pCount: (+ (tObj pCount?) 1))
		else
			((= tObj (RemapObj new:))
				init: aColor aPercent,
			)

			(super add: tObj)
		)
	)

	(method (delete aColor &tmp tObj)
		(= tObj (self firstEqual: #pColor aColor))

		(if tObj
			(tObj pCount: (- (tObj pCount?) 1))

			(if (not (tObj pCount?))
				(tObj dispose:)
				(super delete: tObj)
			)
		)
	)
)

