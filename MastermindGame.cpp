#include "stdafx.h"
#include "MastermindGame.h"
#include <map>
#include <algorithm> // for remove_if

MastermindGame::MastermindGame()
{
}


MastermindGame::~MastermindGame()
{
}

// public methods
void MastermindGame::playGame()
{
	while (continuePlaying)
	{
		
		cout << "Play against player (Y) or a computer (N)?" << endl;
		if (promptBoolean())
			startTwoPlayerGame();
		else
		{
			cout << "Play as the code breaker (Y) or code maker (N)?" << endl;
			if (promptBoolean())
				startComputerGame();
			else startComputerGuessingGame();
		}
		
		finishGame();
		continuePlaying = promptNewGame();
	}
}

// private methods
void MastermindGame::printCode(bool success) const
{
	if (success)
		printf("Congratulations! You guess the secret code in %i guesses!\n", numberOfGuesses);
	else
		printf("Sorry, you've run out of guesses!\n");
	
	cout << secretCode;
}

bool MastermindGame::promptNewGame()
{
	cout << "Game Over! Play again? (Y/N)" << endl;
	return promptBoolean();
}

bool MastermindGame::promptBoolean()
{
	while (true)
	{
		char promptChar;
		cin >> promptChar;
		if (promptChar == 'y' || promptChar == 'Y')
		{
			return true;
		}
		else if (promptChar == 'n' || promptChar == 'N')
		{
			return false;
		}
		else
		{
			cout << "Invalid Input.\n";
		}
	}

	return false;
}

void MastermindGame::startTwoPlayerGame()
{
	cout << "Code Maker, please enter the secret code\n";
	cin >> secretCode;

	for (int i = 0; i < 100; i++)
	{
		cout << endl;
	}

	startGuessing();
}

void MastermindGame::startComputerGame()
{
	secretCode.InitializeRandom();
	startGuessing();
}

void MastermindGame::startComputerGuessingGame()
{
	cout << "Enter the secret code for the computer to guess\n";
	cin >> secretCode;

	startComputerGuessing();
}

void MastermindGame::startGuessing()
{
	while (numberOfGuesses < MAX_GUESSES)
	{
		printf("Code Breaker, you have %i guesses left!\n", MAX_GUESSES - numberOfGuesses++);

		Code guess;
		cin >> guess;

		if (checkCorrect(guess))
		{
			printCode(true);
			return;
		}
		else
			respondToGuess(guess);
	}
	printCode(false);
}

void MastermindGame::startComputerGuessing()
{
	// first create a set of all the possible codes
	vector<Code> consistentCodes;
	for (int w = 0; w < RANGE_CODE_INTS; w++)
	{
		for (int x = 0; x < RANGE_CODE_INTS; x++)
		{
			for (int y = 0; y < RANGE_CODE_INTS; y++)
			{
				for (int z = 0; z < RANGE_CODE_INTS; z++)
				{
					consistentCodes.push_back(Code(w,x,y,z));
				}
			}
		}
	}

	// Then initialize the possible responses vector
	for (int corr = 0; corr <= NUMBER_CODE_INTS; corr++)
	{
		for (int misp = 0; misp <= NUMBER_CODE_INTS - corr; misp++)
		{
			possibleResponses.push_back(GuessResponse(corr,misp));
		}
	}

	// keep guessing until it is solved
	while (!isSolved)
	{
		if (numberOfGuesses == 0)
		{
			// a good first guess is 0011
			Code guess(0, 0, 1, 1);
			cout << "Guessing 0 0 1 1..." << endl;
			numberOfGuesses++; // keep track of the number of guesses to see how well the algorithm is working
			GuessResponse response(secretCode, guess);
			if (response.getNumberCorrectSpot() == NUMBER_CODE_INTS)
			{
				if (!(secretCode == guess))
					throw new exception("Programming Error in guessing!");
				else
				{
					isSolved = true;
					cout << "Too easy!" << endl << guess;
				}

				return;
			}
			else
			{
				// add the guess with its response to the previous guesses structure
				previousGuesses.push_back(pair<Code, GuessResponse>(guess, response));
				// remove all the codes from consistent codes that don't receive the same response
				eliminateImpossibilities(consistentCodes, response, guess);
			}
		}
		else
		{
			// generate a code
			Code guess(generateGuess(consistentCodes, previousGuesses));
			printf("Guessing %i %i %i %i...\n", guess.GetValue(0), guess.GetValue(1), guess.GetValue(2), guess.GetValue(3)); 
			GuessResponse response(secretCode, guess);
			numberOfGuesses++;
			
			// check if it is correct
			if (response.getNumberCorrectSpot() == NUMBER_CODE_INTS)
			{
				if (!(guess == secretCode))
				{
					throw new exception("Programming error!");
				}

				isSolved = true;
				printf("I got it in %i guesses! Better luck next time!\n", numberOfGuesses);
				cout << guess;
				return;
			}
			else
			{
				// add to guess vector
				previousGuesses.push_back(pair<Code, GuessResponse>(guess, response));
				// eliminate from consistent codes
				eliminateImpossibilities(consistentCodes, response, guess);
			}
		}
	}
}

void MastermindGame::eliminateImpossibilities(vector<Code>& consistents, GuessResponse &receivedResponse, Code &guess)
{
	if (consistents.size() < 2)
		throw new exception("Not enough codes for eliminateImpossibilities!");

	for (int i = (int)consistents.size() - 1; i >= 0; i--)
	{
		GuessResponse gr(consistents[i], guess);
		// For a code to be possible x when compared to y must receive a response r,
		// and also y when compared to x must receive a response of r, so non matching
		// responses mark a code as inconsistent
		if (!(receivedResponse == gr) || consistents[i] == guess)
		{
			consistents[i].SetImpossible(); // mark the codes to be deleted
		}
	}

	// delete with an iterator expression to prevent stack overflow
	consistents.erase(
		remove_if(consistents.begin(), consistents.end(), 
			[](Code &c) { return c.IsNotPossible(); }),
			consistents.end());
}

Code MastermindGame::generateGuess(vector<Code> &consistents, vector<pair<Code, GuessResponse>> &previousGuesses) const
{
	if (consistents.size() < 1)
		throw new exception("Error generating guess, no consistents to guess from!");
	else if (consistents.size() == 1)
	{
		return consistents[0];
	}

	// keep track of the minimax score
	int minScore = 2000; // any # over 1296
	int toReturnIdx = 0;

	// iterate through all the possible codes left
	for (int idx = 0; idx < (int)consistents.size(); idx++)
	{
		int maxScore = 0;
		// map based on the response pairs, the value being the number of consistent guesses for that response
		map<pair<int, int>, int> responseConsistentGuesses;

		// start adding to the score by checking all possible responses
		for (int r = 0; r < possibleResponses.size(); r++)
		{
			// check each response against every possible code and the previous guesses
			for (int idx2 = 0; idx2 < (int)consistents.size(); idx2++)
			{
				// used for checking previous
				bool consistent = true;
				for (int prevGuess = 0; prevGuess < (int)previousGuesses.size(); prevGuess++)
				{
					if (!(possibleResponses[r] == previousGuesses[prevGuess].second))
						consistent = false;
				}

				// used to check current
				GuessResponse resp(consistents[idx], consistents[idx2]);
				if (possibleResponses[r] == resp && consistent) // matches all previous guesses and the current consistent guess
				{
					// this part will gather the highest score as it adds to the score for the specified response
					pair<int, int> key(resp.getNumberCorrectSpot(), resp.getNumberCorrectButIncorrectSpot());
					responseConsistentGuesses.insert_or_assign(key, ++responseConsistentGuesses[key]); // int is initialized in map as 0

					if (responseConsistentGuesses.size() > 0 && responseConsistentGuesses[key] > maxScore)
						maxScore = responseConsistentGuesses[key];
				}
			}
		}

		// check each max score to see if it is the minimum (minimax)
		if (maxScore < minScore)
		{
			minScore = maxScore;
			// keep track of this guess
			toReturnIdx = idx;
		}
	}

	// return the guess
	return consistents[toReturnIdx];
}

Code MastermindGame::generateGuess2(vector<Code> &consistents, vector<pair<Code, GuessResponse>> &previousGuesses) const
{
	if (consistents.size() < 1)
		throw new exception("Error generating guess, no consistents to guess from!");

	// map based on the response pairs, the value being the score
	map<pair<int, int>, int> scores;

	// keep track of the highest score
	int maxScore = 0;
	int toReturnIdx = 0;
	// iterate through the previous guesses
	for (int prevGuess = 0; prevGuess < (int)previousGuesses.size(); prevGuess++)
	{
		// iterate through all the possible codes left
		for (int idx = 0; idx < (int)consistents.size(); idx++)
		{
			GuessResponse resp(previousGuesses[prevGuess].first, consistents[idx]);

			// Double check that the code is the consistent one you expect by checking the response
			if (!(resp == previousGuesses[prevGuess].second))
				throw new exception("Unexpected Response!");

			// Whichever type of response has the most matches should be the subset chosen from,
			// this part will gather the highest score and mark the highest index where it found that
			pair<int, int> key(resp.getNumberCorrectSpot(), resp.getNumberCorrectButIncorrectSpot());
			scores.insert_or_assign(key, ++scores[key]); // int is initialized in map as 0
			if (scores[key] > maxScore)
			{
				maxScore = scores[key];
				toReturnIdx = idx;
			}
		}
	}

	return consistents[toReturnIdx];
}

bool MastermindGame::checkCorrect(const Code & guess) const
{
	return (secretCode == guess);
}

void MastermindGame::respondToGuess(Code & guess)
{
	GuessResponse response(secretCode, guess);
	printf("You guessed %i in the correct spot, as well as %i correct, but in the wrong spot\n",
		   response.getNumberCorrectSpot(), response.getNumberCorrectButIncorrectSpot());
}

void MastermindGame::finishGame()
{
	// place cleanup for a new game here
	numberOfGuesses = 0;
	previousGuesses.clear();
	possibleResponses.clear();
	isSolved = false;
}
