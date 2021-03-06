﻿#ifndef __MsgRouteHandleBase_H
#define __MsgRouteHandleBase_H

#include <map>
#include "MsgRouteDelegate.h"

class MsgRouteBase;

class MsgRouteHandleBase
{
public:
	std::map<int, MsgRouteDelegate> m_id2HandleDic;

public:
	virtual void handleMsg(MsgRouteBase* msg);
};

#endif