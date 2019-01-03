#pragma once

#include <QtWidgets/QMainWindow>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QtXml>
#include <QDomDocument>
#include <QFileDialog>
#include "ui_TestTool.h"

struct logdata_t {
    QString strApp;
    QString strCase;
    QString strSuite;
    QString strResult;
    QString strLog;
    QString strOther;
};

class TestTool : public QMainWindow
{
	Q_OBJECT

public:
	TestTool(QWidget *parent = Q_NULLPTR);
	void dragEnterEvent(QDragEnterEvent *e);
	void dropEvent(QDropEvent *e);

private:
	Ui::TestToolClass ui;
	void initTable();
	void insertRow(int row, int column, QString item);
    bool analyzeXml(bool bDrop);
    logdata_t makeNewLogData(QString app, QString tstcase, QString suite, QString result, QString log, QString other);
    void updateTableRow();

    QVector<logdata_t> l;

private slots:
    void slotRefBtnClicked();
    void slotAnalyzeBtnClicked();
	void tableItemClicked(int nRow, int nCol);
	void tableItemChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
};
