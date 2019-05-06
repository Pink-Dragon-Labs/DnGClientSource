;;;;
;;;;	NETGROUP.SC: The Group class for network games.
;;;;
;;;;

(module#	NETGROUP)


(class NetGroup of Set
	(properties
		id		type			0				;class type of members
		int	maximum	 128				;max # of allowed members
		int	hasChanged	0				;has group changed since last polled?
	)


	(methods
		getMembers
		addMember
		deleteMember
		requestMembers
	)

	(method (handleMsg msg &tmp obj)
		(super handleMsg: msg)
		(= obj self)
		(switch (msg command?)
			(SN_GRPMEM
				(self getMembers: (msg members?) (msg number?))
			)
			(SN_GRPJOIN
				(= obj (self addMember: (msg who?)))
			)
			(SN_GRPDEL
				(= obj (self deleteMember: (NetHost lookup: (msg who?))))
			)
		)
		(netMsgHandler	handleMsg: msg self)

		(return (msg claimed?))
	)


	(method (joinNet objNum)
		(NET NetSend
			"wwwbbww"
			SN_OBJNEW
			0
			self
			NEW_GROUP
			(theGame number?)
			objNum
			maximum
		)
	)


	(method (add whom)
		(if argc
			(NET NetSend
				"w+"
				SN_GRPJOIN
				sid
				(whom sid?)
			)
		else
			(super add:)
		)
	)


	(method (delete whom)
		(if (and argc (self contains: whom))
			(NET NetSend
				"w+"
				SN_GRPDEL
				sid
				(whom sid?)
			)
			(return whom)
		)
		(return NULLID)
	)


	(method (getMembers mem num &tmp i)
		(self release:)
		(for	((= i 0))
				(< i num)
				((++ i))

			(self addMember: (mem at: i))
		)
	)

	(method (requestMembers whom)
		(NET NetSend
			"www"
			SN_GRPMEM
			sid
			(whom sid?)
		)
	)

	(method (addMember who &tmp obj)
		(if (== (= obj (NetHost lookup: who)) NULLID)
			(if (not type) (= type Object))
			((= obj (type new:))
				setSid: who,
				init:,
			)
		)
		(super add: obj)
		(= hasChanged TRUE)
		(return obj)
	)


	(method (deleteMember mem)
		(if mem
			(super delete: mem)
			(= hasChanged TRUE)
		)
		(return mem)
	)


	(method (dispose)
		(if (self isEmpty?)
			(super dispose:)
		else
			(Printf "Attempt to dispose \"%s\", which is not empty." name)
		)
	)
)

