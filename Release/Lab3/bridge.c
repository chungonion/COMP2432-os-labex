#include <stdio.h>
#include <stdlib.h>

const int PLAYER_NOS = 4;
const int CARD_NOS = 52;
const int MAX_CARDS = 13;
const int MAX_CARDS_PER_TYPE = 13;

int main(int argc, char *argv[]) {
	// char player1[MAX_CARDS][2];
	// char player2[MAX_CARDS][2];
	// char player3[MAX_CARDS][2];
	// char player4[MAX_CARDS][2];
	char player_cards[MAX_CARDS][2];
	char diamonds_cards[MAX_CARDS_PER_TYPE][2];
	char hearts_cards[MAX_CARDS_PER_TYPE][2];
	char clubs_cards[MAX_CARDS_PER_TYPE][2];
	char spades_cards[MAX_CARDS_PER_TYPE][2];

	int players[PLAYER_NOS];



	int i,j;

	for (i=0; i<PLAYER_NOS; i++) {
		players[i]=fork();
		if (players[i]==0) { //This is a child
            break;
		}else{ //This is a parent
			   //printf("%d Create\n",i); //Debug
			continue;
		}
	}

    if (i==PLAYER_NOS){
        return 0;
    }

    // printf("Child %d is running!\n",i);
    for (j=0; j<((argc-1)/4); j++) {
        player_cards[j][0]=argv[j*4+i+1][0];
        player_cards[j][1]=argv[j*4+i+1][1];
    }
    printf("Child %d pid: %d   ",i+1,getpid());
    for (j=0; j<MAX_CARDS; j++) {
        printf("%c%c ",player_cards[j][0],player_cards[j][1]);
    }
    printf("\n");
    int diamonds_array_index=0;
    int hearts_array_index=0;
    int clubs_array_index=0;
    int spades_array_index=0;
    for (j=0; j<((argc-1)/4); j++) {
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
    for (j=0;j<diamonds_array_index;j++){
        printf("%c%c ",diamonds_cards[j][0],diamonds_cards[j][1]);
    }
    printf(">");
    printf("< ");
    for (j=0;j<hearts_array_index;j++){
        printf("%c%c ",hearts_cards[j][0],hearts_cards[j][1]);
    }
    printf(">");
    printf("< ");
    for (j=0;j<clubs_array_index;j++){
        printf("%c%c ",clubs_cards[j][0],clubs_cards[j][1]);
    }
    printf(">");
    printf("< ");
    for (j=0;j<spades_array_index;j++){
        printf("%c%c ",spades_cards[j][0],spades_cards[j][1]);
    }
    printf(">");

    printf("\n");

    printf("Child %d pid: %d   ",i+1,getpid());

    
    printf("\n");

	return 0;
}
