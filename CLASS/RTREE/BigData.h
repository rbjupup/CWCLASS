#pragma once
//大数据量下面,有一些简洁的处理方法
//比如这里使用的RTREE,先对全部数据进行建树,然后树再来匹配要匹配的对象
//复杂度可以降低很多,因为不优化的复杂度是O(n2)优化后就成了O(MN)
#include "CLASS\RTREE\RTree.h"
class CBigData
{
public:
	CBigData(void);
	~CBigData(void);
//建树
	void createRTree();
//生长
	void InsertData();
//得到结果
	void GetData();
};

