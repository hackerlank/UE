﻿#include "MyProject.h"
#include "MsgRouteHandleBase.h"
#include "MsgRouteBase.h"
#include "UtilContainers.h"
#include "UtilStr.h"
#include "Common.h"

void MsgRouteHandleBase::handleMsg(MsgRouteBase* msg)
{
	int key = ((int)msg->m_msgID);
	if (UtilMap::ContainsKey(m_id2HandleDic, key))
	{
		m_id2HandleDic[key](msg);
	}
	else
	{
		//g_pLogSys->log(UtilStr::Format(Ctx.m_instance.m_langMgr.getText(LangTypeId.eMsgRoute1, LangItemID.eItem1), (int)msg.m_msgID));
	}
}