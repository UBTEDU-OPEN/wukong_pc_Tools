#ifndef UBXWIDGETCONST_H
#define UBXWIDGETCONST_H

namespace UBXUiBaseConstants
{
    const int BTN_PAGES_NUM_1  = 1;
    const int BTN_PAGES_NUM_2  = 2;
    const int BTN_PAGES_NUM_3  = 3;
    const int BTN_PAGES_NUM_4  = 4;
    const int BTN_PAGES_NUM_5  = 5;
    const int BTN_PAGES_NUM_6  = 6;
    const int BTN_PAGES_NUM_7  = 7;
    const int BTN_PAGES_NUM_8  = 8;
    const int BTN_PAGES_NUM_9  = 9;
    const int BTN_PAGES_NUM_10  = 10;

    const int VIDEO_CAPTURE_INTERNAL = 500; // 视频捕获时播放器暂停到播放的时间间隔

    const char* const PROPERTY_PAGE_REAL_NUMBER = "PropertyPageRealNumber";//页码按钮对应的真实页码值属性
        //leo 解决编译问题 暂时
        const QString UBT_DeviceLink = "";
        // 评论控件运行的最大评论字符数
        const int COMMENT_MAX_CHARACTOR_NUMBER = 200;

        // 颜色值
        const QString UBX_COLOR_GREY = "#d3d3d3";
        const QString UBX_COLOR_WHITE = "#ffffff";
        const QString UBX_COLOR_DARK_RED = "#ca0813";
        const QString UBX_COLOR_DARK_GREY = "#858585";

        // 字体大小
        const int FONT_POINT_SIZE_10 = 10; // 10字体大小
        const int FONT_POINT_SIZE_12 = 12; // 12字体大小
        const int FONT_POINT_SIZE_14 = 14; // 14字体大小
        const int FONT_POINT_SIZE_18 = 18;
}

#endif // UBXWIDGETCONST_H
