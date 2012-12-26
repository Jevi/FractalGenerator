#pragma once
#include "ComplexNum.h"
#include <cmath> 

class ComplexNumUtils
{
public:
	static ComplexNum Conjugate(ComplexNum cn);
	static float Abs(ComplexNum cn);
};

