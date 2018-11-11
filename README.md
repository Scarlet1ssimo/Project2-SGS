# README

## INTRODUCTION

A simple and naive version of SanGuoSha in CLI.

With the bonus

- implement a stack to handle the decks.

- Add card "Ease for Fatigue"(以逸待劳)
	- Make Everyone in the same country draw and discard 2 cards, respectively.

There are 3 modes in this GAME:

- HUMAN: All humans play

- SINGLE: One human (as the first player) VS (stupid) AIs
  
- DEMO: All AIs play

## HOW TO PLAY

### Linux (Strongly recommended)

To provide a special CLI for linux, I decided to use ncurses.h with instructer's permission.

#### Install ncurses

- Debian/Ubuntu/Linux Mint: `sudo apt-get install libncurses5-dev`

- Archlinux-based: `sudo pacman -S ncurses`
  
- CentOS: `sudo yum install ncurses-devel`

#### Compile

- HUMAN: `gcc -o main main.c init.c effect.c basic.c print.c -Wall -g -Wextra -pedantic -lncurses`

- SINGLE: `gcc -o main main.c init.c effect.c basic.c print.c -Wall -g -Wextra -pedantic -lncurses -DSINGLE`
  
- DEMO: `gcc -o main main.c init.c effect.c basic.c print.c -Wall -g -Wextra -pedantic -lncurses -DDEMO`

#### Play

- Not your round: Just scan the information and press any key.

- Your round: 
	- Press the number before cards displayed in your hand deck, then assign target or other. (Just follow the instructions)
  
	- R to reveal your general
  
	- G to end your turn

### Windows or MacOS

~~I'm not sure whether my program runs well on macos QAQ~~

#### Compile

- HUMAN: `gcc -o main main.c init.c effect.c basic.c print.c -Wall -g -Wextra -pedantic`

- SINGLE: `gcc -o main main.c init.c effect.c basic.c print.c -Wall -g -Wextra -pedantic -DSINGLE`
  
- DEMO: `gcc -o main main.c init.c effect.c basic.c print.c -Wall -g -Wextra -pedantic -DDEMO`

#### Play

- Not your round: Just scan the information and press any key.

- Your round: 
	- Type your the number before cards displayed in your hand deck, then assign target or guess how to use that card (QAQ).
  
	- Type -1 to reveal your general.

	- Type -2 to end your turn.

## CHANGELOG

See Changelog.md in the same directory.

## THANKS

Thank VG101 teaching group to provide such an intersting project/game for us to relize/play.

Good luck & Have fun!!!