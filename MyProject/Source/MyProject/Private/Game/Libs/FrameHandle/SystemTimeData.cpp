﻿#include "MyProject.h"
#include "SystemTimeData.h"

SystemTimeData::SystemTimeData()
{
	m_preTime = 0;
	m_curTime = 0;
	m_deltaSec = 0.0f;
}

float SystemTimeData::getDeltaSec()
{
	return m_deltaSec;
}

void SystemTimeData::setDeltaSec(float value)
{
	m_deltaSec = value;
}

long SystemTimeData::getCurTime()
{
	return m_curTime;
}
void SystemTimeData::setCurTime(long value)
{
	m_curTime = value;
}

void SystemTimeData::nextFrame()
{
	m_curTime = DateTime.Now.Ticks;
	if (m_preTime != 0f)     // 第一帧跳过，因为这一帧不好计算间隔
	{
		TimeSpan ts = new TimeSpan(m_curTime - m_preTime);
		m_deltaSec = (float)(ts.TotalSeconds);
	}
	else
	{
		m_deltaSec = 1 / 24;        // 每秒 24 帧
	}
	m_preTime = m_curTime;
}