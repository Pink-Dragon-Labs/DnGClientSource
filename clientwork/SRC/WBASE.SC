(module# WBASE)

(include "wobject.sh")

(class WorldObjectBase kindof Object
	(properties
		id		pName	 		0		;; name of object that I represent object that 
		id		pSelf			0		;; I represent list of actions that	I handle
		int	pType			0
	)

	(methods
		buildVerbList
		handlesAction

		modifyProp
		getProp

		updateDisplay
		updateAction

		fromPacket
		getServIDs
		preDispose

		editProps
		readProps
		writeProps
		getPropList
	)

	(method (dispose)
     (DisposeClone self)
	)

	(method (buildVerbList))

	(method (handlesAction aWhatAction)
		(return FALSE)
	)

	(method (modifyProp aWhatProp aWhatVal &tmp tRetVal)
		(= tRetVal _WO_ACTION_ALLOWED)
		(if (self respondsTo: aWhatProp)
			(self
				aWhatProp: (+ (self aWhatProp?) aWhatVal)
			)
		)
	
		(return tRetVal)
	)

	(method (getProp aWhatProp &tmp tRetVal)
		(= tRetVal -666)

		(if (self respondsTo: aWhatProp)
			(= tRetVal (self aWhatProp?))
		)
	
		(return tRetVal)
	)

	(method (updateDisplay aWhatObj))

	(method (updateAction aWhatAction)
		(pSelf setAction: aWhatAction)
	)

	;;read my binary representation from the passed PackedData object
	(method (fromPacket aPacket))

	;;get my servIDs from a packet
	(method (getServIDs aPacket))

	(method (preDispose))

	(method (editProps))

	(method (readProps))

	(method (writeProps))

	(method (getPropList))
)
