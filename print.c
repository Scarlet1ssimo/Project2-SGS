#include "SGS.h"

int MSG(Panel *P,const char *fmt, ...)
{
    char buf[MAXL];
    va_list args; int printed;
    va_start(args, fmt);
    printed = vsprintf(buf, fmt, args);
    va_end(args);
    _MESSAGE(P,buf);
    return printed;
}
void _MESSAGE(Panel *P,char S[])
{
    int pre=(P->n+P->N-1)%P->N,L=strlen(P->LOGS[pre]);
    if(P->LOGS[pre][L-1]!=32)
        memcpy(P->LOGS[P->n%P->N],S,MAXL*sizeof(char)),P->n++;
    else
        memcpy(P->LOGS[pre]+L,S,(MAXL-L)*sizeof(char));
/*	#define T P->LOGS[pre]
    int pre=(P->n+P->N-1)%P->N,L=strlen(T);
	char TT[MAXN];
	for(int i=0;i<MAXN;i++)
		TT[i]=T[i];
    fprintf(stderr,"pre T:%s|S:%s|%d|%d|%d\n",T,S,L,pre,P->n);
	if(strlen(T)!=0&&T[strlen(T)-1]==32)
		strcat(T+strlen(T),S);
	//	sprintf(P->LOGS[P->n%P->N],"%s",S),P->n++;
	else
	{
		memcpy(T,S,MAXN*sizeof(char));
		P->n++;
	}
    fprintf(stderr,"aft T:%s|S:%s|%d|%d\n",T,S,L,P->n);
	//	sprintf(P->LOGS[(P->n-1)%P->N],"%s%s",P->LOGS[(P->n-1)%P->N],S);*/
}
void SHOW(Panel *P)
{
    wclear(P->W);
	wmove(P->W,0,0);
	wborder(P->W,' ',' ',' ',' ',' ',' ',' ',' ');
//	box(P->W,0,0);
	wrefresh(P->W);//getch();
    if(P->n<P->N)
	{
        for(int i=0;i<P->n;i++)
		{
//			wprintw(P->W,"%d %s",i,P->LOGS[i]);wrefresh(P->W);
			wprintw(P->W,"%s",P->LOGS[i]);
//			for(int j=strlen(P->LOGS[i]);j>=0;j--)if(P->LOGS[i][j]=='\n')LNS++;
		}

	}
    else
    {
//		int LNS=0;
        for(int i=P->n%P->N;i<P->N;i++)
		{
//			wprintw(P->W,"%d %s",i,P->LOGS[i]);wrefresh(P->W);
			wprintw(P->W,"%s",P->LOGS[i]);
//			for(int j=strlen(P->LOGS[i]);j>=0;j--)if(P->LOGS[i][j]=='\n')LNS++;
		}
        for(int i=0;i<P->n%P->N;i++)
		{
//			wprintw(P->W,"%d %s",i,P->LOGS[i]);wrefresh(P->W);
			wprintw(P->W,"%s",P->LOGS[i]);
//			for(int j=strlen(P->LOGS[i]);j>=0;j--)if(P->LOGS[i][j]=='\n')LNS++;
		}
    }
//	wprintw(P->W,"%d %d\n",P->n,P->N);
    wrefresh(P->W);
}
void _pD(Deck *D,Game *G)
{
	MSG(&G->IF,"%d: ", D->n);
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
		MSG(&G->IF,"%d%s ", i, Nm[D->a[i].type]);
	MSG(&G->IF,"\n");
	refresh();
}
void printDeck(Deck *D,Game *G)
{
	MSG(&G->GM,"%d: ", D->n);
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
		MSG(&G->GM,"%d%s ", i, Nm[D->a[i].type]);
	MSG(&G->GM,"\n");
	refresh();
}
void printPlayer(Player *X,Game *G)
{
	MSG(&G->GM,"Player %d     Health %d/%d\n", X->Nb, X->hlt, X->hltLv);
//	MSG(&G->GM,"Skip: %d\n", X->Skip);

	if(X->Rvld)
	{
		if (X->ID==2)MSG(&G->GM,"Country %d's General\n", X->ctry);
		else MSG(&G->GM,"Mercenary\n");
	}
	else MSG(&G->GM, "Unrevealed\n");
	if(X->Hnd.n)MSG(&G->GM,"Hand Deck "),printDeck(&X->Hnd,G);
	if(X->Ftz.n)MSG(&G->GM,"Fate Deck "),printDeck(&X->Ftz,G);
	if(X->Eqp.n)MSG(&G->GM,"Equipment Deck "),printDeck(&X->Eqp,G);
	MSG(&G->GM,"\n");
	refresh();
}
void printGame(Player *X,Game *G)
{
//	MSG(&G->GM,"\n#########Print Game########\n");
	while (X->hlt == 0)
		X = X->Nxt;
	printPlayer(X,G);
	forP(X)
		printPlayer(Now,G);
	refresh();
}
