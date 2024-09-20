#include "cricket.h"

void playEM(team_t** teams, int numThreads) {
  team_t* successors[NUMFINALISTS] = {0};
  int numSuccessors = NUMFINALISTS;

  // play groups
  initialize();

  // Start the tournament by playing the group stage
  for (int i = 0; i < NUMGROUPS; i++) {
    team_t *first, *second;
    playGroup(i, teams[i], &first, &second);
    
    // Put the winners and runners-up of each group into the successors
    successors[i*2] = first;
    successors[i*2+1] = second;
  }

  // Start to play the final round
  int numGames = NUMQUARTERFINALS;
  while (numGames > 0) {
    playFinalRound(numGames, successors, successors);
    numGames /= 2;
  }

  // The winner is now in successors[0]
  printf("The winner of the Cricket World Cup is: %s\n", successors[0]->name);
}

void playFinalRound(int numGames, team_t** teams, team_t** successors) {
  team_t *team1, *team2;
  int score1 = 0, score2 = 0;

  for (int i = 0; i < numGames; ++i) {
    // Play the final match between two teams
    team1 = teams[i*2];
    team2 = teams[i*2+1];
    
    playFinalMatch(numGames, i, team1, team2, &score1, &score2);

    // Choose the winner according to the score
    if (score1 > score2) {
      successors[i] = team1;
    } else if (score1 < score2) {
      successors[i] = team2;
    } else {
      // Super over
      playSuperOver(team1, team2, &score1, &score2);
      successors[i] = (score1 > score2) ? team1 : team2;
    }
  }
}