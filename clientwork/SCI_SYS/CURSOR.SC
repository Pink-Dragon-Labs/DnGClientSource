;;;
;;; CURSOR.SC: Cursor class for controlling the mouse cursor.
;;;
;;;     The following class has been implemented to standardize the 
;;; cursor use and to make it easy to have your own game cursors
;;; without worrying too much of handling them. This current cursor
;;; mechanism supports setting the cursor to any view, loop, or cel too
;;; so you do not have to put all cursors in one view. 
;;; 
;;; -------------------------------------------------------------------
;;;

(module# CURSOR)


;;;
;;; This is a list of active cursors that handles making sure
;;; the one on top of the "stack" is active and drawn.
;;;
(instance ActiveCursorList kindof Set
	(method (addToFront)
		(super addToFront: &rest)
		(self doit:)
	)
	(method (delete)
		(super delete: &rest)
		(self doit:)
	)
	(method (doit)
		(if (!= theCursor (self at: 0))
			;;; This will draw the topmost cursor 
			((= theCursor (self at: 0))
				draw:
			)
		)
	)
)


(class Cursor kindof Rectangle
	;;;
	;;; The Cursor class is an object used to control the user's cursor.
	;;;
	(properties
		int top			0
		int left			0
		int bottom		200
		int right		320
		id  client		0
		int view			0
		int loop			0
		int cel			0
		int x				0
		int y				0
		int hotX			-1
		int hotY			-1
	)
	(methods
		init				; intialize the cursor
		dispose			; dispose this cursor
		draw				; draw this cursor
		posn				; move cursor to a new x,y position
		show				; pass TRUE to show, FALSE to hide
		setCursor		; set cursor to specified view/loop/cel
		enable			; enable this as possible to be used
		disable			; disable this cursor from being used
	)

	(method (init theClient)
		(if argc
			(= client theClient)
		else
			(= client ActiveCursorList)
		)
	)
	(method (dispose)
		(self disable:)
		(super dispose:)
	)
	(method (draw)
	 	(SetCursor view loop cel)
	)
	(method (posn theX theY)
		;
		; Move the cursor to a specific set of coordinates
		;

 		(SetCursor theX theY)
	)
	(method (show trueOrFalse)
		;
		; Show (or hide) the cursor if 'trueOrFalse' is TRUE (FALSE)

		(SetCursor (if argc trueOrFalse else TRUE))
	)
	(method (setCursor v l c)
		;
		; Create a new cursor with view, loop, cel
		;
		(if (> argc 1)
			(= view v)
			(= loop l)
			(= cel  c)
		else
			(= view (v view?))
			(= loop (v loop?))
			(= cel  (v cel?))
		)

		(if (< loop 0)
			(Fatal "setCursor called with loop of %d" loop)
		)
	)
	(method (enable)
	 	(if client
			(client addToFront: self)
		)
	)
	(method (disable)
		;;; Get rid of cursor from list.
		;;; Do not get rid of last cursor, though.
	 	(if (and
				client
				(> (client size:) 1)
			)
			(client delete: self)
		)
	)
)


