#include "SGS.h"
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
			if (Now->ctry == Y->ctry)
				if (ASK(Now, 3))
					return USE(&Now->Hnd, 3, &G->Dscd), 1;
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
	DRAW(&G->Dscd, &G->Main, &G->Dscd);
//	DISCARD(&G->Main, G->Main.n - 1, &G->Dscd);
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