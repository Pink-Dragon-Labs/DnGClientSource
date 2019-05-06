(module# CLUT_WORLD_EFFECT)

(include "wobject.sh")

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

;(class ClutWorldEffect of WorldEffect
;	(properties
;		id		pClut		NULL
;	)
;
;	(methods
;		setupClut
;	)
;
;	(method (init)
;		((= pClut (ClutOverride new:))
;			init: client,
;		)
;
;		(self setupClut:)
;		(super init: &rest)
;	)
;
;	(method (dispose)
;		(if pClut
;			(pClut dispose:)
;		)
;
;		(super dispose: &rest)
;	)
;
;	(method (setupClut))
;)


