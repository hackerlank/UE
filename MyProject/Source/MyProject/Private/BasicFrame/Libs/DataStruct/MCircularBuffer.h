#ifndef __CIRCULARBUFFER_H
#define __CIRCULARBUFFER_H

#include "Platform.h"
#include <cstddef>
#include <iostream>
#include <vector>
#include "BufferCV.h"
#include "DynBuffer.h"

//template <class T> class DynBuffer<char>;
class ByteBuffer;

/**
 *@brief 浪费一个自己，这样判断也好判断，并且索引也不用减 1 ，因此浪费一个字节
 */
class MCircularBuffer
{
protected:
	DynBuffer<char>* m_dynBuffer;
	uint32 m_first;             // 当前缓冲区数据的第一个索引
	uint32 m_last;              // 当前缓冲区数据的最后一个索引的后面一个索引，浪费一个字节
	ByteBuffer* m_tmpBA;        // 临时数据

protected:
	bool canAddData(uint32 num);
	void setCapacity(size_t newCapacity);

public:
	MCircularBuffer(size_t initCapacity = BufferCV::INIT_CAPACITY, size_t maxCapacity = BufferCV::MAX_CAPACITY);
	~MCircularBuffer();

public:
	uint32 getFirst();
	uint32 getLast();
	std::size_t getSize();
	void setSize(std::size_t value);
	char* getBuff();

	bool isLinearized();
	bool empty();
	size_t getCapacity();
	bool full();
	void linearize();
	void clear();

	// 添加和获取数据
	void pushBackArr(char* items, uint32 start, std::size_t len);
	void pushBackBA(ByteBuffer* bu);
	void pushFrontArr(char* items, std::size_t len);
	void popFrontBA(ByteBuffer* bytearray, std::size_t len);
	void frontBA(ByteBuffer* bytearray, std::size_t len);
	void popFrontLen(uint32 len);
	void pushBackCB(MCircularBuffer* rhv);
};


#endif				// __CIRCULARBUFFER_H