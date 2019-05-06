;;;
;;; FORMFILE.SC: Formatted (Text) File
;;;
;;; This is just a simple way to add tabs, formatting to a text file.
;;; Further enhancements could be added as needed.
;;;
;;;
(module# FORMFILE)

(class FormattedFile of File
	(properties
		int	pTabs		0
		int	pUseTabs	TRUE
	)
	(methods
		incTabs
		decTabs
	)
	(method (writeString str &tmp i tabStr)
		(if pTabs
			(= tabStr (String new:))
			(for ((= i 0)) (< i pTabs) ((++ i))
				(if pUseTabs
					(tabStr cat: "\t")
				else
					(tabStr cat: "___")
				)
			)
			(tabStr at: (* pTabs (if pUseTabs 1 else 3)) 0)

			(super writeString: tabStr str &rest)
			(tabStr dispose:)
		else
			(super writeString: str &rest)
		)
	)
	(method (incTabs)
		(++ pTabs)
	)
	(method (decTabs)
		(if (< (-- pTabs) 0)
			(= pTabs 0)
		)
	)
)


