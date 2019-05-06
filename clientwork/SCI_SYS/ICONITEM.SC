;;;;
;;;;	ICONITEM.SC - Icon Item for displaying view
;;;;
;;;;

(module# ICONITEM)


(class IconItem kindof Item
	;;; IconItems are simply a view/loop/cel combination created by the view editor.
	;;; 
	(properties
		id		clut:			0
		id		cycler: 		0		; a cycler must be attached dynamically
		int	cycleSpeed: 6		; 60ths second between cels. 
		int	signal: 		0		; just to satisfy cycler
	)

	(methods
		cycle
		lastCel			; required by cycler class
	)

	(method (init whoOwnsMe doCycle)
		(super init: (if argc whoOwnsMe else 0))

		;; setup any cyclers passed in
		(if (and
				(not cycler)
				(> argc 1)
				doCycle
			)
			((= cycler (Forward new:)) init: self)
		)
	)

	(method (cycle &tmp oldCel)
		(if cycler
			(= oldCel cel)
			(cycler doit:)
			(if (!= cel oldCel)
				(self draw:)
			)
		)
	)

	(method (dispose)
		(if cycler
			(cycler dispose:)
		)
		(super dispose:)
	)

	(method (setSize)
		(if (!= view -1)
			(self sizeToCel: view loop cel)
		)
	)

	(method (draw &tmp oldBM)
		(if (super draw:)
			(return)
		)

		(if (= oldBM bitmap)
			(DeleteScreenItem self)
			(gDeadBitmaps add: bitmap)
;;			(Bitmap MapDispose bitmap)
		)

		(= bitmap (CreateBitmap self))
;;		(Bitmap MapSetOrigin bitmap (CelInfo GetOrigX view loop cel) (CelInfo GetOrigY view loop cel))

		(if oldBM
			(AddScreenItem self)
		)
	)

	(method (showMouseOver)
		0
	)
	(method (handleEvent)
		(return FALSE)
	)

	(method (process)
		(return FALSE)
	)

	(method (contains)
		(return FALSE)
	)

	(method (lastCel)
		;;Return the number of the last cel in the current loop of this object.
		;;This method is called by the cycler.
		(return (- (NumCels self) 1))
	)
)

