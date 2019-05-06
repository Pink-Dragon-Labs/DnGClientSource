;;---------------------------------------------------------------------------
;;
;; BSHOP.SC
;; (c) Sierra On-Line, Inc, 1995
;;
;; Author: Stephen Nichols
;;
;; Description:
;;
;; 	This file contains the base component class that represents a shop.
;;		A shop is simply an entity on the server that allows the user to
;;		buy objects, spells or services.
;;
;; Usage:
;;
;;		none
;;	
;;	Example:
;;	
;;		none
;;
;; Code Map:
;;
;; 	Classes: 
;;			BShop
;;
;; 	Procedures: 
;;			none
;;
;;		Instances:
;;			none
;;
;; History:
;;
;;		12/11/1995: Stephen Nichols
;;				 
;;			Setup initial framework for BHead class.  Implemented verb dialogs.		
;;
;;---------------------------------------------------------------------------

(module# BSHOP)

(include "wobject.sh")

(define _VSELL_MENU 11100)
(define _VREPAIR_MENU 11125)

(class BShop of WorldObjectBase
	(properties
		int	pType		_BSHOP
	)

	(method (buildVerbList aWhatObj &tmp tVerbList tBase)
		
		(= tVerbList VerbList)

		;; if the user clicks on me, I will allow a buy

		(if (aWhatObj isKindOf: WorldEgo)
			(tVerbList add: BUY_V)
			(tVerbList add: _VSELL_MENU)
			(tVerbList add: _VREPAIR_MENU)
		else
			(if (and
					(aWhatObj isKindOf: WorldObject)
					(ego owns: aWhatObj 1)

					(or
						(and
							(aWhatObj pMoney?)
							(== (aWhatObj pBaseView?) _MANA_VIEW)
						)

						(not (aWhatObj pMoney?))
					)
				)
				(if (not
						(and
							(= tBase (aWhatObj getBase: BWearable))
							(tBase pIsWorn?)
						)
					)
					(tVerbList add: SELL_V)
				)
				(if (aWhatObj getBase: BUse)
					(tVerbList add: RECHARGE_V)
				)
				(if 
					(or
						(and
							(= tBase (aWhatObj getBase: BWearable))
							(not (tBase pIsWorn?))
						)
						(aWhatObj hasBase: BMix)
					)

					(tVerbList add: REPAIR_V)
				)
			)
		)
	)
)


