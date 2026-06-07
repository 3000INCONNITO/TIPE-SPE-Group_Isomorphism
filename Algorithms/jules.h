#ifndef JULES_H
#define JULES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../Groups/group.h"

typedef struct elem {
  int val;
  struct elem *prec;
  struct elem *suiv;
} elem;

// Structure de la liste doublement chaînée
typedef struct ldc {
  elem *tete;
  elem *queue;
} ldc;

bool abelian_p_group_isomorphism(group_t *g1, group_t *g2, int p);
int *abelian_group_elements_orders(group_t *g, int n);
bool abelian_group_isomorphism(group_t *g1, group_t *g2);

ldc *liste_prime(int k);
void afficherListe(ldc *liste);
void initialiserListe(ldc *liste);
int* list_GCD(int k, int m);
int index_neutre(group_t *g, int n);

//creation groupes abéliens
group_t* direct_product(group_t* g1, group_t* g2);
group_t* build_ZnZ(int n);
group_t* create_abelian_group(int n);

#endif