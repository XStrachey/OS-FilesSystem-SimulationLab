#include "systemStruction.h"
#include "MS_DOSFilesSystem.h"

using namespace std;

handler handlerList[] =//�����뺯��ָ��Ľṹ���ĳ�pair����
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
	initTable();//���б��ʼ������Ҫ�ǽ�����ǰ��λ��Ϊ-1����ʾΪϵͳ��ռ��
	initFiletoRom();//�������
	updateTotalFreeBlocksNum();//���ݴӴ��̶������Ϣ���¿�ռ�ռ����Ϣ
	//findLink(b);
	//showBlockLink();
	while (1)
	{
		cmdParser();
	}
	return 0;
}