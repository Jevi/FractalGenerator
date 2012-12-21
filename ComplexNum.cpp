#include "ComplexNum.h"
#include "ComplexNumUtils.h"


ComplexNum::ComplexNum(float real, float imaginary)
{
	_real = real;
	_imaginary = imaginary;
}


ComplexNum::~ComplexNum(void)
{
}

ComplexNum operator+ (ComplexNum a, ComplexNum b) 
{
	return ComplexNum((a.GetReal() + b.GetReal()), (a.GetImaginary() + b.GetImaginary()));
}

ComplexNum operator- (ComplexNum a, ComplexNum b) 
{
	return ComplexNum((a.GetReal() - b.GetReal()), (a.GetImaginary() - b.GetImaginary()));
}

ComplexNum operator* (ComplexNum a, ComplexNum b) 
{
	return ComplexNum((a.GetReal() * b.GetReal() - a.GetImaginary() * b.GetImaginary()), (a.GetReal() * b.GetImaginary() + a.GetImaginary() * b.GetReal()));
}

ComplexNum operator/ (ComplexNum a, ComplexNum b)
{
	ComplexNum temp = a * ComplexNumUtils::Conjugate(b);
	float bNormSq = b.GetReal() * b.GetReal() + b.GetImaginary() * b.GetImaginary();
	return ComplexNum(temp.GetReal() / bNormSq, temp.GetImaginary() / bNormSq);
}

float ComplexNum::GetReal()
{
	return _real;
}

float ComplexNum::GetImaginary()
{
	return _imaginary;
}

string ComplexNum::ToString()
{
	if (_imaginary >= 0)
	{
		stringstream str;
		str << _real << " + " << _imaginary << "i";
		return str.str();
	}
	else
	{
		stringstream str;
		str << _real << " - " << (-_imaginary) << "i";
		return str.str();
	}
}