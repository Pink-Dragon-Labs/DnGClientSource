#ifndef _OBJINFO_HPP_
#define _OBJINFO_HPP_

class ObjectInfo
{
public:
	ObjectInfo *next;
	ObjectID id;
};

extern ObjectInfo *objInfo;

#endif
