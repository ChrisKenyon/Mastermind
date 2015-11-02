#pragma once

using namespace std;

class Code
{
public:
	Code();
	// Creates a inputted code used for guesses made by the computer
	Code(const int first, const int second, const int third, const int fourth);
	// Copy Constructor
	Code(const Code & other);
	~Code();
	
	// Create a random secret code for playing against a computer
	void InitializeRandom();
	
	// Overload operators for inputting/outputting/comparing codes
	friend istream &operator>>(istream &cstr, Code &c);
	friend ostream &operator<<(ostream &cstr, const Code &c);
	friend bool operator==(const Code &c1, const Code &c2);
	
	// copy assignment operator, necessary for consistents.erase
	Code& operator=(const Code &other);

	// get the length of the code
	int GetLength() const;

	// get the value at a given position of the code
	int GetValue(int position) const;
	
	// Used for erasing from the vector
	bool IsNotPossible();
	bool SetImpossible();
private:

	const int MAX_GUESS_INT = 5;
	const int MIN_GUESS_INT = 0;
	const int CODE_LENGTH = 4;
	bool _impossible = false;
	int codeInts[4];

	// checks that the numbers inputted are within the bounds of the rules
	bool checkValidInput() const;
};

