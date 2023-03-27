#pragma once
#include "actionblockgroupmanager.h"
class CMusicBlockGroupManager :
	public CActionBlockGroupManager
{
public:
	CMusicBlockGroupManager(void);
	virtual ~CMusicBlockGroupManager(void);
protected:
	int m_ActStartTime;
	int m_ActEndTime;
public:
	void SetActStartTime(int nActStartTime);
	void SetActEndTime(int nActTime);
	int  GetActStartTime();
	int  GetActEndTime();
};

