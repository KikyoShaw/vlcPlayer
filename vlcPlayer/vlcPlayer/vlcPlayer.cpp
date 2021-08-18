#include "vlcPlayer.h"
#include <QMouseEvent>
#include "videoControls.h"

vlcPlayer::vlcPlayer(QWidget *parent)
    : QWidget(parent), m_bMove(false), m_point(QPoint())
{
    ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);

	ui.widget_player->installEventFilter(this);

	initVideoControls();

	connect(ui.pushButton_min, &QPushButton::clicked, this, &vlcPlayer::showMinimized);
	connect(ui.pushButton_close, &QPushButton::clicked, this, &vlcPlayer::close);
	connect(ui.pushButton_max, &QPushButton::clicked, this, &vlcPlayer::sltMaxOrNormal);
}

vlcPlayer::~vlcPlayer()
{
}

void vlcPlayer::initVideoControls()
{
	m_videoControls = QSharedPointer<VideoControls>(new VideoControls(this));
	if (m_videoControls) {
		m_videoControls->installEventFilter(this);
		connect(m_videoControls.data(), &VideoControls::sigVideoPlayOrPause, this, &vlcPlayer::sltVideoPlayOrPause);
		connect(m_videoControls.data(), &VideoControls::sigSetPosition, this, &vlcPlayer::sltSetPosition);
		connect(m_videoControls.data(), &VideoControls::sigSoundVoiceValue, this, &vlcPlayer::sltSoundVoiceValue);
	}
}

void vlcPlayer::locateWidgets()
{
	if (m_videoControls) {
		int posX = ui.widget_player->x();
		int posY = ui.widget_top->height() + ui.widget_player->height() + 6 - m_videoControls->height();
		m_videoControls->setFixedWidth(width() - posX - 20);
		m_videoControls->move(mapToGlobal(QPoint(posX, posY)));
		m_videoControls->locateWidgets();
	}
}

void vlcPlayer::sltVideoPlayOrPause(bool state)
{

}

void vlcPlayer::sltSoundVoiceValue(int value)
{

}

void vlcPlayer::sltSetPosition(int value)
{

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
		m_bMove = true;
		//获取移动的位移量
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void vlcPlayer::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}

void vlcPlayer::showEvent(QShowEvent * event)
{
	QWidget::showEvent(event);
	locateWidgets();
}

void vlcPlayer::closeEvent(QCloseEvent * event)
{
	if (m_videoControls) {
		m_videoControls->closeWidget();
	}
	QWidget::closeEvent(event);
}

void vlcPlayer::moveEvent(QMoveEvent * event)
{
	QWidget::moveEvent(event);
	locateWidgets();
}

void vlcPlayer::hideEvent(QHideEvent * event)
{
	if (m_videoControls) {
		m_videoControls->setVisible(false);
	}
	locateWidgets();
	QWidget::hideEvent(event);
}

void vlcPlayer::resizeEvent(QResizeEvent * event)
{
	QWidget::resizeEvent(event);
	locateWidgets();
}

bool vlcPlayer::eventFilter(QObject * obj, QEvent * event)
{
	if (obj == Q_NULLPTR) {
		return false;
	}
	if (m_videoControls == Q_NULLPTR) {
		return false;
	}

	if (QEvent::Enter == event->type()) {
		if (obj == ui.widget_player || obj == m_videoControls) {
			/*if (QMediaPlayer::StoppedState != m_videoPlayer->state()) {
				m_videoControls->setVisible(true);
			}*/
			m_videoControls->setVisible(true);
			return false;
		}
	}
	else if (QEvent::Leave == event->type()) {
		if (obj != ui.widget_player || obj != m_videoControls) {
			auto volumIsVisible = m_videoControls->getVolumVisible();
			if (!volumIsVisible) {
				m_videoControls->setVisible(false);
			}
			return false;
		}
	}
	return QWidget::eventFilter(obj, event);
}
