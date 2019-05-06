(module# WDRAGGER)

(class WDragger of Object
	(properties
		id		pObj		0		;; object that I am dragging
		id		pView		0		;; view of the object that I am dragging
		id		pPlane	0
		int   x        0
		int   y        0
		int	pOfsX		0
		int	pOfsY		0

		int	pDragging	0	
	)

	(methods
		startDragging
		stopDragging
		handleEvent
	)
	
	(method (dispose)
		(self stopDragging:)
		(super dispose: &rest)
	)

	(method (startDragging aWhatObj aWhatBitmap &tmp tEvent tBitmap)
		;; calculate the offset for dragging this image
		(= tEvent (user curEvent?))
		(= pOfsX (- (aWhatObj x?) (tEvent x?)))
		(= pOfsY (- (aWhatObj y?) (tEvent y?)))

		(KDialog DialogDrag ddStart aWhatObj pOfsX pOfsY)
		(= pDragging TRUE)
	)

	(method (stopDragging)
		(= pDragging FALSE)
		(KDialog DialogDrag ddEnd)
	)
)
