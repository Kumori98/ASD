#include "T.h"
#include <stdio.h>
#include <stdlib.h>
#define N_Max 5

struct tree{
    link root;
};

int f(T t) {
    int max=0, sum=0;
    findMaxPath(&max, &sum, t->root);
    return max;
}

void findMaxPath(int *max, int *sum, link root){
    int i;
    if(root->n == 0){
        *sum += root->val;
        if(*sum>*max)
            *max=*sum;
    return;
    }

for(i=0; i<root->n_figli; i++){
    if(root->val>0){
        *sum += root->val;
        findMaxPath(max, sum, root->figli[i]);
        *sum -= root->val;
    }
    else
        break;
    }
    return;
}

T Create_Tree(int key){
    T Tree = malloc(sizeof(T));

    Tree->root = malloc(sizeof(link));
    Tree->root->key = key ;
    Tree->root->N_Figli = 0 ;
    Tree->root->child = malloc(N_Max* sizeof(link)) ;

    for (int i = 0 ; i<N_Max ; i++) Tree->root->child[i] = NULL;

    return Tree;

}

link Get_Tree(T Tree){
    return Tree->root;
}

link New_Node(int key){
    link x = malloc(sizeof(link));

    x->key= key ;
    x->N_Figli = 0 ;
    x->child = malloc(N_Max* sizeof(link));

    for (int i = 0 ; i<N_Max ; i++) x->child[i] = NULL ;

    return x ;
}

void add_Child(int key, link X){

    X->child[X->N_Figli++] = New_Node(key);

}
