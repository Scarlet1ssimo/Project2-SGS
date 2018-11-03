#include "SGS.h"
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