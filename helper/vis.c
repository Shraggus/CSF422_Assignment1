#include "vis.h"
#include "../cricket.h"

int groupScore = 0;

void visualize(int pos, int numMatches){
  	match_result* r = results + pos;

    // displays the results given as parameters in a single row
 	// must be called for every group and final round of same level

	char score1[numMatches][33];
	char score2[numMatches][33];
	int i;

	for(i = 0; i < numMatches; i++){
		if(numMatches == 4) printf("            ");
		if(numMatches == 2) printf("                                    ");
		if(numMatches == 1) printf("                                                                                    ");
		sprintf(score1[i], "%d", r[i].score1);
		sprintf(score2[i], "%d", r[i].score2);
		printf("----------------------- ");
		if(numMatches == 4) printf("            ");
		if(numMatches == 2) printf("                                    ");
		
	}
	printf("\n");

	for(i = 0; i < numMatches; i++){
		if(numMatches == 4) printf("            ");
		if(numMatches == 2) printf("                                    ");
		if(numMatches == 1) printf("                                                                                    ");
		printf("|%*s%*s-", 8+(int)strlen(r[i].name1)/2, r[i].name1, 8-(int)strlen(r[i].name1)/2, "");
		printf("%*s%*s| ", 2+(int)strlen(score1[i])/2,  score1[i], 2-(int)strlen(score1[i])/2, "");
		if(numMatches == 4) printf("            ");
		if(numMatches == 2) printf("                                    ");		
	}

	printf("\n");

	for(i = 0; i < numMatches; i++)	{

		if(numMatches == 4) printf("            ");
		if(numMatches == 2) printf("                                    ");
		if(numMatches == 1) printf("                                                                                    ");
		printf("|%*s%*s-", 8+(int)strlen(r[i].name2)/2, r[i].name2, 8-(int)strlen(r[i].name2)/2, "" );
		printf("%*s%*s| ", 2+(int)strlen(score2[i])/2, score2[i], 2-(int)strlen(score2[i])/2, "");
		if(numMatches == 4) printf("            ");
		if(numMatches == 2) printf("                                    ");
	}

	printf("\n");

	for(i = 0; i < numMatches; i++){
		if(numMatches == 4) printf("            ");
		if(numMatches == 2) printf("                                    ");
		if(numMatches == 1) printf("                                                                                    ");
		printf("----------------------- ");
		if(numMatches == 4) printf("            ");
		if(numMatches == 2) printf("                                    ");
	}
	printf("\n\n");
	
}

void visualizeEM() {
  int g;
  int numGames = NUMQUARTERFINALS ;
  for (g = 0; g < NUMGROUPS; ++g) {
	printf("     Group %c\n", g + 65);
	visualize(NUMMATCHESPERGROUP * g, NUMMATCHESPERGROUP);
  }

  printf("group score: %d ---	Finals!\n", groupScore);

  int numGamesPlayed  = NUMGROUPS*NUMMATCHESPERGROUP;
  
  while (numGames >= 1) {
    visualize(numGamesPlayed, numGames);
    numGamesPlayed += numGames;
    numGames /= 2;
  }
}

void handleGame(int index,
                const char* team1, const char* team2,
                int score1, int score2) {
  strcpy(results[index].name1 , team1);
  strcpy(results[index].name2 , team2);
  results[index].score1 = score1;
  results[index].score2 = score2;
}
