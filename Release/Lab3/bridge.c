#include <stdio.h>
#include <stdlib.h>

//Constants declaration
const int PLAYER_NOS = 4;
const int CARD_NOS = 52;
const int MAX_CARDS = 13;
const int MAX_CARDS_PER_TYPE = 13;


void score_calculation(int card_nos, char cards[MAX_CARDS_PER_TYPE][2], int *score, int *adjusted_score){
	int j;
	// Calculate the score + augmented score of Cards
	for (j=0; j<card_nos; j++) {
		switch (cards[j][1]) {
		case 'A':
		case 'a':
			*score+=4;
			*adjusted_score+=4;
			break;
		case 'K':
		case 'k':
			*score+=3;
			*adjusted_score+=3;
			break;
		case 'Q':
		case 'q':
			*score+=2;
			*adjusted_score+=2;
			break;
		case 'J':
		case 'j':
			*score+=1;
			*adjusted_score+=1;
			break;
		default:
			break;

		}
	}
	// Calculate the set, add to adjusted score.
	switch (card_nos) {
	case 0:
		*adjusted_score+=3;
		break;
	case 1:
		*adjusted_score+=2;
		switch (cards[0][1]) {
		case 'A':
		case 'K':
		case 'Q':
		case 'J':
			*adjusted_score-=1;
			break;
		default:
			break;

		}
		break;
	case 2:
		*adjusted_score+=1;
		break;
	case 5:
		*adjusted_score+=1;
		break;
	case 6:
		*adjusted_score+=2;
		break;
	default:
		break;

	}
	if (card_nos>=7) {
		*adjusted_score+=3;
	}

}

int main(int argc, char *argv[]) {

	//Array Initialisation
	char player_cards[MAX_CARDS][2];
	char diamonds_cards[MAX_CARDS_PER_TYPE][2];
	char hearts_cards[MAX_CARDS_PER_TYPE][2];
	char clubs_cards[MAX_CARDS_PER_TYPE][2];
	char spades_cards[MAX_CARDS_PER_TYPE][2];
	int players[PLAYER_NOS];
	int score=0;
	int adjusted_score=0;

	//Iterators.
	int i,j;

	for (i=0; i<PLAYER_NOS; i++) {
		players[i]=fork();
		if (players[i]==0) { //This is a child, then execute the statements for child below
			break;
		}else{
			continue; //This is a parent, continue to fork new child.
		}
	}

	if (i==PLAYER_NOS) {
		return 0;  // Parent forked enough child, parant terminates the programme(Child still running!).
	}

	for (j=0; j<((argc-1)/4); j++) {
		player_cards[j][0]=argv[j*4+i+1][0]; //Get the card of respective platers.
		player_cards[j][1]=argv[j*4+i+1][1];
	}
	printf("Child: %d, pid %d: ",i+1,getpid());
	for (j=0; j<MAX_CARDS; j++) {
		printf("%c%c ",player_cards[j][0],player_cards[j][1]);
	}
	printf("\n");
	int diamonds_array_index=0;
	int hearts_array_index=0;
	int clubs_array_index=0;
	int spades_array_index=0;
	for (j=0; j<((argc-1)/4); j++) {

		//Sort the respective set of cards.
		switch (player_cards[j][0]) {
		case 'D':
			diamonds_cards[diamonds_array_index][0]=player_cards[j][0];
			diamonds_cards[diamonds_array_index][1]=player_cards[j][1];
			diamonds_array_index++;
			break;
		case 'H':
			hearts_cards[hearts_array_index][0]=player_cards[j][0];
			hearts_cards[hearts_array_index][1]=player_cards[j][1];
			hearts_array_index++;
			break;
		case 'C':
			clubs_cards[clubs_array_index][0]=player_cards[j][0];
			clubs_cards[clubs_array_index][1]=player_cards[j][1];
			clubs_array_index++;
			break;
		case 'S':
			spades_cards[spades_array_index][0]=player_cards[j][0];
			spades_cards[spades_array_index][1]=player_cards[j][1];
			spades_array_index++;
			break;
		}
	}
	printf("Child %d pid: %d   ",i+1,getpid());
	printf("< ");
	for (j=0; j<diamonds_array_index; j++) {
		printf("%c%c ",diamonds_cards[j][0],diamonds_cards[j][1]);
	}
	printf(">");
	printf("< ");
	for (j=0; j<hearts_array_index; j++) {
		printf("%c%c ",hearts_cards[j][0],hearts_cards[j][1]);
	}
	printf(">");
	printf("< ");
	for (j=0; j<clubs_array_index; j++) {
		printf("%c%c ",clubs_cards[j][0],clubs_cards[j][1]);
	}
	printf(">");
	printf("< ");
	for (j=0; j<spades_array_index; j++) {
		printf("%c%c ",spades_cards[j][0],spades_cards[j][1]);
	}
	printf(">");

	printf("\n");

	printf("Child %d pid: %d   ",i+1,getpid());
	//Calculate the score, set by set.
	score_calculation(diamonds_array_index, diamonds_cards,&score,&adjusted_score);
	score_calculation(clubs_array_index, clubs_cards,&score,&adjusted_score);
	score_calculation(hearts_array_index, hearts_cards,&score,&adjusted_score);
	score_calculation(spades_array_index, spades_cards,&score,&adjusted_score);
	printf("Score: %d, Augmented Score: %d",score,adjusted_score);
	//TO-DO, score calculation
	printf("\n");

	return 0;
}
