#include "ComplexNumUtils.h"

ComplexNum ComplexNumUtils::Conjugate(ComplexNum cn)
{
	return ComplexNum(cn.GetReal(), cn.GetImaginary() * (-1));
}

float ComplexNumUtils::Abs(ComplexNum cn)
{
	return sqrt(pow(cn.GetReal(), 2) + pow(cn.GetImaginary(), 2));
}
