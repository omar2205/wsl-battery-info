/** 
 * Battery Status and Capcity reader
 * by omar
 * arg: -t change output for tmux
 */

#define CAP "/sys/class/power_supply/BAT1/capacity"
#define STAT "/sys/class/power_supply/BAT1/status"

// nerd fonts
#define BOLT "\uf0e7"
// Define fa codes
#define FA_BAT0 "\uf244\uf22f"      // <20
#define FA_BAT1 "\uf243\uf22f"      // 20-29
#define FA_BAT2 "\uf242\uf22f"      // 30-49
#define FA_BAT3 "\uf241\uf22f"      // 50-79
#define FA_BAT_FULL "\uf240\uf22f"  // 80-100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include "cc.h"

struct sys {
  bool is_charging;
  int stat; // 1: charging 2: Full 3: Discharging 
  int cap; // battery percentage
};

struct sys *read_sys(void) {
  struct sys *s = malloc(sizeof(struct sys));
  int cFD = open(CAP, O_RDONLY, 0);
  int sFD = open(STAT, O_RDONLY, 0);
  char status[16] = "";
  char capacity[4] = "";

  read(cFD, capacity, 4);
  read(sFD, status, 16);
  close(cFD);
  close(sFD);

  s->cap = strtol(capacity, NULL, 0);
  // remember \n when comparing
  s->is_charging = strcmp(status, "Charging\n") == 0 ? true : false;
  if(strcmp(status, "Charging\n") == 0)
    s->stat = 1;
  else if(strcmp(status, "Full\n") == 0)
    s->stat = 2;
  else if(strcmp(status, "Discharging\n") == 0)
    s->stat = 3;
  else 
    s->stat = -1;

  return s;
}

int main(int argc, char *argv[]) {
  struct sys *sy = NULL;
  sy = read_sys();

  bool isTmux = false;
  char* symbol = "";
  char* color = "";

  if(argc >= 2 && strcmp(argv[1], "-t") == 0)
    isTmux = true;


  // tmux output
  if(isTmux) {
    if(sy->cap < 20) color = "#[fg=colour1,bg=colour233,bold]"; // red
    else if(sy->cap < 60) color = "#[fg=colour3,bg=colour233,bold]"; // yellow
    else color = "#[fg=colour2,bg=colour233,bold]"; // green
  } else {
    if(sy->cap < 20) color = "\005{..r}"; // red
    else if(sy->cap < 60) color = "\005{..y}"; // yellow
    else color = "\005{..g}"; // green
  }

  // symbols are from Nerdfonts
  switch(sy->cap) {
    case 80 ... 100:
      if(sy->is_charging) symbol = BOLT FA_BAT_FULL;
      else symbol = FA_BAT_FULL;
      break;
    case 50 ... 79:
      if(sy->is_charging) symbol = BOLT FA_BAT3;
      else symbol = FA_BAT3;
      break;
    case 30 ... 49:
      if(sy->is_charging) symbol = BOLT FA_BAT2;
      else symbol = FA_BAT2;
      break;
    case 20 ... 29:
      if(sy->is_charging) symbol = BOLT FA_BAT1;
      else symbol = FA_BAT1;
      break;
    default:
      if(sy->is_charging) symbol = BOLT FA_BAT0;
      else symbol = FA_BAT0;
      break;
  }
  
  printf("%s%s %i%%", color, symbol, sy->cap);

  return 0;
}
