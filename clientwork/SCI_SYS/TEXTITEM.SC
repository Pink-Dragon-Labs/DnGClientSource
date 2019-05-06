;;;;
;;;;	TEXTITEM.SC - Text Item for displaying text
;;;;
;;;;
;;;;	Classes:
;;;;		TextItem
;;;;			TitleItem
;;;;

(module# TEXTITEM)


(class TextItem kindof Item
	;;; A non-editable, generally non-selectable text field.

	(properties
		id	 text: 		0					;the text in the field
		int mode: 		JustLeft
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

		(if oldBM
			(AddScreenItem self)
		)
	)
	(method (setSize fixedWidth &tmp r r2 l t)
		(if text
			(= l left)
			(= t top)
			(= r (Rectangle new:))
			(r sizeToStr: (StrData text) font (if argc fixedWidth else gMaxTextWidth))
			(if (!= view -1)
				(= r2 (Rectangle new:))
				(r2 sizeToCel: view loop cel)
				(r union: r2)
				(r2 dispose:)
			)
			(self copy: r)
			(r dispose:)
			(self moveTo: l t)
		)
	)
	(method (dispose)
		(if (== (MemoryInfo MemGetType text) MemObject)
			(text dispose:)
			(= text 0)
		)
		(super dispose:)
	)
)

(class TitleItem of TextItem
	;;; A non-editable, non-selectable text field that appears on
	;;; the top of a dialog and is usually as wide as the dialog.

;**	(method (setSize)
;**		(super setSize: &rest)
;**		(if client
;**			(self width: (client width:))
;**		)
;**	)
)

