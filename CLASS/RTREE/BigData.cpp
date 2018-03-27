#include "StdAfx.h"
#include "BigData.h"


CBigData::CBigData(void)
{
}


CBigData::~CBigData(void)
{
}

void CBigData::createRTree()
{

}


void CBigData::InsertData(std::vector<CvRect> &rect)
{
	for(int i = 0 ; i < rect.size();i++)
	{

	}
}

void CBigData::GetData()
{

}

bool CBigData::MySearchCallback(int id, void* arg)
{
	TRACE("与矩形 %d相交\n", id);
	return true; // keep going
}
