#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAXN 200
#define DEBUG 1
#define TRAINING
#define HUMAN 2
#define _FILE 0
#define forP(P) for (Player *Now = P->Nxt; Now->Nb != P->Nb; Now = Now->Nxt)
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
void printPlayer(Player *X);
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
#ifdef _FILE
//	freopen("error.log", "w", stdout);
#endif
	long _233 = time(NULL);
	//	long _233 = 1540994424l;
	fprintf(stderr, "srand: %ld\n", _233);
	/*
		1540992487:endless
	*/
	srand(_233);
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
Player *getFromNb(Player *X, int id)
{
	if (X->Nb == id)
		return X;
	forP(X) if (Now->Nb == id) return Now;
	return NULL;
}

//EFFECT
int OPP(Player *X, Player *Y)
{
	if (X->ctry == Y->ctry && X->ID == 2 && Y->ID == 2)
		return 0;
	return 1;
}
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
		{
			if (X->Bot)
				return 1;
			else
			{
				int opt;
				if (y == 2)
					printf("Use a DODGE?\n");
				if (y == 1)
					printf("Use a STRIKE?\n");
				printPlayer(X);
				scanf("%d", &opt);
				return opt;
			}
		}
	return 0;
}
int _ASK(Deck *D, int y)
{
	for (int i = 0; i < D->n; i++)
		if (D->a[i].type == y)
			return 1;
	return 0;
}
int __ASK(Deck *D, int y)
{
	for (int i = 0; i < D->n; i++)
		if (D->a[i].type == y)
			return i;
	return -1;
}
int calcDist(Player *X, Player *Y)
{
	if (X->Nb == Y->Nb)
		return 0;
	int d1 = 0, d2 = 0;
	for (Player *Now = X; Now->Nb != Y->Nb; Now = Now->Nxt)
		d1++;
	for (Player *Now = Y; Now->Nb != X->Nb; Now = Now->Nxt)
		d2++;
	return min(1, min(d1, d2) - _ASK(&X->Eqp, 17) - _ASK(&X->Eqp, 18) * 2 + _ASK(&Y->Eqp, 19));
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
	X->hlt = min(X->hlt + 1, X->hltLv);
	printf("%d->hlt=%d\n", X->Nb, X->hlt);
}
void DUEL(Player *X, Player *Y, Game *G)
{
	printf("%d DUELS %d\n", X->Nb, Y->Nb);
	if (ASK(Y, 1))
	{
		USE(&Y->Hnd, 1, &G->Dscd);
		DUEL(Y, X, G);
	}
	else
	{
		calcDamage(Y, 1, G);
		printf("%d->hlt=%d\n", Y->Nb, Y->hlt);
	}
}
void DISMANTLE(Player *X, Player *Y, Game *G)
{
	printf("%d DISMANTLES %d\n", X->Nb, Y->Nb);
	if (X->Bot)
	{
		DISCARD(&Y->Hnd, rd(0, Y->Hnd.n - 1), &G->Dscd);
	}
	else
	{
		int f;
		printf("DISMANTLE:\n");
		scanf("%d", &f);
		while (f < 0 && Y->Hnd.n <= f)
			scanf("%d", &f);
		DISCARD(&Y->Hnd, f, &G->Dscd);
	}
}
void _SNATCH(Player *X, Deck *Y)
{
	if (X->Bot)
	{
		DISCARD(Y, rd(0, Y->n - 1), &X->Hnd);
	}
	else
	{
		int f;
		printf("GET:\n");
		scanf("%d", &f);
		while (f < 0 && Y->n <= f)
			scanf("%d", &f);
		DISCARD(Y, f, &X->Hnd);
	}
}
void SNATCH(Player *X, Player *Y, Game *G)
{
	printf("%d SNATCHES %d\n", X->Nb, Y->Nb);
	if (X->Bot)
	{
		DISCARD(&Y->Hnd, rd(0, Y->Hnd.n - 1), &X->Hnd);
	}
	else
	{
		int f;
		printf("SNATCH:\n");
		scanf("%d", &f);
		while (f < 0 && Y->Hnd.n <= f)
			scanf("%d", &f);
		DISCARD(&Y->Hnd, f, &X->Hnd);
	}
}
Card FATE(Game *G)
{
	Card c = G->Main.a[G->Main.n - 1];
	DISCARD(&G->Main, G->Main.n - 1, &G->Dscd);
	return c;
}
void FATE_LIGHTING(Player *X, Game *G, int i)
{
	Card C = FATE(G);
	if (C.col == 4 && 2 <= C.num && C.num <= 9)
		puts("LIGHTING: YES!!!"),
			DISCARD(&X->Ftz, i, &G->Dscd), calcDamage(X, 3, G);
	else
		puts("LIGHTING: No"),
			DISCARD(&X->Ftz, i, &X->Ftz);
}
void FATE_DROWNINHAPPINESS(Player *X, Game *G, int i)
{
	Card C = FATE(G);
	DISCARD(&X->Ftz, i, &G->Dscd);
	if (C.col != 3)
		puts("DROWN IN HAPPINESS: YES!!!"), X->Skip |= 1;
	else
		puts("DROWN IN HAPPINESS: No");
}
void FATE_STARVATION(Player *X, Game *G, int i)
{
	Card C = FATE(G);
	DISCARD(&X->Ftz, i, &G->Dscd);
	if (C.col != 1)
		puts("STARVATION: YES!!!"), X->Skip |= 2;
	else
		puts("STARVATION: No");
}
void ARROWBARRAGE(Player *X, Game *G)
{
	printf("%d ARROWBAARAGES\n", X->Nb);
	forP(X)
	{
		if (ASK(Now, 2))
		{
			USE(&Now->Hnd, 2, &G->Dscd);
		}
		else
		{
			calcDamage(Now, 1, G);
			printf("%d->hlt=%d\n", Now->Nb, Now->hlt);
		}
	}
}
void INVASION(Player *X, Game *G)
{
	printf("%d INVADES\n", X->Nb);
	forP(X)
	{
		if (ASK(Now, 1))
		{
			USE(&Now->Hnd, 1, &G->Dscd);
		}
		else
		{
			calcDamage(Now, 1, G);
			printf("%d->hlt=%d\n", Now->Nb, Now->hlt);
		}
	}
}
void HARVEST(Player *X, Game *G)
{
	printf("%d HARVESTS\n", X->Nb);
	Deck BTF;
	clearDeck(&BTF);
	for (int i = 0; i < G->n; i++)
		DRAW(&BTF, &G->Main, &G->Dscd);
	_SNATCH(X, &BTF);
	forP(X)
		_SNATCH(Now, &BTF);
}
int BORROWEDSWORD(Player *Y, Game *G)
{
	if (Y->Bot)
	{
		if (_ASK(&Y->Hnd, 1))
		{
			forP(Y)
			{
				int dis = calcDist(Y, Now);
				if (dis <= 1 && OPP(Now, Y))
				{
					USE(&Y->Hnd, 1, &G->Dscd);
					STRIKE(Y, Now, G);
					return 1;
				}
			}
			return 0;
		}
		else
			return 0;
	}
	else
	{
		if (_ASK(&Y->Hnd, 1))
		{
			int x, z;
			scanf("%d", &x);
			if (Y->Hnd.a[x].type != 1)
				return 0;
			scanf("%d", &z);
			Player *Target = getFromNb(Y, z);
			if (Target == NULL)
				return 0;
			DISCARD(&Y->Hnd, x, &G->Dscd);
			STRIKE(Y, Target, G);
			return 1;
		}
		else
			return 0;
	}
}

//CONTROL
void fateState(Player *X, Game *G)
{
	for (int i = X->Ftz.n - 1; i >= 0; i--)
	{
		if (X->Ftz.a[i].type == 14)
			FATE_LIGHTING(X, G, i);
		if (X->Ftz.a[i].type == 15)
			FATE_DROWNINHAPPINESS(X, G, i);
		if (X->Ftz.a[i].type == 16)
			FATE_STARVATION(X, G, i);
	}
}
void drawState(Player *X, Game *G)
{
	DRAW(&X->Hnd, &G->Main, &G->Dscd);
	DRAW(&X->Hnd, &G->Main, &G->Dscd);
}
#define CLS system("cls")
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
						if (OPP(X, Now))
						{
							cnt++;
							DISCARD(&X->Hnd, i, &G->Dscd);
							STRIKE(X, Now, G);
							break;
						}
				}
				break;
			case 2:
				break;

			case 3:
				if (X->hlt < X->hltLv)
					DISCARD(&X->Hnd, i, &G->Dscd);
				PEACH(X, G);
				break;

			case 4:

				break;
			case 5:
				forP(X)
				{
					if (OPP(X, Now))
						DISCARD(&X->Hnd, i, &G->Dscd);
					DUEL(X, Now, G);
					break;
				}
				break;
			case 6:
				forP(X)
				{
					if (OPP(X, Now) && Now->Hnd.n)
					{
						DISCARD(&X->Hnd, i, &G->Dscd);
						DISMANTLE(X, Now, G);
						break;
					}
				}
				break;
			case 7:
				forP(X)
				{
					int dis = calcDist(X, Now);
					if (OPP(X, Now) && Now->Hnd.n && dis <= 1)
					{
						DISCARD(&X->Hnd, i, &G->Dscd);
						SNATCH(X, Now, G);
						break;
					}
				}
				break;
			case 8:
				forP(X) if (OPP(Now, X) && _ASK(&Now->Eqp, 18))
				{
					printf("%d BORRORWS %d’s SWORD\n", X->Nb, Now->Nb);
					DISCARD(&X->Hnd, i, &G->Dscd);
					if (!BORROWEDSWORD(Now, G))
						puts("届不到"), USE(&Now->Eqp, 18, &X->Hnd);
					break;
				}
				break;
			case 9:
				DISCARD(&X->Hnd, i, &G->Dscd);
				ARROWBARRAGE(X, G);
				break;
			case 10:
				DISCARD(&X->Hnd, i, &G->Dscd);
				INVASION(X, G);
				break;
			case 11:
				printf("%d PEACH GARDEN\n", X->Nb);
				DISCARD(&X->Hnd, i, &G->Dscd);
				PEACH(X, G);
				forP(X)
					PEACH(Now, G);
				break;
			case 12:
				DISCARD(&X->Hnd, i, &G->Dscd);
				printf("%d SOMETHING FOR NOTHING\n", X->Nb);
				DRAW(&X->Hnd, &G->Main, &G->Dscd);
				DRAW(&X->Hnd, &G->Main, &G->Dscd);
				break;
			case 13:
				DISCARD(&X->Hnd, i, &G->Dscd);
				HARVEST(X, G);
				break;

			case 14:
				printf("%d LIGHTING\n", X->Nb);
				DISCARD(&X->Hnd, i, &X->Ftz);
				break;
			case 15:
				forP(X)
				{
					if (OPP(X, Now) && Now->Hnd.n)
					{
						printf("%d MAKES %d DROWN IN HAPPINESS\n", X->Nb, Now->Nb);
						DISCARD(&X->Hnd, i, &Now->Ftz);
						break;
					}
				}
				break;
			case 16:
				forP(X)
				{
					int dis = calcDist(X, Now);
					if (OPP(X, Now) && dis <= 1)
					{
						printf("%d MAKES %d STARVE\n", X->Nb, Now->Nb);
						DISCARD(&X->Hnd, i, &Now->Ftz);
						break;
					}
				}
				break;
			case 17:
				printf("%d EQUIPPED BINOCULARS\n", X->Nb);
				if (_ASK(&X->Eqp, 17))
					DISCARD(&X->Hnd, i, &G->Dscd);
				else
					DISCARD(&X->Hnd, i, &X->Eqp);
				break;
			case 18:
				printf("%d EQUIPPED BOW\n", X->Nb);
				if (_ASK(&X->Eqp, 18))
					DISCARD(&X->Hnd, i, &G->Dscd);
				else
					DISCARD(&X->Hnd, i, &X->Eqp);
				break;
			case 19:
				printf("%d EQUIPPED HORSE\n", X->Nb);
				if (_ASK(&X->Eqp, 19))
					DISCARD(&X->Hnd, i, &G->Dscd);
				else
					DISCARD(&X->Hnd, i, &X->Eqp);
				break;
			}
		}
		printGame(X);
		system("pause");
		system("cls");
	}
	else
	{
		for (int x, y, z, cnt = 0; !Terminal(X);)
		{
			CLS;
			printGame(X);
			puts("Commence Your Performance");
			puts("Card Number(0 to n-1)");
			scanf("%d", &x);
			if (x < -1 || x >= X->Hnd.n)
				break;
			if (x == -1)
			{
				Reveal(X, G);
				continue;
			}

			Player *Target;
			switch (X->Hnd.a[x].type)
			{
			case 1:
				scanf("%d", &z);
				if (cnt)
					break;
				Target = getFromNb(X, z);
				if (Target == NULL)
					break;
				if (calcDist(X, Target) <= 1)
					cnt++, DISCARD(&X->Hnd, x, &G->Dscd), CLS,STRIKE(X, Target, G);
				break;
			case 2:
				break;
			case 3:
				DISCARD(&X->Hnd, x, &G->Dscd);
				CLS;
				PEACH(X, G);
				break;
			case 4:
				scanf("%d", &y);
				if (X->Hnd.a[y].type != 1)
					break;
				scanf("%d", &z);
				if (cnt)
					break;
				Target = getFromNb(X, z);
				if (Target == NULL)
					break;
				if (calcDist(X, Target) <= 1)
					cnt++, DISCARD(&X->Hnd, x, &G->Dscd), DISCARD(&X->Hnd, y, &G->Dscd),
						WINE_STRIKE(X, Target, G);
				break;
			case 5:
				scanf("%d", &z);
				Target = getFromNb(X, z);
				if (Target == NULL)
					break;
				DISCARD(&X->Hnd, x, &G->Dscd);
				DUEL(X, Target, G);
				break;
			case 6:
				scanf("%d", &z);
				Target = getFromNb(X, z);
				if (Target == NULL)
					break;
				DISCARD(&X->Hnd, x, &G->Dscd);
				DISMANTLE(X, Target, G);
				break;
			case 7:
				scanf("%d", &z);
				Target = getFromNb(X, z);
				if (Target == NULL)
					break;
				if (calcDist(X, Target) <= 1)
					DISCARD(&X->Hnd, x, &G->Dscd), SNATCH(X, Target, G);
				break;
			case 8:
				scanf("%d", &z);
				Target = getFromNb(X, z);
				if (Target == NULL)
					break;
				if (_ASK(&Target->Eqp, 18))
				{
					printf("%d BORRORWS %d's SWORD\n", X->Nb, Target->Nb);
					DISCARD(&X->Hnd, x, &G->Dscd);
					if (!BORROWEDSWORD(Target, G))
						puts("届不到"), USE(&Target->Eqp, 18, &X->Hnd);
				}
				break;
			case 9:
				DISCARD(&X->Hnd, x, &G->Dscd);
				ARROWBARRAGE(X, G);
				break;
			case 10:
				DISCARD(&X->Hnd, x, &G->Dscd);
				INVASION(X, G);
				break;
			case 11:
				printf("%d PEACH GARDEN\n", X->Nb);
				DISCARD(&X->Hnd, x, &G->Dscd);
				PEACH(X, G);
				forP(X)
					PEACH(Now, G);
				break;
			case 12:
				DISCARD(&X->Hnd, x, &G->Dscd);
				printf("%d SOMETHING FOR NOTHING\n", X->Nb);
				DRAW(&X->Hnd, &G->Main, &G->Dscd);
				DRAW(&X->Hnd, &G->Main, &G->Dscd);
				break;
			case 13:
				DISCARD(&X->Hnd, x, &G->Dscd);
				HARVEST(X, G);
				break;
			case 14:
				printf("%d LIGHTING\n", X->Nb);
				DISCARD(&X->Hnd, x, &X->Ftz);
				break;
			case 15:
				scanf("%d", &z);
				Target = getFromNb(X, z);
				if (Target == NULL)
					break;
				if (!Target->Hnd.n)
					break;
				printf("%d MAKES %d DROWN IN HAPPINESS\n", X->Nb, Target->Nb);
				DISCARD(&X->Hnd, x, &Target->Ftz);
				break;
			case 16:
				scanf("%d", &z);
				Target = getFromNb(X, z);
				if (Target == NULL)
					break;
				if (!Target->Hnd.n)
					break;
				if (calcDist(X, Target) <= 1)
				{
					printf("%d MAKES %d STARVE\n", X->Nb, Target->Nb);
					DISCARD(&X->Hnd, x, &Target->Ftz);
				}
			case 17:
				printf("%d EQUIPPED BINOCULARS\n", X->Nb);
				if (_ASK(&X->Eqp, 17))
					DISCARD(&X->Hnd, x, &G->Dscd);
				else
					DISCARD(&X->Hnd, x, &X->Eqp);
				break;
			case 18:
				printf("%d EQUIPPED BOW\n", X->Nb);
				if (_ASK(&X->Eqp, 18))
					DISCARD(&X->Hnd, x, &G->Dscd);
				else
					DISCARD(&X->Hnd, x, &X->Eqp);
				break;
			case 19:
				printf("%d EQUIPPED HORSE\n", X->Nb);
				if (_ASK(&X->Eqp, 19))
					DISCARD(&X->Hnd, x, &G->Dscd);
				else
					DISCARD(&X->Hnd, x, &X->Eqp);
				break;

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
	char Nm[][20] = {
		"NULL",
		"Strike",
		"Dodge",
		"Peach",
		"Wine",
		"Duel",
		"Dismantle",
		"Snatch",
		"Borrowed",
		"Arrow",
		"Invasion",
		"Garden",
		"Something",
		"Harvest",
		"Lighting",
		"Happiness",
		"Starvation",
		"Binoculars",
		"Bow",
		"Horse"};
	for (int i = 0; i < D->n; i++)
		printf("%d%s ", i, Nm[D->a[i].type]);
	puts("");
}
void printPlayer(Player *X)
{
	printf("Player %d\n", X->Nb);
	printf("Health %d/%d\n", X->hlt, X->hltLv);
	printf("Skip: %d\n", X->Skip);

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
	puts("\n#########Print Game########\n");
	while (X->hlt == 0)
		X = X->Nxt;
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
		if (Now->Skip & 2)
			Now->Skip ^= 2;
		else
			drawState(Now, G);
		printPlayer(Now);
		if (Now->Skip & 1)
			Now->Skip ^= 1;
		else
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
	P->Bot = i == HUMAN ? 0 : 1;
	P->hltLv = rd(3, 5);
	P->hlt = P->hltLv;
	P->ID = 0;
	P->ctry = rd(1, 4);
	P->Rvld = 0;
	P->Skip = 0;
	clearDeck(&P->Hnd);
	clearDeck(&P->Eqp);
	clearDeck(&P->Ftz);
	for (int i = 1; i <= 4; i++)
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
			if (!strcmp(S, "Duel"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 5};
			if (!strcmp(S, "Dismantle"))
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
			if (!strcmp(S, "Binoculars"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 17};
			if (!strcmp(S, "Bow"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 18};
			if (!strcmp(S, "Horse"))
				D->a[i] = (Card){rd(1, 13), rd(1, 4), 19};
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