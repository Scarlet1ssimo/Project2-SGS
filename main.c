#include "SGS.h"

int main(int argc, char **argv)
{
#ifdef linux
	initscr();
	refresh();
	noecho();
	cbreak();
#endif

	freopen("err.log", "w", stderr);
	long _233 = time(NULL);
	fprintf(stderr, "srand: %ld\n", _233);
	fclose(stderr);
	srand(_233);
	int n = rd(5, PLAYERS_NUMBER);
	Game *G = initGame(n, argc, argv[2]);
	Player *Head = initPlayers(n, G);
	Perform(G, Head);

#ifdef linux
	getchar();
	refresh();
	endwin();
#endif
#ifdef _WIN32
	system("pause");
#endif
#ifdef __APPLE__
	PAUSE;
#endif
	return 0;
}

void fateState(Player *X, Game *G)
{
	for (int i = X->Ftz.n - 1; i >= 0; i--)
	{
		if (X->hlt <= 0)
			return;
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
#ifdef linux
void playState(Player *X, Game *G)
{
	if (X->Bot)
	{
		Reveal(X, G);
		for (int i = 0, cnt; i < X->Hnd.n; i++)
		{
			if (X->hlt <= 0)
				break;
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
					DISCARD(&X->Hnd, i, &G->Dscd),
						PEACH(X, G);
				break;

			case 4:

				break;
			case 5:
				forP(X)
				{
					if (OPP(X, Now))
						DISCARD(&X->Hnd, i, &G->Dscd),
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
				} //		SHOW(&G->GM);SHOW(&G->IF);

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
					MSG(&G->IF, "%d BORRORWS %d''s SWORD\n", X->Nb, Now->Nb);
					DISCARD(&X->Hnd, i, &G->Dscd);
					if (!BORROWEDSWORD(Now, G))
						MSG(&G->IF, "GTMD\n"), USE(&Now->Eqp, 18, &X->Hnd);
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
				MSG(&G->IF, "%d PEACH GARDEN\n", X->Nb);
				DISCARD(&X->Hnd, i, &G->Dscd);
				PEACH(X, G);
				forP(X)
					PEACH(Now, G);
				break;
			case 12:
				DISCARD(&X->Hnd, i, &G->Dscd);
				MSG(&G->IF, "%d SOMETHING FOR NOTHING\n", X->Nb);
				DRAW(&X->Hnd, &G->Main, &G->Dscd);
				DRAW(&X->Hnd, &G->Main, &G->Dscd);
				break;
			case 13:
				DISCARD(&X->Hnd, i, &G->Dscd);
				HARVEST(X, G);
				break;

			case 14:
				MSG(&G->IF, "%d LIGHTING\n", X->Nb);
				DISCARD(&X->Hnd, i, &X->Ftz);
				break;
			case 15:
				forP(X)
				{
					if (OPP(X, Now) && Now->Hnd.n)
					{
						MSG(&G->IF, "%d MAKES %d DROWN IN HAPPINESS\n", X->Nb, Now->Nb);
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
						MSG(&G->IF, "%d MAKES %d STARVE\n", X->Nb, Now->Nb);
						DISCARD(&X->Hnd, i, &Now->Ftz);
						break;
					}
				}
				break;
			case 17:
				MSG(&G->IF, "%d EQUIPPED BINOCULARS\n", X->Nb);
				if (_ASK(&X->Eqp, 17))
					DISCARD(&X->Hnd, i, &G->Dscd);
				else
					DISCARD(&X->Hnd, i, &X->Eqp);
				break;
			case 18:
				MSG(&G->IF, "%d EQUIPPED BOW\n", X->Nb);
				if (_ASK(&X->Eqp, 18))
					DISCARD(&X->Hnd, i, &G->Dscd);
				else
					DISCARD(&X->Hnd, i, &X->Eqp);
				break;
			case 19:
				MSG(&G->IF, "%d EQUIPPED HORSE\n", X->Nb);
				if (_ASK(&X->Eqp, 19))
					DISCARD(&X->Hnd, i, &G->Dscd);
				else
					DISCARD(&X->Hnd, i, &X->Eqp);
				break;
			}
		}
		printGame(X, G);
#ifdef linux
		SHOW(&G->GM);
		SHOW(&G->IF);
		MSG(&G->IF, "Press Any Key..\n");
		getch();
#endif
	}
	else
	{
		MSG(&G->IF, "Commence Your Performance\n");
		MSG(&G->IF, "Card Number(0 to n-1)\n");
		MSG(&G->IF, "R to Reveal\n");
		MSG(&G->IF, "G to Break\n");

		for (int x, y, z, cnt = 0; !Terminal(X, G);)
		{
			cP(&G->GM);
			printGame(X, G);
			MSG(&G->GM, "Your Hand Deck");
			printDeck(&X->Hnd, G);
			SHOW(&G->IF), SHOW(&G->GM);
			//			move(LINES,0);
			char ch = getch();
			if (ch == 'G' || ch == 'g')
				break;
			if (ch == 'R' || ch == 'r')
			{
				Reveal(X, G);
				continue;
			}
			if (ch < '0' || '9' < ch)
			{
				MSG(&G->IF, "Undefined Behavior!\n");
				continue;
			}
			Player *Target;
			x = ch - 48;
			if (x < 0 || X->Hnd.n <= x)
			{
				MSG(&G->IF, "Undefined Behavior!\n");
				continue;
			}
			switch (X->Hnd.a[x].type)
			{
			case 1:
				MSG(&G->IF, "You chose a STRIKE, \n");
				if (cnt)
				{
					MSG(&G->IF, "but WINE-STRIKE or STRIKE was used this round!\n");
					break;
				}
				MSG(&G->IF, "then choose your target ID\n");
				z = rdn();
				Target = getFromNb(X, z);
				if (Target == NULL)
				{
					MSG(&G->IF, "Can't Find!\n");
					break;
				}
				if (calcDist(X, Target) <= 1)
					cnt++, DISCARD(&X->Hnd, x, &G->Dscd), STRIKE(X, Target, G);
				else
					MSG(&G->IF, "Can't reach!\n");
				break;
			case 2:
				MSG(&G->IF, "A DODGE can't be used\n");
				break;
			case 3:
				DISCARD(&X->Hnd, x, &G->Dscd);
				PEACH(X, G);
				break;
			case 4:
				MSG(&G->IF, "You chose a WINE, \n");
				if (cnt)
				{
					MSG(&G->IF, "but WINE-STRIKE or STRIKE was used this round!\n");
					break;
				}
				MSG(&G->IF, "then choose a STRIKE\n");
				y = rdn();
				if (X->Hnd.a[y].type != 1)
				{
					MSG(&G->IF, "Not a STRIKE!\n");
					break;
				}
				z = rdn();
				Target = getFromNb(X, z);
				if (Target == NULL)
				{
					MSG(&G->IF, "Can't Find!\n");
					break;
				}
				if (calcDist(X, Target) <= 1)
					cnt++, DISCARD(&X->Hnd, x, &G->Dscd), DISCARD(&X->Hnd, y, &G->Dscd), WINE_STRIKE(X, Target, G);
				else
					MSG(&G->IF, "Can't reach!\n");
				break;
			case 5:
				MSG(&G->IF, "You chose a DUEL, then choose a target\n");
				z = rdn();
				Target = getFromNb(X, z);
				if (Target == NULL)
				{
					MSG(&G->IF, "Can't Find!\n");
					break;
				}
				DISCARD(&X->Hnd, x, &G->Dscd);
				DUEL(X, Target, G);
				break;
			case 6:
				MSG(&G->IF, "You chose a DISMANTLE, then choose a target\n");
				z = rdn();
				Target = getFromNb(X, z);
				if (Target == NULL)
				{
					MSG(&G->IF, "Can't Find!\n");
					break;
				}
				DISCARD(&X->Hnd, x, &G->Dscd);
				DISMANTLE(X, Target, G);
				break;
			case 7:
				MSG(&G->IF, "You chose a SNATCH, then choose a target\n");
				z = rdn();
				Target = getFromNb(X, z);
				if (Target == NULL)
				{
					MSG(&G->IF, "Can't Find!\n");
					break;
				}
				if (calcDist(X, Target) <= 1)
				{
					DISCARD(&X->Hnd, x, &G->Dscd);
					SNATCH(X, Target, G);
				}
				else
					MSG(&G->IF, "Can't reach!\n");
				break;
			case 8:
				MSG(&G->IF, "You chose a BORROWEDSWORD, then choose a target\n");
				z = rdn();
				Target = getFromNb(X, z);
				if (Target == NULL)
				{
					MSG(&G->IF, "Can't Find!\n");
					break;
				}
				if (_ASK(&Target->Eqp, 18))
				{
					MSG(&G->IF, "%d BORRORWS %d's SWORD\n", X->Nb, Target->Nb);
					DISCARD(&X->Hnd, x, &G->Dscd);
					if (!BORROWEDSWORD(Target, G))
						MSG(&G->IF, "GTMD"), USE(&Target->Eqp, 18, &X->Hnd);
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
				MSG(&G->IF, "%d PEACH GARDEN\n", X->Nb);
				DISCARD(&X->Hnd, x, &G->Dscd);
				PEACH(X, G);
				forP(X)
					PEACH(Now, G);
				break;
			case 12:
				DISCARD(&X->Hnd, x, &G->Dscd);
				MSG(&G->IF, "%d SOMETHING FOR NOTHING\n", X->Nb);
				DRAW(&X->Hnd, &G->Main, &G->Dscd);
				DRAW(&X->Hnd, &G->Main, &G->Dscd);
				break;
			case 13:
				DISCARD(&X->Hnd, x, &G->Dscd);
				HARVEST(X, G);
				break;
			case 14:
				MSG(&G->IF, "%d LIGHTING\n", X->Nb);
				DISCARD(&X->Hnd, x, &X->Ftz);
				break;
			case 15:
				MSG(&G->IF, "You chose a DROWN IN HAPPINESS, then choose a target\n");
				z = rdn();
				Target = getFromNb(X, z);
				if (Target == NULL)
				{
					MSG(&G->IF, "Can't Find!\n");
					break;
				}
				MSG(&G->IF, "%d MAKES %d DROWN IN HAPPINESS\n", X->Nb, Target->Nb);
				DISCARD(&X->Hnd, x, &Target->Ftz);
				break;
			case 16:
				MSG(&G->IF, "You chose a STARVATION, then choose a target\n");
				z = rdn();
				Target = getFromNb(X, z);
				if (Target == NULL)
				{
					MSG(&G->IF, "Can't Find!\n");
					break;
				}
				if (calcDist(X, Target) <= 1)
				{
					MSG(&G->IF, "%d MAKES %d STARVE\n", X->Nb, Target->Nb);
					DISCARD(&X->Hnd, x, &Target->Ftz);
				}
				break;
			case 17:
				MSG(&G->IF, "%d EQUIPPED BINOCULARS\n", X->Nb);
				if (_ASK(&X->Eqp, 17))
					DISCARD(&X->Hnd, x, &G->Dscd);
				else
					DISCARD(&X->Hnd, x, &X->Eqp);
				break;
			case 18:
				MSG(&G->IF, "%d EQUIPPED BOW\n", X->Nb);
				if (_ASK(&X->Eqp, 18))
					DISCARD(&X->Hnd, x, &G->Dscd);
				else
					DISCARD(&X->Hnd, x, &X->Eqp);
				break;
			case 19:
				MSG(&G->IF, "%d EQUIPPED HORSE\n", X->Nb);
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
void Perform(Game *G, Player *Head)
{
	Player *Now = Head;
	for (; !Terminal(Now, G); Now = Now->Nxt)
	{
		refresh();
		cP(&G->GM);
		MSG(&G->GM, "\n***Round*** Alive:%d\n", G->n);
		MSG(&G->IF, "                  %d\n", Now->Nb);
		fateState(Now, G);
		if (Now->hlt <= 0)
			continue;
		if (Now->Skip & 2)
			Now->Skip ^= 2;
		else
			drawState(Now, G);
		//		printPlayer(Now,G);
		if (Now->Skip & 1)
			Now->Skip ^= 1;
		else
			playState(Now, G);
		discardState(Now, G);
	}
	cP(&G->GM);
	printGame(Now, G);
	SHOW(&G->GM);
	SHOW(&G->IF);
	free(G);
}
#endif
#ifndef linux
void playState(Player *X, Game *G)
{
	if (X->Bot)
	{
		Reveal(X, G);
		for (int i = 0, cnt; i < X->Hnd.n; i++)
		{
			if (X->hlt <= 0)
				break;
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
					DISCARD(&X->Hnd, i, &G->Dscd),
						PEACH(X, G);
				break;

			case 4:

				break;
			case 5:
				forP(X)
				{
					if (OPP(X, Now))
						DISCARD(&X->Hnd, i, &G->Dscd),
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
					printf("%d BORRORWS %d's SWORD\n", X->Nb, Now->Nb);
					DISCARD(&X->Hnd, i, &G->Dscd);
					if (!BORROWEDSWORD(Now, G))
						puts("GTMD"), USE(&Now->Eqp, 18, &X->Hnd);
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
			case 20:
				DISCARD(&X->Hnd, i, &G->Dscd);
				printf("%d EASE FOR FATIGUE\n", X->Nb);
				EASEFORFATIGUE(X, G);
				forP(X) if (!OPP(X, Now))
					EASEFORFATIGUE(Now, G);
				break;
			}
		}
		printGame(X, G);
#ifdef _WIN32
		system("pause");
		system("cls");
#endif
#ifdef __APPLE__
		PAUSE;
		system("clear");
#endif
	}
	else
	{
		for (int x, y, z, cnt = 0; !Terminal(X, G);)
		{
			CLS;
			printGame(X, G);
			printf("Your Hand Deck");
			_pD(&X->Hnd, G);
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
					cnt++, DISCARD(&X->Hnd, x, &G->Dscd), CLS, STRIKE(X, Target, G);
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
						puts("GTMD"), USE(&Target->Eqp, 18, &X->Hnd);
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
				break;
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
			case 20:
				printf("%d EASE FOR FATIGUE\n", X->Nb);
				DISCARD(&X->Hnd, x, &G->Dscd);
				EASEFORFATIGUE(X, G);
				forP(X) if (!OPP(X, Now))
					EASEFORFATIGUE(Now, G);
				break;
			}
		}
	}
}
void discardState(Player *X, Game *G)
{
	while (X->Hnd.n > X->hlt)
		_dscd(X, G);
}
void Perform(Game *G, Player *Head)
{
	Player *Now = Head;
	for (; !Terminal(Now, G); Now = Now->Nxt)
	{
		printf("\n***Round*** Alive:%d\n", G->n);

		fateState(Now, G);
		if (Now->hlt <= 0)
			continue;
		if (Now->Skip & 2)
			Now->Skip ^= 2;
		else
			drawState(Now, G);
		printPlayer(Now, G);
		if (Now->Skip & 1)
			Now->Skip ^= 1;
		else
			playState(Now, G);
		discardState(Now, G);
	}
	free(G);
}
#endif
