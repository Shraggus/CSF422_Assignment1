#ifndef EM_H
#define EM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "helper/db.h"

#define true 1
#define false 0
#define NUMTEAMS 16
#define NUMGROUPS 4
#define NUMFINALISTS 8
#define NUMTEAMSINGROUP 4
#define NUMMATCHESPERGROUP 6
#define NUMQUARTERFINALS 4

typedef struct{
  char name1[40];
  char name2[40];
  int score1;
  int score2;
} match_result;

extern match_result* results;

void playFinalMatch(int numGames, int gameNo,
                    team_t* team1, team_t* team2, int* score1, int* score2);

void playGroup(int groupNo, team_t* teams, team_t** first, team_t** second);

void playFinalRound(int numGames, team_t** teams, team_t** successors);

void playSuperOver(team_t* team1, team_t* team2, int* score1, int* score2);

void sortTeams(int numTeams, team_t** teams);

void playEM(team_t** teams, int numThreads);

int final_pos(int numGames);

void initialize(); 

#endif // EM_H
