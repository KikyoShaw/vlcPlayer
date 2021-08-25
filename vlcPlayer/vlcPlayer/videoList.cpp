#include "videoList.h"
#include <QFileDialog>
#include <QButtonGroup>
#include <QScrollBar>

VideoList::VideoList(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_TranslucentBackground);

	//�Ż�������
	QFile QSS1(":/qss/qrc/qss/whiteScrollbar.qss");
	if (QSS1.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS1.readAll();
		ui.listWidget_localList->verticalScrollBar()->setStyleSheet(strStyle);
	}

	//�б���ʽ
	QFile QSS2(":/qss/qrc/qss/listWidget.qss");
	if (QSS2.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS2.readAll();
		ui.listWidget_localList->setStyleSheet(strStyle);
	}

	m_videoNameVec.clear();

	connect(ui.pushButton, &QPushButton::clicked, this, &VideoList::sltLoadMiedaList);

	//�б����¼�
	connect(ui.listWidget_localList, &QListWidget::itemDoubleClicked, this, &VideoList::sltListWidgetDoubleClicked);
}

VideoList::~VideoList()
{
}

void VideoList::sltListWidgetDoubleClicked(QListWidgetItem * item)
{
	if (!item)
		return;

	auto row = ui.listWidget_localList->currentRow();
	QString fileName = QString();
	if (m_videoPathMap.contains(row)) {
		emit sigSendIndexToVlc(row);
		fileName = m_videoPathMap.value(row);
	}

	if (!fileName.isEmpty()) {
		emit sigSendPathToVlc(fileName);
	}

}

void VideoList::sltLoadMiedaList()
{
	auto fileList = QFileDialog::getOpenFileNames(this,
		QStringLiteral("ѡ��ý���ļ�"),
		".",
		QStringLiteral("ý���ļ�(*.avi *.mp4 *.flv *.mkv *.mp3 *.wav *.wma);"));

	if (fileList.isEmpty()) return;
	m_videoPathMap.clear();
	ui.listWidget_localList->clear();
	//�洢����ʵ��
	for (auto &each : fileList) {
		if (!m_videoNameVec.contains(each)) {
			m_videoNameVec.append(each);
		}
	}
	//��������ʵ��
	for (int i = 0; i < m_videoNameVec.size(); i++) {
		auto each = m_videoNameVec.value(i);
		QFileInfo fileInfo(each);
		if (fileInfo.exists()) {
			auto videoPath = QDir::toNativeSeparators(each);
			auto videoName = fileInfo.fileName();
			ui.listWidget_localList->insertItem(i, videoName);
			m_videoPathMap.insert(i, videoPath);
		}
	}

	QStringList videoList = m_videoPathMap.values();
	emit sigSendPathListToVlc(videoList);
}
