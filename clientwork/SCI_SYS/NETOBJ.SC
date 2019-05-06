;;;;
;;;;	OBJECT.SC: Root class with Network stuff added
;;;;
;;;;	Classes:
;;;;		Object
;;;;		Code

;(include "net.sh")

(module# OBJECT)

(procedure
	IsObject
)

(public
	IsObject	0
)

;(class RootObj
;	RootObj is defined in the kernel/compiler and contains only that
;	which is necessary for an object to be recognized as such, having
;	no behavior of its own.
;
;	(properties
;		-objID-
;			This is always set to $1234, which helps identify (though not
;			conclusively!) an object in memory.  Don't change it or
;			save/restore, debug, etc. will probably stop working.
;
;		-size-
;			This is the number of properties in the object.  Don't change
;			it or property lookup will no longer work.
;
;		-propDict-
;			This is a pointer to the property dictionary (contained
;			in the defining class) of an object.  Thus if two
;			objects have equal -propDict- properties, they are both
;			instances of the same class.  Do not change this property,
;			or the object will stop working.
;
;		-methDict-
;			This is a pointer to this object's method dictionary.  Don't
;			change it or method lookup will no longer work.
;
;		-script-
;			This is a pointer to the interpreter's internal script node
;			for the script in which the object was defined.  Change it
;			and die.
;
;		-super-
;			This is a pointer to the object's superclass, and is used to
;			look up a method which is not defined locally.  Don't change
;			it, or the object will cease functioning.
;
;		-info-
;			Bit-mapped information about the object.  Bits are:
;				CLONED		0 = object is static
;								1 = object was created with new:
;
;				NODISPOSE	0 = dispose: object if CLONED
;								1 = don't dispose: object
;
;				NODISPLAY	0 = display object in debugger's object display
;								1 = don't display object in debugger's object display
;
;				CLASS			0 = object is an instance of a class
;								1 = object is a class
;	)
;)

(class Object kindof RootObj
	;;; Class Object is the superclass of all other Script objects.  It
	;;; defines the behavior which is expected of all objects, ensuring
	;;; that all objects will respond to a certain set of selectors.
	
	(properties
		id		name	"Obj"			;print name of the object
		int   scratch 0		   ;non-specific property used 
		int	pModule	-1
		int	pLine		-1
	)
	
	(methods
		new					;create a new instance of the class/object
		init					;initialize the object
		doit					;do your thing
		dispose				;dispose of this instance
		show					;show yourself somehow
		perform				;execute code in your environment
		isClass				;is the receiver a class or an instance?
		isKindOf				;is object/class a subclass of something?
		isMemberOf			;is object an instance of something?
		respondsTo			;does object responds to a message?
		yourself				;answer your object ID

		handleMsg			;deal with a remote message
		handleError			;deal with error messages
	)
	
	(method (new)
		;; Returns the ID of a new instance of the receiver.  This new
		;; instance has the same methods and property values as the
		;; receiver.  The property values can be changed, the methods
		;; cannot.
		
		(return (Clone self))
	)
	
	(method (init)
		;; Initializes the receiver.  If the object is to show up on the
		;; screen (Views, Props, Actors and Egos), this is the message
		;; which will make the it show itself.  The default does nothing.
	)

	(method (doit)
		;; Do your thing, whatever that may be.  Sometimes this is sent
		;; when the object is selected, sometimes (as for Actors) it is
		;; sent for each animation cycle.  The default does nothing but
		;; return the object ID.
		
		(return self)
	)
	
	(method (dispose)
		(if gCuees
			(gCuees eachElementDo: #handleError self)
		)

		;; Dispose of the receiver.  If the receiver was created using
		;; new:, this reclaims the memory occupied by that object.  In
		;; this case it is an error to refer to the object ID being
		;; disposed of once the dispose: message has been sent.
		;; Subclasses of Object should make sure that they dispose of
		;; any objects which they created before doing a (super dispose:).
		(DisposeClone self)
	)

	(method (perform theCode)
		;; Executes the doit: method of theCode with the first parameter
		;; of the method being the object ID of the receiver of perform:.
		;; Any number of parameters may be passed.  Returns the result.
		
		(return (theCode doit: self &rest))
	)
	
	(method (respondsTo selector)
		;; Returns TRUE if the receiver has a method corresponding to
		;; selector, FALSE otherwise.
		
		(return (RespondsTo self selector))
	)

	(method (isClass)
		(return (& -info- CLASS))
	)
	
	(method (isMemberOf what)
		;; Returns TRUE if the receiver is an instance of what, FALSE
		;; otherwise.
		
		(return
			(and
				(& (what -info-?) CLASS)
				(not (& -info- CLASS))
				(== -propDict- (what -propDict-?))
			)
		)
	)
	
	(method (isKindOf what &tmp theSuper)
		(return (KIsKindOf what self))
	)
	
	(method (yourself)
		;; Returns the object ID of the receiver.  This is useful as the
		;; last in a series of messages to an object to force the entire
		;; send to return the ID of the object.
		
		(return self)
	)

	(method (handleMsg msg)
		(return FALSE)
	)

	(method (handleError aWhatObj aWhatError)
	)
)



(class Code kindof Object
	;;; Class Code is used to attach code to various objects (as in
	;;; the 'viewer' property of an Actor) or for use in the perform:
	;;; method of Object.  In either case, it is only the doit: method
	;;; of the class which is important.
	

	(method (doit)
		;; This is the code which you want executed.  Calling conventions
		;; will be dictated by the use of this class.
	)
)

(procedure (IsObject obj)
	(return (== (MemoryInfo MemGetType obj) MemObject))
)

