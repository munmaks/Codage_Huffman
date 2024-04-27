#ifndef __BITS_OPERATIONS_H__
#define __BITS_OPERATIONS_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdint.h>


#define NBLETTRES 256
#define MAX_SIZE 1024


typedef struct {
    FILE *fich;          /* descripteur du fichier                  */
    unsigned char stock; /* buffer de 8 bits                        */
    char nbBit;          /* nombre de bits écrits dans le buffer    */
} FileBit;


/**
 * @brief Ouvre un fichier en lecture/Ecriture bit a bit
 * @return 1 si OK et 0 sinon
*/
int fBitopen(FileBit *f, const char *path, const char *mode);


/**
 * @brief Initialise une structure FileBit
 * avec un descripteur de fichier deja ouvert
 * 
 * @param f 
 * @param fich 
 */
void fBitinit(FileBit *f, FILE *fich);


/**
 * @brief Ferme le fichier en argument, et ecrit les bits en attente
 * 
 * @param f 
 * @return int 
 */
int fBitclose(FileBit *f);


/**
 * @brief Ecrit un bit dans le fichier
   Note: l'ecriture prend effet par paquet de 8 bits
 * 
 * @param f 
 * @param bit 
 * @return int 
 */
int fEcrireBit(FileBit *f, int bit);


/**
 * @brief Renvoie un bit lu depuis le fichier 
 * 
 * @param f 
 * @return int 
 */
int fLireBit(FileBit *f);


/**
 * @brief Ecrit les 8 bits d'un char en argument
 * 
 * @param f 
 * @param n 
 */
void fEcritCharbin(FileBit *f, unsigned char n);


/**
 * @brief Renvoie le char contruit par les 8 bits suivant lus dans le fichier
 * 
 * @param f 
 * @return unsigned char 
 */
unsigned char fLireCharbin(FileBit *f);

extern void usage(void);

#endif
