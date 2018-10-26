#include <stdio.h>
#define MAXN 50
typedef struct Cards
{
	int n, a[MAXN];
} Deck;
typedef struct Plyr
{
	int Nb;				//Number
	int hlt, hltLv;		//Health point;Health Level
	int ID;				//Identity: 0:general 1:mercenary
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
void fateState(Player X)
{
}
void drawState(Player X)
{
	X.Hnd.n += 2;
}
void playState(Player X)
{

}
int min(int a, int b)
{
	return a < b ? a : b;
}
void discardState(Player X)
{
	X.Hnd.n = min(X.Hnd.n,X.hltLv);
}
int Terminal(Player X)
{
	int A[MAXN], tot = 1;
	for (Player Y = X; Y.Nb != X.Nb; Y = *Y.Nxt, tot++)
	{
		A[tot] = Y.ID;
		if (Y.Rvld == 0)
			return 0;
	}
	if (tot == 1)
		return 1;
}
void initGame(Game G)
{
}
void initPlayers(int n, Player Head)
{

}
void Perform()
{
	Game G;
	initGame(G);
	Player Head, Now;
	initPlayers(5, Head);
	for (Now = Head; !Terminal(Now); Now = *Now.Nxt)
	{
		fateState(Now);
		drawState(Now);
		playState(Now);
		discardState(Now);
	}
}
int main()
{
	//Perform();
	return 0;
}
