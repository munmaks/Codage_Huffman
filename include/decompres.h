#ifndef __DECOMPRES_H__
#define __DECOMPRES_H__

#include "bits_operations.h"

typedef int64_t ll;
typedef uint64_t ull;

typedef struct _arbre{
    struct _arbre *left;    /* fils gauche */
    struct _arbre *right;   /* fils droite */
    unsigned char valeur;   /* la lettre de 0 a 255 */
} *Arbre;

/**
 * @brief liberer la memoire allouee
 * 
 * @param arbre 
 */
extern void liberer(Arbre * __restrict__ arbre);

/**
 * @brief effectue la decompression d'un fichier binaire
 * passe en parametre
 * 
 * @param fptr 
 * @param out 
 * @return Arbre 
 */
extern Arbre decompression(FileBit *fptr, FILE *out);

/**
 * @brief effectue l'appelle de tous les fonctions necessaires
 * pour decoder le fichier binaire
 * 
 * @param argc 
 * @param nom_fichier 
 * @return int 0 si OK, 1 sinon
 */
extern int decodage_fichier(char *nom_fichier);

#endif
