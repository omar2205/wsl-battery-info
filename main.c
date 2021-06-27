/** 
 * Battery Status and Capcity reader
 * by omar
 * arg: -t change output for tmux
 */

#define CAP "/sys/class/power_supply/BAT1/capacity"
#define STAT "/sys/class/power_supply/BAT1/status"

#define O_RDONLY         00
#define O_WRONLY         01
#define O_RDWR           02

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

  // invert strcmp for some reasone :p
  if(argc >= 2 && !strcmp(argv[1], "-t"))
    isTmux = true;


  // tmux output
  if(isTmux) {
    if(sy->cap < 20)
      color = "#[fg=colour1,bg=colour233,bold]"; // red
    else if(sy->cap < 60)
      color = "#[fg=colour3,bg=colour233,bold]"; // yellow
    else color = "#[fg=colour2,bg=colour233,bold]"; // green
  } else {
    if(sy->cap < 20)
      color = "\005{..r}"; // red
    else if(sy->cap < 60)
      color = "\005{..y}"; // yellow
    else color = "\005{..g}"; // green
  }

  // symbols are from Nerdfonts
  switch(sy->cap) {
    case 96 ... 100:
      if(sy->is_charging) symbol = "\uf0e7\uf578";
      else symbol = "\uf0e7\uf578";
      break;
    case 90 ... 95:
      if(sy->is_charging) symbol = "\uf0e7\uf581";
      else symbol = "\uf581";
      break;
    case 80 ... 89:
      if(sy->is_charging) symbol = "\uf0e7\uf580";
      else symbol = "\uf580";
      break;
    case 70 ... 79:
      if(sy->is_charging) symbol = "\uf0e7\uf57f";
      else symbol = "\uf57f";
      break;
    case 61 ... 69:
      if(sy->is_charging) symbol = "\uf0e7\uf57e";
      else symbol = "\uf57e";
      break;
    case 51 ... 59:
      if(sy->is_charging) symbol = "\uf0e7\uf57d";
      else symbol = "\uf57d";
      break;
    case 41 ... 49:
      if(sy->is_charging) symbol = "\uf0e7\uf57c";
      else symbol = "\uf57c";
      break;
    case 31 ... 39:
      if(sy->is_charging) symbol = "\uf0e7\uf57b";
      else symbol = "\uf57b";
      break;
    case 21 ... 29:
      if(sy->is_charging) symbol = "\uf0e7\uf57a";
      else symbol = "\uf57a";
      break;
    case 11 ... 19:
      if(sy->is_charging) symbol = "\uf0e7\uf579";
      else symbol = "\uf579";
      break;
    case 0 ... 10:
      if(sy->is_charging) symbol = "\uf0e7\uf58d";
      else symbol = "\uf58d";
      break;
  }
  
  printf("%s%s %i", color, symbol, sy->cap);

  return 0;
}
