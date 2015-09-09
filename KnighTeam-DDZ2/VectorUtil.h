#pragma once
#include<vector>
#include<iostream>

using namespace std;

class VectorUtil
{
public:
	VectorUtil(void);
	~VectorUtil(void);
	static vector<unsigned> subVector(unsigned index,vector<unsigned> vi);
	static vector<unsigned> subVector(unsigned start_i,unsigned end_i,vector<unsigned> vi);
	
	static bool isContains(vector<unsigned> vi, unsigned n);
	static bool IsContainsVector(vector<vector<unsigned>> vv, vector<unsigned> vi);

	static bool IsSameVector(vector<unsigned> v1, vector<unsigned> v2);
	static bool IsSameVectors(vector<vector<unsigned>> vv1, vector<vector<unsigned>> vv2);
	static void addElement(vector<unsigned> *vi, unsigned element, int num);
};

