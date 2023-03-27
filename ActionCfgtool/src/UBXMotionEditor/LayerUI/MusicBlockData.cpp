#include "stable.h"
#include "MusicBlockData.h"


CMusicBlockData::CMusicBlockData(void)
{
	m_ActualEndTime = 0;
	m_ActualStartTime = 0;
}


CMusicBlockData::~CMusicBlockData(void)
{
}
int  CMusicBlockData::GetActualStartTime()
{
	return m_ActualStartTime;
}
int  CMusicBlockData::GetActualEndTime()
{
	return m_ActualEndTime;
}
void CMusicBlockData::SetActualStartTime(int nActStartTime)
{
	//GetStartTime()
	m_ActualStartTime = nActStartTime;
}
void CMusicBlockData::SetActualEndTime(int nActEndTime)
{
	m_ActualStartTime = nActEndTime;
}
