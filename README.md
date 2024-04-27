# Codage_Huffman

## Qu'est ce que c'est ?
C'est un TP que nous avons fait pendant 3 semaines.

L'objectif principal était de réaliser le codage de Huffman et le décodage en utilisant le langauge C.

## Comment essayer ?
D'abord vous devez avoir Linux installé.
Après, il faut cloner cette répositoire.

Pour coder et décoder il faut taper:
```bash
$ make

$ ./huffman votre_fichier.txt

$ ./huffman fichier_bin.HUF

$ diff votre_fichier.txt resultat.DEC
```

Testez un grand fichier ```crime-et-chatiment.txt```.

## Recherchez d'une fuite de mémoire en C?

Je vous invite à ajouter ```valgrind``` au début de votre exécutable.

### Auteur
- MUNAITPASOV M.
