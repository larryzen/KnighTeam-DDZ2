#include "VectorUtil.h"
#include<vector>

VectorUtil::VectorUtil(void)
{
}


VectorUtil::~VectorUtil(void)
{
}

/** 取vector部分元素,从index到结束
*	@index   开始索引
*	@vi	     目标vector
*/
vector<unsigned> VectorUtil::subVector(unsigned index,vector<unsigned> vi)
{
	vector<unsigned> back_vi;
	try {
		for(unsigned i=index;i<vi.size();i++)
		{
			back_vi.push_back(vi.at(i));
		}
	}
	catch(exception e)
	{
		cout<<"subvector1索引越界异常"<<endl;
	}

	return back_vi;
}

/** 取vector部分元素
*	@start_i 开始索引
*	@end_i   结束索引
*	@vi	     目标vector
*/
vector<unsigned> VectorUtil::subVector(unsigned start_i,unsigned end_i,vector<unsigned> vi)
{
	vector<unsigned> back_vi;
	try {
		for(unsigned i=start_i;i<=end_i;i++)
		{
			back_vi.push_back(vi.at(i));
		}
	}catch(exception e)
	{
		cout<<"subvector2索引越界异常"<<endl;
	}

	return back_vi;
}

bool VectorUtil::isContains(vector<unsigned> vi,unsigned n)
{
	for(size_t i=0;i<vi.size();i++)
	{
		if(vi[i]==n)
		{
			return true;
		}
	}

	return false;
}