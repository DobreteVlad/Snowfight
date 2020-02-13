// Copyright 2018 Vlad-Gabriel Dobrete (dobrete.vlad@yahoo.com)
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// structura pentru atributele spiridusilor
struct brownie {
    char name[20];
    char namecopy[20];
    int x;
    int y;
    int hp;
    int stamina;
    int glove;
    int condition;
    int wins;
    int winscopy;
    int conditioncopy;
};

void fight(FILE* g, struct brownie* v, int i, int id) {
// functie pentru lupta a 2 spiridusi
    int staminai, staminaid, ok = 0;
    staminai = v[i].stamina;
    staminaid = v[id].stamina;
// se cauta sa se vada cine loveste primul si se incepe lupta
    if (v[i].stamina > v[id].stamina && ok == 0) {
        while (v[i].hp > 0 && v[id].hp > 0) {
            ok = 1;
            v[id].hp = v[id].hp - v[i].glove;
            if (v[id].hp > 0) {
                v[i].hp = v[i].hp - v[id].glove;
            }
        }
// se cauta cine a fost trimis acasa si se initializeaza atributele
        if (v[i].hp <= 0) {
            fprintf(g, "%s sent %s back home.\n", v[id].name, v[i].name);
            v[i].x = -1;
            v[i].y = -1;
            v[i].hp = -1;
            v[i].stamina = -1;
            v[i].condition = 0;
            v[id].stamina = v[id].stamina + staminai;
            v[id].wins++;
            return;
        } else {
            fprintf(g, "%s sent %s back home.\n", v[i].name, v[id].name);
            v[id].x = -1;
            v[id].y = -1;
            v[id].hp = -1;
            v[id].stamina = -1;
            v[id].condition = 0;
            v[i].stamina = v[i].stamina + staminaid;
            v[i].wins++;
            return;
        }
    }
// se repeta procedeul de mai sus
    if (v[i].stamina <= v[id].stamina && ok == 0) {
        while (v[i].hp > 0 && v[id].hp > 0) {
            v[i].hp = v[i].hp - v[id].glove;
            if (v[i].hp > 0) {
                v[id].hp = v[id].hp - v[i].glove;
            }
        }
        if (v[i].hp <= 0) {
            fprintf(g, "%s sent %s back home.\n", v[id].name, v[i].name);
            v[i].x = -1;
            v[i].y = -1;
            v[i].hp = -1;
            v[i].stamina = -1;
            v[i].condition = 0;
            v[id].stamina = v[id].stamina + staminai;
            v[id].wins++;
            return;
        } else {
            fprintf(g, "%s sent %s back home.\n", v[i].name, v[id].name);
            v[id].x = -1;
            v[id].y = -1;
            v[id].hp = -1;
            v[id].stamina = -1;
            v[id].condition = 0;
            v[i].stamina = v[i].stamina + staminaid;
            v[i].wins++;
            return;
        }
    }
}
void verify_move(FILE* g, struct brownie* v, int id, int p, int r, int** b) {
    int i, center = (2 * r + 1) / 2, ok = 0, aux;
// conditie pentru a vedea daca iese de pe ghetar
    if (((v[id].x - center) * (v[id].x - center) +
(v[id].y - center) * (v[id].y - center)) > (r * r)) {
        fprintf(g, "%s fell off the glacier.\n", v[id].name);
// daca iese, se initializeaza unele atribute cu -1 si 0
        v[id].x = -1;
        v[id].y = -1;
        v[id].hp = -1;
        v[id].stamina = -1;
        v[id].condition = 0;
        ok = 1;
        return;
    }
// se verifica daca manusa gasita este mai buna
    if (v[id].x != -1 && b[v[id].x][v[id].y] != -1) {
        if (v[id].glove < b[v[id].x][v[id].y]) {
// se interschimba manusile
            aux = v[id].glove;
            v[id].glove = b[v[id].x][v[id].y];
            b[v[id].x][v[id].y] = aux;
        }
    }
// daca nu a cazut de pe ghetar:
    if (ok == 0) {
        for (i = 0; i < p; i++) {
// se cauta spiridus in casuta in care se afla cel mutat
            if (v[i].x == v[id].x && v[i].y == v[id].y &&
i != id && v[id].x != -1) {
// daca se gaseste un spiridus, se lupta
                fight(g, v, i, id);
            }
        }
    }
}
// functie pentru mutarea spiridusului in functie de comanda primita
void MOVE(FILE* f, FILE* g, struct brownie* v, int** a,
int p, int r, int** b) {
// id este spiridusul ce urmeaza sa fie mutat
    int id, i = 0;
// in dir sunt stocate directiile in care se duce spiridusul
    char dir[20];
    fscanf(f, "%d %s", &id, dir);
    if (v[id].hp != -1) {
        while (dir[i] != '\0') {
// daca spiridusul face o mutare invalida:
            if (dir[i] == 'L') {
                if (v[id].y - 1 == -1) {
                    v[id].y = v[id].y - 1;
// se apeleaza functia de verificare
                    verify_move(g, v, id, p, r, b);
// daca miscarea este valida se muta spiridusul si se scade stamina
                } else if (v[id].y != -1 && v[id].stamina >=
abs(a[v[id].x][v[id].y] - a[v[id].x][v[id].y - 1])) {
                    v[id].stamina = v[id].stamina -
abs(a[v[id].x][v[id].y] - a[v[id].x][v[id].y - 1]);
                    v[id].y = v[id].y - 1;
                }
            }
            if (dir[i] == 'R') {
                if (v[id].y + 1 == 2 * r + 1) {
                    v[id].y = v[id].y + 1;
                    verify_move(g, v, id, p, r, b);
                } else if (v[id].y != -1 && v[id].stamina >=
abs(a[v[id].x][v[id].y] - a[v[id].x][v[id].y + 1])) {
                    v[id].stamina = v[id].stamina -
abs(a[v[id].x][v[id].y] - a[v[id].x][v[id].y + 1]);
                    v[id].y = v[id].y + 1;
                }
            }
            if (dir[i] == 'U') {
                if (v[id].x - 1 == -1) {
                    v[id].x = v[id].x - 1;
                    verify_move(g, v, id, p, r, b);
                } else if (v[id].x != -1 && v[id].stamina >=
abs(a[v[id].x][v[id].y] - a[v[id].x - 1][v[id].y])) {
                    v[id].stamina = v[id].stamina -
abs(a[v[id].x][v[id].y] - a[v[id].x - 1][v[id].y]);
                    v[id].x = v[id].x - 1;
                }
            }
            if (dir[i] == 'D') {
                if (v[id].x + 1 == 2 * r + 1) {
                    v[id].x = v[id].x + 1;
                    verify_move(g, v, id, p, r, b);
                } else if (v[id].x != -1 && v[id].stamina >=
abs(a[v[id].x][v[id].y] - a[v[id].x + 1][v[id].y])) {
                    v[id].stamina = v[id].stamina -
abs(a[v[id].x][v[id].y] - a[v[id].x + 1][v[id].y]);
                    v[id].x = v[id].x + 1;
                }
            }
// daca spiridusul nu a cazut, se apeleaza functia de verificare
// daca nimereste in casuta cu alt spiridus, functia ii face sa se bata
            if (v[id].hp > 0) {
                verify_move(g, v, id, p, r, b);
            }
        i++;
        }
    }
}
void SNOWSTORM(FILE* f, FILE* g, struct brownie* v, int p) {
// functia transforma din binar in baza 10 numarul primit
    int X = 0, Y = 0, R = 0, DMG = 0, i, nr, XC = 1, YC = 1, RC = 1, DMGC = 1;
    fscanf(f, "%d", &nr);
    for (i = 0; i < 32; i++) {
// se imparte fiecare octet fiecarui atribut
        if (i < 8) {
            X = X + ((nr >> i) & 1) * XC;
            XC = XC * 2;
        } else if (i < 16) {
            Y = Y + ((nr >> i) & 1) * YC;
            YC = YC * 2;
        } else if (i < 24) {
            R = R + ((nr >> i) & 1) * RC;
            RC = RC * 2;
        } else if (i < 32) {
            DMG = DMG + ((nr >> i) & 1) * DMGC;
            DMGC = DMGC * 2;
        }
    }
    for (i = 0; i < p; i++) {
        if (R == 0) {
// caz pentru raza 0 in care este afectat doar cel din casuta respectiva
// daca se gaseste si ramane fara hp, se initializeaza cu -1/0 atributele sale
            if (v[i].x == X && v[i].y == Y) {
                v[i].hp = v[i].hp - DMG;
                if (v[i].hp <= 0) {
                    fprintf(g, "%s was hit by snowstorm.\n", v[i].name);
                    v[i].x = -1;
                    v[i].y = -1;
                    v[i].hp = -1;
                    v[i].stamina = -1;
                    v[i].condition = 0;
                }
            }
// daca raza este >0 se cauta spiridusii afectati din raza de actiune
// daca raman fara hp, se initializeaza cu -1/0 atributele sale
        } else if (R > 0) {
            if (v[i].x != -1) {
                if (((v[i].x - X) * (v[i].x - X) +
(v[i].y - Y) * (v[i].y - Y)) <= (R * R)) {
                    v[i].hp = v[i].hp - DMG;
                    if (v[i].hp <= 0) {
                        fprintf(g, "%s was hit by snowstorm.\n", v[i].name);
                        v[i].x = -1;
                        v[i].y = -1;
                        v[i].hp = -1;
                        v[i].stamina = -1;
                        v[i].condition = 0;
                    }
                }
             }
        }
    }
}
void realoc(int** a, int r2) {
// functie pentru realocarea hartii
    int i, j;
    for (i = 1; i <= 2 * r2 + 1; i++) {
        for (j = 1; j <= 2 * r2 + 1; j++) {
            a[i - 1][j] = a[i][j];
        }
    }
    for (i = 0; i <= 2 * r2 + 1; i++) {
        for (j = 1; j <= 2 * r2 + 1; j++) {
            a[i][j - 1] = a[i][j];
        }
    }
}
void MELTDOWN(FILE* f, FILE* g, struct brownie* v,
int r, int p, int r2, int** a, int** b) {
    int i, center = (2 * r + 1) / 2, st;
// am cautat sa vad daca spiridusii raman in afara ghetarului dupa comanda
    fscanf(f, "%d", &st);
    for (i = 0; i < p; i++) {
// daca se gaseste spiridus in afara, se reintializeaza atributele sale
// daca nu, ii creste stamina cu numarul cerut
        if (v[i].hp != -1) {
            if (((v[i].x - center) * (v[i].x - center) +
(v[i].y - center) * (v[i].y - center)) > (r2 * r2)) {
                fprintf(g, "%s got wet because of global warming.\n",
v[i].name);
                v[i].x = -1;
                v[i].y = -1;
                v[i].hp = -1;
                v[i].stamina = -1;
                v[i].condition = 0;
            } else if (v[i].x > 0 && v[i].y > 0) {
                v[i].stamina = v[i].stamina + st;
                v[i].x--;
                v[i].y--;
            }
        }
    }
// realocarea hartii astfel incat ghetarul sa fie inscris in matrice
    realoc(a, r2);
    realoc(b, r2);
}

void PRINT_SCOREBOARD(FILE* g, struct brownie* v, int p) {
    int i, aux1, ok = 0;
    char aux[20];
// se creeaza o copie pentru atributele wins/condition/name
    for (i = 0; i < p; i++) {
        strncpy(v[i].namecopy, v[i].name, sizeof(v[i].namecopy));
        v[i].winscopy = v[i].wins;
        v[i].conditioncopy = v[i].condition;
    }
// algoritm pentru generarea clasamentului in functie de importanta regulilor
    do {
        ok = 1;
        for (i = 0; i < p - 1; i++)
            if ((v[i].conditioncopy > v[i + 1].conditioncopy) ||
(v[i].conditioncopy == v[i + 1].conditioncopy &&
v[i].winscopy > v[i + 1].winscopy) ||
(v[i].conditioncopy == v[i + 1].conditioncopy &&
v[i].winscopy == v[i + 1].winscopy &&
strcmp(v[i].namecopy, v[i + 1].namecopy) < 0)) {
// interschimbarea valorilor
                aux1 = v[i].conditioncopy;
                v[i].conditioncopy = v[i + 1].conditioncopy;
                v[i + 1].conditioncopy = aux1;
                aux1 = v[i].winscopy;
                v[i].winscopy = v[i + 1].winscopy;
                v[i + 1].winscopy = aux1;
                strncpy(aux, v[i].namecopy, sizeof(aux));
                strncpy(v[i].namecopy, v[i + 1].namecopy,
sizeof(v[i].namecopy));
                strncpy(v[i + 1].namecopy, aux, sizeof(v[i + 1].namecopy));
                ok = 0;
            }
// afisarea scoreboard-ului
    } while (ok != 1);
    fprintf(g, "SCOREBOARD:\n");
    for (i = p - 1; i >= 0; i--) {
        fprintf(g, "%s\t", v[i].namecopy);
        if (v[i].conditioncopy == 1)
            fprintf(g, "DRY\t");
        else
            fprintf(g, "WET\t");
        fprintf(g, "%d\n", v[i].winscopy);
    }
}
int winner(struct brownie* v, int p) {
// functie in care se afla daca ramane un player si id-ul sau
    int i, count = 0, x = 0;
    for (i = 0; i < p; i++) {
        if (v[i].condition != 0 || v[i].hp != -1) {
            count++;
            x = i;
        }
    }
// daca ramane 1 player, se returneaza id-ul
    if (count == 1) {
        return x;
    }
    return -1;
}
int main() {
    FILE *f, *g;
    f = fopen("snowfight.in", "r");
    g = fopen("snowfight.out", "w");
    int r, p, i, **a, **b, j, center, r2, n, ok = 0;
    char command[20];
// alocarea dinamica a matricii si a structurii:
    struct brownie* v;
    fscanf(f, "%d%d", &r, &p);
    v = calloc(p, sizeof(struct brownie));
// copie a razei
    r2 = r;
    n = 2 * r + 1;
// centrul matricii
    center = (2 * r + 1) / 2;
    a = calloc(n, sizeof(int*));
    for (i = 0; i < n; i++)
        a[i] = calloc(n, sizeof(int));
    b = calloc(n, sizeof(int*));
    for (i = 0; i < n; i++)
        b[i] = calloc(n, sizeof(int));
    for (i = 0; i < 2 * r + 1; i++) {
        for (j = 0; j < 2 * r + 1; j++) {
            fscanf(f, "%d %d", &a[i][j], &b[i][j]);
        }
    }

    for (i = 0; i < p; i++) {
        fscanf(f, "%s", v[i].name);
        fscanf(f, "%d%d%d%d", &v[i].x, &v[i].y, &v[i].hp, &v[i].stamina);
// spiridusii incep cu 0 wins, conditie 1(DRY) si manusile din prima casuta
        v[i].wins = 0;
        v[i].condition = 1;
        v[i].glove = b[v[i].x][v[i].y];
// dupa ce se iau primul rand de manusi, casuta ramane goala:
        b[v[i].x][v[i].y] = -1;
// daca spiridusul este in afara ghetarului:
        if (((v[i].x - center) * (v[i].x - center) + (v[i].y - center) *
(v[i].y - center)) > (r * r)) {
            fprintf(g, "%s has missed the glacier.\n", v[i].name);
// initializarea atributelor cu 0/-1
            v[i].x = -1;
            v[i].y = -1;
            v[i].hp = -1;
            v[i].stamina = -1;
            v[i].condition = 0;
        }
    }
    while (fscanf(f, "%s", command) != EOF) {
        if (winner(v, p) != -1) {
// se apeleaza winner daca ramane un player
            fprintf(g, "%s has won.", v[winner(v, p)].name);
            fprintf(g, "\n");
            ok = 1;
            break;
        }
// se cauta functia ce urmeaza a fi apelata dupa comanda
        if (strcmp(command, "MOVE") == 0) {
            MOVE(f, g, v, a, p, r, b);
        }
        if (strcmp(command, "SNOWSTORM") == 0) {
            SNOWSTORM(f, g, v, p);
        }
        if (strcmp(command, "MELTDOWN") == 0) {
            r2 = r2 - 1;
            MELTDOWN(f, g, v, r, p, r2, a, b);
            r--;
        }
        if (strcmp(command, "PRINT_SCOREBOARD") == 0) {
            PRINT_SCOREBOARD(g, v, p);
        }
    }
// se verifica daca in urma comenzilor ramane un player prin functia winner
    if (winner(v, p) != -1 && ok != 1) {
        fprintf(g, "%s has won.", v[winner(v, p)].name);
        fprintf(g, "\n");
    }
// eliberarea memoriei
    for (i = 0; i < n; i++)
        free(a[i]);
    free(a);
    for (i = 0; i < n; i++)
        free(b[i]);
    free(b);
    free(v);
    fclose(f);
    fclose(g);
    return 0;
}
