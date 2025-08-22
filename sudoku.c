#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>

#define dimension 9
#define nb_threads 11

int sudoku[dimension][dimension];
int resultats[nb_threads] = {0};
int doublons[9] = {0};

typedef struct {
    int ligne;
    int colonne;
    int id;
} parameters;

int verif_dim(FILE* f) {
    char ligne[100];
    int ligne_count = 0;

    for (int i = 0; i < dimension; i++) {
        long pos = ftell(f);
        if (feof(f) && ligne_count != dimension) {
            printf("La taille de la grille de Sudoku devrait être 9x9\n");
            return 0;
        }

        if (!fgets(ligne, sizeof(ligne), f)) {
            printf("La taille de la grille de Sudoku devrait être 9x9\n");
            return 0;
        }

        if (strspn(ligne, " \t\r\n") == strlen(ligne)) {
            fseek(f, pos, SEEK_SET);
            printf("La taille de la grille de Sudoku devrait être 9x9\n");
            return 0;
        }

        int count = 0;
        char* token = strtok(ligne, " \t\r\n");
        while (token != NULL) {
            int chiffre;
            char reste;

            if (sscanf(token, "%d%c", &chiffre, &reste) == 1) {
                if (chiffre < 1 || chiffre > 9) {
                    printf("la case %d,%d contient un caractère spécial non-entier\n", ligne_count + 1, count + 1);
                    return 0;
                }
                sudoku[ligne_count][count] = chiffre;
            } else if (sscanf(token, "%f%c", (float*)&chiffre, &reste) == 1 || strchr(token, '.')) {
                printf("la case %d,%d contient un caractère non-entier\n", ligne_count + 1, count + 1);
                return 0;
            } else {
                printf("la case %d,%d contient un caractère spécial non admis\n", ligne_count + 1, count + 1);
                return 0;
            }

            count++;
            token = strtok(NULL, " \t\r\n");
        }
        if (count != dimension) {
            printf("La taille de la grille de Sudoku devrait être 9x9\n");
            return 0;
        }
        ligne_count++;
    }

    if (ligne_count != dimension) {
        printf("La taille de la grille de Sudoku devrait être 9x9\n");
        return 0;
    }

    if (!fgets(ligne, sizeof(ligne), f)) {
        return 1;
    }

    if (strspn(ligne, " \t\r\n") != strlen(ligne)) {
        printf("La taille de la grille de Sudoku devrait être 9x9\n");
        return 0;
    }

    return 1;
}

void* verif_lig() {
    bool aucun_doublon = true;
    for (int i = 0; i < dimension; i++) {
        int table_verif[10] = {0};
        for (int j = 0; j < dimension; j++) {
            if (table_verif[sudoku[i][j]] == 0) {
                table_verif[sudoku[i][j]] = 1;
            } else {
                doublons[sudoku[i][j] -1] = sudoku[i][j];
                aucun_doublon = false;
            }
        }
    }
    if (!aucun_doublon)
        pthread_exit(NULL);
    resultats[0] = 1;
    pthread_exit(NULL);
}

void* verif_col() {
    bool aucun_doublon = true;
    for (int j = 0; j < dimension; j++) {
        int table_verif[10] = {0};
        for (int i = 0; i < dimension; i++) {
            if (table_verif[sudoku[i][j]] == 0) {
                table_verif[sudoku[i][j]] = 1;
            } else {
                doublons[sudoku[i][j] -1] = sudoku[i][j];
                aucun_doublon = false;
            }
        }
    }
    if (!aucun_doublon)
        pthread_exit(NULL);
    resultats[1] = 1;
    pthread_exit(NULL);
}

void* verifier_sousgrille(void* arg) {
    parameters* param= (parameters*) arg;
    int table_verif[10] = {0};
    bool aucun_doublon = true;
    for (int i = param->ligne; i < param->ligne+3; i++) {
        for (int j = param->colonne; j < param->colonne+3; j++) {
            if (table_verif[sudoku[i][j]] == 0) {
                table_verif[sudoku[i][j]] = 1;
            } else {
                doublons[sudoku[i][j] -1] = sudoku[i][j];
                aucun_doublon = false;
            }
        }
    }
    if (!aucun_doublon)
        pthread_exit(NULL);
    resultats[param->id] = 1;
    free(param);
    pthread_exit(NULL);

}

void skip_sudoku(FILE * f) {
    char ligne[100];
    fgets(ligne, sizeof(ligne), f);
    while (strspn(ligne, " \t\r\n") != strlen(ligne) && !feof(f)) {
        fgets(ligne, sizeof(ligne), f);
    }
}

int main(int argc, char* argv[]) {
    FILE* file = fopen(argv[1], "r");
    int num = 1;
    while (!feof(file)) {
        printf("----- Sudoku %d -----\n", num++);

        if (!verif_dim(file)) {
            if (feof(file)) break;
            skip_sudoku(file);
            continue;
        }

        pthread_t threads[nb_threads];
        int id = 0;

        pthread_create(&threads[id++], NULL, verif_lig, NULL);
        pthread_create(&threads[id++], NULL, verif_col, NULL);

        for (int i = 0; i < dimension; i += 3) {
            for (int j = 0; j < dimension; j += 3) {
                parameters* param= malloc(sizeof(parameters));
                param->ligne = i;
                param->colonne = j;
                param->id = id;
                pthread_create(&threads[id++], NULL, verifier_sousgrille, param);
            }
        }
        for (int i = 0; i < nb_threads; i++) {
            pthread_join(threads[i], NULL);
        }

        bool valide = true;
        for (int i = 0; i < nb_threads; i++) {
            if (resultats[i] != 1) {
                valide = false;
            }
        }
        if (valide) {
            printf("Bravo! Votre Sudoku est valide!\n");
        } else {
            printf("Il y a un doublon dans la grille 9 x 9 ou une sous-grille 3 x 3\n");
            printf("Dans le Sudoku, il y a au moins un doublon des chiffres suivants :\n");
            for (int i = 0; i < 9; i++) {
                if (doublons[i] != 0)
                    printf("    %d\n", doublons[i]);
            }
        }
        for (int i = 0; i < nb_threads; i++) resultats[i] = 0;
        for (int i = 0; i < 9; i++) doublons[i] = 0;
    }
    return 0;
}
