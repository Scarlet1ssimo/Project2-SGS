#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef __APPLE__
#define PAUSE system("read -n 1 -s -p \"Press any key to continue...\"")
#endif
#ifdef linux
#include <ncurses.h>
#endif

#define MAXN 200
#define MAXL 80
#define DEBUG 1
#define TRAINING
#define HUMAN 1
#define _FILE 0
#define JB 80
#define forP(P) for (Player *Now = P->Nxt; Now->Nb != P->Nb; Now = Now->Nxt)
#define CLS system("cls")
#define PLAYERS_NUMBER 10

typedef struct _Card
{
	int num, col;
	int type;
} Card;

typedef struct _Cards
{
	int n;
	Card a[MAXN];
} Deck;

typedef struct Plyr
{
	int Nb; //Number
	int Bot;
	int Skip;
	int hlt, hltLv;		//Health point;Health Level
	int ID;				//Identity: 2:general 1:mercenary
	int ctry;			//Country
	int Rvld;			//Revealed 0:Unrevealed 1:Revealed
	Deck Hnd, Eqp, Ftz; //Hand Zone;Equipment Zone;Fate Zone
	struct Plyr *Nxt;   //Linked List
} Player;
#ifdef linux
typedef struct PNL
{
	WINDOW *W;
	char LOGS[MAXN][MAXL];
	int n, N;
} Panel;
#endif
typedef struct Gm
{
	Deck Main, Dscd; //Main deck
	int N, n;		 //Total/Live Players
#ifdef linux
	Panel GM, IF;
#endif
} Game;

//print
void printDeck(Deck *D, Game *G);
void _pD(Deck *D, Game *G);
void printPlayer(Player *X, Game *G);
void printGame(Player *X, Game *G);
#ifdef linux
int MSG(Panel *P, const char *fmt, ...);
void _MESSAGE(Panel *P, char S[]);
void SHOW(Panel *P);
#endif

//basic
void DRAW(Deck *D, Deck *G, Deck *GG);
void DISCARD(Deck *D, int k, Deck *G);
void USE(Deck *D, int use, Deck *G);
Player *getFromNb(Player *X, int id);
int OPP(Player *X, Player *Y);
int ASK(Player *X, int y, Game *G);
int _ASK(Deck *D, int y);
int __ASK(Deck *D, int y);
int calcDist(Player *X, Player *Y);
int Terminal(Player *X, Game *G);
int rd(int L, int R);
int min(int a, int b);
int max(int a, int b);
#ifdef linux
int rdn();
#endif

//effects
void Reveal(Player *X, Game *G);
int HELP(Player *Y, Game *G);
void DISABLE(Player *Y, Game *G);
void calcDamage(Player *Y, int Dam, Game *G);
void STRIKE(Player *X, Player *Y, Game *G);
void WINE_STRIKE(Player *X, Player *Y, Game *G);
void PEACH(Player *X, Game *G);
void DUEL(Player *X, Player *Y, Game *G);
void DISMANTLE(Player *X, Player *Y, Game *G);
void _SNATCH(Player *X, Deck *Y, Game *G);
void SNATCH(Player *X, Player *Y, Game *G);
Card FATE(Game *G);
void FATE_LIGHTING(Player *X, Game *G, int i);
void FATE_DROWNINHAPPINESS(Player *X, Game *G, int i);
void FATE_STARVATION(Player *X, Game *G, int i);
void ARROWBARRAGE(Player *X, Game *G);
void INVASION(Player *X, Game *G);
void HARVEST(Player *X, Game *G);
int BORROWEDSWORD(Player *Y, Game *G);

//initialize
#ifdef linux
void initPanel(Panel *P, int h, int w, int sy, int sx);
void cP(Panel *P);
void clearPanel(Panel *P, int L);
#endif
Game *initGame(int n, int argc, char *file);
void initPlayer(Player *P, int i, Game *G);
Player *initPlayers(int n, Game *G);
void shuffle(Deck *D);
void createDeck(Deck *D, int argc, char *file);
void clearDeck(Deck *D);

//Control
void fateState(Player *X, Game *G);
void drawState(Player *X, Game *G);
void playState(Player *X, Game *G);
void discardState(Player *X, Game *G);
void Perform(Game *G, Player *Head);

/*
Clover:		1
Diamond:	2
Hearts:		3
Spades:		4

Cards:					type:

Strike					1
Dodge					2
Peach					3
Wine					4

Duel					5
Dismantle				6
Snatch					7
Borrowed sword			8

Arrow barrage			9
Barbarian invasion		10
Peach garden			11
Something for nothing	12
Bountiful harvest		13

Lighting				14
Drown in happiness		15
Starvation				16

Binoculars				17
Bow						18
Horse					19
*/
