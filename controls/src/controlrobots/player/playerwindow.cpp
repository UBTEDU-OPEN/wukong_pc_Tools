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

#include "playerwindow.h"
#include "titlewidget.h"

#include <QPushButton>
#include <QSlider>
#include <QLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QtAVWidgets>
#include <QTimer>
using namespace QtAV;

const qreal kVolumeInterval = 0.04;

PlayerWindow::PlayerWindow(QWidget *parent) : XDialog(parent)
{
    m_nShowControl = 2;
    m_bNoVolume = false;
    setMouseTracking(true);
    init ();
}

void PlayerWindow::init ()
{
    QtAV::Widgets::registerRenderers();
    setWindowTitle(tr("UBTECH"));
    createTitleWidget ();
    createContens ();
    initAction ();
    setMinimumSize(1024, 768);
}

void PlayerWindow::createContens ()
{
    m_player = new AVPlayer(this);

    m_vo = new VideoOutput(this);
    if (!m_vo->widget()) {
        QMessageBox::warning(0, QString::fromLatin1("player error"), tr("Can not create video renderer"));
        return;
    }
    m_vo->widget()->setMouseTracking(true);
    m_player->setRenderer(m_vo);

    m_pTimeSlider = new QSlider(this);
    m_pTimeSlider->setOrientation(Qt::Horizontal);

    m_pControl = new QWidget(this);
    m_pControl->setMaximumHeight(68);
    m_pControl->setAttribute(Qt::WA_TranslucentBackground);

    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->setSpacing(0);
    controlLayout->setContentsMargins(QMargins(1, 1, 1, 1));
    m_pControl->setLayout(controlLayout);

    m_pPlayTime = new QLabel(m_pControl);
    m_pPlayTime->setContentsMargins(QMargins(2, 2, 2, 2));
    m_pPlayTime->setText("00:00:00/00:00:00");

    m_openBtn = new QPushButton(m_pControl);
    m_openBtn->setFixedSize(32, 32);
    m_playBtn = new QPushButton(m_pControl);
    m_playBtn->setFixedSize(48, 48);
    m_stopBtn = new QPushButton(m_pControl);
    m_stopBtn->setFixedSize(32, 32);

    QHBoxLayout* pMidHBox = new QHBoxLayout;
    pMidHBox->addWidget(m_openBtn);
    pMidHBox->addWidget(m_playBtn);
    pMidHBox->addWidget(m_stopBtn);
    pMidHBox->setSpacing(5);

    m_pVolumeBtn = new QPushButton(m_pControl);
    m_pVolumeBtn->setIcon(QIcon(":/png/png/player/volume.png"));
    m_pVolumeBtn->setFixedSize(24, 24);

    m_pVolumeSlider = new QSlider(m_pControl);
    m_pVolumeSlider->setOrientation(Qt::Horizontal);
    m_pVolumeSlider->setMaximumWidth(88);
    m_pVolumeSlider->setMinimum(0);
    const int kVolumeSliderMax = 100;
    m_pVolumeSlider->setMaximum(kVolumeSliderMax);
    m_pVolumeSlider->setValue(int(1.0/kVolumeInterval*qreal(kVolumeSliderMax)/100.0));

    QHBoxLayout* pVolHbox = new QHBoxLayout;
    pVolHbox->addWidget(m_pVolumeBtn);
    pVolHbox->addWidget(m_pVolumeSlider);

    m_pRoteBtn = new QPushButton(m_pControl);
    m_pRoteBtn->setToolTip(tr("屏幕旋转"));
    m_pRoteBtn->setFixedSize(32, 32);
    m_pRoteBtn->setIcon(QIcon(":/png/png/player/rote.png"));

    m_pFullScreenBtn = new QPushButton(m_pControl);
    m_pFullScreenBtn->setFixedSize(32, 32);
    m_pFullScreenBtn->setIcon(QIcon(":/png/png/player/fullscreen.png"));

    QHBoxLayout* pRightHbox = new QHBoxLayout;
    pRightHbox->addWidget(m_pRoteBtn);
    pRightHbox->addWidget(m_pFullScreenBtn);
    pRightHbox->setSpacing(10);

    QSpacerItem* pSpaceItem = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Fixed);
    controlLayout->addWidget(m_pPlayTime);
    controlLayout->addSpacerItem(pSpaceItem);
    controlLayout->addLayout(pMidHBox);
    controlLayout->addLayout(pVolHbox);
    controlLayout->addSpacerItem(pSpaceItem);
    controlLayout->addLayout(pRightHbox);
    controlLayout->setSpacing(10);
    controlLayout->setMargin(10);
    controlLayout->setContentsMargins(10, 0, 10, 0);

    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    pMainLayout->addWidget(m_pTitleWidget);
    pMainLayout->addWidget(m_vo->widget());
    pMainLayout->addWidget(m_pTimeSlider);
    pMainLayout->addWidget(m_pControl);
    pMainLayout->setSpacing(0);
    pMainLayout->setMargin(0);
    pMainLayout->setContentsMargins(1, 1, 1, 1);
    this->setLayout (pMainLayout);

    m_pTimeSlider->setStyleSheet(
                QString::fromUtf8("QSlider::add-page:Horizontal{"
                                  "background-color: rgb(87, 97, 106);"
                                  "height:4px;"
                                  "}"
                                  "QSlider::sub-page:Horizontal{"
                                  "background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(231,80,229, 255), stop:1 rgba(7,208,255, 255));"
                                  "height:4px;"
                                  "}"
                                  "QSlider::groove:Horizontal {"
                                  "background:transparent;"
                                  "height:6px;"
                                  "}"
                                  "QSlider::handle:Horizontal{"
                                  "border-radius: 3px;"
                                  "height: 2px;"
                                  "width:20px;"
                                  "background: green;"
                                  "margin: -8 0px;"
                                  "}")
                );

    m_pVolumeSlider->setStyleSheet(
                QString::fromUtf8("QSlider::add-page:Horizontal{"
                                  "background-color: rgb(87, 97, 106);"
                                  "height:4px;"
                                  "}"
                                  "QSlider::sub-page:Horizontal{"
                                  "background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(231,80,229, 255), stop:1 rgba(7,208,255, 255));"
                                  "height:4px;"
                                  "}"
                                  "QSlider::groove:Horizontal {"
                                  "background:transparent;"
                                  "height:6px;"
                                  "}"
                                  "QSlider::handle:Horizontal{"
                                  "border-radius: 3px;"
                                  "height: 2px;"
                                  "width:20px;"
                                  "background: green;"
                                  "margin: -8 0px;"
                                  "}")
                );

    m_openBtn->setStyleSheet("QPushButton{"
                             "background: transparent;"
                             "border-image: url(:/png/png/player/open.png);}");
    m_playBtn->setStyleSheet("QPushButton{"
                             "background: transparent;"
                             "border-image: url(:/png/png/player/start.png);}");
    m_stopBtn->setStyleSheet("QPushButton{"
                             "background: transparent;"
                             "border-image: url(:/png/png/player/stop.png);}");
}

void PlayerWindow::initAction ()
{
    connect(m_pTimeSlider, SIGNAL(sliderMoved(int)), SLOT(seekBySlider(int)));
    connect(m_pTimeSlider, SIGNAL(sliderPressed()), SLOT(seekBySlider()));

    connect(m_pVolumeBtn, SIGNAL(clicked()), SLOT(isVolume()));
    connect(m_pVolumeSlider, SIGNAL(sliderPressed()), SLOT(setVolume()));
    connect(m_pVolumeSlider, SIGNAL(valueChanged(int)), SLOT(setVolume()));

    connect(m_player, SIGNAL(started()), this, SLOT(onStartPlay()));
    connect(m_player, SIGNAL(stopped()), this, SLOT(onStopPlay()));
    connect(m_player, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChange(qint64)));
    connect(m_player, SIGNAL(notifyIntervalChanged()), SLOT(updateSliderUnit()));

    connect(m_openBtn, SIGNAL(clicked()), SLOT(openMedia()));
    connect(m_playBtn, SIGNAL(clicked()), SLOT(playPause()));
    connect(m_stopBtn, SIGNAL(clicked()), m_player, SLOT(stop()));

    connect(m_pRoteBtn, SIGNAL(clicked()), this, SLOT(slotMedioRote()));
    connect(m_pFullScreenBtn, SIGNAL(clicked()), this, SLOT(slotFullScreen()));
}

void PlayerWindow::onStartPlay()
{
    m_playBtn->setIcon(QIcon(QString::fromLatin1(":/png/png/player/pause.png")));
    m_pTimeSlider->setMinimum(m_player->mediaStartPosition());
    m_pTimeSlider->setMaximum(m_player->mediaStopPosition());
    m_pTimeSlider->setValue(0);
    m_pTimeSlider->setEnabled(m_player->isSeekable());
    QString strPlayTime = m_pPlayTime->text();
    strPlayTime = strPlayTime.left(strPlayTime.indexOf("/"));
    strPlayTime += QTime(0, 0, 0).addMSecs(m_player->mediaStopPosition())
            .toString(QString::fromLatin1("HH:mm:ss"));
    m_pPlayTime->setText(strPlayTime);
    setVolume();
    m_nShowControl = 0;
    QTimer::singleShot(3000, this, SLOT(tryHideControlBar()));
    updateSlider(m_player->position());
}

void PlayerWindow::onStopPlay()
{
    m_playBtn->setIcon(QIcon(QString::fromLatin1(":/png/png/player/start.png")));
    m_pTimeSlider->setMinimum(0);
    m_pTimeSlider->setMaximum(0);
    m_pTimeSlider->setValue(0);
    m_pTimeSlider->setEnabled(m_player->isSeekable());
    QString strPlayTime = "00:00:00/00:00:00";
    m_pPlayTime->setText(strPlayTime);
    tryShowControlBar ();
}

void PlayerWindow::tryHideControlBar()
{
    if (m_nShowControl > 0) {
        return;
    }
    if (m_pControl->isHidden() && m_pTimeSlider->isHidden())
        return;
    if ( isFullScreen() )
    {
        m_pTitleWidget->hide();
    }
    m_pControl->hide();
    m_pTimeSlider->hide();
    workaroundRendererSize();
}

void PlayerWindow::tryShowControlBar()
{
    unsetCursor();
    if (m_pTimeSlider && m_pTimeSlider->isHidden())
        m_pTimeSlider->show();
    if (m_pControl && m_pControl->isHidden())
        m_pControl->show();
    if ( m_pTitleWidget && m_pTitleWidget->isHidden() )
        m_pTitleWidget->show();
}

void PlayerWindow::onPositionChange(qint64 pos)
{
    if (m_player->isSeekable())
        m_pTimeSlider->setValue(pos);
    QString strTime = QTime(0, 0, 0).addMSecs(pos).toString(QString::fromLatin1("HH:mm:ss")) + "/" +
            QTime(0, 0, 0).addMSecs(m_player->mediaStopPosition()).toString(QString::fromLatin1("HH:mm:ss"));
    m_pPlayTime->setText(strTime);

    updateSlider(pos);
}

void PlayerWindow::slotMedioRote()
{
    VideoRenderer* renderer = m_player->renderer();
    renderer->setOrientation(renderer->orientation() + 90);
}

void PlayerWindow::slotFullScreen()
{
    if ( isFullScreen() )
    {
        m_pFullScreenBtn->setToolTip("全屏");
        m_pFullScreenBtn->setIcon(QIcon(":/png/png/player/fullscreen.png"));
        showNormal();
    }
    else
    {
        m_pFullScreenBtn->setToolTip("退出全屏");
        m_pFullScreenBtn->setIcon(QIcon(":/png/png/player/nofullscreen.png"));
        showFullScreen();
    }
}

void PlayerWindow::workaroundRendererSize()
{
    if (!m_vo->widget())
        return;
    QSize s = rect().size();
    m_vo->widget()->resize(QSize(s.width()+1, s.height()+1));
    m_vo->widget()->resize(s);
}

void PlayerWindow::mouseMoveEvent(QMouseEvent *e)
{
    unsetCursor();
    if ( e->pos().y() > height() - m_pTimeSlider->height() - m_pControl->height() )
    {
        if (m_nShowControl == 0)
        {
            m_nShowControl = 1;
            tryShowControlBar();
        }
    }
    else
    {
        if (m_nShowControl == 1)
        {
            m_nShowControl = 0;
            QTimer::singleShot(3000, this, SLOT(tryHideControlBar()));
        }
    }
}

void PlayerWindow::isVolume()
{
    m_bNoVolume = !m_bNoVolume;
    AudioOutput *ao = m_player ? m_player->audio() : 0;
    if ( m_bNoVolume )
    {
        if ( ao )
        {
            ao->setVolume(0);
        }
        m_pVolumeBtn->setIcon(QIcon(":/png/png/player/novolume.png"));
    }
    else
    {
        setVolume();
        m_pVolumeBtn->setIcon(QIcon(":/png/png/player/volume.png"));
    }
}

void PlayerWindow::setVolume()
{
    AudioOutput *ao = m_player ? m_player->audio() : 0;
    qreal v = qreal(m_pVolumeSlider->value()) * kVolumeInterval;
    if (ao)
    {
        if (qAbs(int(ao->volume()/kVolumeInterval) - m_pVolumeSlider->value()) >= int(0.1/kVolumeInterval))
        {
            ao->setVolume(v);
        }
    }
    m_pVolumeSlider->setToolTip(QString::number(v));
    m_pVolumeBtn->setToolTip(QString::number(v));
}

void PlayerWindow::createTitleWidget ()
{
    m_pTitleWidget = new CTitleWidget(this);
    m_pTitleWidget->setMenuBtnVisible(false);
    m_pTitleWidget->setVerion(tr("集控软件"));
}

void PlayerWindow::openMedia()
{
    QString file = QFileDialog::getOpenFileName(0, tr("Open a video"));
    if (file.isEmpty())
        return;
    m_player->play(file);
}

void PlayerWindow::seekBySlider(int value)
{
    if (!m_player->isPlaying())
        return;
    m_player->seek(qint64(value));
}

void PlayerWindow::seekBySlider()
{
    seekBySlider(m_pTimeSlider->value());
}

void PlayerWindow::playPause()
{
    if (!m_player->isPlaying())
    {
        m_player->play();
        return;
    }

    if ( !m_player->isPaused() )
    {
        m_playBtn->setStyleSheet("QPushButton{"
                                 "background: transparent;"
                                 "border-image: url(:/png/png/player/start.png);}");
    }
    else
    {
        m_playBtn->setStyleSheet("QPushButton{"
                                 "background: transparent;"
                                 "border-image: url(:/png/png/player/pause.png);}");
    }

    m_player->pause(!m_player->isPaused());
}

void PlayerWindow::updateSlider(qint64 value)
{
    m_pTimeSlider->setRange(0, int(m_player->duration()));
    m_pTimeSlider->setValue(int(value));
}

void PlayerWindow::updateSlider()
{
    updateSlider(m_player->position());
}

void PlayerWindow::updateSliderUnit()
{
    updateSlider();
}

void PlayerWindow::play(const QString& file)
{
    show();
    m_player->play(file);
}
