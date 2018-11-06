#include "SGS.h"


void DRAW(Deck *D, Deck *G, Deck *GG)
{
	if (!G->n)
	{
		Deck tmp = *GG;
		*GG = *G;
		*G = tmp;
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

int OPP(Player *X, Player *Y)
{
	if (X->ctry == Y->ctry && X->ID == 2 && Y->ID == 2)
		return 0;
	return 1;
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
int rd(int L, int R)
{
	return rand() % (R - L + 1) + L;
}
int min(int a, int b) { return a < b ? a : b; }