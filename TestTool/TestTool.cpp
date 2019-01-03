#include "TestTool.h"

enum eColumnType
{
	COLUMN_APP		= 0,
	COLUMN_CASE		= 1,
	COLUMN_SUITE	= 2,
	COLUMN_RESULT	= 3,
	COLUMN_LOG		= 4,
	COLUMN_OTHER	= 5
};

TestTool::TestTool(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setAcceptDrops(true);

    connect( ui.RefBtn, SIGNAL( clicked() ),
             this,      SLOT( slotRefBtnClicked() ) );
    connect( ui.analyzeBtn, SIGNAL( clicked() ),
             this,          SLOT( slotAnalyzeBtnClicked() ) );

	initTable();
}

void TestTool::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->mimeData()->hasFormat("text/uri-list"))
	{
		//視覚的にドロップを受付られることを表示し、
		//ドラッグ＆ドロップを受け付ける。（これがないと受付られない）
		e->acceptProposedAction();
	}
}

void TestTool::dropEvent(QDropEvent *e)
{
	//dragEnterEventの後にくるイベント
	//ドロップの際の動作を記述する
	ui.filePathLineEdit->setText(e->mimeData()->urls().first().toLocalFile());
	if (analyzeXml(true))
    {
        updateTableRow();
	}
}

void TestTool::initTable()
{
	ui.tableWidget->setColumnCount( 6 );
	ui.tableWidget->setRowCount( 0 );
    l.clear();
}

void TestTool::insertRow(int nRow, int nColumn, QString item)
{
	ui.tableWidget->setItem( nRow, nColumn, new QTableWidgetItem(item));
}

bool TestTool::analyzeXml(bool bDrop)
{
    initTable();

	QString filePath;
	filePath = ui.filePathLineEdit->text();
	QFile file(filePath);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		return false;
	}

	QDomDocument domDocument;
	domDocument.setContent(&file);

	QDomNodeList domList = domDocument.elementsByTagName("item");
	for ( int i = 0; i < domList.count(); i++ )
	{
		l.push_back( makeNewLogData( "",
									 "",
									 domList.at(i).toElement().attributeNode("id").value(),
									 domList.at(i).firstChildElement("result").text(),
									 domList.at(i).firstChildElement("name").text(),
									 "" ) );
    }

	return true;
}

void TestTool::slotRefBtnClicked()
{
    QString selFilter = tr("*.xml");
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("ファイルを選択する"),
        "/Users",
        tr("*.xml"),
        &selFilter,
        QFileDialog::DontUseCustomDirectoryIcons
    );

    if (fileName.isEmpty()) {
        // キャンセル
    } else {
        ui.filePathLineEdit->setText(fileName);
    }
}

void TestTool::slotAnalyzeBtnClicked()
{
    if (!ui.filePathLineEdit->text().isEmpty())
    {
        if (analyzeXml(false))
        {
            updateTableRow();
        }
    }
}

logdata_t TestTool::makeNewLogData(QString app, QString tstcase, QString suite, QString result, QString log, QString other)
{
    logdata_t log_temp;
	log_temp.strApp = app;
	log_temp.strCase = tstcase;
	log_temp.strSuite = suite;
    log_temp.strResult=result;
    log_temp.strLog=log;
	log_temp.strOther = other;

    return log_temp;
}

void TestTool::updateTableRow()
{
    for (int i = 0; i < l.count(); i++)
    {
        ui.tableWidget->setRowCount(i + 1);
        insertRow(i, COLUMN_APP,    l.at(i).strApp);
		insertRow(i, COLUMN_CASE,   l.at(i).strCase);
		insertRow(i, COLUMN_SUITE,  l.at(i).strSuite);
		insertRow(i, COLUMN_RESULT, l.at(i).strResult);
		insertRow(i, COLUMN_LOG,    l.at(i).strLog);
		insertRow(i, COLUMN_OTHER,  l.at(i).strOther);
    }
}
