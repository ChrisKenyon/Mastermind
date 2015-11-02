#include "stdafx.h"
#include "GuessResponse.h"

GuessResponse::GuessResponse(Code & secretCode, Code & guess)
{
	setCorrectNumber(secretCode, guess);
	setMisplacedNumber(secretCode, guess);
}

GuessResponse::GuessResponse(const int numCorrect, const int numMisplaced)
{
	_numberCorrect = numCorrect;
	_numberMisplaced = numMisplaced;
}

GuessResponse::~GuessResponse()
{
}

int GuessResponse::getNumberCorrectSpot()
{
	return _numberCorrect;
}

int GuessResponse::getNumberCorrectButIncorrectSpot()
{
	return _numberMisplaced;
}

GuessResponse& GuessResponse::operator=(const GuessResponse &other)
{
	if (this == &other)
		return *this;

	_numberCorrect = other._numberCorrect;
	_numberMisplaced = other._numberMisplaced;

	return *this;
}

void GuessResponse::setCorrectNumber(const Code &secret, const Code &guess)
{
	// iterate through each, marking when a code place is accounted for
	for (int i = 0; i < secret.GetLength(); i++)
	{
		if (secret.GetValue(i) == guess.GetValue(i))
		{
			_numberCorrect++;
			guessCodeMemberAccountedFor[i] = true;
			secretCodeMemberAccountedFor[i] = true;
		}
	}
}

void GuessResponse::setCorrectNumber(const int num)
{
	_numberCorrect = num;
}

void GuessResponse::setMisplacedNumber(const Code &secret, const Code &guess)
{
	bool exitNest = false;
	// iterate through each, noting and marking when a code place is accounted for
	for (int i = 0; i < guess.GetLength(); i++)
	{
		exitNest = false;
		if (guessCodeMemberAccountedFor[i]) continue;

		for (int j = 0; j < secret.GetLength(); j++)
		{
			if (exitNest || secretCodeMemberAccountedFor[j]) continue;

			else if (guess.GetValue(i) == secret.GetValue(j))
			{
				_numberMisplaced++;
				guessCodeMemberAccountedFor[i] = true;
				secretCodeMemberAccountedFor[j] = true;
				exitNest = true;
			}
		}
	}
}

void GuessResponse::setMisplacedNumber(const int num)
{
	_numberMisplaced = num; 
}

bool operator==(const GuessResponse &lhs,const GuessResponse &rhs)
{
	if (lhs._numberMisplaced == rhs._numberMisplaced &&
		lhs._numberCorrect == rhs._numberCorrect)
		return true;
	else return false;
}
