#pragma once
//������������,��һЩ���Ĵ�����
//��������ʹ�õ�RTREE,�ȶ�ȫ�����ݽ��н���,Ȼ��������ƥ��Ҫƥ��Ķ���
//���Ӷȿ��Խ��ͺܶ�,��Ϊ���Ż��ĸ��Ӷ���O(n2)�Ż���ͳ���O(MN)
#include "CLASS\RTREE\RTree.h"
#include <vector>
#include "opencv2\core\core.hpp"
#include "opencv\cv.hpp"
class CBigData
{
public:
	CBigData(void);
	~CBigData(void);
//����
	RTree<int, float, 2, float> tree;
	//DATATYPE,ELEMTYPE,NUMDIMS,ELEMTYPEREAL,TMAXNODES = 8,TMINNODES = TMAXNODES / 2>
	void createRTree();
//����
	void InsertData(std::vector<CvRect> &rect);
	bool MySearchCallback(int id, void* arg);
//�õ����
	void GetData();
};

