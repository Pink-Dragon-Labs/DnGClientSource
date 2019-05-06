#include "C++SCI.hpp"

#include "pmachine.hpp"
#include "textid.hpp"
#include "Table.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SetObject(x) \
Property saveObject = pm.object;	\
									\
	pm.object = x;				//	set new current object		

 
#define RestoreObject() pm.object = saveObject;

void CSCI_Object::perform( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_perform, 1, &nValue);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

ObjectID CSCI_Object::newInstance() {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_new, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
	return pm.acc;
}

Property CSCI_Object::doit() {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_doit, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
	return pm.acc;
}

Property CSCI_Object::doit( Property nValue ) {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_doit, 1, &nValue);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()

	return pm.acc;
}

void CSCI_Object::CallMethod( short nMethod )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( nMethod, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_Object::CallMethod( short nMethod, short nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( nMethod, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

Property CSCI_CharInfo::getAffectedState( Property nValue ) {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getAffectedState, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()

	return pm.acc;
}

void CSCI_Sound::play( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_play, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

Property CSCI_LongInt::equal( Property nValue1, Property nValue2 )
{
	Property	nValues[2];

	SetObject( handle )

	nValues[0] = nValue1;
	nValues[1] = nValue2;

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_equal, 2, nValues );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()

	return pm.acc;
}

Property CSCI_LongInt::equalLong( CSCI_LongInt nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_equalLong, 1, (Property*) &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()

	return pm.acc;
}

void CSCI_LongInt::set( Property nValue1, Property nValue2 )
{
	Property	nValues[2];

	SetObject( handle )

	nValues[0] = nValue1;
	nValues[1] = nValue2;

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_set, 2, nValues );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_LongInt::setLong( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_setLong, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_WorldObject::setMessage( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_setMessage, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_WorldObject::handleVerb( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_handleVerb, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_WorldObject::look( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_look, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

CSCI_WorldObject CSCI_WorldObject::FindByServID( int nServID )
{
	return CServIDTable::LookUp( nServID );
}

Property CSCI_WorldObject::createDisplay ( Property nAction )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_createDisplay, 1, &nAction );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()

	return pm.acc;
}

Property CSCI_WorldObject::fromPacket( Property nValue1, Property nValue2 )
{
	short nValue[2];

	nValue[0] = nValue1;
	nValue[1] = nValue2;

	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_fromPacket, 2, nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()

	return pm.acc;
}

void CSCI_WorldObject::fromExtendedPacket( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_fromExtendedPacket, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_WorldObject::addValue( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_addValue, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_WorldObject::subValue( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_subValue, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_WorldObject::addManaValue( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_addManaValue, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_WorldObject::subManaValue( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_subManaValue, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

Property CSCI_WorldObject::getAffectedState( Property nValue ) {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getAffectedState, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()

	return pm.acc;
}

Property CSCI_WorldObject::servIDEquals( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_servIDEquals, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()

	return pm.acc;
}

CSCI_BCharacter CSCI_WorldObject::getBaseCharacter() {
	ObjectID	base = GetClassByName ( "BCharacter" );

	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getBase, 1, (short*) &base);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
	return pm.acc;
}

CSCI_BCarryable CSCI_WorldObject::getBaseCarryable() {
	ObjectID	base = GetClassByName ( "BCarryable" );

	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getBase, 1, (short*) &base);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
	return pm.acc;
}

CSCI_BWearable CSCI_WorldObject::getBaseWearable() {
	ObjectID	base = GetClassByName ( "BWearable" );

	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getBase, 1, (short*) &base);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
	return pm.acc;
}

CSCI_BWeapon CSCI_WorldObject::getBaseWeapon() {
	ObjectID	base = GetClassByName ( "BWeapon" );

	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getBase, 1, (short*) &base);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
	return pm.acc;
}

CSCI_BNPC CSCI_WorldObject::getBaseNPC() {
	ObjectID	base = GetClassByName ( "BNPC" );

	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getBase, 1, (short*) &base);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
	return pm.acc;
}

CSCI_BContainer CSCI_WorldObject::getBaseContainer() {
	ObjectID	base = GetClassByName ( "BContainer" );

	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getBase, 1, (short*) &base);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
	return pm.acc;
}

CSCI_BHead CSCI_WorldObject::getBaseHead() {
	ObjectID	base = GetClassByName ( "BHead" );

	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getBase, 1, (short*) &base);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
	return pm.acc;
}

void CSCI_WorldObject::getVerb ( CSCI_WorldObject hObject )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany ( s_getVerb, 1, (short *)&hObject );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short)handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr );

	RestoreObject ()
}

ObjectID CSCI_WorldObject::getBitmap() {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getBitmap, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
	return pm.acc;
}

StringID CSCI_WorldObject::getName() {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getName, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
	return pm.acc;
}

void CSCI_WorldObject::buildVerbList( CSCI_WorldObject hObject )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_buildVerbList, 1, (short*) &hObject);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_BCharacter::setProperName( short nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_setProperName, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_BCharacter::setTitle( short nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_setTitle, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_BCharacter::setBiography( short nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_setBiography, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_BHead::deltaFeatureSelect ( short nFeature, short nDelta )
{
	short nValue[2];

	nValue[0] = nFeature;
	nValue[1] = nDelta;

	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_deltaFeatureSelect, 2, nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_BHead::nextFeature( short nValue ) {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_nextFeature, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);
	
	RestoreObject ()
}

void CSCI_BHead::prevFeature( short nValue ) {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_prevFeature, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);
	
	RestoreObject ()
}

ObjectID CSCI_fNBody::getHeadBitmap () {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getHeadBitmap, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
	
	return pm.acc;
}

ObjectID CSCI_fNBody::getAttackBitmap () {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getAttackBitmap, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
	
	return pm.acc;
}

#if 0
CSCI_fNBody
enum {
	_WOA_FACE,					//	0
	_WOA_STAND,					//	1
	_WOA_WALK,					//	2
	_WOA_SIT,					//	3
	_WOA_GIVE,					//	4
	_WOA_GET,					//	5
	_WOA_THROW,					//	6
	_WOA_JUMP,					//	7
	_WOA_PUSH,					//	8
	_WOA_CAST,					//	9
	_WOA_CLUB_STANCE,			//	10
	_WOA_CLUB_ADVANCE,			//	11
	_WOA_CLUB_STRIKE,			//	12
	_WOA_DAGGER_STANCE,			//	13
	_WOA_DAGGER_ADVANCE,		//	14
	_WOA_DAGGER_STRIKE,			//	15
	_WOA_COMBAT_READY,			//	16
	_WOA_SWORD_STANCE,			//	17
	_WOA_SWORD_ADVANCE,			//	18
	_WOA_SWORD_STRIKE,			//	19
	_WOA_COMBAT_ADVANCE,		//	20
	_WOA_HAND_STANCE,			//	21
	_WOA_HAND_ADVANCE,			//	22
	_WOA_HAND_STRIKE,			//	23
	_WOA_KICK_STRIKE,			//	24
	_WOA_CROSSBOW_STANCE,		//	25
	_WOA_CROSSBOW_ADVANCE,		//	26
	_WOA_CROSSBOW_ATTACK,		//	27
	_WOA_DIE,					//	28
	_WOA_GROUND,				//	29
	_WOA_OPEN,					//	30
	_WOA_BURN,					//	31
	_WOA_EXPLODE,				//	32
	_WOA_INVENTORY,				//	33
	_WOA_CLOSEUP,				//	34
	_WOA_LIQUID_SIZE,			//	35
	_WOA_TRANSFORM,				//	36
	_WOA_DISPLAY,				//	37
}
#endif

ObjectID CSCI_fNBody::getStanceBitmap( short nValue ) {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getStanceBitmap, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
	
	return pm.acc;
}

void CSCI_WorldActor::setSex( short nSex ) {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_setSex, 1, &nSex );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);
	
	RestoreObject ()
}

void CSCI_WorldActor::setRace( short nRace ) {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_setRace, 1, &nRace );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);
	
	RestoreObject ()
}

short CSCI_WorldActor::getSex() {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getSex, 0, NULL );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
	
	return pm.acc;
}

short CSCI_WorldActor::getRace() {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getRace, 0, NULL );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
	
	return pm.acc;
}

void CSCI_WorldEgo::stopAction() {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_stopAction, 0, NULL );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);
	
	RestoreObject ()
}

void CSCI_WorldEgo::stopTargetting( void ) {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_stopTargetting, 0, NULL );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);
	
	RestoreObject ()
}

void CSCI_WorldEgo::setPassword( void ) {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_setPassword, 0, NULL );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);
	
	RestoreObject ()
}

Property CSCI_Collection::contains ( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_contains, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr );

	RestoreObject ()

	return pm.acc;
}

Property CSCI_List::at( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_at, 1, &nValue );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()

	return pm.acc;
}

Property CSCI_Array::at( Property nX )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_at, 1, &nX );

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	pm.object = saveObject;

	return pm.acc;
}

void CSCI_Game::controlOff()
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_controlOff, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_Game::controlOn()
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_controlOn, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_Game::handsOff()
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_handsOff, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_Game::handsOn()
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_handsOn, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

Property CSCI_GameServer::getShopItemImage( Property nIcon, Property nColorStart, Property nColor )
{
	Property nValues[3];

	nValues[0] = nIcon;
	nValues[1] = nColorStart;
	nValues[2] = nColor;

	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getShopItemImage, 3, nValues);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 3 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()

	return pm.acc;
}

void CSCI_GameServer::updatePingText()
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_updatePingText, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_GameServer::leaveGroup( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_leaveGroup, 1, &nValue);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_GameServer::kickGroup( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_kickGroup, 1, &nValue);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_GameServer::getQuestList()
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getQuestList, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_GameServer::restartGame()
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_restartGame, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_GameServer::getMail() {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getMail, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_GameServer::getMailMessage( short nMsg ) {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_getMailMessage, 1, &nMsg);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_GameServer::eraseMailMessage( short nMsg ) {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_eraseMailMessage, 1, &nMsg);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_GameServer::sendMail( TextID sTo, TextID sSubject, TextID sMsg ) {
	SetObject( handle )

	short nValue[3];

	nValue[0] = (SOL_Handle) sTo;
	nValue[1] = (SOL_Handle) sSubject;
	nValue[2] = (SOL_Handle) sMsg;

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_sendMail, 3, nValue);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 3 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_Script::cue() {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_cue, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 0 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_Object::dispose() {
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_dispose, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 0 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_Spell::cast()
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_cast, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 0 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

Property CSCI_Spell::canCast()
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_canCast, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 0 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()

	return pm.acc;
}

void CSCI_Collection::release()
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_release, 0, NULL);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 0 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}

void CSCI_List::addToEnd( Property nValue )
{
	SetObject( handle )

	//	push the selector, # of Args, ptr to arg list
	pmPushMany( s_addToEnd, 1, &nValue);

	//	call the method
	// MUST be cast as short or else CPP converts them signed (Bryan Waters)
	pmQuickMessage( (unsigned short) handle, (unsigned short) ( 2 + 1 ) * sizeof ( Property ), pm.StackPtr);

	RestoreObject ()
}
