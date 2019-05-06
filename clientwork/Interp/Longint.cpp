//
// LONGINT.CPP
//
// LongInt SCI class manipulating kernel call.
//
// author: Stephen Nichols
//

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "sol.hpp"
#include	"kernel.hpp"
#include "object.hpp"
#include "pmachine.hpp"
#include "selector.hpp"
#include "textid.hpp"
#include	"msg.hpp"

extern unsigned int gSerialNumber;
extern int gAllocSize;

void KLongInt ( kArgs args )
{
	enum {
		LongSet,
		LongAdd,
		LongSub,
		LongMul,
		LongDiv,
		LongAnd,
		LongOr,
		LongXor,
		LongEqual,
		LongLess,
		LongLessEqual,
		LongGreater,
		LongGreaterEqual,
		LongNoArgs,
		LongNot = LongNoArgs,
		LongSetBit,
		LongResetBit,
		LongToggleBit,
		LongGetBit,
		LongShiftLeft,
		LongShiftRight,
		LongSwap,
		LongNeg,
		LongAbs,
		LongAsStr,
		LongFromStr,
		LongGetSerial,
		LongMemory
	};

	// check to make sure that the minimum number of arguments are passed
	if ( argCount < 2 ) 
		msgMgr->Fatal ( "Less than two arguments passed to KLongInt" );

	// get the function index
	int function = arg ( 1 );

	// get the LongInt SCI object ID
	ObjectID longID = (ObjectID)arg ( 2 );

	// validate the object
	if ( !longID.IsObject() )
		msgMgr->Fatal ( "Invalid object passed to KLongInt (0x%x)", longID );

	// get the 32 bit integer value from the low/hi word properties
	long objValue = (SCIUWord)longID.GetIndexedProperty ( longLowWord ) + ((SCIUWord)longID.GetIndexedProperty ( longHiWord ) << 16);
	long origObjValue = objValue;

//	msgMgr->Mono ( "KLongInt objValue == %u\n", objValue );

	// get the lowWord and highWord from the argument list, if needed
	long argValue = 0;

	if ( function < LongNoArgs ) {
		// validate the number of arguments
		if ( argCount < 3 )
			msgMgr->Fatal ( "Less than three arguments passed to low/hi word expecting KLongInt call." );

		if ( argCount > 3 ) {
			argValue = (long)(SCIUWord)arg ( 3 );
			argValue += (long)((SCIUWord)arg ( 4 )) << 16;
		} else {
			argValue = (long)(short)arg ( 3 );
		}
	}

//	msgMgr->Mono ( "KLongInt argValue == %u\n", argValue );

	switch ( function ) {
		// set the hi/low word values of a LongInt
		case LongSet:
			objValue = argValue;	
			break;

		// add argValue to objValue
		case LongAdd: 
			objValue += argValue;
			break;

		// subtract argValue from objValue
		case LongSub:
			objValue -= argValue;
			break;

		// multiply argValue and objValue
		case LongMul:
			objValue *= argValue;
			break;

		// divide objVal by argValue
		case LongDiv:
			if ( !argValue )
				msgMgr->Fatal ( "LongInt divide by goose egg." );

			objValue /= argValue;
			break;

		// perform a bitwise AND between objVal and argVal
		case LongAnd:
			objValue &= argValue;
			break;

		// perform a bitwise OR between objVal and argVal
		case LongOr:
			objValue |= argValue;
			break;

		// perform a bitwise XOR between objVal and argVal
		case LongXor:
			objValue ^= argValue;
			break;

		// perform a bitwise NOT on objValue
		case LongNot: 
			objValue = ~objValue;
			break;

		// set a bit of objValue
		case LongSetBit:
			objValue |= 1 << arg(3);
			break;

		// clear a bit of objValue
		case LongResetBit:
			objValue &= ~(1 << arg(3));
			break;

		// toggle a bit of objValue
		case LongToggleBit:
			objValue ^= 1 << arg(3);
			break;

		// get the value of a bit of objValue
		case LongGetBit:
			pm.acc = (Acc)(objValue & (1 << arg(3)))? 1 : 0;
			break;

		// shift objValue to the left by arg(3) bits
		case LongShiftLeft:
			objValue <<= arg(3);
			break;

		// shift objValue to the right by arg(3) bits
		case LongShiftRight:
			objValue >>= arg(3);
			break;

		// swap the hi/low word of objValue
		case LongSwap: {
			long tmp = objValue;

			objValue >>= 16;
			objValue |= tmp << 16;
		}

		break;

		// negate objValue
		case LongNeg:
			objValue *= -1;
			break;

		// calc the absolute value of objValue
		case LongAbs:
			objValue = labs ( objValue );
			break;

		// objVal == argVal?
		case LongEqual:
			pm.acc = (Acc)objValue == argValue;
			break;

		// objVal < argVal?
		case LongLess:
			pm.acc = (Acc)objValue < argValue;
			break;

		// objVal <= argVal?
		case LongLessEqual:
			pm.acc = (Acc)objValue <= argValue;
			break;

		// objVal > argVal?
		case LongGreater:
			pm.acc = (Acc)objValue > argValue;
			break;

		// objVal >= argVal?
		case LongGreaterEqual:
			pm.acc = (Acc)objValue >= argValue;
			break;

		case LongAsStr: {
			char temp[32];

			if ( arg(4) ) {
				sprintf ( temp, "%d", objValue );
			} else {
				sprintf ( temp, "%u", (unsigned int)objValue );
			}

			TextID theID = (TextID)arg(3);
			theID.Add ( temp );
		}

		break;

		case LongFromStr: {
			TextID theID = (TextID)arg(3);
			objValue = atoi ( *theID ); 
		}

		break;

		case LongGetSerial: {
			objValue = gSerialNumber; 
		}

		break;

		case LongMemory: {
			objValue = gAllocSize; 
		}

		break;

		// fatal out because invalid kernel call indexed
		default:
			msgMgr->Fatal ( "Invalid function number in call to KLongInt (call == %d)", arg ( 1 ) );
			break;
	}

	if ( origObjValue != objValue ) {
		longID.SetIndexedProperty ( longLowWord, objValue & 0x0000ffff );
		longID.SetIndexedProperty ( longHiWord, objValue >> 16 );
	}
}

