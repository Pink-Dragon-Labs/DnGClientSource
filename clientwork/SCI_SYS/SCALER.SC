;;;;
;;;;	SCALER.SC: Scale an actor
;;;;
;;;;
;;;;	Classes:
;;;;		Scaler
;;;;

(module# SCALER)


(class Scaler kindof Code
	(properties
		id  client		0		; who we are attached to
		int frontY		190	; y coordinate to stop scaling up
		int backY		0		; y coordinate to stop scaling down
		int frontSize	100	; % size at frontY
		int backSize	0		; % size at backY
		int slopeNum 	0		; numerator of slope - difference in %
		int slopeDen 	0		; denominator of slope - difference in y
		int const 		0		; constant adjustement for result %
		int yLast		-999	; Last y position
	)
	(method (init who fs bs fy by)
		(if argc
			(= client who)
			(= frontSize 	fs)
			(= backSize 	bs)
			(= frontY		fy)
			(= backY			by)
		)
		(= slopeNum (- frontSize backSize))
		(if (not (= slopeDen (- frontY backY)))
			(Print "<Scaler> frontY cannot be equal to backY")
			(return FALSE)
		)
		(= const (- backSize (/ (* slopeNum backY) slopeDen) ))
		(self doit:)
	)
	(method (doit aOverride &tmp theY newScale tAdjX tAdjY tPercent)
		(= theY (client y?))
		(if 
			(or 
				(and
					argc
					aOverride
				)

				(!= theY yLast)
			)

			(cond
				((< theY backY)
					(= newScale backSize) 
				)
				((> theY frontY)
					(= newScale frontSize) 
				)
				(else
					(= newScale (+ (/ (* slopeNum theY) slopeDen) const))			 
				)
			)
			
			(= yLast theY)
			(= tAdjX (/ (* newScale (client xScaleAdj?)) 100))
			(= tAdjY (/ (* newScale (client yScaleAdj?)) 100))

			(client
				scaleX: (Max 1 tAdjX),
				scaleY: (Max 1 tAdjY),
			)
			(client -info-: (| (client -info-) GRAPH_UPD))
		)
	)
)

