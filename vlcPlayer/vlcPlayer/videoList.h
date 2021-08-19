#pragma once

#include <QWidget>
#include "ui_videoList.h"

class VideoList :public QWidget
{
	Q_OBJECT
public:
	VideoList(QWidget *parent = Q_NULLPTR);
	~VideoList();

private:

private slots:
	void sltLoadMiedaList();
	void sltListWidgetDoubleClicked(QListWidgetItem * item);

signals:
	void sigSendPathToVlc(const QString& fileName);

private:
	Ui::mediaList ui;
	//����·���洢
	QMap<int, QString> m_videoPathMap;
	//����ʵ��
	QVector<QString> m_videoNameVec;
};