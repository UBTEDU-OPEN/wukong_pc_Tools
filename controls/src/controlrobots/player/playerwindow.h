/******************************************************************************
    Simple Player:  this file is part of QtAV examples
    Copyright (C) 2012-2016 Wang Bin <wbsecg1@gmail.com>

*   This file is part of QtAV

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QWidget>
#include <QtAV>
#include <QLabel>
#include "ccustomdialog.h"

QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
class CTitleWidget;
QT_END_NAMESPACE

class PlayerWindow : public XDialog
{
    Q_OBJECT
public:
    explicit PlayerWindow(QWidget *parent = 0);

public:
    void init ();
    void createContens ();
    void initAction ();
    void createTitleWidget ();

public slots:
    void tryHideControlBar();
    void tryShowControlBar();
    void onPositionChange(qint64 pos);
    void slotMedioRote();
    void slotFullScreen();

private:
    void workaroundRendererSize();

public Q_SLOTS:
    void openMedia();
    void seekBySlider(int value);
    void seekBySlider();
    void playPause();
    void isVolume();
    void setVolume();
    void onStartPlay ();
    void onStopPlay();

private Q_SLOTS:
    void updateSlider(qint64 value);
    void updateSlider();
    void updateSliderUnit();

protected:
    void mouseMoveEvent(QMouseEvent *e);

public:
    void play(const QString& file);

private:
    CTitleWidget*     m_pTitleWidget;
    QtAV::VideoOutput *m_vo;
    QtAV::AVPlayer *m_player;
    QSlider *m_pTimeSlider;          // 时间进度条

    QWidget     *m_pControl;
    QLabel      *m_pPlayTime;        // 显示格式为：已播放时间/总播放时长

    QPushButton *m_openBtn;
    QPushButton *m_playBtn;
    QPushButton *m_stopBtn;
    QPushButton *m_pRoteBtn;
    QPushButton *m_pFullScreenBtn;

    QPushButton* m_pVolumeBtn;       // 是否静音
    QSlider *m_pVolumeSlider;        // 声音进度条
    int m_nShowControl; //0: can hide, 1: show and playing, 2: always show(not playing)
    int m_unit;
    bool m_bNoVolume;
};

#endif // PLAYERWINDOW_H
