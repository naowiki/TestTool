#pragma once

#include <QtWidgets/QMainWindow>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QtXml>
#include <QDomDocument>
#include <QFileDialog>
#include "ui_TestTool.h"

class TestTool : public QMainWindow
{
	Q_OBJECT

public:
	TestTool(QWidget *parent = Q_NULLPTR);
	void dragEnterEvent(QDragEnterEvent *e);
	void dropEvent(QDropEvent *e);

	QStringList m_testIDList;
	QStringList m_resultList;

private:
	Ui::TestToolClass ui;
	void initTable();
	void insertRow(int row, int column, QString item);
    bool analyzeXml(bool bDrop);

private slots:
    void slotRefBtnClicked();
    void slotAnalyzeBtnClicked();
};
