#include "quotazioni.h"

typedef struct nodo_ {
  struct nodo_ *l, *r;
  quotazione_t q;
  int cnt;
} nodo;

typedef nodo* link;

struct quotazioni {
  link radice;
  int n;
};

quotazioni_t BSTquotazioniInit() {
  quotazioni_t bst = calloc(1,sizeof(*bst));
  return bst;
}

int BSTquotazioniEmpty(quotazioni_t bst) {
  if (bst == NULL || bst->radice == 0)
    return 1;
  return 0;
}

quotazione_t searchR(link h, data_t d) {
  int cmp;
  if (h == NULL)
    return initQuotazione();
  cmp = confrontaDate(d, dataQuotazione(h->q));
  if (cmp == 0)
    return h->q;
  else if (cmp < 0)
  	return searchR(h->l, d);
  else
	return searchR(h->r, d);
}

quotazione_t BSTquotazioniSearch(quotazioni_t bst, data_t d) {
  return searchR(bst->radice, d);
}

static link NEW(data_t d, float val, int n, link left, link right) {
  link l = calloc(1, sizeof(*l));
  l->q.data = d;
  l->q.val = val * n;
  l->q.n = n;
  l->l = left;
  l->r = right;
  l->cnt = 1;
  return l;
}

static void minmax(link h, float *min, float *max) {
  float q = valQuotazione(h->q);
  if (h == NULL)
    return;
  if (*min < 0 || *min > q)
    *min = q;
  if (*max < 0 || *max < q)
    *max = q;
  if (h->r)
    minmax(h->r, min, max);
  if (h->l)
    minmax(h->l, min, max);
}

void BSTgetMinMax(quotazioni_t bst, float *min, float *max) {
  if (BSTquotazioniEmpty(bst))
    return;
  minmax(bst->radice, min, max);
}

static link insertR(link h, data_t d, float val, int qta, int *add) {
  int cmp;
  link n;
  if (h == NULL) {
    *add = 1;
    n = NEW(d, val, qta, NULL, NULL);
    return n;
  }
  cmp = DATAcmp(d, QUOTAZIONEgetData(h->q));
  if (cmp < 0) {
    h->l = insertR(h->l, d, val, qta, add);
  } else if (cmp > 0) {
    h->r = insertR(h->r, d, val, qta, add);
  } else {
    h->q.val += (h->q.val + qta * val);
    h->q.n += qta;
  }
  if (*add) h->cnt++;
  return h;
}

void BSTquotazioniInsert(quotazioni_t bst, data_t d, float val, int qta) {
  int add = 0;
  bst->radice = insertR(bst->radice, d, val, qta, &add);
  if (add) bst->n++;
  return;
}


static void treePrint(link h, FILE *fp) {
  if (h == NULL)
    return;

  treePrint(h->l, fp);
  stampaQuotazione(fp, h->q);
  treePrint(h->r, fp);
}

void BSTquotazioniStore(quotazioni_t bst, FILE *fp) {
  if (BSTquotazioniEmpty(bst))
    return;
  treePrint(bst->root, fp);
}

void static treeMinmaxRange(link r, data_t d1, data_t d2, float *f1, float *f2) {
  int cmp1 = DATAcmp(d1, dataQuotazione(r->q));
  int cmp2 = DATAcmp(d2, dataQuotazione(r->q));
  float qv;
  if (r == NULL)
    return;
  if (cmp1 < 0)
    treeMinmaxRange(r->l, d1, d2, f1, f2);
  if (cmp2 > 0)
    treeMinmaxRange(r->r, d1, d2, f1, f2);
  if (cmp1 <= 0 && cmp2 >= 0) {
    qv = valQuotazione(r->q);
    if (*f1 < 0 || *f1 > qv)
      *f1 = qv;
    if (*f2 < 0 || *f2 < qv)
      *f2 = qv;
  }
}

void BSTquotazioniMinmaxRange(quotazioni_t bst, data_t d1, data_t d2, float *f1, float *f2) {
  if (bst == NULL || bst->radice == NULL || bst->n == 0)
    return;
  treeMinmaxRange(bst->radice, d1, d2, f1, f2);
}

static int treeMinmaxHeight(link h, int *max, int depth) {
  int min_l, min_r;
  if (h == NULL)
    return 0;
  if (h->l == NULL && h->r == NULL) {
    if (depth > *max)
      *max = depth;
    return 1;
  }
  min_l = treeMinmaxHeight(h->l, max, depth+1);
  min_r = treeMinmaxHeight(h->r, max, depth+1);
  if (h->l == NULL)
    return min_r+1;
  if (h->r == NULL)
    return min_l+1;
  return (min_l < min_r) ? min_l+1 : min_r+1;
}

link rotR(link h) {
  link x = h->l;
  h->l = x->r;
  x->r = h;
  x->cnt = h->cnt;
  h->cnt = 1;
  h->cnt += (h->l) ? h->l->cnt : 0;
  h->cnt += (h->r) ? h->r->cnt : 0;
  return x;
}

link rotL(link h) {
  link x = h->r;
  h->r = x->l;
  x->l = h;
  x->cnt = h->cnt;
  h->cnt = 1;
  h->cnt += (h->l) ? h->l->cnt : 0;
  h->cnt += (h->r) ? h->r->cnt : 0;
  return x;
}

static link treePartition(link h, int k) {
  int t = (h->l) ? h->l->cnt : 0;
  if (h == NULL)
    return NULL;
  if (t > k) {
    h->l = treePartition(h->l, k);
    h = rotR(h);
  }
  if (t < k) {
    h->r = treePartition(h->r, k-t-1);
    h = rotL(h);
  }
  return h;
}

static void treeBalance(quotazioni_t bst) {
  int k = (bst->n+1)/2-1;
  bst->radice = treePartition(bst->radice, k);
}

void BSTquotazioniBalance(quotazioni_t bst) {
  int min = -1, max = -1;
  if (BSTquotazioniEmpty(bst))
    return;
  min = treeMinmaxHeight(bst->radice, &max, 1);
  if (bst->radice && (bst->radice->l == NULL || bst->radice->r == NULL))
    min = 0;
  if (min == 0 || (max / (float) min) > S) {
    printf("Pre-bilanciamento min_path = %d max_path = %d\n", min, max);
    treeBalance(bst);
    min = max = -1;
    min = treeMinmaxHeight(bst->radice, &max, 1);
    printf("Post-bilanciamento min_path = %d max_path = %d\n", min, max);
  }
}