#include "stdafx.h"
#include "Code.h"


Code::Code()
{
}

Code::Code(const int first, const int second, const int third, const int fourth)
{
	codeInts[0] = first;
	codeInts[1] = second;
	codeInts[2] = third;
	codeInts[3] = fourth;
}

Code::Code(const Code& other)
{
	for (int i = 0; i < CODE_LENGTH; i++)
	{
		codeInts[i] = other.codeInts[i];
	}
}

Code::~Code()
{
}

void Code::InitializeRandom()
{
	for (int i = 0; i < CODE_LENGTH; i++)
	{
		codeInts[i] = rand() % 6; // 0 - 5
	}
}

Code& Code::operator=(const Code &other)
{
	if (this == &other)
		return *this;

	codeInts[0] = other.codeInts[0];
	codeInts[1] = other.codeInts[1];
	codeInts[2] = other.codeInts[2];
	codeInts[3] = other.codeInts[3];
	_impossible = false;
	return *this;
}

int Code::GetLength() const
{
	return CODE_LENGTH;
}

int Code::GetValue(int position) const
{
	if (position < CODE_LENGTH && position > -1)
	{
		return codeInts[position];
	}
	else throw exception("Invalid Code Position");
}

bool Code::IsNotPossible()
{
	return _impossible;
}

bool Code::SetImpossible()
{
	return _impossible = true;
}

bool Code::checkValidInput() const
{
	for (int i = 0; i < CODE_LENGTH; i++)
	{
		if (codeInts[i] > MAX_GUESS_INT ||
			codeInts[i] < MIN_GUESS_INT)
		{
 			return false;
		}
	}

	return true;
}

istream& operator>>(istream &cstr, Code &c)
{	
	cstr >> c.codeInts[0] >> c.codeInts[1] >> c.codeInts[2] >> c.codeInts[3];
	if (cstr.fail() || !c.checkValidInput())
	{
		throw new exception("Invalid Input!");
	}

	return cstr;
}

ostream& operator<<(ostream &cstr, const Code &c)
{
	cstr << "The code is " << c.codeInts[0] << ", " << c.codeInts[1] << ", "
						   << c.codeInts[2] << ", " << c.codeInts[3] << "\n";
	return cstr;
}

bool operator==(const Code &c1, const Code &c2)
{
	for (int i = 0; i < c1.CODE_LENGTH; i++)
	{
		if (c1.codeInts[i] != c2.codeInts[i])
			return false;
	}
	return true;
}