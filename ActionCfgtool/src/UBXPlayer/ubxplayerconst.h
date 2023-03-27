#ifndef UBXPLAYERCONST_H
#define UBXPLAYERCONST_H

namespace UBXPlayerConstants
{
    const int PLAYER_MAX_VOLUME = 100;  // 播放器声音控制最大值（QSlider中最大值）
    const int VIDEO_CAPTURE_FRAME_INDEX  = 15; // 视频播放捕获的帧id，用做视频播放器封面
    const int MEDIA_PLAYER_NOTIFY_INTERNAL = 500; // 播放器更新时间间隔
    const int MEIDA_PLAY_END_OFF_SET = 50;  // 视频文件播放结束前的偏差值，当播放到视频长度-MEIDA_PLAY_END_OFF_SET时，认为播放结束

    const int PLAY_POSITION_NOT_CHANGED_MAX_CNT = 5; // 视频播放位置没有发生变化的次数

    // 视频播放器界面上播放背景按钮的大小
    const int PLAYER_BACKGROUND_PLAY_BUTTON_WIDTH = 87;
    const int PLAYER_BACKGROUND_PLAY_BUTTON_HEIGHT = 87;

    const int CAPTURE_VIDEO_FRAME_TIMEOUT = 10000; // 捕获视频第一帧超时时间

    const int VIDEO_OUTPUT_BLANK_HEIGHT = 30; // 视频播放区域空白高度
    const qreal GRAPHVIEW_X_SCALE = 1.18; // QGraphcisView水平拉伸，具体数值根据调试得出
    const qreal GRAPHVIEW_Y_SCALE = 1.10; // QGraphcisView竖直拉伸

    const QString VIDEO_CAPTURE_PREVIEW_FRAME = "videoCaptureCoverFrame.png"; // 视频封面捕获帧图片名称
}

#endif // UBXPLAYERCONST_H

