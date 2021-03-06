﻿#ifndef __ResLoadMgr_H
#define __ResLoadMgr_H

#include "MsgRouteHandleBase.h"
#include "Platform.h"
#include <string>
#include "MList.h"
#include "ResPackType.h"
#include "EventDispatchDelegate.h"
#include "IDispatchObject.h"

class ResLoadData;
class LoadItem;
class ResItem;
class LoadParam;

class ResLoadMgr : public MsgRouteHandleBase, public IDispatchObject
{
protected:
	uint32 m_maxParral;                             // 最多同时加载的内容
    uint32 m_curNum;                                // 当前加载的数量
    ResLoadData* m_LoadData;
    LoadItem* m_retLoadItem;
    ResItem* m_retResItem;
    //ResMsgRouteCB m_resMsgRouteCB;
    MList<std::string> m_zeroRefResIDList;      // 没有引用的资源 ID 列表
    int m_loadingDepth;      // 加载深度

public:
	ResLoadMgr();
	void postInit();
    // 重置加载设置
protected:
	void resetLoadParam(LoadParam* loadParam);
public:
	ResItem* getResource(std::string path);
	void loadData(LoadParam* param);
    // eBundleType 打包类型资源加载
	void loadBundle(LoadParam* param);
    // eLevelType 打包类型资源加载，都用协程加载
	void loadLevel(LoadParam* param);
    // eResourcesType 打包类型资源加载
	void loadResources(LoadParam* param);
	ResItem* createResItem(LoadParam* param);
	LoadItem* createLoadItem(LoadParam* param);
    // 资源创建并且正在被加载
	void loadWithResCreatedAndLoad(LoadParam* param);
	void loadWithResCreatedAndNotLoad(LoadParam* param, ResItem* resItem);
	void loadWithNotResCreatedAndNotLoad(LoadParam* param);
    // 通用类型，需要自己设置很多参数
public:
	void load(LoadParam* param);
	ResItem* getAndLoad(LoadParam* param);
    // 这个卸载有引用计数，如果有引用计数就卸载不了
	void unload(std::string path, EventDispatchDelegate loadEventHandle);
    // 添加无引用资源到 List
protected:
	void addNoRefResID2List(std::string path);
    // 卸载没有引用的资源列表中的资源
	void unloadNoRefResFromList();
    // 不考虑引用计数，直接卸载
	void unloadNoRef(std::string path);
public:
	void onLoadEventHandle(IDispatchObject* dispObj);
	void onLoaded(LoadItem* item);
	void onFailed(LoadItem* item);
protected:
	void releaseLoadItem(LoadItem* item);
	void loadNextItem();
	ResItem* findResFormPool(ResPackType type);
	LoadItem* findLoadItemFormPool(ResPackType type);
    // 资源加载完成，触发下一次加载
	//void onMsgRouteResLoad(MsgRouteBase msg);
};

#endif