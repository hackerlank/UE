﻿#include "MyProject.h"
#include "PointF.h"

PointF::PointF(float x, float y)
{
	m_x = x;
	m_y = y;
}

float PointF::getX()
{
	return m_x;
}
void PointF::setX(float value)
{
	m_x = value;
}

float PointF::getY()
{
	return m_y;
}

void PointF::setY(float value)
{
	m_y = value;
}