#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TestTool.h"

class TestTool : public QMainWindow
{
	Q_OBJECT

public:
	TestTool(QWidget *parent = Q_NULLPTR);

private:
	Ui::TestToolClass ui;
};
