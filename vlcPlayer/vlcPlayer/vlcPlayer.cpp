#include "vlcPlayer.h"
#include <QMouseEvent>

vlcPlayer::vlcPlayer(QWidget *parent)
    : QWidget(parent), m_bMove(false), m_point(QPoint())
{
    ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);

	connect(ui.pushButton_min, &QPushButton::clicked, this, &vlcPlayer::showMinimized);
	connect(ui.pushButton_close, &QPushButton::clicked, this, &vlcPlayer::close);
	connect(ui.pushButton_max, &QPushButton::clicked, this, &vlcPlayer::sltMaxOrNormal);
}

vlcPlayer::~vlcPlayer()
{
}

void vlcPlayer::sltMaxOrNormal()
{

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
}

void vlcPlayer::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
}
