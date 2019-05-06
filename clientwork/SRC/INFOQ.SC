(module# INFOQ)

(class InfoQueue of Set
	(properties
		int	pText		0
		int	pDelay	5
		int	pEnabled	TRUE
		int	pChannel	0
		int	pShutUp	TRUE	;; can this be enabled disabled?
		id		pObj		NULL	;; used to locate text on an object
		int	pColor	255	;; default text color
	)

	(methods 
		cue
		clear
	)

	(method (cue &tmp tDelay tRoom tPlane tShadowText)
		;; set the current room to be ego's room
		(= tRoom curRoom)

		(if ego
			(= curRoom (ego pRoom?))
		)

		;; get the current plane...
		(if curRoom
			(= tPlane (curRoom plane?))
		else
			(= tPlane 0)
		)

		;; zero out any temps...
		(= tShadowText 0)

		(if pText
			(self delete: pText)
			(pText dispose:)
		)

		(if (and tPlane (= pText (self at: 0)))
			(if 
				(and
					ego
					(not ((theIconBar plane?) isHidden?))
				)

				(cond
					((> (self size:) 15)
						(= tDelay 1)
					)
						
					((> (self size:) 10)
						(= tDelay (/ pDelay 3))
					)

					((> (self size:) 5)
						(= tDelay (/ pDelay 2))
					)

					(else
						(= tDelay pDelay)
					)
				)

				(if 
					(and
						(IsObject pText)
						(pText isKindOf: String)
					)

					((= tShadowText (ShadowText new:))
						fore: pColor,
						init: pText (tPlane right?) (+ (tPlane bottom?) 16) tDelay self  ;; + 16 is a kludge for placement
					)

				else
					(self delete: pText)
					(= pText (String with: "Bad string in InfoQueue."))
					(self addToFront: pText)

					((= tShadowText (ShadowText new:))
						init: pText (tPlane right?) (+ (tPlane bottom?) 16) tDelay self ;; + 16 is a kludge for placement
					)
				)
			else
				(= pText NULL)

				((GlobalTimer setReal: self 1)
					cueSelector: #cue,
				)
			)
		)

		;; make sure the shadow text is on screen...
		(if (and tShadowText tPlane)
			(if (> (tShadowText right?) (tPlane right?))
				(tShadowText moveBy: (- (tPlane right?) (tShadowText right?)) 0)
				(UpdateScreenItem tShadowText)
			)
		)

		;; restore the old curRoom
		(= curRoom tRoom)
	)

	(method (add aString &tmp tSize tStr)
		(if (not pEnabled)
			(aString dispose:)
			(return)
		)

		(if 
			(and
				(IsObject aString)
				(aString isKindOf: String)
			)

			(super add: aString)

			(if (== (self size:) 1)
				((GlobalTimer setReal: self 1)
					cueSelector: #cue,
				)
			)
		else
			(= tStr (String format: "Bad string added to InfoQueue (0x%x)" aString))

			(Print tStr
				#p_title "DOH!"
				#p_button "Okay" 0
			)
		)
	)

	(method (clear aOverride)
		(if 
			(or
				pText

				(and
					argc
					aOverride
				)
			)

			(if pText
				(self delete: pText)
			)

			(self eachElementDo: #dispose)
			(self release:)

			(if pText
				(super add: pText)
			)
		)
	)
)
