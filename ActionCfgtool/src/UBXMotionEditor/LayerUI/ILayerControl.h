#pragma once

class ILayerControlEvent
{
public:
	// 获取没有用到的层
	virtual void ILayerControlEvent_GetUnUseLayers(int **pLayerID, int& nLayerCount) = 0;
	virtual CString ILayerControlEvent_GetLayerName(int nLayerID) = 0;
	virtual void ILayerControlEvent_AddLayer(int nLayerID) = 0;
	virtual void LayerControlEvent_DeleteLayer(int nLayerID) = 0;
	virtual void LayerControlEvent_SetTimeScaleValue(int timeScale) = 0;
	// 获取时间粒度
	virtual int LayerControlEvent_GetTimeScaleValue() = 0;
	virtual void LayerControlEvent_LayerPlaying(int layerType) = 0;
	virtual void LayerControlEvent_AddMusic() = 0;
	virtual void LayerControlEvent_StopLayerDebug() = 0;
	virtual void LayerControlEvent_LayerStop() = 0;
	virtual void LayerControlEvent_SetCurrentOption(int layerType) = 0;
	virtual void LayerControlEvent_SetMusicSource(int nLayerType , void *pData , int nDataLen , int nDataID) = 0;
};

class ILayerControl
{
public:
	virtual bool Connect(CWnd* pParent, ILayerControlEvent* pEvent) = 0;
	virtual void ReleaseConnection() = 0;
	virtual void LayerControlToolsEvent_AddLayerControl(int nLayerID) = 0;
	virtual void LayerControlToolsEvent_DeleteLayer(int nLayerID) = 0;
	virtual void LayerControlToolsEvent_SetTimeScaleValue(int timeScale) = 0;
	virtual int LayerControlToolsEvent_GetTimeScaleValue() = 0;
	virtual void LayerControlToolsEvent_EnablePlay(int nLayerType ,bool bState) {};
	virtual CWnd* GetCWnd() = 0;
	virtual void LayerControl_DeleteAllControl() = 0;
	virtual void LayerControl_LayerPlaying(int layerType) = 0;
	virtual void LayerControl_SetCurrentOption(int layerType) = 0;
	virtual void LayerControl_SetMusicSource(int nLayerType , void *pData , int nDataLen , int nDataID) = 0;
	virtual void LayerControl_StopLayerDebug() = 0;
	virtual void LayerControl_LayerDebugStop(int layerType) = 0;

};

bool CreateLayerControl(ILayerControl** ppLayerControl);