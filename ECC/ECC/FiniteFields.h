#pragma once
#define POINT int*


class FiniteFieldFP
{
public:
	FiniteFieldFP(int, int , int , POINT, int , int);
	~FiniteFieldFP();

	const int p,a,b,n,h;
	const POINT G;
};

