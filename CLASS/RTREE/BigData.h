#pragma once
//������������,��һЩ���Ĵ�����
//��������ʹ�õ�RTREE,�ȶ�ȫ�����ݽ��н���,Ȼ��������ƥ��Ҫƥ��Ķ���
//���Ӷȿ��Խ��ͺܶ�,��Ϊ���Ż��ĸ��Ӷ���O(n2)�Ż���ͳ���O(MN)
#include "CLASS\RTREE\RTree.h"
class CBigData
{
public:
	CBigData(void);
	~CBigData(void);
//����
	void createRTree();
//����
	void InsertData();
//�õ����
	void GetData();
};

