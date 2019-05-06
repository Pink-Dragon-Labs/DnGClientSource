(module# SHADOW)

(class ShadowText of TextItem
	(properties
		int	fore				255
		int	font				666
		int	pShadowColor	0
		id		caller			0
		id		pTimer			0
		id		pObj				NULL
	)

	(methods
		follow
	)

	(method (centerOn aWhatObj &tmp tObjW tObjH tCurRoomPlane tTop)
		(if (not (IsObject aWhatObj))
			(return)
		)

		(= pObj aWhatObj)

		(SetNowSeen pObj)

		(= tObjW (pObj width?))
		(= tObjH (pObj height?))

		(= tTop (pObj top?))

;		(if (pObj respondsTo: #pHeadingCue)
;			(-= tTop (* (pObj pHeadingCue?) 15))
;			(pObj pHeadingCue: (+ (pObj pHeadingCue?) 1))
;		)

		(super centerOn: (- (pObj right?) (/ tObjW 2)) tTop)

		;; be sure the window stays on screen

		(= tTop (+ (CelHigh vInfoBar lInfoBar 0) 1))

		(if (< left 0)
			(self moveTo: 0 top) 
		)

		(if (< top tTop)
			(self moveTo: left tTop)
		)

		(= tCurRoomPlane (curRoom plane?))

		(if (> right (tCurRoomPlane right?))
			(self moveBy: (- (tCurRoomPlane right?) right) 0)
		)

		(if (> bottom (tCurRoomPlane bottom?))
			(self moveBy: 0 (- (tCurRoomPlane bottom?) bottom))
		)

		(= x left)
		(= y top)

		(UpdateScreenItem self)
	)

	(method (follow)

;**		(+= x (- (pObj x?) (pObj xLast?)))
;**		(+= y (- (pObj y?) (pObj yLast?)))

		(self centerOn: pObj)

;**		(UpdateScreenItem self)
	)

	(method (init aText aWhatX aWhatY aDelay aCaller aCast)
		(self setPri: 10000)

		(= text (String with: aText))
		(self setSize:)

		(if (> argc 5)
			(super init: aCast)
		else
			(super init:)
		)

		(self moveTo: aWhatX aWhatY)
		(UpdateScreenItem self)

		(if (> argc 3)
			(if (!= aDelay -1)
				((= pTimer (GlobalTimer setReal: self aDelay))
					cueSelector: #dispose,
				)
			)

			(if (> argc 4)
				(= caller aCaller)
			)
		)
	)

	(method (dispose)
		(if pTimer
			(pTimer client: 0)
		)

		(if caller
			(GlobalTimer setCycle: caller 1)
		)

		(if (and
				(IsObject pObj?)
				(pObj isKindOf: WorldObject)
				(== (pObj pMsgStr?) self)
			)
			(pObj pMsgStr: 0)
			(= pObj 0)
		)
		(super dispose: &rest)
	)

	(method (setSize)
		(super setSize: &rest)

		(self 
			widen: 1,
			heighten: 1,
		)
	)

	(method (draw &tmp oldBM)
		(if (= oldBM bitmap)
			(DeleteScreenItem self)
			(gDeadBitmaps add: bitmap)
;;			(Bitmap MapDispose bitmap)
		)

		(= bitmap 
			(Bitmap MapCreate
				(+ (self width:) 32)
				(+ (self height:) 32)
				(self skip?)
				(self back?)
				(plane resX?)
				(plane resY?)
			)
		)

		(Bitmap MapAddText
			bitmap
			(StrData text)
			1 1 (self width:) (self height:)
			pShadowColor back skip font mode borderColor dimmed
			0
		)

		(Bitmap MapAddText
			bitmap
			(StrData text)
			0 0 (self width:) (self height:)
			fore back skip font mode borderColor dimmed
			0
		)

		(if oldBM
			(AddScreenItem self)
		)
	)
)


