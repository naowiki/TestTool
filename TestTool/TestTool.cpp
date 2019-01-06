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
    : QMainWindow(parent),
      m_nRowCount(0)
{
	ui.setupUi(this);
	setAcceptDrops(true);
    this->resize( this->width(), this->height() - 320 );

    connect( ui.refBtn, SIGNAL( clicked() ),
             this,      SLOT( slotRefBtnClicked() ) );
    connect( ui.analyzeBtn, SIGNAL( clicked() ),
             this,          SLOT( slotAnalyzeBtnClicked() ) );
	connect( ui.tableWidget, SIGNAL(cellDoubleClicked(int, int)),
			 this,			 SLOT(tableItemClicked(int, int)));
	connect( ui.tableWidget, SIGNAL(currentCellChanged(int, int, int, int)),
			 this,			 SLOT(tableItemChanged(int, int, int, int)));
    connect( ui.extBtn, SIGNAL( clicked() ),
             this,      SLOT( slotExtBtnClicked() ) );
    connect( ui.failChkBox, SIGNAL( stateChanged(int) ),
             this,      SLOT( slotFailChkBoxChanged() ) );

	initTable();
    ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 編集禁止
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
    l.clear();
    if (analyzeXml())
    {
        updateTableRow();
	}
}

void TestTool::initTable()
{
    m_nRowCount = 0;
	ui.tableWidget->setColumnCount( 6 );
	ui.tableWidget->setRowCount( 0 );
}

void TestTool::insertRow(int nRow, int nColumn, QString item)
{
	ui.tableWidget->setItem( nRow, nColumn, new QTableWidgetItem(item));
}

bool TestTool::analyzeXml()
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
    QString selFilter = "*.xml";
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "ファイルを選択",
        "/Users/nao/Desktop/",
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
        l.clear();
        if (analyzeXml())
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
    log_temp.strResult = result;
    log_temp.strLog = log;
	log_temp.strOther = other;

    return log_temp;
}

void TestTool::updateTableRow()
{
    m_nRowCount = 0;
    for (int i = 0; i < l.count(); i++)
    {
        if (ui.failChkBox->isChecked())
        {
            if("FAIL"==l.at(i).strResult)
            {
                ui.tableWidget->setRowCount(m_nRowCount + 1);
                insertRow(m_nRowCount, COLUMN_APP,    l.at(i).strApp);
                insertRow(m_nRowCount, COLUMN_CASE,   l.at(i).strCase);
                insertRow(m_nRowCount, COLUMN_SUITE,  l.at(i).strSuite);
                insertRow(m_nRowCount, COLUMN_RESULT, l.at(i).strResult);
                insertRow(m_nRowCount, COLUMN_LOG,    l.at(i).strLog);
                insertRow(m_nRowCount, COLUMN_OTHER,  l.at(i).strOther);
                m_nRowCount++;
            }
        }
        else
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
    emit slotImageUpdated( 0 );
}

void TestTool::tableItemClicked(int nRow, int nCol)
{

}

void TestTool::tableItemChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    emit slotImageUpdated(currentColumn);
}

void TestTool::slotImageUpdated(int nCol)
{
    QImage *mImage = new QImage();
    mImage->load( "/Users/nao/Desktop/mario.png" );

    QPixmap pMap = QPixmap::fromImage( *mImage );
    pMap = pMap.scaled( ui.imageLbl->size() );
    ui.imageLbl->setPixmap( pMap );
}

void TestTool::slotExtBtnClicked()
{
    if ( ">>" ==ui.extBtn->text() )
    {
        this->resize( this->width(), this->height() + 320 );
        ui.extBtn->setText("<<");
    }
    else
    {
        this->resize( this->width(), this->height() - 320 );
        ui.extBtn->setText(">>");
    }
}

void TestTool::slotFailChkBoxChanged()
{
    if ( l.isEmpty() )
    {
        return;
    }

    updateTableRow();
}
