#ifndef __DYNBUFRESIZEPOLICY_H
#define __DYNBUFRESIZEPOLICY_H


class DynBufResizePolicy
{
public:
	static uint32 getCloseSize(uint32 needSize, uint32 capacity, uint32 maxCapacity);
};


#endif			// __DYNBUFRESIZEPOLICY_H	