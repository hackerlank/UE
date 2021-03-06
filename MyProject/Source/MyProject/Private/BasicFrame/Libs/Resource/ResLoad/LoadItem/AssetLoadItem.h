﻿#ifndef __FileLoadItem_H
#define __FileLoadItem_H

#include "LoadItem.h"
#include "CoreUObject.h"	// Class 

/**
 * @brief 从 uasset 扩展名的本地 Package 中加载资源
 */
class AssetLoadItem : public LoadItem
{
protected:
	UClass* m_resObj = nullptr;		// uasset 类型的资源

protected:
	//void loadFromDefaultAssetBundle()
	//{
	//	m_prefabObj = Resources.Load<Object>(m_pathNoExt);

	//	if (m_prefabObj != null)
	//	{
	//		nonRefCountResLoadResultNotify.resLoadState.setSuccessLoaded();
	//	}
	//	else
	//	{
	//		nonRefCountResLoadResultNotify.resLoadState.setFailed();
	//	}
	//	nonRefCountResLoadResultNotify.loadResEventDispatch.dispatchEvent(this);
	//}

	//IEnumerator loadFromDefaultAssetBundleByCoroutine()
	//{
	//	ResourceRequest req = Resources.LoadAsync<Object>(m_pathNoExt);
	//	yield return req;

	//	if (req.asset != null && req.isDone)
	//	{
	//		m_prefabObj = req.asset;
	//		nonRefCountResLoadResultNotify.resLoadState.setSuccessLoaded();
	//	}
	//	else
	//	{
	//		nonRefCountResLoadResultNotify.resLoadState.setFailed();
	//	}

	//	nonRefCountResLoadResultNotify.loadResEventDispatch.dispatchEvent(this);
	//}

public:
	AssetLoadItem();
	UClass* getResObj();
	void setResObj(UClass* value);
	virtual void load() override;
    // 这个是卸载，因为有时候资源加载进来可能已经不用了，需要直接卸载掉
	virtual void unload() override;
};

#endif