#include "ComplexNumUtils.h"

ComplexNum ComplexNumUtils::Conjugate(ComplexNum cn)
{
	return ComplexNum(cn.GetReal(), cn.GetImaginary() * (-1));
}

double ComplexNumUtils::Abs(ComplexNum cn)
{
	return sqrt(pow(cn.GetReal(), 2) + pow(cn.GetImaginary(), 2));
}
