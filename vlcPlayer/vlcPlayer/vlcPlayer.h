#pragma once

#include <QtWidgets/QWidget>
#include "ui_vlcPlayer.h"

class vlcPlayerManager;
class QThread;
class VideoList;
class QMovie;

class vlcPlayer : public QWidget
{
    Q_OBJECT

public:
    vlcPlayer(QWidget *parent = Q_NULLPTR);
	~vlcPlayer();

private:
	void initVlcPlayer();
	void initVideoControls();
	void initVideoList();
	void initMain();
	void initLoadGif();
	void updateVlcPlayerTime(int sec);
	void locateWidgets();

private slots:
	void sltMaxOrNormal();
	void sltVideoPlayOrPause(bool state);
	void sltSoundVoiceValue(int value);
	void sltSetPosition(int value);
	void sltPlayVlcByLink();
	void sltPlayVlcByLocal();
	void sltVlcMediaPlayerVount(int sec);
	void sltVlcMediaPlayerTimeChange(int sec);
	void sltSendPathToVlc(const QString& path);

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void closeEvent(QCloseEvent *event);
	virtual void showEvent(QShowEvent *event);
	virtual void moveEvent(QMoveEvent *event);
	virtual void hideEvent(QHideEvent *event);
	virtual void resizeEvent(QResizeEvent *event);

private:
    Ui::vlcPlayerClass ui;
	bool m_bMove;
	QPoint m_point;
	//播放器对象
	vlcPlayerManager *m_vlcPlayer = nullptr;
	//播放器线程
	QThread *m_thread = nullptr;
	//记录媒体文件总时长
	int m_totalTime;
	//媒体文件初始播放音量
	int m_volumn;
	//是否完成播放
	bool m_isFinishPlay;
	//播放列表
	QSharedPointer <VideoList> m_videoList = nullptr;
	//gif
	QMovie *m_loadGif = nullptr;
};
