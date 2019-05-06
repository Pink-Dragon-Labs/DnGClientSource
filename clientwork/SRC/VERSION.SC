;;---------------------------------------------------------------------------
;;
;; VERSION.SC
;; (c) Sierra On-Line, Inc, 1996
;;
;; Author: Stephen Nichols
;;
;; Description:
;;
;;		This file contains the class that provides version information about
;;		this project.  This is a separate module so as to facilitate easier
;;		downloading of a smaller file.
;;
;; Usage:
;;	
;;		To see what the version number is just use the following code fragment:
;;
;;		(= version (GameVersion pMinorVersion?))
;;
;;	Example:
;;	
;;		(Printf "The current version is %d.%d" (GameVersion pMajorVersion?) (GameVersion pMinorVersion?)) 
;;
;; Code Map:
;;
;; 	Classes: 
;;			GameVersion
;;
;; 	Procedures: 
;;			none
;;
;;		Instances:
;;			none
;;
;; History:
;;
;;		02/06/1996: Stephen Nichols
;;						
;;			Created the GameVersion class.  Oooh, how hard.
;;

(module# VERSION)

(class GameVersion of RootObj
	(properties
		int	pMajorVersion 3
		int pMinorVersion 448
	)
)
