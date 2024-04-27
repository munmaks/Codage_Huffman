#include "../include/bits_operations.h"


/****************************************************************/
/************    LES FONCTIONS POUR BIT OPERATIONS    ***********/
/****************************************************************/


int fBitopen(FileBit *f, const char *path, const char *mode) {
    if ( NULL == (f->fich = fopen(path, mode)) )
        return 0;
    f->stock = 0;
    f->nbBit = 0;
    return 1;
}


void fBitinit(FileBit *f, FILE *fich) {
    f->fich = fich;
    f->stock = 0;
    f->nbBit = 0;
}


int fBitclose(FileBit *f) {
    if (f->nbBit != 0) {
        /* decaler a gauche les derniers bits si necessaire*/
        if (f->nbBit != 8)
            f->stock <<= 8 - f->nbBit;
        fputc(f->stock, f->fich);
    }
    return fclose(f->fich);
}


int fEcrireBit(FileBit *f, int bit) {
    int coderetour = 1;
    if (f->nbBit == 8) {
        f->nbBit = 0;
        coderetour = fputc(f->stock, f->fich);
    }
    f->nbBit++;
    f->stock = (f->stock << 1) | bit;
    return coderetour;
}


int fLireBit(FileBit *f) {
    int n;
    if (f->nbBit == 0) {
        if ((n = fgetc(f->fich)) == EOF)
            return EOF;
        f->stock = n;
        f->nbBit = 8;
    }
    f->nbBit--;
    return (f->stock >> f->nbBit) & 1;
}


void fEcritCharbin(FileBit *f, unsigned char n) {
    int i;
    unsigned int mask = 1;
    for (i = 7; i >= 0; i--) {
        mask = 1 << i;
        fEcrireBit(f, (n & mask) != 0);
    }
}


unsigned char fLireCharbin(FileBit *f) {
    int i;
    unsigned char c = 0;
    for (i = 0; i < 8; i++) {
        c = (c << 1) | fLireBit(f);
    }
    return c;
}

void usage(void){
    printf("Bonjour!\n");
    printf("Cette algorithme utilise le codage d'Huffman\n");
    printf("Si vous voulez essayer de coder/decoder votre fichier\n");
    printf("Je vous invite d'abord a lire README.md fichier\n");
}
