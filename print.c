#include "SGS.h"

#ifdef linux
int MSG(Panel *P, const char *fmt, ...)
{
	char buf[MAXL];
	va_list args;
	int printed;
	va_start(args, fmt);
	printed = vsprintf(buf, fmt, args);
	va_end(args);
	_MESSAGE(P, buf);
	SHOW(P);
	return printed;
}
void _MESSAGE(Panel *P, char S[])
{
	int pre = (P->n + P->N - 1) % P->N, L = strlen(P->LOGS[pre]);
	if (P->LOGS[pre][L - 1] != 32)
		memcpy(P->LOGS[P->n % P->N], S, MAXL * sizeof(char)), P->n++;
	else
		memcpy(P->LOGS[pre] + L, S, (MAXL - L) * sizeof(char));
}
void SHOW(Panel *P)
{
	wclear(P->W);
	wmove(P->W, 0, 0);
	wborder(P->W, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(P->W);
	if (P->n < P->N)
		for (int i = 0; i < P->n; i++)
			wprintw(P->W, "%s", P->LOGS[i]);
	else
	{
		for (int i = P->n % P->N; i < P->N; i++)
			wprintw(P->W, "%s", P->LOGS[i]);
		for (int i = 0; i < P->n % P->N; i++)
			wprintw(P->W, "%s", P->LOGS[i]);
	}
	wrefresh(P->W);
}
void _pD(Deck *D, Game *G)
{
	MSG(&G->IF, "%d: ", D->n);
	char Nm[][25] = {
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
		"Horse",
		"Ease"};
	for (int i = 0; i < D->n; i++)
		MSG(&G->IF, "%d%s ", i, Nm[D->a[i].type]);
	MSG(&G->IF, "\n");
	refresh();
}
void printDeck(Deck *D, Game *G)
{
	MSG(&G->GM, "%d: ", D->n);
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
		"Horse",
		"Ease"};
	for (int i = 0; i < D->n; i++)
		MSG(&G->GM, "%d%s ", i, Nm[D->a[i].type]);
	MSG(&G->GM, "\n");
	refresh();
}
void printPlayer(Player *X, Game *G)
{
	MSG(&G->GM, "Player %d     Health %d/%d\n", X->Nb, X->hlt, X->hltLv);
	//	MSG(&G->GM,"Skip: %d\n", X->Skip);

	if (X->Rvld)
	{
		if (X->ID == 2)
			MSG(&G->GM, "Country %d's General\n", X->ctry);
		else
			MSG(&G->GM, "Mercenary\n");
	}
	else
		MSG(&G->GM, "Unrevealed\n");
	if (X->Hnd.n)
	{
		MSG(&G->GM, "Hand Deck ");
#ifdef DEMO
		printDeck(&X->Hnd, G);
#else
		MSG(&G->GM, "%d\n", X->Hnd.n);
#endif
	}

	if (X->Ftz.n)
		MSG(&G->GM, "Fate Deck "), printDeck(&X->Ftz, G);
	if (X->Eqp.n)
		MSG(&G->GM, "Equipment Deck "), printDeck(&X->Eqp, G);
	MSG(&G->GM, "\n");
	refresh();
}
void printGame(Player *X, Game *G)
{
	//	MSG(&G->GM,"\n#########Print Game########\n");
	while (X->hlt == 0)
		X = X->Nxt;
	printPlayer(X, G);
	forP(X)
		printPlayer(Now, G);
	refresh();
}
#endif
#ifndef linux
		void _pD(Deck * D, Game * G)
		{
			G->n = G->n;
			printf("%d: ", D->n);
			char Nm[][25] = {
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
				"Horse",
				"Ease"};
			for (int i = 0; i < D->n; i++)
				printf("%d%s ", i, Nm[D->a[i].type]);
			puts("");
		}
		void printDeck(Deck * D, Game * G)
		{
			G->n = G->n;
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
				"Horse",
				"Ease"};
			for (int i = 0; i < D->n; i++)
				printf("%d%s ", i, Nm[D->a[i].type]);
			puts("");
		}
		void printPlayer(Player * X, Game * G)
		{
			printf("Player %d     Health %d/%d\n", X->Nb, X->hlt, X->hltLv);
			if (X->Rvld)
			{
				if (X->ID == 2)
					printf("Country %d's General\n", X->ctry);
				else
					printf("Mercenary\n");
			}
			else
				printf("Unrevealed\n");
			if (X->Hnd.n)
			{
				printf("Hand Deck ");
#ifdef DEMO
				printDeck(&X->Hnd, G);
#else
				printf("%d\n", X->Hnd.n);
#endif
			}
			if (X->Ftz.n)
				printf("Fate Deck "), printDeck(&X->Ftz, G);
			if (X->Eqp.n)
				printf("Equipment Deck "), printDeck(&X->Eqp, G);
			printf("\n");
		}
		void printGame(Player * X, Game * G)
		{
			puts("\n#########Print Game########\n");
			while (X->hlt == 0)
				X = X->Nxt;
			printPlayer(X, G);
			forP(X)
				printPlayer(Now, G);
		}
#endif
