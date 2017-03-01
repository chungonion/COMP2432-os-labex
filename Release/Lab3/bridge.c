#include <stdio.h>
#include <stdlib.h>

const int PLAYER_NOS = 4;

int main(int argc, char *argv[]) {
    char player1[13][2];
    char player2[13][2];
    char player3[13][2];
    char player4[13][2];



    int i;
    for (i=0;i<((argc-1)/4);i++){
        player1[i][0]=argv[i*4+1][0];
        player1[i][1]=argv[i*4+1][1];
        player2[i][0]=argv[i*4+2][0];
        player2[i][1]=argv[i*4+2][1];
        player3[i][0]=argv[i*4+3][0];
        player3[i][1]=argv[i*4+3][1];
        player4[i][0]=argv[i*4+4][0];
        player4[i][1]=argv[i*4+4][1];

    }

    /* code */

    for (i=0;i<13;i++){
        printf("%c%c ",player1[i][0],player1[i][1]);
    }
    printf("\n");
    return 0;
}
