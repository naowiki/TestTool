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
		//���o�I�Ƀh���b�v����t���邱�Ƃ�\�����A
		//�h���b�O���h���b�v���󂯕t����B�i���ꂪ�Ȃ��Ǝ�t���Ȃ��j
		e->acceptProposedAction();
	}
}

void TestTool::dropEvent(QDropEvent *e)
{
	//dragEnterEvent�̌�ɂ���C�x���g
	//�h���b�v�̍ۂ̓�����L�q����
}
