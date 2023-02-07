typedef struct tree *T;

typedef nodo *link;

typedef struct {
    int val, n_figli;
    link *figli;
}nodo;

void add_Child(int key, link X);
T Create_Tree(int key);
link New_Node(int key);
link Get_Tree(T Tree);
int f(T t);