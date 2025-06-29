#ifndef __CODAGE_H__
#define __CODAGE_H__

#include "bits_operations.h"

typedef struct _noeud
{
    int32_t occur;
    int32_t fg;
    int32_t fd;
    uint8_t lettre;
} Noeud;

typedef struct _huffman
{
    Noeud *array;
    uint32_t nbLeaves;
    uint32_t nbNodes;
    uint32_t nextLeaf;
    uint32_t nextInternal;
} Huffman;

/**
 * @brief effectue l'appelle de tous les fonctions necessaires
 * en utilisant le codage d'Huffman pour obtenir le fichier binaire (coder)
 *
 * @param argc
 * @param nom_fichier
 * @return int 0 si OK, 1 sinon
 */
extern int codage_fichier(char *nom_fichier);

#endif
