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

bool VectorUtil::IsSameVector(vector<unsigned> v1, vector<unsigned> v2)
{
	unsigned len1 = v1.size();
	unsigned len2 = v2.size();

	if (len1 != len2)
	{
		return false;
	}
	else
	{
		for (size_t i = 0; i < len1; i++)
		{
			if (!isContains(v1, v2[i]))
			{
				return false;
			}
		}
	}

	return true;
}

bool VectorUtil::IsSameVectors(vector<vector<unsigned>> vv1, vector<vector<unsigned>> vv2)
{
	if (vv1.size() != vv2.size())
		return false;
	else
	{
		for (int i = 0; i < vv1.size(); i++)
		{
			if (!IsContainsVector(vv2, vv1[i]))
				return false;
		}
	}
	return true;
}

bool VectorUtil::IsContainsVector(vector<vector<unsigned>> vv, vector<unsigned> vi)
{
	for (int i = 0; i < vv.size(); i++)
	{
		if (IsSameVector(vv[i], vi))
			return true;
	}
	return false;
}

void VectorUtil::addElement(vector<unsigned> *vi, unsigned element, int num)
{
	for (int i = 0; i < num; i++)
	{
		vi->push_back(element);
	}
}