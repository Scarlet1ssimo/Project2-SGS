#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAXN 120
#define DEBUG
/*
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
*/
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
	int Nb;				//Number
	int hlt, hltLv;		//Health point;Health Level
	int ID;				//Identity: 2:general 1:mercenary
	int ctry;			//Counttry
	int Rvld;			//Revealed 0:Unrevealed 1:Revealed
	Deck Hnd, Eqp, Ftz; //Hand Zone;Equipment Zone;Fate Zone
	struct Plyr *Nxt;   //Linked List
} Player;
typedef struct Gm
{
	Deck Main; //Main deck
	int CTRY;  //Main Country
	int N;	 //Live Players
} Game;
void fateState(Player *X, Game *G);
void drawState(Player *X, Game *G);
void playState(Player *X, Game *G);
void discardState(Player *X, Game *G);
int Terminal(Player *X);
void createDeck(Deck *D, int argc, char *file);
void clearDeck(Deck *D);
Game *initGame(int n, int argc, char *file);
void initPlayer(Player *P, int i,Game *G);
Player *initPlayers(int n,Game *G);
void Perform(Game *G, Player *Head);
int rd(int L,int R){return rand() % (R - L + 1) + L;}
int min(int a, int b);

int main(int argc, char **argv)
{
	freopen("err.log", "w", stderr);
	srand(time(NULL));
	int n = 5;
	Game *G = initGame(n, argc, argv[2]);
	Player *Head = initPlayers(n,G);
	Perform(G, Head);
	return 0;
}

void fateState(Player *X, Game *G)
{
	for (int i = 0; i < X->Ftz.n; i++);
}
void DRAW(Deck *D,Deck *G){D->a[D->n++] = G->a[--G->n];}
void drawState(Player *X, Game *G)
{
	DRAW(&X->Hnd, &G->Main);
	DRAW(&X->Hnd, &G->Main);
}
void playState(Player *X, Game *G)
{

}
void discardState(Player *X, Game *G)
{
	X->Hnd.n = min(X->Hnd.n, X->hltLv);
}
int Terminal(Player *X)
{
	int A[MAXN], tot = 1;
	for (Player *Y = X; Y->Nb != X->Nb; Y = Y->Nxt, tot++)
	{
		A[tot] = Y->ID;
		if (Y->Rvld == 0)
			return 0;
	}
	if (tot == 1)
		return 1;
	for (int i = 1; i < tot;i++)
		if(A[i]!=A[1])
			return 0;
	return 1;
}
void shuffle(Deck *D)
{
	for (int i = 1; i < D->n;i++)
	{
		int j = rd(0, i);
		Card t = D->a[i];
		D->a[i] = D->a[j];
		D->a[j] = t;
	}
}
void createDeck(Deck *D, int argc, char *file)
{
	clearDeck(D);
	FILE *fp;
	if (argc==3)
		fp = fopen(file, "r");
	else 
	#ifdef DEBUG
		fp = fopen("w.in", "r");
	#else
		fp = stdin;
	#endif
	char S[MAXN];
	D->n = 0;
	while (~fscanf(fp, "%s", S))
	{
		int num;
		fscanf(fp, "%d", &num);
		for (int i = D->n; i < D->n+ num;i++)
		{
			if (!strcmp(S, "Strike"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 1};
			if (!strcmp(S, "Dodge"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 2};
			if (!strcmp(S, "Peach"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 3};
			if (!strcmp(S, "Wine"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 4};
			if (!strcmp(S, "Dismatle"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 5};
			if (!strcmp(S, "Duel"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 6};
			if (!strcmp(S, "Snatch"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 7};
			if (!strcmp(S, "BorrowedSword"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 8};
			if (!strcmp(S, "ArrowBarrage"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 9};
			if (!strcmp(S, "BarbarianInvasion"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 10};
			if (!strcmp(S, "PeachGarden"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 11};
			if (!strcmp(S, "SomethingForNothing"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 12};
			if (!strcmp(S, "BountifulHarvest"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 13};
			if (!strcmp(S, "Lighting"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 14};
			if (!strcmp(S, "DrownInHappiness"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 15};
			if (!strcmp(S, "Starvation"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 16};
		}
		D->n += num;
	}
	shuffle(D);
	for (int i = 0; i < D->n;i++)fprintf(stderr, "%d\n", D->a[i].type);
	fclose(fp);
}
void clearDeck(Deck *D)
{
	memset(D, 0, sizeof(Deck));
}
Game *initGame(int n, int argc, char *file)
{
	Game *G = (Game *)malloc(sizeof(Game));
	G->N = n;
	createDeck(&G->Main, argc, file);
	G->CTRY = 0;
	return G;
}
void initPlayer(Player *P, int i,Game *G)
{
	P->Nb = i;
	P->hltLv = rd(3, 5);
	P->hlt = P->hltLv;
	P->ID = 0;
	P->ctry = rd(1, 4);
	P->Rvld = 0;
	clearDeck(&P->Hnd);
	clearDeck(&P->Eqp);
	clearDeck(&P->Ftz);
	for (int i = 1; i <= 5;i++)
		DRAW(&P->Hnd, &G->Main);
}
Player *initPlayers(int n,Game *G)
{
	Player *Head = (Player *)malloc(sizeof(Player)), *Now = Head;
	for (int i = 1; i < n; i++)
	{
		initPlayer(Now, i,G);
		Now->Nxt = (Player *)malloc(sizeof(Player));
		Now = Now->Nxt;
	}
	initPlayer(Now, n,G);
	Now->Nxt = Head;
	return Head;
}
void Perform(Game *G, Player *Head)
{
	Player *Now = Head;
	for (; !Terminal(Now); Now = Now->Nxt)
	{
		fateState(Now, G);
		drawState(Now, G);
		playState(Now, G);
		discardState(Now, G);
	}
	free(G);
}
int min(int a, int b)
{
	return a < b ? a : b;
}