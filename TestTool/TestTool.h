#pragma once

#include <QtWidgets/QMainWindow>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include "ui_TestTool.h"

class TestTool : public QMainWindow
{
	Q_OBJECT

public:
	TestTool(QWidget *parent = Q_NULLPTR);
	void dragEnterEvent(QDragEnterEvent *e);
	void dropEvent(QDropEvent *e);

private:
	Ui::TestToolClass ui;
};
