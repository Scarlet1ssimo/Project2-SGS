#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAXN 120
#define DEBUG
#define TRAINING
#define forP(P) for (Player *Now = P->Nxt; Now->Nb != P->Nb; Now = Now->Nxt)
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
	int Nb; //Number
	int Bot;
	int hlt, hltLv;		//Health point;Health Level
	int ID;				//Identity: 2:general 1:mercenary
	int ctry;			//Counttry
	int Rvld;			//Revealed 0:Unrevealed 1:Revealed
	Deck Hnd, Eqp, Ftz; //Hand Zone;Equipment Zone;Fate Zone
	struct Plyr *Nxt;   //Linked List
} Player;
typedef struct Gm
{
	Deck Main, Dscd; //Main deck
	int N, n;		 //Total/Live Players
} Game;

//CONTROL
void fateState(Player *X, Game *G);
void drawState(Player *X, Game *G);
void playState(Player *X, Game *G);
void discardState(Player *X, Game *G);
int Terminal(Player *X);
void Perform(Game *G, Player *Head);
Game *initGame(int n, int argc, char *file);
void printGame(Player *X);
//CARD
void createDeck(Deck *D, int argc, char *file);
void clearDeck(Deck *D);
//PLAYER
void initPlayer(Player *P, int i, Game *G);
Player *initPlayers(int n, Game *G);
//BASIC
int rd(int L, int R) { return rand() % (R - L + 1) + L; }
int min(int a, int b);

int main(int argc, char **argv)
{
	freopen("err.log", "w", stderr);
	srand(time(NULL));
	int n = 5;
	Game *G = initGame(n, argc, argv[2]);
	Player *Head = initPlayers(n, G);
	Perform(G, Head);
	fclose(stderr);
	return 0;
}

//CARD
void DRAW(Deck *D, Deck *G, Deck *GG)
{
	if (!G->n)
	{
		Deck *tmp = GG;
		GG = G;
		G = tmp;
		clearDeck(GG);
	}
	D->a[D->n++] = G->a[--G->n];
}
void DISCARD(Deck *D, int k, Deck *G)
{
	G->a[G->n++] = D->a[k];
	D->a[k] = D->a[--D->n];
}
void USE(Deck *D, int use, Deck *G)
{
	for (int i = 0; i < D->n; i++)
		if (D->a[i].type == use)
		{
			DISCARD(D, i, G);
			return;
		}
}

//EFFECT
void Reveal(Player *X, Game *G)
{
	//	Player *Now = X->Nxt;
	if (X->ID)
		return;
	int cnt = 1;
	forP(X) if (Now->Rvld == 1 && Now->ctry == X->ctry)
		cnt++;
	X->Rvld = 1;
	X->ID = 2 * cnt > G->N ? 1 : 2;
}
int ASK(Player *X, int y)
{
	for (int i = 0; i < X->Hnd.n; i++)
		if (X->Hnd.a[i].type == y)
			if (X->Bot)
				return 1;
			else
			{
				int opt;
				printf("Use a DODGE?");
				scanf("%d", &opt);
				return opt;
			}

	return 0;
}
int HELP(Player *Y, Game *G)
{
	if (Y->Bot == 1)
	{
		if (ASK(Y, 4))
			return USE(&Y->Hnd, 4, &G->Dscd), 1;
		else if (ASK(Y, 3))
			return USE(&Y->Hnd, 3, &G->Dscd), 1;
	}
	forP(Y)
	{
		if (Now->Bot == 1)
		{
			if (Now->Rvld == 1 && Now->ctry == Y->ctry)
				if (ASK(Now, 2))
					return USE(&Now->Hnd, 2, &G->Dscd), 1;
		}
		else
		{
			if (ASK(Now, 2))
				return USE(&Now->Hnd, 2, &G->Dscd), 1;
		}
	}
	return 0;
}
void DISABLE(Player *Y, Game *G)
{
	printf("%d GG\n", Y->Nb);
	forP(Y)
	{
		if (Now->Nxt->Nb == Y->Nb)
		{
			Now->Nxt = Y->Nxt;
			break;
		}
	}
	//	free(Y);
	G->n--;
}
void calcDamage(Player *Y, int Dam, Game *G)
{
	Y->hlt -= Dam;
	while (Y->hlt <= 0)
	{
		if (HELP(Y, G))
			Y->hlt++;
		else
			break;
	}
	if (Y->hlt <= 0)
		DISABLE(Y, G);
}
void STRIKE(Player *X, Player *Y, Game *G)
{
	printf("%d STRIKES %d\n", X->Nb, Y->Nb);
	USE(&X->Hnd, 1, &G->Dscd);
	if (ASK(Y, 2))
		printf("%d DODGES\n", Y->Nb),
			USE(&Y->Hnd, 2, &G->Dscd);
	else
		calcDamage(Y, 1, G),
			printf("%d->hlt=%d\n", Y->Nb, Y->hlt);
}
void WINE_STRIKE(Player *X, Player *Y, Game *G)
{
	printf("%d STRIKES %d\n", X->Nb, Y->Nb);
	USE(&X->Hnd, 4, &G->Dscd);
	USE(&X->Hnd, 1, &G->Dscd);
	if (ASK(Y, 2))
		printf("%d DODGES\n", Y->Nb),
			USE(&Y->Hnd, 2, &G->Dscd);
	else
		calcDamage(Y, 2, G),
			printf("%d->hlt=%d\n", Y->Nb, Y->hlt);
}
void PEACH(Player *X, Game *G)
{
	printf("%d PEACHES\n", X->Nb);
	USE(&X->Hnd, 3, &G->Dscd);
	X->hlt = min(X->hlt + 1, X->hltLv);
	printf("%d->hlt=%d\n", X->Nb, X->hlt);
}

//CONTROL
void fateState(Player *X, Game *G)
{
	for (int i = 0; i < X->Ftz.n; i++)
		;
}
void drawState(Player *X, Game *G)
{
	DRAW(&X->Hnd, &G->Main, &G->Dscd);
	DRAW(&X->Hnd, &G->Main, &G->Dscd);
}
Player *getFromNb(Player *X, int id)
{
	if (X->Nb == id)
		return X;
	forP(X) if (Now->Nb == id) return Now;
	return NULL;
}
int calcDist(Player *X, Player *Y)
{
	int d1 = 0, d2 = 0;
	for (Player *Now = X; Now->Nb != Y->Nb; Now = Now->Nxt)
		d1++;
	for (Player *Now = Y; Now->Nb != X->Nb; Now = Now->Nxt)
		d2++;
	return min(d1, d2);
}
void playState(Player *X, Game *G)
{
	if (X->Bot)
	{
		Reveal(X, G);
		for (int i = 0, cnt; i < X->Hnd.n; i++)
		{
			//	Player *Now;
			switch (X->Hnd.a[i].type)
			{
			case 1:
				if (cnt)
					break;

				forP(X)
				{
					int dist = calcDist(X, Now);
					if (dist <= 1)
						if (Now->ctry == X->ctry && Now->ID == 2 && X->ID == 2)
							;
						else
						{
							cnt++;
							STRIKE(X, Now, G);
							break;
						}
				}
				break;
			case 2:
				break;

			case 3:
				if (X->hlt < X->hltLv)
					PEACH(X, G);
				break;

			case 4:

				break;
			default:
				break;
			}
		}
	}
	else
	{
		for (int x, y, z, cnt = 0;!Terminal(X);)
		{
			system("cls");
			printGame(X);
			puts("Commence Your Performance");
			scanf("%d", &x);
			if (x < -1 || x >= X->Hnd.n)
				break;
			if (x == -1)
				Reveal(X, G);
			Player *Target;
			switch (X->Hnd.a[x].type)
			{
			case 4:
				scanf("%d", &y);
				if (X->Hnd.a[y].type != 1)
					break;
				scanf("%d", &z);
				//	if (cnt)break;
				Target = getFromNb(X, z);
				if (Target == NULL)
					break;
				if (calcDist(X, Target) <= 1)
					cnt++, WINE_STRIKE(X, Target, G);
				break;
			case 1:
				scanf("%d", &z);
				//	if (cnt)break;
				Target = getFromNb(X, z);
				if (Target == NULL)
					break;
				if (calcDist(X, Target) <= 1)
					cnt++, STRIKE(X, Target, G);
				break;
			case 3:
				PEACH(X, G);
			default:
				break;
			}
		}
	}
}
void discardState(Player *X, Game *G)
{
	while (X->Hnd.n > X->hlt)
		DISCARD(&X->Hnd, rd(0, X->Hnd.n - 1), &G->Dscd);
}
int Terminal(Player *X)
{
	int A[MAXN], tot = 0;
	A[tot++] = X->ctry;
	if (X->Rvld == 0)
		return 0;
	forP(X)
	{
		A[tot++] = Now->ctry;
		if (Now->Rvld == 0)
			return 0;
	}
	if (tot == 1)
		return 1;
	for (int i = 0; i < tot; i++)
		if (A[i] != A[1])
			return 0;
	return 1;
}
void printDeck(Deck *D)
{
	printf("%d: ", D->n);
	for (int i = 0; i < D->n; i++)
		printf("%d ", D->a[i].type);
	puts("");
}
void printPlayer(Player *X)
{
	printf("Player %d\n", X->Nb);
	printf("Health %d/%d\n", X->hlt, X->hltLv);
		
#ifdef TRAINING
	puts(X->Rvld ? "Revealed" : "Unrevealed");
	printf("Identity with Country %d %d\n", X->ID, X->ctry);
	printf("Hand Deck ");
	printDeck(&X->Hnd);
#elif
	puts(X->Rvld ? "Revealed" : "Unrevealed");
	if (X->Rvld || !X->Bot)
		printf("Identity with Country %d %d\n", X->ID, X->ctry);
#endif
	printf("Fate Deck ");
	printDeck(&X->Ftz);
	printf("Equipment Deck ");
	printDeck(&X->Eqp);
	puts("\n");
}
void printGame(Player *X)
{
	printPlayer(X);
	forP(X)
		printPlayer(Now);
}
void Perform(Game *G, Player *Head)
{
	Player *Now = Head;
	for (; !Terminal(Now); Now = Now->Nxt)
	{
		printf("\n***Round*** Alive:%d\n", G->n);

		fateState(Now, G);
		drawState(Now, G);
		printPlayer(Now);
		playState(Now, G);
		discardState(Now, G);
	}
	free(G);
}
Game *initGame(int n, int argc, char *file)
{
	Game *G = (Game *)malloc(sizeof(Game));
	G->N = n;
	G->n = n;
	createDeck(&G->Main, argc, file);
	clearDeck(&G->Dscd);
	return G;
}

void initPlayer(Player *P, int i, Game *G)
{
	P->Nb = i;
	P->Bot = i == 2 ? 0 : 1;
	P->hltLv = rd(3, 5);
	P->hlt = P->hltLv;
	P->ID = 0;
	P->ctry = rd(1, 4);
	P->Rvld = 0;
	clearDeck(&P->Hnd);
	clearDeck(&P->Eqp);
	clearDeck(&P->Ftz);
	for (int i = 1; i <= 5; i++)
		DRAW(&P->Hnd, &G->Main, &G->Dscd);
}
Player *initPlayers(int n, Game *G)
{
	Player *Head = (Player *)malloc(sizeof(Player)), *Now = Head;
	for (int i = 1; i < n; i++)
	{
		initPlayer(Now, i, G);
		Now->Nxt = (Player *)malloc(sizeof(Player));
		Now = Now->Nxt;
	}
	initPlayer(Now, n, G);
	Now->Nxt = Head;
	return Head;
}

int min(int a, int b)
{
	return a < b ? a : b;
}

void shuffle(Deck *D)
{
	for (int i = 1; i < D->n; i++)
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
	if (argc == 3)
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
		for (int i = D->n; i < D->n + num; i++)
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
	for (int i = 0; i < D->n; i++)
		fprintf(stderr, "%d\n", D->a[i].type);
	fclose(fp);
}
void clearDeck(Deck *D)
{
	memset(D, 0, sizeof(Deck));
}