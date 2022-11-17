#include <stdio.h>
#include <stdlib.h>
#define NOMEFILE "hard_test_set.txt"
#define N 4

int ****alloca4d(int *pietre);
int ricerca_max(int *scelte, int *pietre, int tot);
int fZ(int ****Z, int ****R, int ****T, int ****S, int z, int r, int t, int s);
int fR(int ****Z, int ****R, int ****T, int ****S, int z, int r, int t, int s);
int fT(int ****Z, int ****R, int ****T, int ****S, int z, int r, int t, int s);
int fS(int ****Z, int ****R, int ****T, int ****S, int z, int r, int t, int s);
int max (int n1, int n2);



int main(){
    FILE *fp;
    int n_test, i, pietre[4], tot, max, j, pietre_dist=0, *scelte;

    fp = fopen(NOMEFILE, "r");
    if(fp==NULL){
        printf("File non trovato");
        return -1;
    }

    fscanf(fp, "%d", &n_test);

    for(i=0; i<n_test; i++){
        pietre_dist=0;
        tot=0;
        for(j=0; j<4; j++){
            fscanf(fp, "%d", &pietre[j]);
            if(pietre[j]!=0)
                pietre_dist++; 
            tot += pietre[j];
        }
        scelte = (int *) malloc(pietre_dist*sizeof(int));
        for(j=0; j<pietre_dist;j++)
            scelte[j]=j;
        max = ricerca_max(scelte, pietre, tot);
        printf("TEST #%d\n", i+1);
        printf("zaffiri = %d, rubini = %d, topazi = %d, smeraldi = %d, TOT=%d \n", pietre[0], pietre[1], pietre[2], pietre[3], tot);
        printf("La collana massima è lunga %d\n", max);
    }
    free(scelte);
    fclose(fp);
    return 0;
}

int ****alloca4d(int *pietre){
  int ****mat4D;
  int i, j, x, y;
  mat4D = malloc((1+pietre[0]) * sizeof(int***));
  for(i=0;i<=pietre[0];i++) {
    mat4D[i] = malloc((1+pietre[1]) * sizeof(int**));
    for(j=0;j<=pietre[1];j++) {
      mat4D[i][j] = malloc((1+pietre[2]) * sizeof(int*));
      for(x=0;x<=pietre[2];x++) {
        mat4D[i][j][x] = malloc((1+pietre[3]) * sizeof(int));
        for(y=0;y<=pietre[3];y++) {
          mat4D[i][j][x][y] = -1; 
        }
      }
    }
  }
  return mat4D;
}

int ricerca_max(int *scelte, int *pietre, int tot){
    int ****Z, ****R, ****S, ****T;
    int maxZ, maxR, maxT, maxS;
    int z=pietre[0], r=pietre[1], t=pietre[2], s=pietre[3];

  Z = alloca4d(pietre);
  R = alloca4d(pietre);
  T = alloca4d(pietre);
  S = alloca4d(pietre);

  maxZ = fZ(Z, R, T, S, z, r, t, s);
  maxR = fR(Z, R, T, S, z, r, t, s);
  maxT = fT(Z, R, T, S, z, r, t, s);
  maxS = fS(Z, R, T, S, z, r, t, s);

  return max(maxZ, max(maxR, max(maxT, maxS)));
}

int fZ(int ****Z, int ****R, int ****T, int ****S, int z, int r, int t, int s) {
  int nextZ, nextR;
  if (z <= 0)
    return 0;
  if (Z[z][r][t][s] != -1)
    return Z[z][r][t][s];
  /* Z seguito da Z o R */
  nextZ = fZ(Z,R,T,S,z-1,r,t,s);
  nextR = fR(Z,R,T,S,z-1,r,t,s);
  Z[z][r][t][s] = 1+max(nextZ,nextR);
  return Z[z][r][t][s];
}

int fR(int ****Z, int ****R, int ****T, int ****S, int z, int r, int t, int s) {
  int nextS, nextT;
  if (r <= 0)
    return 0;
  if (R[z][r][t][s] != -1)
    return R[z][r][t][s];
  /* R seguito da S o T */
  nextS = fS(Z,R,T,S,z,r-1,t,s);
  nextT = fT(Z,R,T,S,z,r-1,t,s);
  R[z][r][t][s] = 1+max(nextS,nextT);
  return R[z][r][t][s];
}

int fT(int ****Z, int ****R, int ****T, int ****S, int z, int r, int t, int s) {
  int nextZ, nextR;
  if (t <= 0)
    return 0;
  if (T[z][r][t][s] != -1)
    return T[z][r][t][s];
  /* T seguito da Z o R */
  nextZ = fZ(Z,R,T,S,z,r,t-1,s);
  nextR = fR(Z,R,T,S,z,r,t-1,s);
  T[z][r][t][s] = 1+max(nextZ,nextR);
  return T[z][r][t][s];
}

int fS(int ****Z, int ****R, int ****T, int ****S, int z, int r, int t, int s) {
  int nextS, nextT;
  if (s <= 0)
    return 0;
  if (S[z][r][t][s] != -1)
    return S[z][r][t][s];
  /* S seguito da S o T */
  nextS = fS(Z,R,T,S,z,r,t,s-1);
  nextT = fT(Z,R,T,S,z,r,t,s-1);
  S[z][r][t][s] = 1+max(nextS,nextT);
  return S[z][r][t][s];
}

int max (int n1, int n2){
    if (n1 > n2)
        return n1;
    return n2;
}

