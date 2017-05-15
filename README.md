# Hanoi
An implementation of the puzzle game Towers of Hanoi for the linux terminal.
This games requires the ncurses library.

## Compilation
```
make
```

## Instructions
To run the program
```
hanoi [-c color] [-n # of disks | -m]
```

### Runtime Options
-c <color>: choose a color for the disks
-n   <num>: choose the number of disks (default 5)
-m        : play with the maximum number of disks that can fit (note: This may result in a game that is unsolvable before the heat death of the universe)

### Controls
Use j,k,l or left,down,right to select a disk to move. A white marker will appear above the peg that disk is on. Use j,k,l or left,down,right to select the destination peg that disk will be moved to.

#### Note
If you experience flickering, use a buffered terminal or terminal multiplexer such as GNU screen or tmux.
