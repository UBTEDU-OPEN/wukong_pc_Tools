#include "StdAfx.h"
#include "Header.h"
#include "constant.h"

CHeader::CHeader(void)
{
	m_nFlag = ALPHA_SOCKET_FLAG;
	m_nLength = 0;
	m_nVersion = ALPHA_SOCKET_VERSION;
	m_nExtInfo = 0;
	m_pMsg = NULL;
	m_nMsgLen = 0;
}


CHeader::~CHeader(void)
{
	if (m_pMsg)
	{
		delete [] m_pMsg;
		m_pMsg = NULL;
	}
}

bool CHeader::SetMsg(const char* pData, int nLen )
{
	if (m_pMsg)
	{
		delete [] m_pMsg;
		m_pMsg = NULL;
	}

	if (pData && nLen)
	{
		m_pMsg = new char[nLen];
		memcpy(m_pMsg, pData, nLen);
		m_nMsgLen = nLen;
	}

	return true;
}

int CHeader::GetHeaderData(char** ppData)
{
	m_nLength = 4 + m_nMsgLen;

	int nPos = 0;
	char* pData = new char[m_nLength+6];
	*ppData = pData;

	// m_nFlag
	memcpy(pData, &m_nFlag, sizeof(m_nFlag));
	pData += sizeof(m_nFlag);

	// m_nLength
	memcpy(pData, &m_nLength, sizeof(m_nLength));
	pData += sizeof(m_nLength);

	// m_nVersion
	memcpy(pData, &m_nVersion, sizeof(m_nVersion));
	pData += sizeof(m_nVersion);

	// m_nExtInfo
	memcpy(pData, &m_nExtInfo, sizeof(m_nExtInfo));
	pData += sizeof(m_nExtInfo);

	if (m_nMsgLen)
	{
		memcpy(pData, m_pMsg, m_nMsgLen);
	}

	return m_nLength + 6;
}



/////////////////////////////////////////////////////////////////////////
// 功能： 通过二进制数据生成CHeader数据
// 说明： 
// 参数： 二进制数据pData，指向存储pData数据长度的指针， 在pData数据中的偏移值
// 返回值：-1失败  其他为已经接收的字节数，既数据中char指针的偏移值
/////////////////////////////////////////////////////////////////////////
int CHeader::SetHeaderData(char* pData , int nLen ,int &nOffset)
{
	//int nOffset = 0;
	if ((nLen - nOffset) < 10)   //数据长度不足
	{
		//pData = NULL;
		
		nOffset = 0;
		return -1;
	}

	pData += nOffset;  //首先将其指向偏移量
	//PrintInHex(pData,*nLen);

	//接收并校验FLAG
	memcpy((char*)&m_nFlag, pData , sizeof(m_nFlag));
	pData += sizeof(m_nFlag);
	if (ALPHA_SOCKET_FLAG != m_nFlag)
	{
		return -1;   //过滤掉杂的消息
	}
	nOffset += sizeof(m_nFlag);
	//PrintInHex(pData,*nLen);

	//校验数据长度
	memcpy((char*)&m_nLength , pData ,sizeof(m_nLength) );
	if (m_nLength < 4)
	{
		return -1; //包消息长度不足
	}

	pData += sizeof(m_nLength);
	nOffset += sizeof(m_nLength);
	//PrintInHex(pData,*nLen);

	//接收校验版本号
	memcpy((char*)&m_nVersion , pData ,sizeof(m_nVersion));
	pData += sizeof(m_nVersion);
	nOffset += sizeof(m_nVersion);
	//PrintInHex(pData,*nLen);
	
	//接收校验附加信息
	memcpy((char*)&m_nExtInfo , pData ,sizeof(m_nExtInfo));
	pData += sizeof(m_nExtInfo);
	nOffset += sizeof(m_nExtInfo);
	//PrintInHex(pData,*nLen);
	
	//接收CHeaderCmd的MSGPACK封装数据
	SetMsg(pData, m_nLength-4);
	//memcpy((char*)&m_pMsg , pData ,sizeof(m_nLength - 4));
	//SetMsg()

	pData += m_nLength - 4;
	nOffset += m_nLength - 4;
	//}
	//PrintInHex(pData,*nLen);
	/*memcpy((char*)&m_nMsgLen , pData ,sizeof(m_nLength - 4));
	pData += sizeof(m_nMsgLen);
	nOffset += sizeof(m_nMsgLen);*/
	m_nMsgLen = m_nLength -4;
	//*nLen -= nOffset;
	return nOffset; 
}


bool CHeader::PrintInHex(char* pData, int nLen)
{
//	TRACE(L"\n");
//	for (int i=0; i<nLen; i++)
//	{
//		WCHAR temp[100] = {L""};
//		_stprintf(temp, L"%02X ", pData[i]);
//		TRACE(temp);
//	}
//	TRACE(L"\n");
	return false;
}
