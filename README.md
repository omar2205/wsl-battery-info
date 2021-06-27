## Windows Battery info

Display battery windows info in tmux or screen-gnu

Make sure that you have installed any font from <a href="https://github.com/ryanoasis/nerd-fonts">ryanoasis/nerd-fonts</a> and changed your wsl/windows cmd default font.

usage:
cbattery for screen-gnu
cbattery -t for tmux

### tmux

in ~/.tmux.conf:

```
set -g status-right "#(cbattery -t)"
```

### screen-gnu

in ~/.screenrc:

```
backtick 1 1 1 $HOME/bin/cbattery
hardstatus string '[%1`%{g}]'
```

### Not working?

if it isn't working change "CAP" or "STAT" headers in main.c:12

use `build.sh` to build and copy to ~/bin/cbattery

- requires gcc

```
/sys/class/power_supply/BAT1/capacity
/sys/class/power_supply/BAT0/capacity
/sys/class/power_supply/battery/capacity
```
