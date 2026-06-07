#include "jules.h"

// Find generators, algorithm. O(log(n))
// Calculate generated subgroup, gen not in the generated subgroup, ie
// not in current generated subgroup.
// Return index of new generator.

int calc_gen_sub(group_t *g, list_t *curr_gen, bool *gen_sub, bool *not_seen,
                 int gen) {
  assert(!gen_sub[gen]);
  int n = g->n;
  int next_gen = gen + 1;
  push(curr_gen, gen);

  int to_add;
  for (int i = 0; i < n; i++) {
    if (gen_sub[i]) {
      to_add = calculate(g, i, gen);
      gen_sub[to_add] = true;
    }
    if (gen_sub[next_gen]) {
      next_gen++;
    }
  }
  gen_sub[gen] = true;
  not_seen[gen] = false;

  // AFFICHAGE
  // printf("Current subgroup:\n");
  // for (int i = 0; i < n; i++) {
  //   if (gen_sub[i]) {
  //     printf("%d ", i);
  //   }
  // }
  // printf("\n");

  return next_gen;
}

list_t *find_generators(group_t *g) {
  int n = g->n;
  list_t *curr_gen = create_empty_list();
  bool *gen_sub = malloc(n * sizeof(bool));
  bool *not_seen = malloc(n * sizeof(bool));

  int gen = 0;
  int next_gen;
  while (curr_gen->length < g->n && gen < g->n) {
    next_gen = calc_gen_sub(g, curr_gen, gen_sub, not_seen, gen);
    gen = next_gen;
  }
  return curr_gen;
}

int ordre(int i, group_t *g, int p) {
  int x = g->table[i][0];
  int power = x;
  int r = 1;
  while (power != 0) {
    power = g->table[power][x];
    r++;
  }
  return r;
}

// Calcul de l'ordre de i par exponentiation rapide
int ordre_p(int i, group_t *g, int p, int *A) {
  int pow = i;
  int ord = 1;
  while (pow != 0) { // 0 est l'indice de l'élément neutre
    ord *= p;
    if (p == 2) {
      pow = g->table[pow][pow];
    } else {
      int temp = pow;
      int m = 1;
      while (A[m] != -1) {
        temp = g->table[temp][temp];
        if (A[m] == 1) {
          pow = g->table[pow][temp];
        }
        m++;
      }
    }
  }
  return ord;
}

// Fonction pour initialiser une liste vide
void initialiserListe(ldc *liste) {
  liste->tete = NULL;
  liste->queue = NULL;
}

// Fonction pour ajouter un élément en tête de liste
void ajouterEnTete(ldc *liste, int val) {
  elem *nouveau = (elem *)malloc(sizeof(elem));
  if (nouveau == NULL) {
    fprintf(stderr, "Erreur d'allocation mémoire\n");
    exit(EXIT_FAILURE);
  }
  nouveau->val = val;
  nouveau->prec = NULL;
  nouveau->suiv = liste->tete;

  if (liste->tete != NULL) {
    liste->tete->prec = nouveau;
  } else {
    liste->queue = nouveau; // Si la liste était vide
  }
  liste->tete = nouveau;
}

// Fonction pour ajouter un élément en queue de liste
void ajouterEnQueue(ldc *liste, int val) {
  elem *nouveau = (elem *)malloc(sizeof(elem));
  if (nouveau == NULL) {
    fprintf(stderr, "Erreur d'allocation mémoire\n");
    exit(EXIT_FAILURE);
  }
  nouveau->val = val;
  nouveau->suiv = NULL;
  nouveau->prec = liste->queue;

  if (liste->queue != NULL) {
    liste->queue->suiv = nouveau;
  } else {
    liste->tete = nouveau; // Si la liste était vide
  }
  liste->queue = nouveau;
}

// Fonction pour supprimer un élément de la liste
void supprime_elem(ldc *liste, elem *elem) {
  if (elem == NULL)
    return;

  if (elem->prec != NULL) {
    elem->prec->suiv = elem->suiv;
  } else {
    liste->tete = elem->suiv; // Si c'est la tête
  }

  if (elem->suiv != NULL) {
    elem->suiv->prec = elem->prec;
  } else {
    liste->queue = elem->prec; // Si c'est la queue
  }

  free(elem);
}

// Fonction pour afficher la liste de la tête à la queue
void afficherListe(ldc *liste) {
  elem *actuel = liste->tete;
  printf("Liste (tête -> queue) : ");
  while (actuel != NULL) {
    printf("%d ", actuel->val);
    actuel = actuel->suiv;
  }
  printf("\n");
}

// Renvoie la liste des nombres premiers entre 1 et k-1 en O(k)
ldc *liste_prime(int k) {
  ldc *Q = malloc(sizeof(ldc));
  initialiserListe(Q);

  ldc *L = malloc(sizeof(ldc));
  initialiserListe(L);

  bool *N_mark = malloc((k - 1) * sizeof(bool));
  elem **N = malloc((k - 1) * sizeof(elem *));
  for (int i = 0; i < k - 1; i++) {
    N_mark[i] = false;
    ajouterEnQueue(L, i + 1);
    N[i] = L->queue;
  }

  bool continuer = true;
  int ind_non_marque = 2;
  while (continuer) {
    ajouterEnQueue(Q, ind_non_marque);
    elem *curr = L->tete;

    while (ind_non_marque * curr->val <= k - 1) {
      curr = curr->suiv;
    }

    while (curr->prec != NULL) {
      curr = curr->prec;
      int l = curr->val;

      N_mark[l * ind_non_marque - 1] = true;
      // elimination du noeud
      elem *t = N[l * ind_non_marque - 1];
      supprime_elem(L, t);
    }
    while (ind_non_marque < k && N_mark[ind_non_marque - 1]) {
      ind_non_marque++;
    }
    if (ind_non_marque >= k) {
      continuer = false;
    }
  }

  // Libération approximative de la mémoire
  supprime_elem(L, L->tete);
  free(N_mark);
  free(N);
  free(L);
  return Q;
}

// Renvoie les éléments dans Q qui ne sont pas multiples d'éléments dans S
ldc *liste_non_multiple(ldc *S, int k) {
  ldc *Q = malloc(sizeof(ldc));
  initialiserListe(Q);

  ldc *L = malloc(sizeof(ldc));
  initialiserListe(L);

  bool *N_mark = malloc((k - 1) * sizeof(bool));
  elem **N = malloc((k - 1) * sizeof(elem *));
  for (int i = 0; i < k - 1; i++) {
    N_mark[i] = false;
    ajouterEnQueue(L, i + 1);
    N[i] = L->queue;
  }

  elem *p = S->tete;
  while (p != NULL) {

    elem *curr = L->tete;
    while (p->val * curr->val < k) {
      curr = curr->suiv;
    }
    while (curr->prec != NULL) {
      curr = curr->prec;
      int l = curr->val;

      N_mark[l * p->val - 1] = true;
      // elimination du noeud
      elem *t = N[l * p->val - 1];
      supprime_elem(L, t);
    }
    p = p->suiv;
  }
  for (int i = 1; i < k-1; i++) {
    if (!N_mark[i]) {
      ajouterEnQueue(Q, i+1);
    }
  }
  return Q;
}

// m = ord(x)
// Renvoie les PGCD(m, i) pour i entre 1 et k-1
int *list_GCD(int k, int m) {
  int *GCD = malloc((k - 1) * sizeof(int));

  // On aura besoin que de l'un des deux
  ldc *good = malloc(sizeof(ldc));
  ldc *bad = malloc(sizeof(ldc));
  initialiserListe(good);
  initialiserListe(bad);

  ldc *Q = liste_prime(k);
  elem *curr = Q->tete;
  while (curr != NULL) {
    if (m % curr->val == 0) {
      ajouterEnQueue(good, curr->val);
    } else {
      ajouterEnTete(bad, curr->val);
    }
    curr = curr->suiv;
  }
  ldc *good_powers = liste_non_multiple(bad, k);
  ldc *bad_powers = liste_non_multiple(good, k);
 
  for (int i = 1; i < k; i++) {
    GCD[i - 1] = 1;
  }
 
  elem *t = good_powers->tete;
  while (t != NULL) {
    GCD[t->val-1] = t->val;
    t = t->suiv;
  }

  elem *q = good->tete;
  while (q != NULL) {
    int power = 1;
    int r_q = 0;
   
    int power_curr = 1;
    int r_q_curr = 0;
    while (power_curr < k) {
      if (m % power_curr == 0){
        power = power_curr;
        r_q = r_q_curr;
      }
      power_curr *= q->val;
      r_q_curr++;
    }
    power *= q->val;
    // power = q ^ (r_q + 1)
    while (power < k) {
      int i = 1;
      while (i * power < k) {
        GCD[i * power-1] /= q->val;
       
        i++;
      }
      power *= q->val;
    }
    q = q->suiv;
  }
 
  t = bad_powers->tete;
  while (t != NULL) {
    GCD[t->val-1] = 1;
    t = t->suiv;
  }
 
  elem *g = good_powers->tete;
  while (g != NULL) {
    elem *b = bad_powers->tete;
    while (b != NULL && g->val * b->val < k) {
      GCD[g->val * b->val - 1] = GCD[g->val - 1];
      b = b->suiv;
    }
    g = g->suiv;
  }

  return GCD;
}

// Algorithme 2 - Tests d'isomorphismes entre 2 p-groupes abéliens
// En O(nlog(n)) où n est la taille des 2 groupes
// On suppose que |g1| = |g2| = n = p ^ m
// On considère que dans nos groups g->table[i][k] correspond à e * g[k] = g[k]
bool abelian_p_group_isomorphism(group_t *g1, group_t *g2, int p) {
  int n = g1->n;

  // On détermine la représentation de p en binaire, p < 2 ^ 63
  int q = p;
  int A[64];
  int i = 0;
  while (q > 0) {
    A[i] = q % 2;
    q /= 2;
    i++;
  }
  A[i] = -1;

  int *power_g1 = malloc(n * sizeof(int));
  int *power_g2 = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    power_g1[i] = 0;
    power_g2[i] = 0;
  }

  // Calcul de l'ordre des éléments
  for (int i = 0; i < g1->n; i++) {
    power_g1[ordre_p(i, g1, p, A)]++;
    power_g2[ordre_p(i, g2, p, A)]++;
  }

  for (int i = 0; i < n; i++) {
    if (power_g1[i] != power_g2[i]) {
      return false;
    }
  }
  return true;
}

int nombre_diviseur(int n) {
  int s = 0;
  for (int i = 1; i <= n; i++) {
    if (n % i == 0) {
      s++;
    }
  }
  return s;
}

// Si a, b <= n, gcd(a, b) est effectué en O(log(n))
// selon l'algorithme d'Euclide
int gcd(int a, int b) {
  while (b != 0) {
    int t = b;
    b = a % b;
    a = t;
  }
  return a;
}

int index_neutre(group_t *g, int n){
  for (int i = 0; i < n; i++){
    if (calculate(g, i, i) == i){
      return i;
    }
  }
  return -1; // Si g n'a pas de neutre = pas un groupe
}

int *abelian_group_elements_orders(group_t *g, int n) {
  int index_e = index_neutre(g, n);
 
  int *ORD = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    ORD[i] = -1;
  }
  ORD[index_e] = 1;
  // Précalcul de T : T[a][b] = a ^ b
  int d_n = nombre_diviseur(n);
  int *DIV = malloc((n+1) * sizeof(int));
  // DIV[i] = t si i est le t iéme diviseur de n

  // L : Liste des diviseurs de n
  int* L = malloc(d_n * sizeof(int));
  int ind = 0;
  for (int i = 1; i <= n; i++){
    if (n % i == 0){
      L[ind] = i;
      ind++;
    }  
  }
 
  int **T = malloc(d_n * sizeof(int *));

  for (int i = 0; i < d_n; i++) {
    T[i] = malloc(d_n * sizeof(int));
  }
  int t = 0;
  for (int i = 1; i <= n; i++) {
    if (n % i == 0) {
      DIV[i] = t;
      t++;
    }
  }

  for (int i = 0; i < d_n; i++) {
    for (int j = 0; j < d_n; j++){
      int a = L[i];
      int b = L[j];
      T[DIV[a]][DIV[b]] = a * b / gcd(a, b);
    }
  }
 
 
  // boucle principale :    pour 0 ?
  for (int i = 1; i < n; i++) {
    int x = g->table[index_e][i];
    if (ORD[x] == -1) {
      // Calcul de l'ordre de x
      bool recherche_ordre = true;
      int curr = x;
      int k = 1;
      while (recherche_ordre) {
        if (curr == index_e) {
          ORD[x] = k;
          recherche_ordre = false;
        } else {
          int suiv = calculate(g, curr, x);
          if (ORD[curr] != -1 && ORD[suiv] != -1) {
            int l = T[DIV[ORD[curr]]][DIV[ORD[suiv]]];
            ORD[x] = l;
            recherche_ordre = false;
          }
          curr = suiv;
          k++;
        }
      }

      // Calcul des ordres de x², x^3, ..., x^(k-1)
      // test ord(x i ) = ord(x)/ gcd(ord(x), i)
      int *liste_GCD = list_GCD(k, ORD[x]);
      int pow = x;
      for (int j = 2; j < k; j++) {
        pow = calculate(g, pow, x);
        ORD[pow] = ORD[x] / liste_GCD[j-1];
        //ORD[pow] = ORD[x] / gcd(ORD[x], j);
      }
     
    }
  }
  return ORD;
}

bool abelian_group_isomorphism(group_t *g1, group_t *g2) {
  int n = g1->n;
  int m = g1->n;
  assert(n == m);
  int *ORD1 = abelian_group_elements_orders(g1, n);
  int *ORD2 = abelian_group_elements_orders(g2, n);

  int* dict_ord1 = malloc(n*sizeof(int));
  int* dict_ord2 = malloc(n*sizeof(int));

  for (int i = 0; i < n; i++){
    dict_ord1[i] = 0;
    dict_ord2[i] = 0;
  }
 
  for (int i = 0; i < n; i++){
    dict_ord1[ORD1[i]-1] ++;
    dict_ord2[ORD2[i]-1] ++;
  }
 
  for (int i = 0; i < n; i++) {
    if (dict_ord1[i] != dict_ord2[i]) {
      return false;
    }
  }
  return true;
}

group_t* direct_product(group_t* g1, group_t* g2){
  int n = g1->n * g2->n;
  group_t* product = group_empty(n);
  for (int i = 0; i < n; i++){
    for (int j = 0; j < n; j++){
      int a1 = i / g2->n;  // Composante dans G1
      int b1 = i % g2->n;  // Composante dans G2
      int a2 = j / g2->n;
      int b2 = j % g2->n;
      int comp1 = calculate(g1, a1, a2);
      int comp2 = calculate(g2, b1, b2);
      product->table[i][j] = comp1 * g2->n + comp2;
    }
  }
  return product;
}

group_t* build_ZnZ(int n) {
    group_t* g = group_empty(n);
    // Remplir la table : table[i][j] = (i + j) % n
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g->table[i][j] = (i + j) % n;
        }
    }
    return g;
}

group_t* create_abelian_group(int n) {
  if (n <= 2){
    return build_ZnZ(n);
  }
  int k = (rand() % n)+1;
  while (n % k != 0){
    k = (rand() % n)+1;
  }

  group_t* g1 = create_abelian_group(n/k);
  group_t* g2 = build_ZnZ(k);

  group_t* res = direct_product(g1, g2);
  group_free(g1);
  group_free(g2);

  return res;
}