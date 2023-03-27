#include "stable.h"
#include "MusicBlockGroupManager.h"


CMusicBlockGroupManager::CMusicBlockGroupManager(void)
{
	m_ActStartTime = 0;
	m_ActEndTime = 0;
}


CMusicBlockGroupManager::~CMusicBlockGroupManager(void)
{
	int i =1;
}
void CMusicBlockGroupManager::SetActStartTime(int nActStartTime)
{
	m_ActStartTime = nActStartTime;
}
void CMusicBlockGroupManager::SetActEndTime(int nActEndTime)
{
	m_ActEndTime = nActEndTime;
}
int  CMusicBlockGroupManager::GetActStartTime()
{
	return m_ActStartTime;
}
int  CMusicBlockGroupManager::GetActEndTime()
{
	return m_ActEndTime;
}
