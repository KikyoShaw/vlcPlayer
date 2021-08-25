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
	void sltSetListPlayModel();

signals:
	void sigSendIndexToVlc(int index);
	void sigSendPathToVlc(const QString& fileName);
	void sigSendPathListToVlc(const QStringList& videoList);
	void sigListModel(int id);

private:
	Ui::mediaList ui;
	//播放路径存储
	QMap<int, QString> m_videoPathMap;
	//数据实体
	QVector<QString> m_videoNameVec;
};