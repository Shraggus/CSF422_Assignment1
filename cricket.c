#include "cricket.h"
#include "helper/vis.h"

int gameNum[NUMGROUPS];

void initialize() {
  memset(&gameNum, 0, NUMGROUPS * sizeof(int));
}

int team1DominatesTeam2(team_t* team1, team_t* team2)
{
  if (team1->points > team2->points ||
      (team1->points == team2->points && team1->score >= team2->score))
    return true;
  else
    return false;
}

double getScorePerGame(const player_t* players, int numPlayers) {
  int i;
  double scorePerGame = 0.0;

  for (i = 0; i < numPlayers; ++i) {
    if (players[i].games) {
      scorePerGame += (double)players[i].score /
                      (double)players[i].games;
    }
  }

  return scorePerGame;
}

void playMatchGen(team_t* team1, team_t* team2, int* score1, int* score2) {
  match_t* matches;
  player_t *players1, *players2;
  int i, numMatches, numPlayers1, numPlayers2;
  double s1 = 0.0, s2 = 0.0;

  getMatches(team1, team2, &matches, &numMatches);
  if (numMatches) {
    for (i=0; i < numMatches; ++i) {
      getPlayersOfMatch(matches + i,
                        &players1, &numPlayers1,
                        &players2, &numPlayers2);
      s1 += getScorePerGame(players1, numPlayers1);
      free(players1);
      s2 += getScorePerGame(players2, numPlayers2);
      free(players2);
      s1 += (double)matches[i].score1 / (double)numMatches;
      s2 += (double)matches[i].score2 / (double)numMatches;
    }
    free(matches);
  } else if (team1->difference > team2->difference) {
    s1 = 1;
    s2 = 0;
  } else {
    s2 = 1;
    s1 = 0;
  }
  *score1 = s1;
  *score2 = s2;
}

void playGroupMatch(int groupNo,
                    team_t* team1, team_t* team2, int* score1, int* score2) {
  playMatchGen(team1, team2, score1, score2);
  // handle visualization
  handleGame(gameNum[groupNo] + NUMMATCHESPERGROUP * groupNo,
             team1->name, team2->name, *score1, *score2);
  gameNum[groupNo]++;
}

int final_pos (int numGames){
  if (numGames == NUMQUARTERFINALS)
    return 0;
  return  final_pos (2 * numGames)+ 2*numGames;
}

void playFinalMatch(int numGames, int gameNo,
                    team_t* team1, team_t* team2, int* score1, int* score2) {
  playMatchGen(team1, team2, score1, score2);

  // handle visualization
  handleGame(NUMMATCHESPERGROUP*NUMGROUPS + final_pos(numGames) + gameNo,
             team1->name, team2->name, *score1, *score2);
}

void playGroup(int groupNo, team_t* teams, team_t** first, team_t** second)
{
  int i, j, score1, score2, dominates;
  int tmpScore = groupScore;

  // play matches
  for (i = 0; i < NUMTEAMSINGROUP; ++i)
  {
    for (j = NUMTEAMSINGROUP - 1; j > i; --j)
    {
      playGroupMatch(groupNo, teams + i, teams + j, &score1, &score2);
                        teams[i].score += score1;
      teams[i].score -= score2;
      teams[j].score += score2;
      teams[j].score -= score1;
      if (score1 > score2) {
        teams[i].points += 3;
      }
      else if (score1 < score2)
        teams[j].points += 3;
      else
      {
        teams[i].points += 1;
        teams[j].points += 1;
      }
      tmpScore += score2 + score1;
    }
  }

  groupScore = tmpScore;
  // return first and second team
  for (i = 0; i < NUMTEAMSINGROUP; ++i) {
    dominates = 0;
    for (j = NUMTEAMSINGROUP - 1; j >= 0; --j) {
      if (i != j) {
        if (team1DominatesTeam2(teams + i, teams + j)) {
          dominates += 1;
        }
      }
    }
    if (dominates == NUMTEAMSINGROUP - 1) *first = teams + i;
    else if (dominates == NUMTEAMSINGROUP - 2) *second = teams + i;
  }
}

void playSuperOver(team_t* team1, team_t* team2,
                 int* score1, int* score2) {
  match_t* matches;
  int numMatches;
  int i;
  int diff = 0;

  getMatches(team1, team2, &matches, &numMatches);
  for (i = 0; i < numMatches; ++i) {
    if (matches[i].finalRound) {
      diff += matches[i].score1;
      diff -= matches[i].score2;
    }
    if (diff >= 0) {
      *score1 += 1;
      *score2 += 0;
    }
    else {
      *score1 += 0;
      *score2 += 1;
    }
  }
  free(matches);
}
