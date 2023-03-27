#pragma once

class IUILayerControl
{
public:
	virtual bool Connect(QWidget* pParent) = 0;
	virtual void ReleaseConnection() = 0;
	virtual int GetType() = 0;
	virtual QWidget* GetQWidget() = 0;
	virtual void LayerDebugStop() = 0;
	virtual void EnablePlay(bool bState) = 0;
};

bool CreateUILayerControl(IUILayerControl** ppUILayerControl);