#pragma once
#include "Code.h"

class GuessResponse
{
public:
	// cant be const anymore due to inconst consistents
	GuessResponse(Code &consistentCode, Code &guess);
	// For initializing possible responses
	GuessResponse(const int numCorrect, const int numMisplaced);
	
~GuessResponse();

	// gets that return the number of (in)correctly guessed values
	int getNumberCorrectSpot();
	int getNumberCorrectButIncorrectSpot();

	friend bool operator==(const GuessResponse &lhs, const GuessResponse &rhs);
	
	GuessResponse& operator=(const GuessResponse &other);

private:
	int _numberCorrect = 0;
	int _numberMisplaced = 0;

	// sets the number of correct numbers in the correct position
	void setCorrectNumber(const Code &secret, const Code &guess);
	// setter for copy constructor
	void setCorrectNumber(const int num);

	// sets the number of correct numbers that are in the incorrect position
	// this method must be called after the getNumberCorrect call
	void setMisplacedNumber(const Code &secret, const Code &guess);
	// setter for copy constructor
	void setMisplacedNumber(const int num);

	// used for the incorrect position algorithm
	bool guessCodeMemberAccountedFor[4] = { false, false, false, false };
	bool secretCodeMemberAccountedFor[4] = { false, false, false, false };
};

