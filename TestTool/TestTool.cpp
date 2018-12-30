#include "TestTool.h"

TestTool::TestTool(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setAcceptDrops(true);
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
}
