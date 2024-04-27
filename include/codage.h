#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include "bits_operations.h"


typedef struct {
    unsigned char lettre;
    int occur;
    int fg;
    int fd;
} Noeud;


typedef struct {
    Noeud *array;
    int nbLeaves;
    int nbNodes;
    int nextLeaf;
    int nextInternal;
} Huffman;

/**
 * @brief construit le fichier binaire
 * en utilisant le codage d'Huffman
 * 
 * @param in Fichier input
 * @param out Fichier binaire
 */
extern void codeHuffmanX8(FILE *in, FileBit *out);

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
