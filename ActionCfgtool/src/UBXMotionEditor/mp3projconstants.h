/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：mp3projconstants.h
* 创建时间：2016/01/07
* 文件标识：
* 文件摘要：mp3proj模块常量定义
*
* 当前版本：1.0.0.0
* 作    者：hels
* 完成时间：2016/01/07
* 版本摘要：
*/

#include "motioneditor_global.h"
#include <QColor>

#ifndef MP3PROJCONSTANTS
#define MP3PROJCONSTANTS

namespace Constants
{
    //波形控件常量定义，波形下方每个方框为一个cell
    const int DEFAULT_CELL_COUNT = 300; //默认Cell个数
    const int DEFAULT_CELL_WIDTH = 9; //每个Cell宽度
    const int DEFAULT_CELL_HEIGHT = 18;//Cell高度
    const int DEFAULT_LINE_THICK = 1; //刻度线宽度
    const int DEFULAT_PLOT_THICK = 0;//波形图线宽

    const int ADD_OFFSETS = 0;//cellcount的偏移个数

    //没有单独定义宽高的，取DEFAULT_CELL_WIDTH和DEFAULT_CELL_HEIGHT默认值
    const int DEFAULT_CELL_TIME_HEIGHT = 26;//时间Cell高度
    const int DEFAULT_CELL_MUSIC_HEIGHT = 110/*123*/;
//    const int DEFAULT_CELL_MUSIC_HEIGHT = 117/*186*/;//音乐层Cell高度 117 = 186 - 20*3 - 3*3
    const int DEFAULT_CELL_GAP_HEIGHT = 3; //层与层之间的间隙高度

    //相关颜色定义
    const QColor COLOR_SEL_LINE = QColor("#f1c232");//选择线颜色
    const QColor COLOR_LINE =  QColor("#dae5e9");//刻度线颜色
    const QColor COLOR_DEF_BRUSH_CELL = QColor("#f4f8fb");//底部cell默认填充颜色
    const QColor COLOR_BRUSH_CELL = QColor("#f4f8ee");//底部cell填充颜色
    const QColor COLOR_TIME_BG = QColor("#f4f8f8"); //时间背景颜色
    const QColor COLOR_TIME_FONT = QColor("#75a5b9"); //时间字体颜色
    const QColor COLOR_MP3_BG = QColor("#eaeff1"); //时间线、波形、动作组窗体背景颜色
    const QColor COLOR_WAVE_BACK = QColor("#dae5e9");  //波形背景色
    const QColor COLOR_WAVE_LINE = QColor("#20b3f1");//波形颜色
    const QColor COLOR_WAVE_BASE_LINE = QColor("#75a5b9");//波形中间基线
    const QColor COLOR_GROUP_BLOCK_DEFAULT = QColor("#c3eafb");//动作组默认颜色
    const QColor COLOR_GROUP_BLOCK_SELECT = QColor("#48c9ff");//动作组选中颜色
    const QColor COLOR_ACTION_BLOCK_SELECT = QColor("#31e4fa");//Action动作块选中颜色
}

#endif // MP3PROJCONSTANTS

