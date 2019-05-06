;;;
;;; CURSREG.SC: Cursor Region Handling
;;;---------------------------------------------------------------------
;;;
;;;
;;;
;;;
;;;
;;;
;;;
;;;
;;;
;;;
;;;
;;;
;;;---------------------------------------------------------------------

(module# CURSREG)


(class CursorRegionController of Set
	(properties
		id 	curRect	  				0 ;; the current cursorRegion that cursor is in
		int 	specialCursorInUse	0 ;; this is for special cases
	)
	(methods
		reset
	)
	(method (init)
		(if (!= cursRegController self)
			(if cursRegController
				(cursRegController dispose:)
			)
			(= cursRegController self)
			(GlobalDemons add: self)

			(super init:)
		)
	)
	(method (dispose)
		(GlobalDemons delete: self)
		(= curRect 0)

		(super dispose:)
		(= cursRegController 0)
	)
	;;; update the cursor every doit: if it needs to
	(method (doit forceUpdate &tmp i reg cEvt cx cy newRect)
		(= cEvt (user curEvent?))
		(= cx (cEvt x?))
		(= cy (cEvt y?))

		(cond
			;;; if no user controls or not nullEvt, then exit
			((and
				;;; if force update, then always do
				(not (and (== argc 1) forceUpdate))
				(or
					(== (user controls?) FALSE)
					(!= (cEvt type?) nullEvt)
				)
				(return)
			  )
			)
			;;; if current mouse cursor has moved outside the active region
			;;; then do an update if it needs to.
			;;; if no curRect or cursor not in curRect, then update curRect
			;;; to new one.
			;;;
			((or
					(not curRect)
					(not (and		;;; this hardcoded for speed (-> contains:)
						 	(<= (curRect top?)  cy (curRect bottom?))
						 	(<= (curRect left?) cx (curRect right?))
					  	))
				)
				(= curRect 0)
				(for ((= i 0)) (< i (self size:)) ((++ i))
					(if (and
							(= reg (self at: i))
							(reg contains: cx cy)
						)
						(if (= curRect (reg inWhichList: cEvt))
							;;; make this cursorRegion the current cursor
							(curRect doit:)
						)
					)
				)
			)
			(else
				;;;
				;;; if has moved to other rect within, then update curRect
				;;;
				(if (and 
						curRect 
						(!= (= i (curRect inWhichList: cEvt)) curRect)
						i
					)
					((= curRect i) doit:)
				)
			)
		)
	)
	(method (reset)
		(= curRect 0)
		(self doit: TRUE)
	)

)

(class CursorRegion of Rectangle
	(properties 
		int aCursor	0 	;; pointer to Cursor instance
		id list		0
	)
	(methods
		add
		delete
		eachElementDo
		inWhichList
		setCursor
	)
	(method (init client)
		(super init:)

		(if (not aCursor)
			;; which cursor does this go to
			(= aCursor (Cursor new: &rest))
			(= list (Set new:))
		)

		(if argc
			(client add: self)
		)
	)
	(method (dispose)
		(aCursor dispose:)
		(= aCursor 0)

		(if list 
			(list dispose:)
			(= list 0)
		)

		(if cursRegController
			(cursRegController delete: self)
		)
		(super dispose:)
	)
	(method (doit)
		;;; what will do if cursor is in
		(if (!= theCursor aCursor)
			(theGame setCursor: aCursor TRUE)
			(theCursor draw:)
		)
	)
	(method (add)
		(if list
			(list add: &rest)
		)
	)
	(method (delete)
		(if list
			(list delete: &rest)
		)
	)
	(method (eachElementDo)
		(if list
			(list eachElementDo: &rest)
		)
	)
	(method (inWhichList whichEvt &tmp i r evt)
	 	(= evt (if argc whichEvt else (user curEvent?)))
		(if (or (not list) (not (list size:)))
			(return 
				(if (and (<= top (evt y?) bottom) (<= left (evt x?) right)) 
					 self
				else
					 FALSE
				)
			)
		)
		(for ((= i 0)) (< i (list size:)) ((++ i))
			(if ((= r (list at: i)) inWhichList: evt)
				(return r)
			)
		)
		(return self)
	)
	(method (setCursor)
		(if (not aCursor)
			(= aCursor (Cursor new:))
		)
		(aCursor setCursor: &rest)
	)
)



