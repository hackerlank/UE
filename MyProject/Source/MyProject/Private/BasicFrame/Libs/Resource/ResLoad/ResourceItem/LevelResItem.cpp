﻿#include "MyProject.h"
#include "LevelResItem.h"
#include "LoadItem.h"
#include "RefCountResLoadResultNotify.h"
#include "ResEventDispatch.h"

void LevelResItem::init(LoadItem* item)
{
	ResItem::init(item);
	m_refCountResLoadResultNotify->getLoadResEventDispatch()->dispatchEvent(this);
}

void LevelResItem::setLevelName(std::string lvlName)
{

}