## Windows Battery info

Display battery windows info in tmux or screen-gnu

if it isn't working change "CAP" or "STAT" headers in main.c:12

use `build.sh` to build and copy to ~/bin/cbattery

- requires gcc

```
/sys/class/power_supply/BAT1/capacity
/sys/class/power_supply/BAT0/capacity
/sys/class/power_supply/battery/capacity
```
