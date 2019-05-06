;;;;
;;;;	SOUND.SC
;;;;
;;;;	(c) Sierra On-Line, Inc, 1993
;;;;
;;;;	A Sound class is the SCI embodiment of a sound effect, audio, or
;;;;	piece of midi music.
;;;;
;;;;	Classes:
;;;;		Sound

(module#	MUSIC)

(class Sound of Object
	(properties
		int	pHandle	-1
		int 	handle 	0
		int 	number 	0
		int 	nodePtr 	0
		int 	loop	 	0
		int 	priority	0
		int	vol		0
		int	signal	0
		int	min		0
		int	sec		0
		int	frame		0
		int	flags		0
		int	dataInc	0
	)

	(methods 
		play
		stop
	)

	(method (dispose)
		(self stop:)
		(super dispose: &rest)
	)

	(method (play aName)
		(if (!= pHandle -1)
			(self stop:)
		)

		(= pHandle (KSound SndPlay aName))
		(KSound SndLock pHandle)
	)

	(method (stop)
		(KSound SndStop pHandle)
		(KSound SndUnlock pHandle)
		(= pHandle -1)
	)
)

(class MusicMgr of Sound)

(class PanHandler of Object
	(properties
		id		pObject	0
		int	pSoundID	0
	)

	(method (doit &tmp tPan tScaleVal)
		(if (IsObject pObject)
			(= tScaleVal 128)

			(if (pObject scaler?)
				(= tScaleVal ((pObject scaler?) frontSize?))
			)

			(if (== (KSound SndSetPanPosn pSoundID (pObject x?) (pObject scaleY?) tScaleVal) -1)
				(SoundList delete: self)
				(self dispose:)
				(return)
			)
		else
			(SoundList delete: self)
			(self dispose:)
		)
	)
)

(instance SoundList of Set)

(class SoundMgr of Object
	(methods
		play
	)

	(method (play aName aObject aCount &tmp tID tPan tObj)
		(= tID (KSound SndPlay aName (if (> argc 2) aCount else 1)))

		(if 
			(and
				(> argc 1)
				aObject
			)

			((= tObj (PanHandler new:))
				pObject: aObject,
				pSoundID: tID,
			)

			(SoundList add: tObj)
			(tObj doit:)
		)

		(return tID)
	)

	(method (doit)
		(SoundList eachElementDo: #doit)
	)
)


