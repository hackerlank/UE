﻿#ifndef __MLOCK_H
#define __MLOCK_H

#include "MyProject.h"

class MMutex;

/**
 * @brief 锁操作
 */
class MLock
{
protected:
	FScopeLock m_Lock;

public:
	MLock(MMutex* mutex);
	~MLock();
};

#endif