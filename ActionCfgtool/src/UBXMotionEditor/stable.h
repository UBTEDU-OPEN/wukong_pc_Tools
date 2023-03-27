#ifndef STABLE
#define STABLE

#include <QWidget>
#include <QString>
#include <QPoint>
#include <QtMath>
#include <QDebug>
#include <QPen>
#include <QPainter>
#include "../LibTreeNavigation/ILibTreeNavigation.h"
#include "constant.h"
//#include "configs.h"
#include "ubxdatabase.h"

#define MAX_PATH  256

typedef struct
{
    int nTotalTime;		// 总的时间
    int nMyPos;         // 偏移值

}LAYER_CONFIGURE, *PLAYER_CONFIGURE;

typedef struct _UBXMusicInfo{
    const char* szFilePath;
    const char* szFileName;
    const char* szWavePath;
//    UBXStreamTime StreamTime;
    int           nBPM;
    //int           nLength;
    //UBXMusicInfo::
    _UBXMusicInfo()
    {
        szFileName = NULL;
        szFilePath= NULL;
        nBPM = 0;
        szWavePath = NULL;
        //nLength = sizeof(_UBXMusicInfo);
    }
} UBXMusicInfo;


#define SecsPerMinute   60000    //每分钟秒数
enum PLAYSTATE
{
    PLAYSTATE_STOP = 0 ,    //层停止
    PLAYSTATE_PLAYING  ,    //层播放
    PLAYSTATE_PAUSING  ,    //层暂停
};
#define  SINGLE_LAYER_DEBUG 1
#define  ALL_LAYER_DEBUG 2


#define GRID_MAX_TIMEVALUE  65535   //最大格数
#define GRID_MIN_TIMEVALUE  0       //最小格数

//#define GRID_MAX_ANGLEVALUE  180    //一代舵机的最大角度是180
#define GRID_MAX_ANGLEVALUE  240    //二代舵机的最大角度是240
#define GRID_MIN_ANGLEVALUE  -1

//舵机的最大运动时间
#define MOTOR_MAX_MOVING_TIME 5000   //ms

//舵机数（此处可以改进，通过读XML文件的方式读取舵机数）
//#define SERVO_COUNT 16

enum GROUP_LIST_CHOSEN_ACTION
{
    NO_ACTION_CHOSEN = -1,
    COPY_ACTION_CHOSEN = 0,
    PASTE_ACTION_CHOSEN,
    CUT_ACTION_CHOSEN,
    DELETE_ACTION_CHOSEN,
    SELECTEDBLOCKPLAY_ACTION_CHOSEN,
    CANCELBLOCKPLAY_ACTION_CHOSEN,
    INSERTBLANKTIME_ACTION_CHOSEN,
    SAVETOCOMMANDBOX_ACTION_CHOSEN,
    MODIFYGROUP_ACTION_CHOSEN
};

extern bool g_IsUpdateUIByData;
extern bool g_IsModifyGroup;



#endif // STABLE

