#include "systemStruction.h"
#include "MS_DOSFilesSystem.h"

using namespace std;

handler handlerList[] =//命令与函数指针的结构，改成pair好了
{
	{ "Help", do_Help },
    { "Create", do_Create },
    { "Write", do_Write },
    { "Insert", do_Insert },
    { "Delete",do_Delete },
    { "showLink", showBlockLink },
    { "dir",do_dir },
    { "sysc",do_sysc },
    { "clear", do_Clear },
    { "exit", do_exit },
    { NULL, NULL }
};

int main()
{
	initTable();//块列表初始化，主要是将数组前两位置为-1，表示为系统所占用
	initFiletoRom();//读入磁盘
	updateTotalFreeBlocksNum();//根据从磁盘读入的信息更新块空间占用信息
	//findLink(b);
	//showBlockLink();
	while (1)
	{
		cmdParser();
	}
	return 0;
}