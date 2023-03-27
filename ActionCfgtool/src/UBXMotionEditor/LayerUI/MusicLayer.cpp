#include "MusicLayer.h"
#include <QFileInfo>
#include <QDir>
#include "configs.h"
CMusicLayer::CMusicLayer(void)
{
}

CMusicLayer::~CMusicLayer(void)
{
}

//bool CMusicLayer::Connect(QWidget* managerHandle)
//{
//    CLayerCommonImpl::Connect(managerHandle);
//    return true;
//}

//bool CMusicLayer::ReleaseConnection()
//{
//    CLayerCommonImpl::ReleaseConnection();
//    return true;
//}


bool CMusicLayer::GetData(char** ppData, int& nLen)
{
    CLayerCommonImpl::GetData(ppData, nLen);

    return true;
}

bool CMusicLayer::SetData(char* pData, int nLen)
{
    //leo add 20170318
    //AddMusicFromMusicDir();
    CLayerCommonImpl::SetData(pData, nLen);

    return true;
}

//从Music目录下获取第一个音乐文件并添加
bool CMusicLayer::AddMusicFromMusicDir()
{
    QString strMusicPath;
    QString strMusicFilePath;

    strMusicPath = CConfigs::getLocalMusicDir();
    QFileInfoList infoList = QDir(strMusicPath).entryInfoList();
    foreach (QFileInfo fileInfo, infoList)
    {
        if(!fileInfo.isFile()) continue;

        QString sExt = '.' + fileInfo.suffix();
        if(0 == sExt.compare(MUSIC_FILE_EXT, Qt::CaseInsensitive))
        {
            strMusicFilePath = QDir::toNativeSeparators(strMusicPath + '/' + fileInfo.fileName());
            break;
        }
    }

    if (strMusicFilePath.isEmpty())
    {
        return false;
    }
    else
    {
        AddMusic(strMusicFilePath);
    }

    return true;
}


void CMusicLayer::AddMusic(QString strMusicFile)
{
    CLayerCommonImpl::ILayerOptionHandler_LoadMusic(strMusicFile);
//    if (!m_pMusicChooseDlg && !m_pMusicChooseDlg->GetSafeHwnd())
//    {
//        return;
//    }
//    if (bFromAes)
//    {
//        m_pMusicChooseDlg->ReadMusicFromAesFile(strMusicFile);
//    }
//    else
//        m_pMusicChooseDlg->OnBnClickedButton1();
}
// 取得类型，具体见宏定义
emActionLayerType CMusicLayer::GetType()
{
    return ACTION_LAYER_MUSIC;
}

QString CMusicLayer::GetLayerName()
{
    QString strName = QObject::tr("Music");
    return strName;
}

void CMusicLayer::CreateOptionHandler()
{
    if (m_pOptionHandler)
    {
        m_pOptionHandler->ReleaseConnection();
    }

    CreateMusicLayOptionHandler(&m_pOptionHandler);

    if (m_pOptionHandler)
    {
         m_pOptionHandler->Connect(m_ManagerHandle, this);
    }
}


