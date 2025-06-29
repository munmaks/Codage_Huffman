#ifndef __DECOMPRES_H__
#define __DECOMPRES_H__

#include "bits_operations.h"

typedef struct _noeud_array
{
    uint8_t valeur; /* la lettre de 0 a 255 */
    int16_t left;   /* index of left child (-1 if none) */
    int16_t right;  /* index of right child (-1 if none) */
} NoeudArray;

typedef struct _arbre_array
{
    NoeudArray *nodes; /* array of nodes */
    uint32_t capacity; /* total capacity */
    uint32_t count;    /* current number of nodes */
    int32_t root;      /* index of root node */
} ArbreArray;

/**
 * @brief effectue l'appelle de tous les fonctions necessaires
 * pour decoder le fichier binaire
 *
 * @param nom_fichier le nom du fichier binaire
 * @return int 0 si OK, 1 sinon
 */
extern int decodage_fichier(const char *__restrict__ nom_fichier);

#endif
