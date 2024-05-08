#include "../include/decompres.h"


static ll compteur = 0;


/****************************************************************/
/**********     LES FONCTIONS POUR HUFFMAN DECODAGE     *********/
/****************************************************************/


static Arbre alloue_noeud(char valeur){
    Arbre arbre = NULL;
    if ( !(arbre = malloc(sizeof *arbre)) ){
        fprintf(stderr, "Pas assez de memoire pour l'arbre\n"); return NULL;
    }
    arbre->valeur = valeur;
    arbre->left = arbre->right = NULL;
    return arbre;
}


void liberer(Arbre * __restrict__ arbre){
    if ( !*arbre ) return;

    if ( (*arbre)->left ) { liberer(&(*arbre)->left); }

    if ( (*arbre)->right ) { liberer(&(*arbre)->right); }

    /* suppression du noeud et son nom */
    free(*arbre);
    *arbre = NULL;
}


static int creer_arbre(FileBit *fptr, Arbre *arbre){
    unsigned char c = '\0';
    int bit_lu = 0;

    if (!fptr) { fprintf(stderr, "Fichier est NULL\n"); return 0; }

    bit_lu = fLireBit(fptr);
    ++compteur;

    *arbre = alloue_noeud(0);    /* pour voir les noeuds internes */
    if (!*arbre) { fprintf(stderr, "Pas de memoire\n"); return 0; }

    if ( 1 == bit_lu ){

        c = fLireCharbin(fptr);

        (*arbre)->valeur = c;
        compteur += 8;
        return 1;
    }
    
    return creer_arbre(fptr, &((*arbre)->left)) &&
           creer_arbre(fptr, &((*arbre)->right));
}


static void message_decrypte_aux(FileBit *fptr, FILE *out, Arbre arbre, ll nb_chars, int padding){

    int bit_lu = 0;
    if (!fptr || !out){
        fprintf(stderr, "fptr ou out est null\n");
        return;
    }

    if (arbre){
        if (!arbre->left && !arbre->right){
            fprintf(out, "%c", arbre->valeur);
            return;
        }
    }

    if (nb_chars <= compteur + padding)
        return;

    bit_lu = fLireBit(fptr);

    if ( 0 == bit_lu )
        message_decrypte_aux(fptr, out, arbre->left, nb_chars, padding);
    else if ( 1 == bit_lu )
        message_decrypte_aux(fptr, out, arbre->right, nb_chars, padding);
    else {
        fprintf(stderr, "Il faut avoir que des 0 ou des 1\n"); return;
    }
    ++compteur; /* global */
}



static void message_decrypte(FileBit *fptr, FILE *out, Arbre arbre, ll nb_chars, int padding){
    int bit_lu = 0;

    if (!fptr || !arbre || !out) { fprintf(stderr, "Fichiers ou arbre est null\n"); return;  }

    while ((bit_lu = fLireBit(fptr)) != EOF){
        if (nb_chars <= compteur + padding)
            return;

        if ( 0 == bit_lu )
            message_decrypte_aux(fptr, out, arbre->left, nb_chars, padding);
        else if ( 1 == bit_lu )
            message_decrypte_aux(fptr, out, arbre->right, nb_chars, padding);
        else {
            fprintf(stderr, "Il faut avoir que des 0 ou des 1\n"); return;
        }
        ++compteur; /* global */
    }
}


Arbre decompression(FileBit *fptr, FILE *out){
    int i = 0, padding = 0;
    int bit_lu = 0;
    int multiply = 4;

    ll nb_chars = 0;
    Arbre arbre = NULL;
    if (!fptr || !out ) { fprintf(stderr, "fichier est null\n"); return NULL; }

    fseek(fptr->fich, 0, SEEK_END);   /* le curseur a la fin */
    nb_chars = ftell(fptr->fich) * 8;
    fseek(fptr->fich, 0, SEEK_SET);

    for (i = 0; i < 3; ++i) {
        bit_lu = fLireBit(fptr);

        if (bit_lu != 0 && bit_lu != 1) {
            fprintf(stderr, "Erreur, padding doit etre entre 0 et 7, padding: %d\n", padding);
            return NULL;
        }

        padding += bit_lu * multiply;
        multiply >>= 1;     /* {4, 2, 1} */

        ++compteur; /* global */
    }

    /* lire chaque bit */
    if ( !creer_arbre(fptr, &arbre )){
        fprintf(stderr, "On n'a pas cree arbre de fichier\n");
        return NULL;
    }
    /*
    fprintf(stderr, "padding: %d\n", padding);
    fprintf(stderr, "nb: %lld \n", nb_chars);
    fprintf(stderr, "compteur: %llu \n", compteur);
    */
    message_decrypte(fptr, out, arbre, nb_chars, padding);
    for (i = 0; i < padding; ++i){
        fLireBit(fptr);
    }
    return arbre;
}


int decodage_fichier(char *nom_fichier){
    FileBit fptr = {NULL, 0, 0};
    FILE *out = NULL;
    Arbre arbre = NULL;

    if ( !fBitopen(&fptr, nom_fichier, "r") ) {
        fprintf(stderr, "Le fichier %s n'est pas ouvert\n", nom_fichier);
        return 1;
    }

    if ( !(out = fopen("resultat.DEC", "w")) ){
        fprintf(stderr, "Le fichier %s n'est pas ouvert\n", "out.DEC");
        fBitclose(&fptr);
        return 1;
    }

    arbre = decompression(&fptr, out);

    if (arbre){
        liberer(&arbre);
    }

    fclose(out);
    fBitclose(&fptr);
    return 0;
}
