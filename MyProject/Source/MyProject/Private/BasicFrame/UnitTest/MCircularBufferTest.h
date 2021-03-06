#ifndef __CIRCULARBUFFERTEST_H
#define __CIRCULARBUFFERTEST_H

#include "MCircularBuffer.h"
#include "gtest/gtest.h"

class MCircularBufferTest : public testing::Test
{
protected:
	MCircularBuffer m_circularBuffer;

protected:
	static void SetUpTestCase();
	static void TearDownTestCase();

	virtual void SetUp();
	virtual void TearDown();

public:
	MCircularBufferTest();
	virtual ~MCircularBufferTest();
};


#endif				// __CIRCULARBUFFERTEST_H