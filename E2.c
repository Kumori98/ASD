#include "T.h"

int main() {

    T Tree = Create_Tree(-1) ;
    link Root = Get_Tree(Tree);

    add_Child(2,Root);
    add_Child(-1,Root);
    add_Child(4,Root);

    add_Child(-1,Root->child[0]);
    add_Child(6,Root->child[0]);
    add_Child(4,Root->child[0]->child[1]);
    add_Child(6,Root->child[1]);
    add_Child(-1,Root->child[1]->child[0]);
    add_Child(6,Root->child[1]->child[0]->child[0]);
    add_Child(-1,Root->child[1]->child[0]->child[0]->child[0]);
    add_Child(6,Root->child[1]->child[0]->child[0]->child[0]->child[0]);


    f(Tree);
    return 0;
}