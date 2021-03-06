#include "vlcPlayer.h"
#include <QMouseEvent>
#include "videoControls.h"
#include "vlcPlayerManager.h"
#include <QThread>
#include "videoList.h"
#include <QMovie>
#include <QTime>
#include <QDesktopServices>

vlcPlayer::vlcPlayer(QWidget *parent)
    : QWidget(parent), m_bMove(false), m_point(QPoint()), 
	m_totalTime(QString()), m_volumn(100), m_isFinishPlay(false),
	m_position(0), m_vlcList(QStringList())
{
    ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	//setAttribute(Qt::WA_TranslucentBackground);
	ui.lineEdit_search->setText("http://vfx.mtime.cn/Video/2019/03/14/mp4/190314223540373995.mp4");
	ui.widget_player->installEventFilter(this);

	initVideoControls();
	initVlcPlayer();
	initVideoList();
	initMain();
	initLoadGif();

	connect(ui.pushButton_min, &QPushButton::clicked, this, &vlcPlayer::showMinimized);
	connect(ui.pushButton_close, &QPushButton::clicked, this, &vlcPlayer::close);
	connect(ui.pushButton_max, &QPushButton::clicked, this, &vlcPlayer::sltMaxOrNormal);
	connect(ui.pushButton_find, &QPushButton::clicked, this, &vlcPlayer::sltPlayVlcByLink);
	connect(ui.pushButton_local, &QPushButton::clicked, this, &vlcPlayer::sltPlayVlcByLocal);
}

vlcPlayer::~vlcPlayer()
{
	if (m_vlcPlayer){
		m_vlcPlayer->Release();
	}
	if (m_thread){
		m_thread->quit();
		m_thread->wait();
	}
}

void vlcPlayer::initVlcPlayer()
{
	m_vlcPlayer = new vlcPlayerManager(this);
	if (m_vlcPlayer) {
		m_thread = new QThread(this);
		if (m_thread) {
			m_vlcPlayer->moveToThread(m_thread);
			m_thread->start();
		}
		//初始化vlc
		m_vlcPlayer->initVlc();
		//初始化音量
		m_vlcPlayer->SetVolume(m_volumn);
		//连接信号与槽
		connect(m_vlcPlayer, &vlcPlayerManager::playToralTime, this, &vlcPlayer::sltVlcMediaPlayerVount);
		connect(m_vlcPlayer, &vlcPlayerManager::playCurrentTime, this, &vlcPlayer::sltVlcMediaPlayerTimeChange);
		connect(m_vlcPlayer, &vlcPlayerManager::playToralTimeAudio, this, &vlcPlayer::sltVlcMediaPlayerVountAudio);
	}
}

void vlcPlayer::initVideoControls()
{
	ui.widget_title_top->setVoiceValue(m_volumn);
	connect(ui.widget_title_top, &VideoControls::sigVideoPlayOrPause, this, &vlcPlayer::sltVideoPlayOrPause);
	connect(ui.widget_title_top, &VideoControls::sigSetPosition, this, &vlcPlayer::sltSetPosition);
	connect(ui.widget_title_top, &VideoControls::sigSoundVoiceValue, this, &vlcPlayer::sltSoundVoiceValue);
	connect(ui.widget_title_top, &VideoControls::sigSetRate, this, &vlcPlayer::sltSetRate);
}

void vlcPlayer::initVideoList()
{
	m_videoList = QSharedPointer<VideoList>(new VideoList(this));
	if (m_videoList) {
		m_videoList->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool); 
		connect(m_videoList.data(), &VideoList::sigSendPathToVlc, this, &vlcPlayer::sltSendPathToVlc);
		connect(m_videoList.data(), &VideoList::sigSendIndexToVlc, this, &vlcPlayer::sltSendIndexToVlc);
		connect(m_videoList.data(), &VideoList::sigListModel, this, &vlcPlayer::sltSetListPlayModel);
		connect(m_videoList.data(), &VideoList::sigSendPathListToVlc, this, [=](const QStringList &videoList) {
			m_vlcList = videoList;
			if (m_vlcPlayer) {
				m_vlcPlayer->addPlayList(m_vlcList, (void*)ui.widget_player->winId());
			}
		});
	}
}

void vlcPlayer::initMain()
{
	connect(ui.pushButton_return, &QPushButton::clicked, this, [=]() {
		if (!ui.page_mian->isVisible()) {
			ui.stackedWidget->setCurrentWidget(ui.page_mian);
		}
		else {
			auto state = m_vlcPlayer->GetPlayState();
			if (libvlc_Playing == state || libvlc_Paused == state) {
				ui.stackedWidget->setCurrentWidget(ui.page_player);
			}
		}
	});
	auto csdnText = QStringLiteral("CSDN：<a style='text-decoration:none;'href = 'https://blog.csdn.net/qq_36651243'>%1</a>").arg(QStringLiteral("点击进入"));
	auto githubText = QStringLiteral("GitHub：<a style='text-decoration:none;'href = 'https://github.com/KikyoShaw'>%1</a>").arg(QStringLiteral("点击进入"));
	ui.label_csdn->setText(csdnText);
	ui.label_github->setText(githubText);
	connect(ui.label_csdn, &QLabel::linkActivated, this, [=](const QString & text) {
		QDesktopServices::openUrl(QUrl(text));
	});
	connect(ui.label_github, &QLabel::linkActivated, this, [=](const QString & text) {
		QDesktopServices::openUrl(QUrl(text));
	});
	ui.stackedWidget->setCurrentWidget(ui.page_mian);
}

void vlcPlayer::initLoadGif()
{
	m_loadGif = new QMovie(this);
	m_loadGif->setFileName(":/qrc/qrc/load.gif");
	ui.label_loading->setMovie(m_loadGif);
	m_loadGif->start();
}

void vlcPlayer::locateWidgets()
{
	ui.widget_title_top->locateWidgets();
	if (m_videoList) {
		int pWidth = ui.stackedWidget->width();
		int pHeight = ui.stackedWidget->height();
		int posX = 10;
		int posY = ui.widget_top->height() + 7;
		m_videoList->resize(pWidth, pHeight);
		m_videoList->move(mapToGlobal(QPoint(posX, posY)));
	}
}

void vlcPlayer::sltVideoPlayOrPause(bool state)
{
	if (!state) {
		m_vlcPlayer->Pause();
	}
	else {
		if (m_isFinishPlay) {
			m_vlcPlayer->StopPlaying();
			m_isFinishPlay = false;
		}
		m_vlcPlayer->Play();
	}
}

void vlcPlayer::sltSoundVoiceValue(int value)
{
	if (m_vlcPlayer){
		m_vlcPlayer->SetVolume(value);
	}
}

void vlcPlayer::sltSetPosition(int value)
{
	if (m_vlcPlayer){
		m_vlcPlayer->SetPlayTime(value * 1000);
	}
}

void vlcPlayer::sltPlayVlcByLink()
{
	auto linkUrl = ui.lineEdit_search->text();
	if (!linkUrl.isEmpty()) {
		if (m_vlcPlayer) {
			ui.stackedWidget->setCurrentWidget(ui.page_loading);
			m_vlcPlayer->PlayUrl(linkUrl, (void*)ui.widget_player->winId());
		}
	}
}

void vlcPlayer::sltPlayVlcByLocal()
{
	if (m_videoList) {
		auto isVisible = m_videoList->isVisible();
		m_videoList->setVisible(!isVisible);
	}
}

void vlcPlayer::sltVlcMediaPlayerVount(int duration)
{
	if (m_vlcPlayer) {
		ui.widget_title_top->setProgressDuration(duration);
		// 保存视频的总长度
		duration = duration * 1000;
		auto hh = duration / 3600000;
		auto mm = (duration % 3600000) / 60000.0;
		auto ss = (duration % 60000) / 1000.0;
		QTime allTime(hh, mm, ss);
		m_totalTime = allTime.toString(tr("hh:mm:ss"));
		sltVlcMediaPlayerTimeChange(0);
		ui.stackedWidget->setCurrentWidget(ui.page_player);
	}
	if (!m_isFinishPlay) {
		ui.widget_title_top->setPlaying(true);
	}
}

void vlcPlayer::sltVlcMediaPlayerVountAudio(int duration)
{
	if (m_vlcPlayer) {
		ui.widget_title_top->setProgressDuration(duration);
		// 保存视频的总长度
		duration = duration * 1000;
		auto hh = duration / 3600000;
		auto mm = (duration % 3600000) / 60000.0;
		auto ss = (duration % 60000) / 1000.0;
		QTime allTime(hh, mm, ss);
		m_totalTime = allTime.toString(tr("hh:mm:ss"));
		sltVlcMediaPlayerTimeChange(0);
		ui.stackedWidget->setCurrentWidget(ui.page_mian);
	}
	if (!m_isFinishPlay) {
		ui.widget_title_top->setPlaying(true);
	}
}

void vlcPlayer::sltVlcMediaPlayerTimeChange(int position)
{
	// 刷新进度条
	//if (m_position != position) {
		m_position = position;
		ui.widget_title_top->setSliderPosition(position);
		position = position * 1000;
		auto hh = position / 3600000;
		auto mm = (position % 3600000) / 60000.0;
		auto ss = (position % 60000) / 1000.0;
		QTime duration(hh, mm, ss);
		auto localTime = duration.toString(tr("hh:mm:ss"));
		auto text = QString("%1/%2").arg(localTime).arg(m_totalTime);
		ui.widget_title_top->setProgressText(text);
		if (localTime == m_totalTime) {
			m_isFinishPlay = true;
			ui.widget_title_top->setPlaying(false);
		}
	/*}*/
}

void vlcPlayer::sltSendIndexToVlc(int index)
{
	if (m_videoList) {
		m_videoList->setVisible(false);
	}
	if (m_vlcPlayer) {
		m_vlcPlayer->PlayList(index);
	}
}

void vlcPlayer::sltSendPathToVlc(const QString & path)
{
	if (m_videoList) {
		m_videoList->setVisible(false);
	}
	if (m_vlcPlayer) {
		//m_vlcPlayer->Play(path, (void*)ui.widget_player->winId());
		if (m_vlcList.contains(path)) {
			m_vlcPlayer->PlayList(path);
		}
	}
}

void vlcPlayer::sltSetRate(float rate)
{
	if (m_vlcPlayer) {
		m_vlcPlayer->setRate(rate);
	}
}

void vlcPlayer::sltSetListPlayModel(int id)
{
	if (m_vlcPlayer) {
		m_vlcPlayer->setListPlayModel(id);
	}
}

void vlcPlayer::sltMaxOrNormal()
{
	if (isMaximized())
		showNormal();
	else
		showMaximized();
}

void vlcPlayer::mouseMoveEvent(QMouseEvent * event)
{
	//判断左键是否被按下，只有左键按下了，其返回值就是1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void vlcPlayer::mousePressEvent(QMouseEvent * event)
{
	//鼠标事件中包含“按住的是左键”
	if ((event->button() == Qt::LeftButton) && (event->pos().y() < 56) && (event->pos().y() > 0))
	{
		if (!isMaximized()) {
			m_bMove = true;
			//获取移动的位移量
			m_point = event->globalPos() - frameGeometry().topLeft();
		}
	}
	QWidget::mousePressEvent(event);
}

void vlcPlayer::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}

void vlcPlayer::closeEvent(QCloseEvent * event)
{
	if (m_vlcPlayer){
		m_vlcPlayer->Stop();
	}
	QWidget::closeEvent(event);
}

void vlcPlayer::showEvent(QShowEvent * event)
{
	QWidget::showEvent(event);
	locateWidgets();
}

void vlcPlayer::moveEvent(QMoveEvent * event)
{
	QWidget::moveEvent(event);
	locateWidgets();
}

void vlcPlayer::hideEvent(QHideEvent * event)
{
	ui.widget_title_top->closeWidget();
	QWidget::hideEvent(event);
}

void vlcPlayer::resizeEvent(QResizeEvent * event)
{
	QWidget::resizeEvent(event);
	locateWidgets();
}

//上下调节音量，间隔为5; 左右快退快进，间隔为15s
void vlcPlayer::keyReleaseEvent(QKeyEvent *event)
{
	auto key = event->key();
	switch (key)
	{
	case Qt::Key_Left: {
		if (m_vlcPlayer) {
			if (!m_isFinishPlay) {
				m_position = m_position - 15;
				m_vlcPlayer->SetPlayTime(m_position * 1000);
			}
		}
		break;
	}
	case Qt::Key_Right: {
		if (m_vlcPlayer) {
			if (!m_isFinishPlay) {
				m_position = m_position + 15;
				m_vlcPlayer->SetPlayTime(m_position * 1000);
			}
		}
		break;
	}
	case Qt::Key_Up: {
		if (m_vlcPlayer) {
			if (!m_isFinishPlay) {
				m_volumn = m_volumn + 5;
				//最大不超过200
				m_volumn = qMin(m_volumn, 200);
				ui.widget_title_top->setVoiceValue(m_volumn);
			}
		}
		break;
	}
	case Qt::Key_Down: {
		if (m_vlcPlayer) {
			if (!m_isFinishPlay) {
				m_volumn = m_volumn - 5;
				//最小不小过0
				m_volumn = qMax(m_volumn, 0);
				ui.widget_title_top->setVoiceValue(m_volumn);
			}
		}
		break;
	}
	default:
		break;
	}
	QWidget::keyReleaseEvent(event);
}