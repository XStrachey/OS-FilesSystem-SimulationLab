#pragma once
#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <fstream>

using namespace std;
//using mfd = tuple<userName, userPassword, userOwnUFDLink>;
typedef struct mfd//һ��mfd32�ֽڣ�һ����512�ֽڣ����Դ�16��mfd
{
	char userName[14];
	char userPassword[14];
	int userOwnUFDLink; //���û���UFD���ڵ��̺ţ�4�ֽ�

}mfd;

//using ufd = tuple<fileName, mode, fileMaxLength, fileAddress>;
typedef struct ufd//��ǰһ��ufd��30�ֽڣ������Կ������һ����Ϣ������󳤶ȣ�ʵ�ֿ���ͨ����Ѱʱ�����״�����ռ�Ĵ�С������Ҫ���������ļ�����ʱ���µĿ��ÿռ�ı仯
{
	char fileName[14];//�ļ���14�ֽ�
	int mode; // �ļ�Ȩ��1 - readonly;  2 - writeonly;  3 - read / write��4�ֽ�
	int fileLength;//�ļ����ȣ���ʼΪ0
	int fileMaxLength;//�ļ���󳤶�
	int fileAddress;//�ļ���ַ��4�ֽڣ��õ�ַΪ��
}ufd;

//using uof = tuple<fileName, state, mode, readPointer, writePointer>;
typedef struct uof//��ǰ����£��û�������ͬʱ����4���ļ�����ǰ��Ƶ�uofΪ30�ֽڣ�ȱ���ļ��򿪺�Ĳ�����ʽ�������ļ�������ʽ��
{
	char fileName[14];//�ļ���14�ֽ�
	int mode;// �ļ�Ȩ��1 - readonly;  2 - writeonly;  3 - read / write��4�ֽ�
	int readPointer; //��дָ��
	int writePointer;
	int fileLength;
	int fileAddress;
	int state; //0��ʼ����/�򿪣�1Ϊ����д��δ�رգ�һ�ּ򻯵�ռ�ñ�ʾ����״̬Ӧ�ÿ��Ƕ�Ӽ�������Ŀ¼/��ռ��/�Լ���/�½�
}uof;
/*
using nextAddress = int;
using occupationState = bool;
using content = char[64];
using cluster = tuple<nextAddress, occupationState, content>;
*/

typedef struct cluster//�أ�һ��������Ϊ64�ֽڵĴ�С����Ҫ�Ӷ������Ϣ8�ֽ�
{
	int nextAddress;//ָ����һ����
	bool occupationState;//�ÿ�ռ�����
	char content[64];//������

}cluster;

using command = string[4];
command cmd_in;

typedef void(*func)(void);

typedef struct hand//һ�����Դ洢�����ַ����ͺ���ָ������ݽṹ�����Կ�����pair���
{
	const char *pname;
	func handler;
}handler;
extern handler handlerList[];