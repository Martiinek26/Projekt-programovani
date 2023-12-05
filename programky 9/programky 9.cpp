// programky 9.cpp: Definuje vstupní bod pro aplikaci.
//

#include "programky 9.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZNACKA_SIZE 100
#define KOMENTAR_SIZE 100
#define MAX_KOMENTARE 10

struct Komentar {
    char text[KOMENTAR_SIZE];
    struct Komentar* dalsi;
};

struct Kapela {
    char znacka[ZNACKA_SIZE];
    int hodnoceni;
    char komentar[KOMENTAR_SIZE];
    struct Komentar* komentare;
    struct Kapela* dalsi;
};

struct Komentar* vytvorKomentar(const char* text) {
    struct Komentar* novyKomentar = (struct Komentar*)malloc(sizeof(struct Komentar));
    if (!novyKomentar) {
        printf("Chyba alokace pameti pro komentar.");
        exit(1);
    }
    strcpy(novyKomentar->text, text);
    novyKomentar->dalsi = NULL;
    return novyKomentar;
}

//Čištění bufferu

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void vlozSeznamPodleNazvu(struct Kapela** prvni, struct Kapela* novaKapela) {
    struct Kapela* aktKapela;

    if (*prvni == NULL || strcmp(novaKapela->znacka, (*prvni)->znacka) < 0) {
        novaKapela->dalsi = *prvni;
        *prvni = novaKapela;
        return;
    }

    aktKapela = *prvni;
    while (aktKapela->dalsi && strcmp(novaKapela->znacka, aktKapela->dalsi->znacka) > 0) {
        aktKapela = aktKapela->dalsi;
    }

    novaKapela->dalsi = aktKapela->dalsi;
    aktKapela->dalsi = novaKapela;
}

void vlozSeznamPodleHodnoceni(struct Kapela** prvni, struct Kapela* novaKapela) {
    struct Kapela* aktKapela;

    if (*prvni == NULL || novaKapela->hodnoceni > (*prvni)->hodnoceni) {
        novaKapela->dalsi = *prvni;
        *prvni = novaKapela;
        return;
    }

    aktKapela = *prvni;
    while (aktKapela->dalsi && novaKapela->hodnoceni <= aktKapela->dalsi->hodnoceni) {
        aktKapela = aktKapela->dalsi;
    }

    novaKapela->dalsi = aktKapela->dalsi;
    aktKapela->dalsi = novaKapela;
}

void seradPodleNazvu(struct Kapela** prvni) {
    struct Kapela* serazeny = NULL;

    while (*prvni) {
        struct Kapela* novaKapela = *prvni;
        *prvni = novaKapela->dalsi;
        novaKapela->dalsi = NULL;

        vlozSeznamPodleNazvu(&serazeny, novaKapela);
    }

    *prvni = serazeny;
}

void seradPodleHodnoceni(struct Kapela** prvni) {
    struct Kapela* serazeny = NULL;

    while (*prvni) {
        struct Kapela* novaKapela = *prvni;
        *prvni = novaKapela->dalsi;
        novaKapela->dalsi = NULL;

        vlozSeznamPodleHodnoceni(&serazeny, novaKapela);
    }

    *prvni = serazeny;
}

void vlozitKomentar(struct Komentar** prvni, const char* text) {
    struct Komentar* novyKomentar = vytvorKomentar(text);
    novyKomentar->dalsi = *prvni;
    *prvni = novyKomentar;
}

void vypisKomentare(const struct Komentar* prvni) {
    const struct Komentar* aktKomentar = prvni;

    while (aktKomentar) {
        printf("Komentar: %s\n", aktKomentar->text);
        aktKomentar = aktKomentar->dalsi;
    }
}

bool vlozitDalsiKomentar() {
    char volba;
    printf("Chcete vlozit dalsi komentar? (K/V): ");
    if (scanf(" %c", &volba) != 1) {
        printf("Chyba pri nacitani volby.");
        exit(1);
    }
    return toupper(volba) == 'K';
}

void pridat(struct Kapela** prvni) {
    struct Kapela* novaKapela = (struct Kapela*)malloc(sizeof(struct Kapela));

    if (!novaKapela) {
        printf("Chyba alokace pameti.");
        exit(1);
    }

    // Vyprázdnit buffer
    clearBuffer();

    printf("\nNapiste nazev kapely: ");
    if (fgets(novaKapela->znacka, ZNACKA_SIZE, stdin) == NULL) {
        printf("Chyba pri nacitani nazvu kapely.");
        free(novaKapela);
        return;
    }
    novaKapela->znacka[strcspn(novaKapela->znacka, "\n")] = '\0';  // Odstranění nového řádku

    printf("\nHodnoceni kapely (1-10): ");
    if (scanf("%d", &novaKapela->hodnoceni) != 1 || novaKapela->hodnoceni < 1 || novaKapela->hodnoceni > 10) {
        printf("Chyba pri nacitani nebo neplatne hodnoceni kapely.");
        free(novaKapela);
        return;
    }
    clearBuffer();

    novaKapela->komentare = NULL;

    char volba;

    do {
        printf("\nNapiste komentar: ");
        if (fgets(novaKapela->komentar, KOMENTAR_SIZE, stdin) == NULL) {
            printf("Chyba pri nacitani komentare.");
            free(novaKapela);
            return;
        }
        novaKapela->komentar[strcspn(novaKapela->komentar, "\n")] = '\0';  // Odstranění nového řádku

        vlozitKomentar(&novaKapela->komentare, novaKapela->komentar);

        printf("Chcete vlozit dalsi komentar? Ano - A / Ne - N   :");
        if (scanf(" %c", &volba) != 1) {
            printf("Chyba pri nacitani volby.");
            free(novaKapela);
            return;
        }
        clearBuffer();

    } while (toupper(volba) == 'A');

    novaKapela->dalsi = NULL;

    vlozSeznamPodleNazvu(prvni, novaKapela);
}


void odebrat(const char* znacka, struct Kapela** prvni) {
    struct Kapela* aktKapela = *prvni;

    while (aktKapela && strcmp(aktKapela->znacka, znacka) == 0) {
        struct Kapela* novaPrvni = aktKapela->dalsi;
        free(aktKapela);
        aktKapela = novaPrvni;
    }

    aktKapela = *prvni;
    while (aktKapela && aktKapela->dalsi) {
        if (strcmp(aktKapela->dalsi->znacka, znacka) == 0) {
            struct Kapela* novaDalsi = aktKapela->dalsi->dalsi;
            free(aktKapela->dalsi);
            aktKapela->dalsi = novaDalsi;
        }
        aktKapela = aktKapela->dalsi;
    }
}

void vyhledat(const char* hledanyNazev, const struct Kapela* prvni) {
    const struct Kapela* aktKapela = prvni;
    int nalezeno = 0;

    printf("\nVysledek vyhledavani:\n");

    while (aktKapela) {
        if (strstr(aktKapela->znacka, hledanyNazev) != NULL) {
            printf("Nazev Kapely: %s\n Hodnoceni: %d\n", aktKapela->znacka, aktKapela->hodnoceni);
            printf("Komentare:\n");
            vypisKomentare(aktKapela->komentare);
            printf("\n");
            nalezeno = 1;
        }
        aktKapela = aktKapela->dalsi;
    }

    if (!nalezeno) {
        printf("Zadna kapela s hledanym nazvem nenalezena.\n");
    }
}

void vypisSeznam(const struct Kapela* prvni) {
    const struct Kapela* aktKapela = prvni;

    while (aktKapela) {
        printf("\nNazev Kapely: %s\n Hodnoceni: %d\n", aktKapela->znacka, aktKapela->hodnoceni);
        printf("Komentare:\n");
        vypisKomentare(aktKapela->komentare);
        printf("\n");

        aktKapela = aktKapela->dalsi;
    }
}

void smazatKapelu(struct Kapela** prvni, const char* hledanyNazev) {
    struct Kapela* predchozi = NULL;
    struct Kapela* aktKapela = *prvni;

    while (aktKapela != NULL) {
        if (strstr(aktKapela->znacka, hledanyNazev) != NULL) {
            printf("\nKapela k odstraneni:\n");
            vypisSeznam(aktKapela);

            // Dotaz na potvrzení smazání
            char volba;
            printf("Opravdu chcete kapelu odstranit? (A/N): ");
            if (scanf(" %c", &volba) != 1) {
                printf("Chyba pri nacitani volby.");
                exit(1);
            }

            if (toupper(volba) == 'A') {
                if (predchozi == NULL) {
                    *prvni = aktKapela->dalsi;
                }
                else {
                    predchozi->dalsi = aktKapela->dalsi;
                }

                // Uvolnění paměti pro komentáře
                struct Komentar* aktKomentar = aktKapela->komentare;
                while (aktKomentar) {
                    struct Komentar* novyKomentar = aktKomentar->dalsi;
                    free(aktKomentar);
                    aktKomentar = novyKomentar;
                }

                // Uvolnění paměti pro samotnou kapelu
                free(aktKapela);

                printf("Kapela byla uspesne odstranena.\n");
                return; // Důležité - ukončit funkci po smazání kapely
            }
            else {
                printf("Odstraneni kapely zruseno.\n");
                return; // Důležité - ukončit funkci po zrušení odstranění
            }
        }

        predchozi = aktKapela;
        aktKapela = aktKapela->dalsi;
    }

    printf("Zadna kapela s hledanym nazvem nenalezena.\n");
}


int main() {
    struct Kapela* prvni = NULL;
    char cmd;

    do {
        system("cls");
        printf("Vitejte v databazi kapel!\n");
        printf("A: Pridat novou kapelu   ");
        printf("P: Vypsat cely seznam kapel   ");
        printf("S: Vyhledat kapelu podle nazvu   ");
        printf("D: Smazat kapelu   ");
        printf("Q: Konec\n\n");

        printf("Vyber moznost: ");
        if (scanf(" %c", &cmd) != 1) {
            printf("Chyba pri nacitani volby.");
            exit(1);
        }

        switch (toupper(cmd)) {
        case 'A':
            pridat(&prvni);
            break;

        case 'D': {
            char hledanyNazev[ZNACKA_SIZE];
            printf("\nZadejte nazev kapely k odstraneni: ");
            if (scanf("%99s", hledanyNazev) != 1) {
                printf("Chyba pri nacitani hledaneho nazvu kapely.");
                exit(1);
            }
            clearBuffer();  // Nové přidání pro odstranění nového řádku v bufferu
            smazatKapelu(&prvni, hledanyNazev);
            break;
        }


        case 'P':
            printf("N: Seradit podle nazvu\nH: Seradit podle hodnoceni\nVyber moznost: ");
            if (scanf(" %c", &cmd) != 1) {
                printf("Chyba pri nacitani volby.");
                exit(1);
            }

            if (toupper(cmd) == 'N') {
                seradPodleNazvu(&prvni);
            }
            else if (toupper(cmd) == 'H') {
                seradPodleHodnoceni(&prvni);
            }

            vypisSeznam(prvni);
            while (getchar() != '\n') {
            }
            break;

        case 'S': {
            char hledanyNazev[ZNACKA_SIZE];
            printf("\nZadejte nazev kapely k vyhledani: ");
            if (scanf("%99s", hledanyNazev) != 1) {
                printf("Chyba pri nacitani hledaneho nazvu kapely.");
                exit(1);
            }
            vyhledat(hledanyNazev, prvni);
            while (getchar() != '\n') {
            }
            break;
        }
        }
        clearBuffer();  // Pro odstranění nového řádku v bufferu
    } while (toupper(cmd) != 'Q');

    // Uvolnění paměti při ukončení programu
    while (prvni) {
        struct Kapela* novaPrvni = prvni->dalsi;
        free(prvni);
        prvni = novaPrvni;
    }

    return 0;
}
