#pragma once

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class ComplexNum
{
public:
	ComplexNum(float real, float imaginary);
	~ComplexNum(void);

	friend ComplexNum operator+ (ComplexNum a, ComplexNum b);
	friend ComplexNum operator- (ComplexNum a, ComplexNum b);
	friend ComplexNum operator* (ComplexNum a, ComplexNum b);
	friend ComplexNum operator/ (ComplexNum a, ComplexNum b);

	float GetReal();
	float GetImaginary();
	string ToString();

private:
	float _real;
	float _imaginary;
};

