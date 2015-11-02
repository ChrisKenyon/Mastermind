#pragma once
#include "Code.h"
#include "GuessResponse.h"
#include <cstdlib>
#include <vector>

using namespace std;

class MastermindGame
{
public:
	MastermindGame();
	~MastermindGame();

	// called by main.cpp to start the game
	void playGame();
private:
	const int MAX_GUESSES = 5;
	const int RANGE_CODE_INTS = 6;
	const int NUMBER_CODE_INTS = 4;
	
	Code secretCode;
	int numberOfGuesses = 0;
	bool continuePlaying = true;
	bool isSolved = false;
	vector<pair<Code, GuessResponse>> previousGuesses;
	vector<GuessResponse> possibleResponses;

	// prints the secret code at the end of the game with a param to say Congrats/Sorry
	void printCode(bool success) const;
	
	// asks to start a new game or quit
	bool promptNewGame();
	
	// helper used for boolean prompts that takes inputs of Y/y or N/n
	bool promptBoolean();

	// Starts a game for two people
	void startTwoPlayerGame();
	
	// Starts a game against the computer
	void startComputerGame();

	// Start a game where the computer is guessing your code
	void startComputerGuessingGame();

	// checks if the guess is correct
	bool checkCorrect(const Code &guess) const;
	
	// responds when the guess is incorrect
	void respondToGuess(Code &guess);

	// starts the guess loop for the player
	void startGuessing();

	// starts the guess loop for the computer (Project 1b algorithm)
	void startComputerGuessing();

	// narrows the subset by removing any codes that wouldn't generate the same response
	void eliminateImpossibilities(vector<Code>& consistents, GuessResponse & receivedResponse, Code & guess);

	// This is the algorithm to focus on - it has a worst case of 6 but mostly guesses in 5 or less
	// Runs through every consistent guess left, and for each possible response, it counts the 
	// number of consistents that would give that response. The maximum count is stored, and then
	// the guess corresponding to the minimum of those maximums is selected
	Code generateGuess(vector<Code>& consistents, vector<pair<Code, GuessResponse>>& previousGuesses) const;
	
	// This was my original algorithm - it guesses with a worst case of 7, yet is faster
	// generates the best guess given the possible guesses
	// This algorithm checks every guess made against all the possible choices
	// and creates a score for each response. Whichever response code is scored
	// the highest is the subset from which a guess should be picked
	Code generateGuess2(vector<Code>& consistents, vector<pair<Code, GuessResponse>>& previousGuesses) const;

	// cleans up the game for starting a new game
	void finishGame();
};

