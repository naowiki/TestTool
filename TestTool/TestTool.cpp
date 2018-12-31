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
		int currentRow = ui.tableWidget->rowCount();
		for (int i=0; i < m_testIDList.count(); i++)
		{
			ui.tableWidget->setRowCount(i + 1);
			insertRow(currentRow + i, COLUMN_RESULT, m_resultList.at(i));
			insertRow(currentRow + i, COLUMN_SUITE, m_testIDList.at(i));
		}
	}
}

void TestTool::initTable()
{
	ui.tableWidget->setColumnCount( 6 );
	ui.tableWidget->setRowCount( 0 );
}

void TestTool::insertRow(int nRow, int nColumn, QString item)
{
	ui.tableWidget->setItem( nRow, nColumn, new QTableWidgetItem(item));
}

bool TestTool::analyzeXml(bool bDrop)
{
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
		m_testIDList << domList.at(i).firstChildElement("name").toElement().text();
		m_resultList << domList.at(i).firstChildElement("result").toElement().text();
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
        analyzeXml(false);
    }
}
