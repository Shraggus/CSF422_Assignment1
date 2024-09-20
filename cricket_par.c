#include "cricket.h"
#include <omp.h>

// The last script did this tournament in a sequential manner. This script will do it in parallel. Set it up so that the group stage is played in parallel, and the final rounds are played in parallel as well.
void playEM(team_t** teams, int numThreads) {
  team_t* successors[NUMFINALISTS] = {0};
  //team_t* bestThirds[NUMGROUPS];
  int numSuccessors = NUMFINALISTS;
  omp_set_num_threads(numThreads);

  // Initialize
  initialize();

  // Play group stage in parallel
  #pragma omp parallel for
  for (int i = 0; i < NUMGROUPS; i++) {
    team_t *first, *second;
    playGroup(i, teams[i], &first, &second);

    #pragma omp critical
    {
      successors[i*2] = first;
      successors[i*2+1] = second;
    }
  }

  // Play final rounds
  playFinalRound(NUMQUARTERFINALS, successors, successors);
}

void playFinalRound(int numGames, team_t** teams, team_t** successors) {
  if (numGames == 1) {
    // Final match
    int score1 = 0, score2 = 0;
    playFinalMatch(numGames, 0, teams[0], teams[1], &score1, &score2);
    
    if (score1 == score2) {
        playSuperOver(teams[0], teams[1], &score1, &score2);
    }
    
    successors[0] = (score1 > score2) ? teams[0] : teams[1];
    printf("The winner of the Cricket World Cup is: %s\n", successors[0]->name);
    return;
  }

  team_t* nextRound[NUMFINALISTS/2] = {0};

  #pragma omp parallel for
  for (int i = 0; i < numGames; i++) {
    int score1 = 0, score2 = 0;
    playFinalMatch(numGames, i, teams[i*2], teams[i*2+1], &score1, &score2);

    team_t* winner;
    if (score1 == score2) {
        playSuperOver(teams[i*2], teams[i*2+1], &score1, &score2);
    }
    winner = (score1 > score2) ? teams[i*2] : teams[i*2+1];

    #pragma omp critical
    {
        nextRound[i] = winner;
    }
  }

  // Recursively play the next round
  playFinalRound(numGames / 2, nextRound, successors);
}