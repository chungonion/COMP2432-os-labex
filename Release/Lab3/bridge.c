#include <stdio.h>
#include <stdlib.h>

const int PLAYER_NOS = 4;
const int CARD_NOS = 52;
const int MAX_CARDS = 13;

int main(int argc, char *argv[]) {
    char player1[MAX_CARDS][2];
    char player2[MAX_CARDS][2];
    char player3[MAX_CARDS][2];
    char player4[MAX_CARDS][2];
    char player_cards[13][2];
    int players[PLAYER_NOS];



    int i,j;

    for (i=0;i<PLAYER_NOS;i++){
        players[i]=fork();
        if (players[i]==0){ //This is a child
            // printf("Child %d is running!\n",i);
            for (j=0;j<((argc-1)/4);j++){
                player_cards[j][0]=argv[j*4+i+1][0];
                player_cards[j][1]=argv[j*4+i+1][1];
            }
            printf("Child %d pid: %d   ",i,players[i]);
            for (j=0;j<13;j++){
                printf("%c%c ",player_cards[j][0],player_cards[j][1]);
            }
            printf("\n");
            break;
        }else{ //This is a parent
            //printf("%d Create\n",i); //Debug
            continue;
        }
    }
    // for (i=0;i<((argc-1)/4);i++){
    //     player1[i][0]=argv[i*4+1][0];
    //     player1[i][1]=argv[i*4+1][1];
    //     player2[i][0]=argv[i*4+2][0];
    //     player2[i][1]=argv[i*4+2][1];
    //     player3[i][0]=argv[i*4+3][0];
    //     player3[i][1]=argv[i*4+3][1];
    //     player4[i][0]=argv[i*4+4][0];
    //     player4[i][1]=argv[i*4+4][1];
    //
    // }
    //
    // /* code */
    //
    // for (i=0;i<13;i++){
    //     printf("%c%c ",player1[i][0],player1[i][1]);
    // }
    // printf("\n");
    return 0;
}
