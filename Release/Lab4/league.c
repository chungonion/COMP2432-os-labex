#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int const MAX_TEAM_NOS = 30;
int const PLAYED_COLUMN = 0;
int const WIN_COLUMN = 1;
int const DRAW_COLUMN = 2;
int const LOSE_COLUMN = 3;
int const GF_COLUMN = 4;
int const GA_COLUMN = 5;
int const SCORE_COLUMN = 6;



void result_display(int team_nos,int game_result[MAX_TEAM_NOS][7],char const *team_name[MAX_TEAM_NOS]){
	int i,j;
	int team_order[MAX_TEAM_NOS];
	for (i=0; i<team_nos; i++) {
		team_order[i]=i;
	}

	for (i=0; i<team_nos; i++) {
		int temp_largest=i;
		int temp;
		for (j=team_nos-1; j>i; j--) {
			if (game_result[team_order[j]][SCORE_COLUMN]>(game_result[team_order[temp_largest]][SCORE_COLUMN])) {
				temp_largest=j;
			}
		}
		temp = team_order[i];
		team_order[i]=team_order[temp_largest];
		team_order[temp_largest]=temp; //Swap

	}
	printf("%-8s%-12s%-5s%-5s%-5s%-5s%-6s%-6s%-6s%-9s\n","Rank","Team","P","W","D","L","GF","GA","GD","Points");
	for (i=0; i<team_nos; i++) {
		printf("%-8d",i+1);
		printf("%-12s",team_name[team_order[i]]);
		if (game_result[team_order[i]][PLAYED_COLUMN]==((team_nos-1)*2)) {
			printf("%-5d",game_result[team_order[i]][PLAYED_COLUMN]);
		}else{
			char temp[10];
			sprintf(temp, "%d(%d)",((team_nos-1)*2),game_result[team_order[i]][PLAYED_COLUMN]);
			printf("%-5s",temp);
		}
		printf("%-5d",game_result[team_order[i]][WIN_COLUMN]);
		if (game_result[team_order[i]][PLAYED_COLUMN]==((team_nos-1)*2)) {
			printf("%-5d",game_result[team_order[i]][DRAW_COLUMN]);
		}else{
			char temp[10];
			game_result[team_order[i]][SCORE_COLUMN]+=(((team_nos-1)*2)-game_result[team_order[i]][PLAYED_COLUMN]);
			sprintf(temp, "%d(%d)",((team_nos-1)*2)-game_result[team_order[i]][PLAYED_COLUMN]+game_result[team_order[i]][DRAW_COLUMN],game_result[team_order[i]][DRAW_COLUMN]);
			printf("%-5s",temp);
		}
		printf("%-5d",game_result[team_order[i]][LOSE_COLUMN]);
		printf("%-6d",game_result[team_order[i]][GF_COLUMN]);
		printf("%-6d",game_result[team_order[i]][GA_COLUMN]);
		printf("%-6d",game_result[team_order[i]][GF_COLUMN]-game_result[team_order[i]][GA_COLUMN]);
		printf("%-9d",game_result[team_order[i]][SCORE_COLUMN]);
		printf("\n");

	}
}

int main(int argc, char const *argv[]) {
	int team_nos;
	int game_per_round;
	int total_rounds;
	int game_result[MAX_TEAM_NOS][7] = {0};
	char const *team_name[MAX_TEAM_NOS];
	char const *filename;



	// team_name[0]="Testing!";
	// printf("%s\n", team_name[0]);
	team_nos = argc -2;
	total_rounds = team_nos * 2;
	game_per_round = team_nos/2;
	filename=argv[argc-1];

	//Iterators
	int i;
	for (i=0; i<argc-2; i++) {
		team_name[i]=argv[i+1]; //TODO: REALLY A BUG?
	}

	for (i=0; i<total_rounds; i++) {
		FILE *infile;
		char this_round_file[100];
		sprintf(this_round_file, "%s%d",filename,(i+1));
		infile = fopen(this_round_file,"r");
		if (infile == NULL) {
			printf("Error in opening input file\n");
			continue;
		}
		//inner loop iterators
		int j;
		for (j=0; j<game_per_round; j++) {
			char team_A_name[40];
			char team_B_name[40];
			int team_A_score;
			int team_B_score;
			int team_A_index;
			int team_B_index;
			int k;
			int teamfound;
			//TODO: confirm the team name is in the name array

			fscanf(infile,"%s %d %s %d ",team_A_name,&team_A_score,team_B_name,&team_B_score);
			// printf("%s %d %s %d ",team_A_name,team_A_score,team_B_name,team_B_score);
			teamfound = 0;
			for (k=0; k<team_nos; k++) {
				if (strcmp(team_A_name,team_name[k])==0) {
					team_A_index = k;
					teamfound=1;
					break;
				}
			}
			if (teamfound==0) {
				// printf("Exit! A ");
				continue;
			}
			teamfound = 0;
			for (k=0; k<team_nos; k++) {
				if (strcmp(team_B_name,team_name[k])==0) {
					team_B_index = k;
					teamfound=1;
					break;
				}
			}
			if (teamfound==0) {
				// printf("Exit! B ");
				continue;
			}

			//game calculation
			game_result[team_A_index][PLAYED_COLUMN]+=1;
			game_result[team_B_index][PLAYED_COLUMN]+=1;

			if (team_A_score>team_B_score) {
				game_result[team_A_index][WIN_COLUMN]+=1;
				game_result[team_B_index][LOSE_COLUMN]+=1;
				game_result[team_A_index][SCORE_COLUMN]+=3;

			}else if (team_A_score==team_B_score) {
				game_result[team_A_index][DRAW_COLUMN]+=1;
				game_result[team_B_index][DRAW_COLUMN]+=1;
				game_result[team_A_index][SCORE_COLUMN]+=1;
				game_result[team_B_index][SCORE_COLUMN]+=1;

			}else if (team_B_score>team_A_score) {
				game_result[team_A_index][LOSE_COLUMN]+=1;
				game_result[team_B_index][WIN_COLUMN]+=1;
				game_result[team_B_index][SCORE_COLUMN]+=3;

			}
			game_result[team_A_index][GF_COLUMN]+=team_A_score;
			game_result[team_B_index][GF_COLUMN]+=team_B_score;

			game_result[team_A_index][GA_COLUMN]+=team_B_score;
			game_result[team_B_index][GA_COLUMN]+=team_A_score;





			// printf("Team are %s, %s.",team_name[team_A_index],team_name[team_B_index]);
		}
		close(infile);
	}
	//Basic Principle: read the files round by round, and then add that to corresponding
	/* code */
	result_display(team_nos,game_result,team_name);

	return 0;
}
