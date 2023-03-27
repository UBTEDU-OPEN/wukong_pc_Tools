#pragma once
#include "actionblockdata.h"
class CMusicBlockData :
	public CActionBlockData
{
public:
	CMusicBlockData(void);
	~CMusicBlockData(void);
	int  GetActualStartTime();
	int  GetActualEndTime();
	void SetActualStartTime(int nActStartTime);
	void SetActualEndTime(int nActEndTime);
protected:
	// 实际的起始时间
	int m_ActualStartTime;
	// 实际的截止时间
	int m_ActualEndTime;
};

