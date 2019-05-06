(module# BWEAPON)

(include "wobject.sh")

;; BWeapon: Any world object that is a weapon should have this 
;; as a base.
;;

(class BWeapon of BWearable
	(properties
		int	pType				_BWEAPON
		int	pSize				_WEAPON_MEDIUM
		int	pDamageType		_DAMAGE_SLASH
		int	pSpeed			5
		int	pPointBonus		0
		int	pAreaWorn		_WA_WEAR_WEAPON
	)
	
	(method (bePutOn aWhatObj &tmp tRetVal tView tFightingStance)
		(switch (aWhatObj pAction?)
			(_WOA_CLUB_STANCE
				(= tFightingStance 1)
			)

			(_WOA_DAGGER_STANCE
				(= tFightingStance 1)
			)

			(_WOA_SWORD_STANCE
				(= tFightingStance 1)
			)

			(else 
				(= tFightingStance 0)
			)
		)

		(= tView (pSelf pBaseView?))

		;; find the valid damage type
		(cond
			((ResCheck RES_VIEW (+ tView _WOA_CLUB_STANCE))
				(= pDamageType _DAMAGE_BLUDGEON)
				(if tFightingStance
					(aWhatObj setAction: _WOA_CLUB_STANCE)
				)

			)

			((ResCheck RES_VIEW (+ tView _WOA_DAGGER_STANCE))
				(= pDamageType _DAMAGE_PIERCE)
				(if tFightingStance
					(aWhatObj setAction: _WOA_DAGGER_STANCE)
				)

			)

			((ResCheck RES_VIEW (+ tView _WOA_SWORD_STANCE))
				(= pDamageType _DAMAGE_SLASH)
				(if tFightingStance
					(aWhatObj setAction: _WOA_SWORD_STANCE)
				)

			)

			(else
;;				(MonoOut "found no view for %d\n" tView)
			)
		)

		(= tRetVal (super bePutOn: aWhatObj &rest))
		
;		(if (== tRetVal _WO_ACTION_HANDLED)
			(pWearer pWeapon: self)
;		)

		(return tRetVal)
	)

	(method (fromPacket aPacket &tmp tOwner)
		(= pSize (aPacket getByte:))
		(= pDamageType (aPacket getByte:))
		(= pSpeed (aPacket getByte:))
		(= pPointBonus (aPacket getByte:))

		(= tOwner (server lookup: (aPacket getLong:)))

		(if tOwner
			(self bePutOn: tOwner)
		)
	)
)
