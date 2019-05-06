;;;
;;; TELEPORT.SC: Teleport to where want to go
;;;
;;; This works with a 'teleport' file. Can have:
;;;		? = let choose
;;;		# = goto module#
;;;		# # # = goto virtual room #,#,#

(module# TELEPORT)

(public
	Teleport 0
)

(instance Teleport of StaticRoom
	(method (init &tmp tpFile str i nr firstChar isNetworked)
		(super init: &rest)

		(= nr START)
		(= isNetworked (= firstChar 0))

		;;; if read in file, then process
		((= tpFile (File new:)) 
			name: "teleport"
		)
		(if (tpFile open: fRead)
			(= str (String new:))
			(tpFile readString: str 10)
			(= firstChar (str at: 0))

			(if (= isNetworked 
					(or
						(== (toupper firstChar) `N)
						haveConnection
					)
				)
				(str at: 0 32)
			)

			(cond
				((== firstChar `?)
					(str at: 0 0)
					(if (Print "Teleport to which room:"
							#p_btnColor: red grey
							#p_edit:  str 6
							#p_button:"OK" 	 1
							#p_button:"Cancel" 0
						)
						(= nr (str asInteger:))
					)
				)
				(isNetworked
				 	(cond
						((== prevRoomNum LOGON)
							(= nr NETLOGIN)
						)
						(else
							(= nr 777)
						)
					)
				)
				((= i (str asInteger:))
					(= nr i)
				)
			)
			(str dispose:)
		)
		(tpFile dispose:)

		;;;
		;;; will this work here or should put in script?
		;;;
		(= newRoomNum nr)
	)
)

