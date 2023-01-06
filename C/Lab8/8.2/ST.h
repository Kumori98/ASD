typedef struct ST *ST;

ST  	STinit(int maxN);
void  	STinsert(ST st,Item x);
void    STfree(ST st);
int     STcount(ST st);
int     STsearch(ST st, Key k);