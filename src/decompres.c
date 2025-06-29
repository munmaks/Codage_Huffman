#include "decompres.h"

/* (pire cas: 2 * 256 - 1 = 511 noeuds) */
#define MAX_TREE_NODES (512U)

static void alloue_arbre_array(ArbreArray *arbre)
{
    if (!(arbre->nodes = malloc(MAX_TREE_NODES * sizeof(*(arbre->nodes)))))
    {
        fprintf(stderr, "Pas assez de memoire pour les noeuds\n");
        return;
    }

    arbre->capacity = MAX_TREE_NODES;
    arbre->count = 0U;
    arbre->root = -1;
}

static void liberer_array(ArbreArray *__restrict__ arbre)
{
    if (arbre && arbre->nodes)
    {
        free(arbre->nodes);
    }
}

static int32_t ajouter_noeud(ArbreArray *arbre, unsigned char valeur)
{
    int32_t index = 0U;
    if (arbre->count >= arbre->capacity)
    {
        fprintf(stderr, "Capacité de l'arbre dépassée\n");
        return -1;
    }

    index = (int32_t)arbre->count++;
    arbre->nodes[index].valeur = valeur;
    arbre->nodes[index].left = -1;
    arbre->nodes[index].right = -1;

    return index;
}

static int creer_arbre_array(FileBit *fptr, ArbreArray *arbre, int64_t *compteur)
{
    unsigned char c = '\0';
    int bit_lu = 0;
    int left_child = 0, right_child = 0;
    int32_t node_index = 0;
    if (!fptr || !arbre)
    {
        fprintf(stderr, "Fichier ou arbre est NULL\n");
        return -1;
    }

    bit_lu = fLireBit(fptr);
    ++(*compteur);

    node_index = ajouter_noeud(arbre, 0); /* noeud interne par defaut */
    if (node_index == -1)
    {
        fprintf(stderr, "Pas de memoire\n");
        return -1;
    }

    if (1 == bit_lu)
    {
        c = fLireCharbin(fptr);
        arbre->nodes[node_index].valeur = c;
        (*compteur) += 8;
        return node_index;
    }

    left_child = creer_arbre_array(fptr, arbre, compteur);
    right_child = creer_arbre_array(fptr, arbre, compteur);

    if (left_child == -1 || right_child == -1)
    {
        return -1;
    }

    arbre->nodes[node_index].left = (int16_t)left_child;
    arbre->nodes[node_index].right = (int16_t)right_child;

    return node_index;
}

static void message_decrypte_aux_array(
    FileBit *fptr, FILE *out,
    ArbreArray *arbre, int node_index,
    int64_t nb_chars, uint8_t padding, int64_t *compteur)
{
    int bit_lu = 0;
    NoeudArray *node = NULL;
    if (!fptr || !out || !arbre || node_index < 0)
    {
        fprintf(stderr, "Paramètres invalides\n");
        return;
    }

    node = &arbre->nodes[node_index];

    /* si c'est une feuille */
    if (node->left == -1 && node->right == -1)
    {
        fprintf(out, "%c", node->valeur);
        return;
    }

    if (nb_chars <= (*compteur) + padding)
    {
        return;
    }

    bit_lu = fLireBit(fptr);

    if (0 == bit_lu && node->left != -1)
        message_decrypte_aux_array(fptr, out, arbre, node->left, nb_chars, padding, compteur);
    else if (1 == bit_lu && node->right != -1)
        message_decrypte_aux_array(fptr, out, arbre, node->right, nb_chars, padding, compteur);
    else
    {
        fprintf(stderr, "Erreur de navigation dans l'arbre\n");
        return;
    }
    ++(*compteur);
}

static void message_decrypte_array(
    FileBit *fptr, FILE *out,
    ArbreArray *arbre, int64_t nb_chars,
    uint8_t padding, int64_t *compteur)
{
    int bit_lu = 0;
    NoeudArray *root_node = NULL;
    if (!fptr || !arbre || !out || arbre->root == -1)
    {
        fprintf(stderr, "Fichiers ou arbre est null\n");
        return;
    }

    while ((bit_lu = fLireBit(fptr)) != EOF)
    {
        if (nb_chars <= (*compteur) + padding)
            return;

        root_node = &arbre->nodes[arbre->root];

        if (0 == bit_lu && root_node->left != -1)
            message_decrypte_aux_array(fptr, out, arbre, root_node->left, nb_chars, padding, compteur);
        else if (1 == bit_lu && root_node->right != -1)
            message_decrypte_aux_array(fptr, out, arbre, root_node->right, nb_chars, padding, compteur);
        else
        {
            fprintf(stderr, "Erreur de navigation dans l'arbre\n");
            return;
        }
        ++(*compteur);
    }
}

static void decompression_array(ArbreArray *__restrict__ arbre, FileBit *fptr, FILE *out)
{
    int64_t compteur = 0L, nb_chars = 0L;
    uint8_t padding = 0U, i = 0U, multiply = 4U;

    if (!fptr || !out)
    {
        fprintf(stderr, "fichier / out / arbre est null\n");
        return;
    }
    alloue_arbre_array(arbre);

    fseek(fptr->fich, 0, SEEK_END);
    nb_chars = ftell(fptr->fich) * 8;
    fseek(fptr->fich, 0, SEEK_SET);

    for (i = 0U; i < 3U; ++i)
    {
        padding += (uint8_t)(fLireBit(fptr) * multiply);
        multiply >>= 1;
        ++compteur;
    }

    if ((arbre->root = creer_arbre_array(fptr, arbre, &compteur)) == -1)
    {
        fprintf(stderr, "On n'a pas cree arbre de fichier\n");
        liberer_array(arbre);
        return;
    }

    message_decrypte_array(fptr, out, arbre, nb_chars, padding, &compteur);

    for (i = 0; i < padding; ++i)
    {
        fLireBit(fptr);
    }
}

extern int decodage_fichier(const char *__restrict__ nom_fichier)
{
    FileBit fptr = {NULL, 0, 0};
    ArbreArray arbre = {NULL, 0, 0, -1};
    FILE *out = NULL;

    if (!fBitopen(&fptr, nom_fichier, "r"))
    {
        fprintf(stderr, "Le fichier %s n'est pas ouvert\n", nom_fichier);
        return 1;
    }

    if (!(out = fopen("resultat.DEC", "w")))
    {
        fprintf(stderr, "Le fichier %s n'est pas ouvert\n", "out.DEC");
        fBitclose(&fptr);
        return 1;
    }

    decompression_array(&arbre, &fptr, out);

    if (arbre.nodes)
    {
        liberer_array(&arbre);
    }

    fclose(out);
    fBitclose(&fptr);
    return 0;
}
