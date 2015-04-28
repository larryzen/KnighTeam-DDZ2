#pragma once
#include<vector>
#include<iostream>

using namespace std;

class VectorUtil :public vector<unsigned>
{
public:
	VectorUtil(void);
	~VectorUtil(void);
	static vector<unsigned> subVector(unsigned index,vector<unsigned> vi);
	static vector<unsigned> subVector(unsigned start_i,unsigned end_i,vector<unsigned> vi);
	static bool isContains(vector<unsigned> vi,unsigned n);
};

