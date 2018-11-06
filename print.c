#include "SGS.h"

void _MESSAGE(const char S[])
{
	static char LOG[MAXN][MAXN];
	static int N = 0;

#ifdef _WIN32
	if (N < 5)
	{
	//	mvprintw()

	}
		

#endif
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