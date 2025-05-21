#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <time.h>


//CONSTANTS VARIABLES
#define ROUNDS 13
#define NUM_CARDS 52
#define NUM_PLAYER_CARDS 13
#define MAX_PLAYERS 4


//FUNCTIONS PROTOTYPES
void numPlayersAndNames(int* numPlayers, char playersNames[][30]);
void shuffleDeck(int cards[]);
void handCards(int playersCards[][NUM_PLAYER_CARDS], int cards[], int numPlayers);
int playRound(int playersCards[][NUM_PLAYER_CARDS], int numPlayers, int playedCards[], char playersNames[][30], int carriedScore, int scores[]);
int cardScore(int cardValue);
void roundMenu(int numPlayers, char playersNames[][30], int playersCards[][NUM_PLAYER_CARDS], int scores[], int carriedScore, int round);
void saveGame(int numPlayers, char playersNames[][30], int playersCards[][NUM_PLAYER_CARDS], int scores[], int carriedScore, int round);
void loadGame(int* numPlayers, char playersNames[][30], int playersCards[][NUM_PLAYER_CARDS], int scores[], int* carriedScore, int* round);
int getHighestUniqueCard(int playedCards[], int numPlayers);
void getWinner(int numPlayers, int scores[MAX_PLAYERS], char playersNames[][30], int rounds);
void showStates(char playersNames[][30], int scores[], int rounds, int numPlayers);


void main()
{
	//VARIABLES AND ARRAYS
	int numPlayers = 0, carriedScore = 0, menuChoice, round = 0;
	int i, j;
	int playedCards[MAX_PLAYERS];
	srand(time(NULL));
	int playersCards[MAX_PLAYERS][NUM_PLAYER_CARDS];
	char playersNames[MAX_PLAYERS][30];
	int scores[MAX_PLAYERS] = { 0 };
	int cards[NUM_CARDS] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,
							 1,2,3,4,5,6,7,8,9,10,11,12,13,
							 1,2,3,4,5,6,7,8,9,10,11,12,13,
							 1,2,3,4,5,6,7,8,9,10,11,12,13 };

	//start-up menu
	printf("WELCOME TO WAR!\n");
	printf("Press 1 to start a new game\n");
	printf("Press 2 to load saved game\n");
	printf("Press 3 to exit\n");
	scanf("%d", &menuChoice);
	while (menuChoice < 1 || menuChoice >3) {
		printf("Wrong input\n");
		printf("Press 1 to start a new game\n");
		printf("Press 2 to load saved game\n");
		printf("Press 3 to exit\n");
		scanf("%d", &menuChoice);
	}
	if (menuChoice == 1) {
		//start new game

		//get the number of players and their names by calling the numPlayersAndNames function
		numPlayersAndNames(&numPlayers, playersNames);

		//shuffle the cards by calling the shuffleDeck function
		shuffleDeck(cards);

		//hand the cards to the players by calling the handCards function
		handCards(playersCards, cards, numPlayers);

		//loop the playRound funcyion for 13 rounds
		for (int i = 0; i < ROUNDS; i++) {
			printf("\nRound %d:\n", i + 1);
			carriedScore = playRound(playersCards, numPlayers, playedCards, playersNames, carriedScore, scores);
			printf("-------------------------------------------\n\n");
			if (i + 1 == ROUNDS) {

				getWinner(numPlayers, scores, playersNames, i + 1);
			}
			else {
				roundMenu(numPlayers, playersNames, playersCards, scores, carriedScore, i + 1);
			}
		}
	}
	else if (menuChoice == 2) {
		//load saved game
		loadGame(&numPlayers, playersNames, playersCards, scores, &carriedScore, &round);
		if (numPlayers > 0) { // If the game was successfully loaded
			printf("Continuing from Round %d...\n", round + 1);
			// Proceed with the next round or display options
			for (int i = round; i < ROUNDS; i++) {
				printf("\nRound %d:\n", i + 1);
				carriedScore = playRound(playersCards, numPlayers, playedCards, playersNames, carriedScore, scores);
				printf("-------------------------------------------\n\n");
				if(i+1==ROUNDS) {

					getWinner(numPlayers, scores, playersNames, i+1);
				}
				else {
					roundMenu(numPlayers, playersNames, playersCards, scores, carriedScore, i + 1);	
				}
			}

			
		}
	}
	else {
		//exit
		exit();
	}

}
//FUNCTIONS

//functon to play a single round 
int playRound(int playersCards[][NUM_PLAYER_CARDS], int numPlayers, int playedCards[], char playersNames[][30], int carriedScore, int scores[]) {
	int cardChosen;

	//show the cards to the player and 
	for (int i = 0; i < numPlayers; i++) {
		printf("%s's hand: ", playersNames[i]);
		for (int j = 0; j < NUM_PLAYER_CARDS; j++) {
			if (playersCards[i][j] != -1) { // Show only unplayed cards
				printf("%d ", playersCards[i][j]);
			}
		}
		printf("\n");

		// Prompt for card selection
		do {
			printf("%s, choose a card from your hand: ", playersNames[i]);
			scanf("%d", &cardChosen);

			// Validate card selection
			int isValid = 0;
			for (int j = 0; j < NUM_PLAYER_CARDS; j++) {
				if (playersCards[i][j] == cardChosen) {
					isValid = 1;
					playersCards[i][j] = -1; // Mark the card as played
					break;
				}
			}

			if (!isValid) {
				printf("Invalid choice. Please select a card from your hand.\n");
			}
			else {
				break;
			}
		} while (1);

		playedCards[i] = cardChosen; // Store the selected card for the round
	}

	//set round score as 0 
	int roundScore = 0;

	//calculate the round score 
	for (int i = 0; i < numPlayers; i++) {
		int score = cardScore(playedCards[i]);
		roundScore += score;
	}
	if (carriedScore > 0) {
		roundScore += carriedScore;
	}

	//get the winner in this round and assign score if there is a winner, or pass the score to the next round if there is no winner
	int highestPlayedCard = getHighestUniqueCard(playedCards, numPlayers);
	if (highestPlayedCard > 0) {
		// Find the player who played the highest unique card
		int highestCardCol = -1;
		for (int i = 0; i < numPlayers; i++) {
			if (playedCards[i] == highestPlayedCard) {
				highestCardCol = i;
				break;
			}
		}

		// Update their score
		scores[highestCardCol] += roundScore;

		printf("Highest Played card in this round is: %d, and was played by %s\n%s win this round\n", highestPlayedCard, playersNames[highestCardCol], playersNames[highestCardCol]);
		printf("%s collected %d points from this round\n", playersNames[highestCardCol], roundScore);
		int nextRoundPoints = 0;
		return nextRoundPoints;
	}
	else {
		printf("no winner in this round, points will be carried to the next round.\nRound score is %d\n", roundScore);
		int nextRoundPoints = roundScore;
		return nextRoundPoints;
	}

}

//function to hand cards to the player
void handCards(int playersCards[][NUM_PLAYER_CARDS], int cards[], int numPlayers) {
	for (int i = 0; i < numPlayers; i++) {
		for (int j = 0; j < NUM_PLAYER_CARDS; j++) {
			playersCards[i][j] = cards[i * NUM_PLAYER_CARDS + j];
		}
	}
}

//function to shuffle the deck of cards
void shuffleDeck(int cards[]) {
	for (int i = NUM_CARDS - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		int temp = cards[i];
		cards[i] = cards[j];
		cards[j] = temp;
	}
}

//function to get number of players and their names
void numPlayersAndNames(int* numPlayers, char playersNames[][30]) {
	printf("Please enter number of players (2-4)\n");
	scanf("%d", numPlayers);

	while (*numPlayers < 2 || *numPlayers>4)
	{
		printf("Wrong input, Please enter the number 2, 3 or 4\n");
		scanf("%d", numPlayers);
	}

	printf("Please enter players names\n");
	for (int i = 0; i < *numPlayers; i++) {
		printf("Player %d name: \n", i + 1);
		scanf("%s", playersNames[i]);

	}
}

//function to calculate score of a card
int cardScore(int cardValue) {

	switch (cardValue) {
	case 1: return 14;
	case 2: return 2;
	case 3: return 3;
	case 4: return 4;
	case 5: return 5;
	case 6: return 6;
	case 7: return 7;
	case 8: return 8;
	case 9: return 9;
	case 10: return 10;
	case 11: return 11;
	case 12: return 12;
	case 13: return 13;

	default: return 0;
	}
}

//function to compare the cards 
int getHighestUniqueCard(int playedCards[], int numPlayers) {
	int frequency[14] = { 0 }; // To store the frequency of each card (1 to 14)

	// Count the occurrences of each card
	for (int i = 0; i < numPlayers; i++) {
		frequency[playedCards[i]]++;
	}

	// Find the highest card with a frequency of 1
	int highestUniqueCard = 0;
	for (int i = 13; i >= 1; i--) { 
		if (frequency[i] == 1) {
			return i;
		}
	}

	return 0;
}

//function to show a menu after every completed round
void roundMenu(int numPlayers, char playersNames[][30], int playersCards[][NUM_PLAYER_CARDS], int scores[], int carriedScore, int round) {
	int choice;
	printf("\nOptions:\n");
	printf("Press 1 to complete the next round\n");
	printf("Press 2 to save the game\n");
	printf("Press 3 to output the game status\n");
	printf("Press 4 to exit without saving\n");
	scanf("%d", &choice);

	while (choice < 1 || choice >4) {
		printf("Wrong input\n");
		printf("Press 1 to complete the next round\n");
		printf("Press 2 to save the game\n");
		printf("Press 3 to output the game status, and continue the next round10\n");
		printf("Press 4 to exit without saving\n");
	}
	if (choice == 2) {
		//save the game
		saveGame(numPlayers, playersNames, playersCards, scores, carriedScore, round);
	}
	else if (choice == 3) {
		//output game states
		printf("---GAME STATES---\n");
		showStates(playersNames, scores, round, numPlayers);
	}
	else if (choice == 4) {
		//exit the game without saving
		exit();
	}
}

//function to save the game to a file
void saveGame(int numPlayers, char playersNames[][30], int playersCards[][NUM_PLAYER_CARDS], int scores[], int carriedScore, int round) {
	FILE* fp;

	fp = fopen("SavedGame.txt", "w");

	if (fp == NULL)
	{
		printf("Sorry, the file could not be opened and the game can't be saved\n");
	}

	else
	{
		// Save the number of players and their names
		fprintf(fp, "%d\n", numPlayers);
		for (int i = 0; i < numPlayers; i++) {
			fprintf(fp, "%s\n", playersNames[i]);
		}

		// Save player scores
		for (int i = 0; i < numPlayers; i++) {
			fprintf(fp, "%d ", scores[i]);
		}
		fprintf(fp, "\n");

		// Save the carried score and current round
		fprintf(fp, "%d\n", carriedScore);
		fprintf(fp, "%d\n", round);

		// Save the player hands
		for (int i = 0; i < numPlayers; i++) {
			for (int j = 0; j < NUM_PLAYER_CARDS; j++) {
				fprintf(fp, "%d ", playersCards[i][j]);
			}
			fprintf(fp, "\n");
		}

		fclose(fp);
		printf("Game has been successfully saved!\n");
	}
}

//function to load the game from a file
void loadGame(int* numPlayers, char playersNames[][30], int playersCards[][NUM_PLAYER_CARDS], int scores[], int* carriedScore, int* round) {
	FILE* fp;

	fp = fopen("SavedGame.txt", "r");

	if (fp == NULL) {
		printf("No saved game found or the file could not be opened.\n");
		return;
	}

	// Load the number of players
	fscanf(fp, "%d", numPlayers);

	// Load the player names
	for (int i = 0; i < *numPlayers; i++) {
		fscanf(fp, "%s", playersNames[i]);
	}

	// Load the player scores
	for (int i = 0; i < *numPlayers; i++) {
		fscanf(fp, "%d", &scores[i]);
	}

	// Load the carried score
	fscanf(fp, "%d", carriedScore);

	// Load the current round number
	fscanf(fp, "%d", round);

	// Load the player hands
	for (int i = 0; i < *numPlayers; i++) {
		for (int j = 0; j < NUM_PLAYER_CARDS; j++) {
			fscanf(fp, "%d", &playersCards[i][j]);
		}
	}

	fclose(fp);
	printf("Game has been successfully loaded!\n");
}


//function to show game states
void showStates(char playersNames[][30], int scores[], int rounds, int numPlayers) {
	printf("Rounds completed: %d\n",rounds);
	printf("Players scores:\n");
	for (int i = 0; i < numPlayers; i++) {
		printf("%s:\t%d\n",playersNames[i], scores[i]);
	}
}

//function to define the winner
void getWinner(int numPlayers, int scores[MAX_PLAYERS],char playersNames[][30], int rounds) {
	int winner = scores[0];
	int winnerCol = 0;
	for (int i = 0; i < numPlayers; i++) {
		if (scores[i] > winner) {
			winner = scores[i];
			winnerCol = i;
		}
	}
	printf("CONGRATS to %s for winning the game!\n",playersNames[winnerCol]);
	printf("------------------------------------------------------\n\n");
	printf("FINALE GAME STATES\n");
	//call show states function
	showStates(playersNames, scores, rounds, numPlayers);

}
