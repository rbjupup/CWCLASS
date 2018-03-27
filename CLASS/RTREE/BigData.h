#pragma once
//大数据量下面,有一些简洁的处理方法
//比如这里使用的RTREE,先对全部数据进行建树,然后树再来匹配要匹配的对象
//复杂度可以降低很多,因为不优化的复杂度是O(n2)优化后就成了O(MN)
#include "CLASS\RTREE\RTree.h"
#include <vector>
#include "opencv2\core\core.hpp"
#include "opencv\cv.hpp"
class CBigData
{
public:
	CBigData(void);
	~CBigData(void);
//建树
	RTree<int, float, 2, float> tree;
	//DATATYPE,ELEMTYPE,NUMDIMS,ELEMTYPEREAL,TMAXNODES = 8,TMINNODES = TMAXNODES / 2>
	void createRTree();
//生长
	void InsertData(std::vector<CvRect> &rect);
	bool MySearchCallback(int id, void* arg);
//得到结果
	void GetData();
};

