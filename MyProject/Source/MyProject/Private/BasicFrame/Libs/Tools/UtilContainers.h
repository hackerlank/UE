#ifndef __UtilContainers_H
#define __UtilContainers_H

//==================== vector 区域 ===================

#include <vector>

/**
 *@brief 主要是一些容器辅助功能
 */
class UtilVector
{
public:
	// 查找向量某个元素索引，如果没有查找到就返回 -1
	template<class T>
	static int IndexOf(std::vector<T>& vec, T& item);

	// 移除一个元素，如果移除成功，就返回 true
	template<class T>
	static bool Remove(std::vector<T>& vec, T& item);

	// 移除指定索引处的元素
	template<class T>
	static void RemoveAt(std::vector<T>& vec, int index);

	// 在指定索引位置插入一个值
	template<class T>
	static void Insert(std::vector<T>& vec, int index, T& item);

	template<class T>
	static void Add(std::vector<T>& vec, T& item);
};

//================== list 区域 =========================

#include <list>

class UtilList
{
public:
	// 移除一个元素，如果移除成功，就返回 true
	template<class T>
	static bool Remove(std::list<T>& list, T& item);

	// 根据索引移除元素
	template<class T>
	static void RemoveAt(std::list<T>& list, int index);

	// 返回元素的数量
	template<class T>
	static int Count(std::list<T>& list);

	// 添加一个元素
	template<class T>
	static int Add(std::list<T>& list, T& item);

	// 获取一个元素
	//template<class T>
	//static T& operator[](std::list<T>& list, int index);

	template<class T>
	static T& At(std::list<T>& list, int index);
};

//=================== map 区域 ========================

#include <map>

class UtilMap
{
public:
	// 查找 map 中某个 key 是否存在
	template<class TKey, class TValue>
	static bool ContainsKey(std::map<TKey, TValue>& map, TKey& key);

	// 删除某个 key
	template<class TKey, class TValue>
	static bool Remove(std::map<TKey, TValue>& map, TKey& key);
};

//================== 实现区域 ====================
#include "UtilContainers.inl"


#endif