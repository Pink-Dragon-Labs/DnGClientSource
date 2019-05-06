;;---------------------------------------------------------------------------
;;
;; BCHAR.SC
;; (c) Sierra On-Line, Inc, 1995
;;
;; Author: Stephen Nichols
;;
;; Description:
;;
;; 	This file contains the base component class that represents a
;;		character.  
;;
;; Usage:
;;	
;;		There are a few special usage concerns that the programmer that
;;		needs to use this base must observe.  First off, you should be
;;		aware that all of the properties are NOT set automatically
;;		by the server.  In fact, the bulk of the properties are not set
;;		by normal server means.  You must call a special GameServer method in
;;		order to request the values from it.  This method is called 
;;		getCharacterInfo.  Secondly, you must NOT directly modify either
;;		the properName or title.  Use the provided utility methods to
;;		modify them.
;;
;;	Example:
;;	
;;		none
;;
;; Code Map:
;;
;; 	Classes: 
;;			BCharacter
;;
;; 	Procedures: 
;;			none
;;
;;		Instances:
;;			none
;;
;; History:
;;
;;		11/27/1995: Stephen Nichols
;;						
;;			Setup initial BCharacter class.
;;
;;---------------------------------------------------------------------------

(module# BCHARACTER)

(include "wobject.sh")

(class BCharacter kindof WorldObjectBase
	(properties
		int	pType					_BCHARACTER

		int	pProfession			0
		int	pExperience			0
		int	pRace					0
		int	pSex					0

		int	pBuildPoints		0
		int	pHomeTown			0

		id		pSkillArray			0
		id		pSpellArray			0

		id		pProperName			0
		id		pTitle				0
		id		pBiography			0

		id		pStealingCount		0
		id		pStealingUnserved	0
		id		pKillingCount		0
		id		pKillingUnserved	0

		int	pPeaceful			0
	)

	(methods
		setProperName		;; set the proper name property
		setTitle				;; set the title property
		setBiography		;; set the biography property

		;** IS this OBSOLETE? Not used anywhere that I can tell.

		fromFullPacket		;; populate all properties from a packet 
	)

	(method (init)
		(super init: &rest)
		(self setTitle: "")
		(self setProperName: "")

		(= pSpellArray (ByteArray new:))
		(= pSkillArray (ByteArray new:))
	)


	;; This method is called to dispose of this BCharacter.  The programmer
	;; should never call this method directly.  It's called by the dispose
	;; method of WorldObject when the base list is disposed.
	;;
	(method (dispose)
		(self setProperName: 0)
		(self setTitle: 0)
		(self setBiography: 0)

		(if pSpellArray
			(pSpellArray dispose:)
			(= pSpellArray 0)
		)

		(if pSkillArray
			(pSkillArray dispose:)
			(= pSkillArray 0)
		)

		(if pExperience
			(pExperience dispose:)
			(= pExperience 0)
		)

		(if pStealingCount
			(pStealingCount dispose:)
			(= pStealingCount 0)
		)

		(if pStealingUnserved
			(pStealingUnserved dispose:)
			(= pStealingUnserved 0)
		)

		(if pKillingUnserved
			(pKillingUnserved dispose:)
			(= pKillingUnserved 0)
		)

		(if pKillingCount
			(pKillingCount dispose:)
			(= pKillingCount 0)
		)

		(super dispose: &rest)
	)

	;; This method is called to set the proper name of this BCharacter.  This
	;; method will dispose of the previous proper name property and clone
	;; the passed value if non-zero.
	;;
	(method (setProperName aStr)
		(if pProperName
			(pProperName dispose:)
			(= pProperName 0)
		)

		(if aStr
			(= pProperName (String with: aStr))
		)
	)

	;; This method is called to set the title of this BCharacter.  This method
	;; will dispose of the previous title property and clone the passed value
	;; if non-zero.
	;;
	(method (setTitle aStr)
		(if pTitle
			(pTitle dispose:)
			(= pTitle 0)
		)

		(if aStr
			(= pTitle (String with: aStr))
		)
	)

	;; This method is called to set the biography property of this BCharacter.
	;; This method will dispose of the previous biography property and clone
	;; the passed value if non-zero.
	;;
	(method (setBiography aStr)
		(if pBiography
			(pBiography dispose:)
			(= pBiography 0)
		)

		(if aStr
			(= pBiography (String with: aStr))
		)
	)

	;; This method is called to populate the properties of this BCharacter
	;; with data from the server.  This method only processes the default
	;; format of this class.  The programmer should never need to call
	;; this method directly.  It's called by the fromPacket method
	;; of WorldObject.
	;;
	(method (fromPacket aPacket &tmp tProperName tTitle)
		(= pProfession (aPacket getByte:))
		(= pRace (aPacket getByte:))
		(= pSex (aPacket getByte:))

		(if 
			(or
				(== (pSelf pBaseView?) 100)
				(== (pSelf pBaseView?) 200)
			)

			(if (not pSex)
				(pSelf pBaseView: 100)
			else
				(pSelf pBaseView: 200)
			)
		)

		(= tProperName (aPacket getString:))
		(self setProperName: tProperName)

		(= pPeaceful (aPacket getByte:))

		(if (pSelf respondsTo: #pPeaceful)
			(pSelf pPeaceful: pPeaceful)
		)

;; Big/Little Endian
		(pSelf pHealth: (aPacket getWord:))
		(pSelf pHealthHi: (aPacket getWord:))

;; Big/Little Endian
		(pSelf pHealthMax: (aPacket getWord:))
		(pSelf pHealthMaxHi: (aPacket getWord:))

		(tProperName dispose:)
	)

	;; This method is called to fully populate the properties of this
	;; BCharacter with data from the server.  The programmer should never
	;; need to call this method directly.  It's called by the GameServer
	;; when processing a getCharacterInfo method.
	;;
	(method (fromFullPacket aPacket &tmp tProperName tTitle)
		;; set the default properties
		(= pProfession (aPacket getByte:))
		(= pExperience (aPacket getLong:))
		(= pRace (aPacket getByte:))
		(= pSex (aPacket getByte:))

		;; set the proper name
		(= tProperName (aPacket getString:))
		(self setProperName: tProperName)
		(tProperName dispose:)

		;; set the title
		(= tTitle (aPacket getString:))
		(self setTitle: tTitle)
		(tTitle dispose:)
	)

	(method (buildVerbList aWhatObj &tmp tVerbList tID tBase)

		;; if a aWhatObj is a WorldActor, I can be manipulated

		(if (and (aWhatObj isKindOf: WorldActor) 
					(!= pSelf ego) 
					(= tBase (aWhatObj getBase: BCharacter))
			)
			(= tVerbList VerbList)


		;;;
		;;; allow characters to be attacked
		;;;

			(if (& (aWhatObj pMode?) _WO_MODE_COMBAT)
				(if (and 
						(== (GetDistance (pSelf pCombatX?) (pSelf pCombatY?) (aWhatObj pCombatX?) (aWhatObj pCombatY?)) 1)
						(== (pSelf pFriend?) FALSE)
					)
					(tVerbList add: ATTACK_V)
				)
			else

		;;;
		;;; allow characters to be thieved from .. either players or NPC's with heads
		;;;
				(if 
					(or
						(and
							(& gCombatAllowed 1)
							(not (pSelf getBase: BNPC))
							(not (pSelf getBase: BGatekeeper))
							(not (ego getAffectedState: _AFF_JAIL))
							(> ((tBase pSkillArray?) at: _SKILL_PICK_POCKETS) 0)
						)
						(and
							(pSelf getBase: BNPC)
							(not (pSelf getBase: BGatekeeper))
							(pSelf pHead?)
							(not (ego getAffectedState: _AFF_JAIL))
							(> ((tBase pSkillArray?) at: _SKILL_PICK_POCKETS) 0)
						)
					)
					(tVerbList add: ROB_V)
				)
				
		;;;
		;;; allow characters to be engaged in combat
		;;;

				(= tID (LongInt new:))

				(tID set: (pSelf pServIDLo?) (pSelf pServIDHi?))

				(if
					(not (pSelf getBase: BShop))
					
					(if 
						(or
							(& gCombatAllowed 4)	;; always allow for tourney fights

							(and
								(& gCombatAllowed 1)
								(not (pSelf getBase: BNPC))
								(not (GroupList findByServID: tID))
								(not (pSelf pPeaceful?))
							)
							(and
								(& gCombatAllowed 2)
								(pSelf getBase: BNPC)
								(not (GroupList findByServID: tID))
								(not (pSelf pPeaceful?))
							)
						)
						(tVerbList add: START_COMBAT_V)
					)
				)

				(tID dispose:)
			)
		)
	)
)

