#include "cricket.h"
#include "helper/vis.h"
#include "time.h"

match_result* results;

int main(int argc, char *argv[])
{
  setbuf(stdout,0);

  //declarations
  long int num_threads = 4;
  int totalMatchCount = NUMMATCHESPERGROUP * NUMGROUPS;
  int counter  = NUMQUARTERFINALS;

  while(counter){
    totalMatchCount += counter;
    counter /= 2;
  }
  
  results = (match_result*)calloc(sizeof(match_result), totalMatchCount);
  struct timespec begin, end;

  // argument handling
  if (argc < 1 || argc > 2) {
    fprintf(stderr, "usage: %s [#threads]\n", argv[0]);
    return 1;
  }
  if (argc == 2) {
    if ((num_threads = strtol(argv[1], NULL, 0)) == 0 || num_threads < 0) {
          fprintf(stderr, "#threads not valid!\n");
          return 1;
      }
  }

  printf("Process cricket.db \n");

  team_t* group1 = (team_t*) malloc(sizeof(team_t) * NUMTEAMS / NUMGROUPS);
  team_t* group2 = (team_t*) malloc(sizeof(team_t) * NUMTEAMS / NUMGROUPS);
  team_t* group3 = (team_t*) malloc(sizeof(team_t) * NUMTEAMS / NUMGROUPS);
  team_t* group4 = (team_t*) malloc(sizeof(team_t) * NUMTEAMS / NUMGROUPS);
  team_t* group5 = (team_t*) malloc(sizeof(team_t) * NUMTEAMS / NUMGROUPS);
  team_t* group6 = (team_t*) malloc(sizeof(team_t) * NUMTEAMS / NUMGROUPS);

  team_t* teams[NUMGROUPS];

  if (initDB()) {
    printf("Error during loading database: cricket.db\n");
    return 1;
  }

  // group A
  getTeam("Australia", group1 + 0);
  getTeam("West Indies", group1 + 1);
  getTeam("Ireland", group1 + 2);
  getTeam("Pakistan", group1 + 3);
  teams[0] = group1;

  // group B
  getTeam("England", group2 + 0);
  getTeam("New Zealand", group2 + 1);
  getTeam("Zimbabwe", group2 + 2);
  getTeam("Kenya", group2 + 3);
  teams[1] = group2;

  // group C
  getTeam("India", group3 + 0);
  getTeam("Sri Lanka", group3 + 1);
  getTeam("Scotland", group3 + 2);
  getTeam("Netherlands", group3 + 3);
  teams[2] = group3;

  // group D
  getTeam("South Africa", group4 + 0);
  getTeam("Bangladesh", group4 + 1);
  getTeam("Canada", group4 + 2);
  getTeam("Bermuda", group4 + 3);
  teams[3] = group4;


  clock_gettime(CLOCK_MONOTONIC, &begin);
  playEM(teams, num_threads);
  clock_gettime(CLOCK_MONOTONIC, &end);

  // visualize the results
  visualizeEM();
  // print timing information
  printf("\n\nTime: %.5f seconds\n", ((double)end.tv_sec + 1.0e-9*end.tv_nsec) -
                     ((double)begin.tv_sec + 1.0e-9*begin.tv_nsec));

  closeDB();
  free(results);

  return 0;
}
