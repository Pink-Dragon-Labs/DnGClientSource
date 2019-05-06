(module# CLUTOVERRIDE)

(include "wobject.sh")

(local
	lRemapTable = [
		11
		17
		20
		26
		29
		37
		42
		44
		cWhite
		cOlive
		cBlue
		cRed
		cLightRed
		cYellow
		cMagenta
		cLeather
		cAqua
		cGreen
		cOrange
		cPurple
		100
		cFlesh
		-1
	]
)

(class ClutOverride of Clut
	(properties
		id 	pClut 		NULL		;; this is the clut that we are overriding
		id 	pClient		NULL		;; this is the WorldObject to which we are attached
		int	pPriority	0			;; this allows us to have multiple clut overrides
	)

	(methods
		selfAt
	)

	(method (new)
		(return (Clone self))
	)

	(method (init aClient &tmp i)
		(if (not data)
			(= data (self callKernel: ArrayNew 256 type))
		)

		(for ((= i 0))(< i 256)((++ i))
			(self selfAt: i i)
		)

		(= pClient aClient)
		(= pClut (aClient clut?))
		(aClient clut: self)
	)

	(method (dispose)
		(if pClient
			(pClient clut: pClut)
		)

		(super dispose: &rest)
	)

	;; make sure that any modifications to the clut get passed up to
	;; the one that we are chained to
	(method (at)
		(if pClut
			(return (pClut at: &rest))
		)
	)

	;; this method allows the programmer to set this object's clut information
	(method (selfAt)
		(return (super at: &rest))
	)
)

(class InvisibleClut of Clut
	(method (init &tmp i)
		(super init: &rest)

		(= i 0)

		(while (!= [lRemapTable i] -1)
			(self at: [lRemapTable i] 240 241 242 243)
			(++ i)
		)

		;; Hide eyes.
		(self at: 15 240 241)

		;; Hide lips.
		(self at: 24 240)

		(self at: cBrown 243 242 241 240)
	)
)

(class FrozenClut of Clut
	(method (init &tmp i)
		(super init: &rest)

		(= i 0)

		(while (!= [lRemapTable i] -1)
			(self at: [lRemapTable i] cBlue (+ cBlue 1) (+ cBlue 2) (+ cBlue 3))
			(++ i)
		)

		(self at: cBrown (+ cBlue 3) (+ cBlue 2) (+ cBlue 1) cBlue)
	)
)


