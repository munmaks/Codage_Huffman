#include "../include/codage.h"

static __inline__ int longueur(const char *mot)
{
    int i = 0;
    while (*mot && i < NBLETTRES - 1){
        ++i;
        ++mot;
    }
    return i;
}

static __inline__ int est_alpha(unsigned char ch)
{
    return (('a' <= ch && ch <= 'z') ||
            ('A' <= ch && ch <= 'Z'));
}

static void affiche_code(char traduc[][NBLETTRES])
{
    uint8_t count = 0;
    unsigned char c = 0;
    for (c = 0; c < NBLETTRES-1; ++c)
        if (longueur(traduc[c]) > 0)
        {
            printf("<%d> %c: %s \n", c, c, traduc[c]);
            ++count;
        }
    printf("Nombre de lettres: %d\n", count);
    printf("Nombre de noeuds: %d\n", count + count - 1);
}

/*
static void affiche_huffman(Huffman *huffman)
{
    int i = 0;
    for (; i < huffman->nbNodes; ++i)
    {
        printf("lettre: %c, occur: %d, fg: %d, fd: %d\n",
               huffman->array[i].lettre, huffman->array[i].occur,
               huffman->array[i].fg, huffman->array[i].fd);
    }
}
*/


/*
static __inline__ void destroy_traduc(char **traduc)
{
    int i = 0;
    if (!traduc)
        return;
    for (i = 0; i < NBLETTRES; ++i)
        free(traduc[i]);
    free(traduc);
}
*/

static __inline__ void destroy_huffman(Huffman *huffman)
{
    if (huffman && huffman->array)
    {
        free(huffman->array);
    }
}

static void stockeOccurrences(FILE *fptr, int *array)
{
    int c;
    do
    {
        c = fgetc(fptr);
        if (feof(fptr))
        {
            break;
        }

        (array[c])++;

    } while (1);
}

static __inline__ int compteLettres(int *array)
{
    int count = 0, i = 0;
    for (i = 0; i < NBLETTRES; ++i)
        if (array[i])
            ++count;

    return count;
}

static void initHuffman(Huffman *huffman, int *tab, int len)
{
    int cmpt = 0;
    int i = 0;
    /*
    Huffman *huffman = malloc(sizeof *huffman);
    if (!huffman)
    {
        return NULL;
    }
    */
    huffman->array = malloc((2 * len - 1) * sizeof *(huffman->array));
    if (!huffman->array)
    {
        return;
        /*
        free(huffman);
        return NULL;
        */
    }

    for (i = 0; i < NBLETTRES; ++i)
    {
        if (tab[i])
        {
            huffman->array[cmpt].lettre = i;
            huffman->array[cmpt].occur = tab[i];
            huffman->array[cmpt].fg = -1;
            huffman->array[cmpt].fd = -1;
            ++cmpt;
        }
    }

    huffman->nbLeaves = len;
    huffman->nbNodes = len;
    huffman->nextLeaf = 0;
    huffman->nextInternal = len;

    /* return huffman; */
}

static int cmpfunc(const void *a, const void *b)
{
    Noeud *left = (Noeud *)a;
    Noeud *right = (Noeud *)b;

    if ((left->occur == right->occur) && (left->occur))
    {
        return left->lettre - right->lettre;
    }
    return left->occur - right->occur;
}

static void tri(Noeud *array, int len) { qsort(array, len, sizeof *array, cmpfunc); }

static int minNode(Huffman *huffman)
{

    if (huffman->nextLeaf >= huffman->nbLeaves)
        return (huffman->nextInternal)++;

    if (huffman->nextInternal >= huffman->nbNodes)
        return (huffman->nextLeaf)++;

    /* Comparaison des occurences des noeuds */
    if ((huffman->array[huffman->nextLeaf].occur <=
         huffman->array[huffman->nextInternal].occur))
        return (huffman->nextLeaf)++;

    return (huffman->nextInternal)++;
}

static void augmenteArbre(Huffman *huffman)
{
    int first = 0, second = 0;

    Noeud parent;

    if (!huffman)
        return;

    first = minNode(huffman);
    second = minNode(huffman);

    parent.lettre = -1;
    parent.occur = huffman->array[first].occur + huffman->array[second].occur;
    parent.fg = first;
    parent.fd = second;

    huffman->array[huffman->nbNodes] = parent;
    (huffman->nbNodes)++;
}

static void huffmanTree(Huffman *huffman, FILE *fptr)
{
    int array[NBLETTRES] = {0};
    int nb = 0;
    // Huffman huffman = {NULL, 0, 0, 0, 0};
    if (!fptr)
    {
        return;
    }

    stockeOccurrences(fptr, array);

    nb = compteLettres(array);

    initHuffman(huffman, array, nb);

    tri(huffman->array, huffman->nbNodes);

    while (huffman->nbNodes < (nb * 2) - 1)
    {
        augmenteArbre(huffman);
    }

    /* return huffman; */
}

static void construit_aux(Noeud *array, char traduc[NBLETTRES][NBLETTRES], char buffer[], int profondeur, int indice)
{
    if (array[indice].fg != -1)
    {
        buffer[profondeur] = '0';
        buffer[profondeur + 1] = '\0';
        construit_aux(array, traduc, buffer, profondeur + 1, array[indice].fg);
    }
    else
    {
        strcpy(traduc[array[indice].lettre], buffer);
        return;
    }

    if (array[indice].fd != -1)
    {
        buffer[profondeur] = '1';
        buffer[profondeur + 1] = '\0';
        construit_aux(array, traduc, buffer, profondeur + 1, array[indice].fd);
    }
    else
    {
        strcpy(traduc[array[indice].lettre], buffer);
        return;
    }
}

static void construitCode(Huffman *huffman, char traduc[NBLETTRES][NBLETTRES])
{
    int i = 0;
    /* int j = 0; */
    /* char **traduc = NULL; */
    char buffer[NBLETTRES];
    if (!huffman)
    {
        fprintf(stderr, "Huffman est NULL\n");
        return;
    }
    /*
    traduc = malloc(NBLETTRES * (sizeof(*traduc)));
    */
    fprintf(stderr, "sizeof(*traduc): %lu\n", sizeof(*traduc));
    /*
    if (!traduc)
    {
        return 0;
    }
    */
    /*
    for (i = 0; i < NBLETTRES; ++i)
    {
        traduc[i] = malloc(huffman->nbNodes * sizeof *(traduc + i));
        if (!traduc[i])
        {
            for (j = 0; j < i; ++j)
                free(traduc[j]);
            free(traduc);
            return NULL;
        }
    }
    */

    for (i = 0; i < NBLETTRES; ++i)
    {
        traduc[i][0] = '\0';
    }

    construit_aux(huffman->array, traduc, buffer, 0, huffman->nbNodes - 1);
    /* return traduc; */
}

static int compteur_bits(Huffman *huffman, int indice)
{
    int left = 0, right = 0;
    if (!huffman)
    {
        fprintf(stderr, "huffman est null\n");
        return 0;
    }
    if (0 <= indice && indice < huffman->nbLeaves)
        return 9; /*1 + 8, comme: 1['a']*/

    left = compteur_bits(huffman, huffman->array[indice].fg);
    right = compteur_bits(huffman, huffman->array[indice].fd);
    return left + right + 1; /* les sous-arbres + noeud interne actuel */
}

static void codeHuffmanX8_aux(Huffman *huffman, FileBit *out, int indice)
{
    int left = 0, right = 0;
    if (!huffman)
    {
        fprintf(stderr, "huffman est null\n");
        return;
    }

    /* si on est des feuilles */
    if (0 <= indice && indice < huffman->nbLeaves)
    {
        fEcrireBit(out, 1);
        fEcritCharbin(out, huffman->array[indice].lettre);
        return;
    }

    /* on est un noeud interne, donc 0 */
    fEcrireBit(out, 0);

    left = huffman->array[indice].fg;
    right = huffman->array[indice].fd;
    codeHuffmanX8_aux(huffman, out, left);
    codeHuffmanX8_aux(huffman, out, right);
}

void codeHuffmanX8(FILE *in, FileBit *out)
{
    int i = 0, bit = 0;
    Huffman huffman = {
        .array = NULL,
        .nbNodes = 0,
        .nbLeaves = 0,
        .nextLeaf = 0,
        .nextInternal = 0
    };
    char traduc[NBLETTRES][NBLETTRES];
    int c = 0;
    int nb_bits = 0, padding = 0, message = 0;
    int length = 0;

    if (!in || !out)
    {
        fprintf(stderr, "input/out est NULL\n");
        return;
    }

    huffmanTree(&huffman, in);
    
    /*
    if (!huffman)
    {
        fprintf(stderr, "Input est incorrect, huffman n'est pas construit\n");
        return;
    }
    */

    construitCode(&huffman, traduc);
    /* traduc = construitCode(huffman);*/

    
    affiche_code(traduc);
    
    /*
    if (!traduc)
    {
        fprintf(stderr, "Pas assez de memoire pour codage\n");
        destroy_huffman(huffman);
        return;
    }
    */

    /* compter le nombre de bits dans message */
    fseek(in, 0, SEEK_SET);
    while ((c = fgetc(in)) != EOF)
    {
        message += longueur(traduc[c]);
    }

    /* nombre de bits total:
    3 bits de padding,
    n bits arbre huffman,
    m bits message
    */
    nb_bits = 3 + compteur_bits(&huffman, huffman.nbNodes - 1) + message;

    /* quantite de chiffres a la fin pour obtenir modulo 8*/
    padding = (8 - (nb_bits % 8)) % 8;

    /* padding 3 chiffres premiers */
    for (i = 0; i < 3; ++i)
    {
        /* decalage a gauche i bits , i = {0, 1, 2} */
        bit = (padding & (4 >> i)) ? (1) : (0);
        fEcrireBit(out, bit);
    }

    codeHuffmanX8_aux(&huffman, out, huffman.nbNodes - 1);

    /* le message chiffre */
    fseek(in, 0, SEEK_SET);

    while ((c = fgetc(in)) != EOF)
    {
        length = longueur(traduc[c]);
        for (i = 0; i < length; ++i)
        {
            /* 0 ou 1 */
            fEcrireBit(out, (traduc[c][i] - '0'));
        }
    }

    /* padding derniers chiffes, par defaut c'est 0 */
    for (i = 0; i < padding; ++i)
    {
        fEcrireBit(out, 0);
    }
    destroy_huffman(&huffman);
    /* destroy_traduc(traduc); */
}

int codage_fichier(char *nom_fichier)
{
    FILE *fptr = NULL;
    FileBit out = {NULL, 0, 0};

    /* output */
    if (!fBitopen(&out, "fichier_bin.HUF", "w"))
    {
        fprintf(stderr, "Fichier out n'existe pas\n");
        fBitclose(&out);
        return 1;
    }

    fptr = fopen(nom_fichier, "r");
    if (!fptr)
    {
        fprintf(stderr, "%s probleme de fichier\n", nom_fichier);
        return 1;
    }

    codeHuffmanX8(fptr, &out);

    if (fclose(fptr) == EOF)
        fprintf(stderr, "N'a pas reussi a fermer le fichier fptr\n");
    if (fBitclose(&out) == EOF)
        fprintf(stderr, "N'a pas reussi a fermer le fichier out\n");
    return 0;
}




