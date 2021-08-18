#pragma once

#include <QtWidgets/QWidget>
#include "ui_vlcPlayer.h"

class vlcPlayer : public QWidget
{
    Q_OBJECT

public:
    vlcPlayer(QWidget *parent = Q_NULLPTR);

private:
    Ui::vlcPlayerClass ui;
};
