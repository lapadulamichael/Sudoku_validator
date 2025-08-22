TP1 INF3173
Auteur : Michael Lapadula

Description
-
Ce programme est un validateur de Sudoku. Le programme propose une solution 
multithread développée en C dans le cadre du cours INF3173. 11 threads sont 
utilisés. 
- 1 thread pour verifier que chaque ligne contient les chiffres de 1 à 9.
- 1 thread pour verifier que chaque colonne contient les chiffres de 1 à 9.
- 9 threads pour verifier que chaque sous-grille 3x3 contient les chiffres de 1 à 9.

Le programme peut prendre plusieurs Sudokus de suite (en format 9x9), tant qu'ils sont 
marqués par une ligne vide. Une ligne vide sera automatiquement considérée 
comme étant la fin d'un Sudoku. Le programme vérifie que :
- Le format 9x9 est respecté.
- Chaque element du Sudoku est un entier valide.
- Le Sudoku est valide (1 à 9 dans chaque colonne, ligne et sous-grille 3x3).

Un espace DOIT être present entre chaque chiffre pour marquer chaque valeur.
(Rien est précisé dans l'énoncé du TP et les exemples fournis 
sont TOUS présentés sous ce format)

ATTENTION : Comme mentionné plus tot, chaque ligne vide sera considérée comme 
la fin d'un Sudoku. S'il y a une ligne vide en début de fichier alors le 
programme considérera qu'on a essayé de valider un Sudoku vide. La meme 
chose si 2 lignes vides se succedent. Une sera la fin du Sudoku et l'autre la 
fin d'un Sudoku vide. À noter que l'éditeur de texte Linux mets par défaut un 
retour de ligne invisible à la fin du fichier. Pour plus de constance, il est 
conseillé de ne pas skip de ligne apres le dernier Sudoku.

Le programme annoncera la premiere erreur détectée lors de la validation de la 
grille. Cela peut être une erreur de dimension ou un "chiffre" non valide (char 
special ou non entier de 1 à 9).

Parmi les erreurs possibles :
- Pas la bonne dimension -> "La taille de la grille de Sudoku devrait être
  9x9".
- Contient un caractère autre que les entiers de 1 à 9 -> "la case X,Y (vous 
  identifiez la case par le numéro de colonne et le numéro de la ligne) 
  contient un caractère non-entier".
- Contient un caractère spécial ($, %, !, etc.) -> "la case X,Y (vous 
  identifiez la case par le numéro de colonne et le numéro de la ligne)
  contient un caractère spécial non admis".
- Contient des doublons des chiffres de 1 à 9 -> "Il y a un doublon dans 
  la grille 9 x 9 » ou « il y a un doublon dans une sous-grille 3 x 3". 
  Ensuite, chaque chiffre contenant un doublon sera donné.

Exécution du programme
-
Une Makefile est fourni avec le programme. Pour lancer le programme, 
simplement faire `make sudoku`. Cela validera le contenu du fichier `test.txt`. 
Pour valider un Sudoku, mettre dans le fichier `test.txt` le Sudoku en question 
en respectant le format demandé. Des exemples sont fournis dans `test.txt`. Le 
2e est valide, les trois autres comportent des erreurs pour démontrer les erreurs
levées par le validateur.
