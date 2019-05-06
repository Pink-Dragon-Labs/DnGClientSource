;;;;
;;;;	EVTHNDLR.SC: Event handlers
;;;;
;;;;	A Set that passes an event to each member until the event is claimed.
;;;;
;;;;	Classes:
;;;;		Event
;;;;		  Cast
;;;;

(module# EVTHANDLER)

(class EventHandler kindof Set

	(methods
		handleEvent
	)

	(method (handleEvent event &tmp i tSize)
		;; Invoke the 'action' method of each element of the List
		;; with an arbitrary number of parameters.
		
		(= tSize (self size?))

		(for ((= i 0)) (< i tSize) ((++ i))
			((self at: i) handleEvent: event)
			(if (event claimed?)
				(break)
			)
		)

		(return (event claimed?))
	)

)

(class Cast kindof EventHandler
	(properties
		id  plane		0		; plane we belong to
	)
	(method (dispose)
		(if plane
			((plane casts?) delete: self)
			(= plane 0)
		)
		(super dispose:)
	)
)

