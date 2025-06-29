#include "codage.h"
#include "decompres.h"

/*
Vous pouvez utiliser votre fichier text au lieu de crime-et-chatiment.txt
$ make
$ ./huffman crime-et-chatiment.txt
$ ./huffman fichier_bin.HUF

Pour voir la difference entre fichier source et le resultat:
$ diff crime-et-chatiment.txt resultat.DEC

Pour nettoyer
$ make mrproper

Si vous voulez savoir s'il y a la perte de memoire,
Vous pouvez utiliser par exemple "valgrind"
*/

int main(int argc, char *argv[]){
    int codage_res = 0, decodage_res = 0;
    int i = 0;

    if (argc < 2){
        fprintf(stderr, "Il faut plus de parametres\n");
    }

    for (i = 0; i < argc; ++i){
        if (strstr(argv[i], ".HUF")){
            decodage_res = 1;
            break;
        }
    }

    if (decodage_res){
        decodage_res = decodage_fichier(argv[i]);

        /* la suppression d'un fichier binaire */
        if (remove(argv[i])) { fprintf(stderr, "On ne peut pas supprimer le fichier binaire\n"); }
    } else {
        /*
        usage();
        */
        codage_res = codage_fichier(argv[1]);
    }
    return codage_res || decodage_res;
}
