#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define histogram mapa[l][m]-'A'//pozri riadok 36
#define zaciatslov  slovo[0]-'A'
#define  N 4

//Prostredie CLion, Daniel Gašparík.
char **nacitaj(char **mapa, int *riadky, int *stlpce, FILE *fr, int *pozicie[], int pocet[]) {
    fscanf(fr, "%d %d\n", riadky, stlpce);
    char c;
    int rcount = 0;
    int scount = 0;//pocita stlpce
    int count = 1;
    mapa = (char **) malloc(*riadky * sizeof(char *));//malloc 2d pola
    for (int k = 0; k < *stlpce; k++) {
        mapa[k] = (char *) malloc(*stlpce * sizeof(char));
    }
    //načítanie osemsmerovky do 2D poľa
    while ((c = getc(fr))) {
        if (c == '\n') {
            scount = 0;
            rcount++;//pocita riadky
            count++;//pocita riadky aby ich mohol porovnat kedy skonci osemsmerovka
        }
        if (count == *riadky + 1) { //nastavenie file pointera na zaciatok slova pre tajnicku
            fseek(fr, -1, SEEK_CUR);
            break;
        }
        if (c >= 'A' && c <= 'Z') {
            mapa[rcount][scount++] = c;
        }
    }
    //realokovanie pola indexov a zapísanie do pola indexov podľa výskytu písmen
    for (int l = 0; l < *riadky; l++) {
        for (int m = 0; m < *stlpce; m++) {
            if (pocet[histogram] ==0) {//histogram zistuje poziciu v poli indexov podla pravidla 0=A,1=B,2=C; toto som dosiahol odcitanim 'A'cka od charu na pozici mapa[l][m]
                pozicie[histogram] = (int *) realloc(pozicie[histogram],((N + pocet[histogram]) * 2) * sizeof(int));//realloc
                pozicie[histogram][0] = l;
                pozicie[histogram][1] = m;
                pocet[histogram]++;
            } else {
                pozicie[histogram] = (int *) realloc(pozicie[histogram],((N + pocet[mapa[l][m] - 'A']) * 2) * sizeof(int));
                pozicie[histogram][pocet[histogram] *2] = l;//toto je 300IQ kód (Copyright Gašparík 2020) ktorý zväčšuje indexy podla vyskytu, ked sa Acko vyskytuje 0krát zapíše suradnice na indexoch 0 a 1, ked sa vyskituje už 1 krát zapíše na indexoch 1 a 2
                pozicie[histogram][pocet[histogram] * 2 + 1] = m;
                pozicie[histogram][pocet[histogram] * 2 + 2] = -1;
                pozicie[histogram][pocet[histogram] * 2 + 3] = -1;
                pocet[histogram]++;
            }
        }
    }
    return mapa;
}

void zhora(char **mapa, int x, int y, unsigned int len, char *slovo, int *riadky,int *podarilo) {//funkcia ktora hlada vyskyt slova zhora kym neprejdeme hranice poctu riadkov a zaroven neprejdeme dlzku slova
    int i, a, b;      //x a y je oznacenie riadok a stlpec pre prve pismenko zo slova
    i = a = b = 0;        //tento riadok a stlpec dostaneme z pola indexov konkretneho pismenka vo funkcii zisti, cize ked berieme slovo lata, tak sem pride prva dvojica vyskytu pismena l a potom dalsia v dalsej iteraci foru vo funkcii zisti.
    int nachadza = 0;
    for (i = 0, a = x, b = y; i < len && a <*riadky; i++, a++) {//podmienky ktore sa nachadzaju v kazdom smere kde si nastavime aby sme neprešli začiatkom/koncom alebo nezašli za dĺžku slova
        if (mapa[a][b] == slovo[i] ||
            mapa[a][b] == tolower(slovo[i])) {//skusi ci pismeno zo slova == pismenu z tajnicky
            nachadza = 1;
        } else {
            nachadza = 0;
            break;
        }
    }
    if (i == len && nachadza == 1) {//ak sme prešli po dĺžku slova a zaroven sme našli všetky písmená
        for (i = 0, a = x, b = y; i < len && a < *riadky; i++, a++) {//prepíšeme nájdené na malé
            mapa[a][b] = tolower(mapa[a][b]);
        }
        *podarilo = 1;
    }
}

void zdola(char **mapa, int x, int y, unsigned int len, char *slovo, int *podarilo) {

    int i, c, d;
    i = c = d = 0;
    int nachadza = 0;
    for (i = 0, c = x, d = y; i < len && c > -1; i++, c--) {
        if (mapa[c][d] == slovo[i] || mapa[c][d] == tolower(slovo[i])) {
            nachadza = 1;
        } else {
            nachadza = 0;
            break;
        }
    }
    if (i == len && nachadza == 1) {
        for (i = 0, c = x, d = y; i < len && c > -1; i++, c--) {
            mapa[c][d] = tolower(mapa[c][d]);
        }
    }
    *podarilo = 1;

}

void zprava(char **mapa, int x, int y, unsigned int len, char *slovo, int *podarilo) {
    int i, a, b;
    i = a = b = 0;
    int nachadza = 0;
    for (i = 0, a = x, b = y; i < len && b > -1; b--, i++) {
        if (mapa[a][b] == slovo[i] || mapa[a][b] == tolower(slovo[i])) {
            nachadza = 1;
        } else {
            nachadza = 0;
            break;
        }
    }
    if (i == len && nachadza == 1) {
        for (i = 0, a = x, b = y; i < len && b > -1; b--, i++) {
            mapa[a][b] = tolower(mapa[a][b]);
        }
        *podarilo = 1;
    }

}

void zlava(char **mapa, int x, int y, unsigned int len, char *slovo, int *stlpce, int *podarilo) {
    int i, a, b;
    i = a = b = 0;
    int nachadza = 0;
    for (i = 0, a = x, b = y; i < len && b < *stlpce; b++, i++) {
        if (mapa[a][b] == slovo[i] || mapa[a][b] == tolower(slovo[i])) {
            nachadza = 1;
        } else {
            nachadza = 0;
            break;
        }
    }
    if (i == len && nachadza == 1) {
        for (i = 0, a = x, b = y; i < len && b < *stlpce; b++, i++) {
            mapa[a][b] = tolower(mapa[a][b]);
        }
        *podarilo = 1;
    }

}

void diaglh(char **mapa, int x, int y, unsigned int len, char *slovo, int *stlpce, int *riadky, int *podarilo) {
    int i, a, b;
    i = a = b = 0;
    int nachadza = 0;
    for (i = 0, a = x, b = y; i < len && a < *riadky && b < *stlpce; a++, b++, i++) {
        if (mapa[a][b] == slovo[i] || mapa[a][b] == tolower(slovo[i])) {
            nachadza = 1;
        } else {
            nachadza = 0;
            break;
        }
    }
    if (i == len && nachadza == 1) {
        for (i = 0, a = x, b = y; i < len && b < *stlpce && a < *riadky; a++, b++, i++) {
            mapa[a][b] = tolower(mapa[a][b]);
        }
        *podarilo = 1;
    }

}

void diagph(char **mapa, int x, int y, unsigned int len, char *slovo, int *stlpce, int *riadky, int *podarilo) {
    int i, a, b;
    i = a = b = 0;
    int nachadza = 0;
    for (i = 0, a = x, b = y; i < len && a < *riadky && b > -1; a++, b--, i++) {
        if (mapa[a][b] == slovo[i] || mapa[a][b] == tolower(slovo[i])) {
            nachadza = 1;
        } else {
            nachadza = 0;
            break;
        }
    }
    if (i == len && nachadza == 1) {
        for (i = 0, a = x, b = y; i < len && b > -1 && a < *riadky; a++, b--, i++) {
            mapa[a][b] = tolower(mapa[a][b]);
        }
        *podarilo = 1;
    }

}

void diagld(char **mapa, int x, int y, unsigned int len, char *slovo, int *stlpce, int *riadky, int *podarilo) {
    int i, a, b;
    i = a = b = 0;
    int nachadza = 0;
    for (i = 0, a = x, b = y; i < len && a > -1 && b < *stlpce; a--, b++, i++) {
        if (mapa[a][b] == slovo[i] || mapa[a][b] == tolower(slovo[i])) {
            nachadza = 1;
        } else {
            nachadza = 0;
            break;
        }
    }
    if (i == len && nachadza == 1) {
        for (i = 0, a = x, b = y; i < len && b < *stlpce && a > -1; a--, b++, i++) {
            mapa[a][b] = tolower(mapa[a][b]);
        }
        *podarilo = 1;
    }

}

void diagpd(char **mapa, int x, int y, unsigned int len, char *slovo, int *stlpce, int *riadky, int *podarilo) {
    int i, a, b;
    i = a = b = 0;
    int nachadza = 0;
    for (i = 0, a = x, b = y; i < len && a > -1 && b > -1; a--, b--, i++) {
        if (mapa[a][b] == slovo[i] || mapa[a][b] == tolower(slovo[i])) {
            nachadza = 1;
        } else {
            nachadza = 0;
            break;
        }
    }
    if (i == len && nachadza == 1) {
        for (i = 0, a = x, b = y; i < len && b > -1 && a > -1; a--, b--, i++) {
            mapa[a][b] = tolower(mapa[a][b]);
        }
        *podarilo = 1;
    }
}

void zisti(char **mapa, int *pozicie[], char *slovo, int *riadky, int *stlpce, int pocet[]) {
    int podarilo = 0;
    if (pozicie[zaciatslov][0] > -1) {
        for (int i = 0, j = 0, k = 1; i <pocet[zaciatslov]; i++, j++, k += 2) {//passujeme zakladne pozicie po argumentoch 0,1  2,3  4,5  z pola indexov(cize v prvej iteraci to budu hodnoty z  index[0][1] v druhej index[2][3] atd...),, ideme po pocet vyskytov daneho zaciatocneho pismenka zo slova
            zhora(mapa, pozicie[zaciatslov][j * 2], pozicie[slovo[0] - 'A'][k], strlen(slovo), slovo, riadky,&podarilo);//zaciatslov určuje začiatočné písmenko slova v dec forme čiže ak je písmeno A tak prejde na index 0 kedze 'A'-'A'
            zdola(mapa, pozicie[zaciatslov][j * 2], pozicie[zaciatslov][k], strlen(slovo), slovo, &podarilo);// je 0;
            zprava(mapa, pozicie[zaciatslov][j * 2], pozicie[zaciatslov][k], strlen(slovo), slovo,&podarilo);// skusame vsetky smery pre danu dvojicu z pola indexov
            zlava(mapa, pozicie[zaciatslov][j * 2], pozicie[zaciatslov][k], strlen(slovo), slovo, stlpce, &podarilo);
            diaglh(mapa, pozicie[zaciatslov][j * 2], pozicie[zaciatslov][k], strlen(slovo), slovo, stlpce, riadky,
                   &podarilo);
            diagph(mapa, pozicie[zaciatslov][j * 2], pozicie[zaciatslov][k], strlen(slovo), slovo, stlpce, riadky,
                   &podarilo);
            diagld(mapa, pozicie[zaciatslov][j * 2], pozicie[zaciatslov][k], strlen(slovo), slovo, stlpce, riadky,
                   &podarilo);
            diagpd(mapa, pozicie[zaciatslov][j * 2], pozicie[zaciatslov][k], strlen(slovo), slovo, stlpce, riadky,
                   &podarilo);
        }
    }
    if (podarilo == 0) {
        printf("Slovo %s sa nepodarilo najst\n", slovo);
    }
}

void uvolni(char **mapa, int *pozicie[], char *slovo, FILE *fr, int pocet[]) {//uvolnenie polí a ukoncenie suboru
    for (int i = 0; i < sizeof(mapa) / sizeof(char *); i++) {
        free(mapa[i]);
    }
    for (int k = 0; k < 26; k++) {
        if (pocet[k] > 0) {
            free(pozicie[k]);
        }
    }
    free(slovo);
    if (fclose(fr) == EOF) {
        printf("nepodarilo sa zatvorit subor");
        exit(1);
    }

}

void tajnicka(char **mapa, int *riadky, int *stlpce) {//vypis zostatkovej tajnicky
    int prazdna = 1;
    for (int i = 0; i < *riadky; i++) {
        for (int j = 0; j < *stlpce; j++) {
            if (mapa[i][j] >= 'A' && mapa[i][j] <= 'Z') {
                printf("%c", mapa[i][j]);
                prazdna = 0;
            }
        }
    }
    if (prazdna == 1) {
        printf("Tajnicka je prazdna\n;");
    }
}

void vypisindex(int *pozicie[], int pocet[]) {
    for (int i = 0; i < 26; i++) {
        if (pocet[i] > 0) { printf("%c: ", 'A' + i); }
        for (int j = 0; j < pocet[i] * 2; j++) {
            if (pozicie[i][j] > -1 && pocet[i] > 0) {
                printf("%d ", pozicie[i][j]);
            }
        }
        if (pocet[i] > 0) { printf("\n"); }
    }
}

int kontrolaSum(char **mapa, int riadky, int stlpce) {
    int parneSum = 0;
    int neparneSum = 0;
    for (int i = 0; i < riadky; i++) {
        for (int j = 0; j < stlpce; j++) {
            if (i % 2 == 0) {
                if (mapa[i][j] >= 'A' && mapa[i][j] <= 'Z') {
                    parneSum += mapa[i][j] - 'A';
                } else {
                    parneSum += mapa[i][j] - 'a';
                }
            }
            if (j % 2 == 1) {
                if (mapa[i][j] >= 'A' && mapa[i][j] <= 'Z') {
                    neparneSum += mapa[i][j] - 'A';
                } else {
                    neparneSum += mapa[i][j] - 'a';
                }

            }
        }
    }
    if (parneSum == neparneSum) {
        return 1;
    }
    return 0;
}

void zmenIndex(int *pozicie[], char **mapa, int rvymen, int svymen, char c, int pocet[]) {
    char akejepismeno = toupper(mapa[rvymen][svymen]);//aby sme neriešili podmienky či je velke alebo male pismeno
    int zmen = c - 'A';//kod pismenka ktory chceme zmenit
    int *p = pocet;
    int *q = &pocet[akejepismeno - 'A'];
    for (int i = 0; i < pocet[akejepismeno - 'A'] * 2 + N; i++) {
        if (pozicie[akejepismeno - 'A'][i] == rvymen && pozicie[akejepismeno - 'A'][i + 1] == svymen) {
            for (int j = i; j < pocet[akejepismeno - 'A'] * 2 + N; j += 2) {
                if (c != akejepismeno) {//posuvanie a premazavanie
                    pozicie[akejepismeno - 'A'][j] = pozicie[akejepismeno - 'A'][j + 2];
                    pozicie[akejepismeno - 'A'][j + 1] = pozicie[akejepismeno - 'A'][j + 3];
                    *q -= 1;//odpocitanie poctu vyskytov zmazaneho pismenka
                }
            }
            break;
        }
    }

    if (c != akejepismeno) {
        pozicie[zmen][pocet[zmen] * 2] = rvymen;
        pozicie[zmen][pocet[zmen] * 2 + 1] = svymen;
        pozicie[zmen][pocet[zmen] * 2 + 2] = -1;
        *(p + zmen) += 1;//zvysime pocet vyskytu pismenka o 1 cez pointer
    }


    if (pozicie[akejepismeno - 'A'][0] == -1) {
        free(pozicie[akejepismeno - 'A']);
    }

}

int main() {
    FILE *fr;
    if ((fr = fopen("osemsmerovka.txt", "r")) == NULL) {
        printf("subor sa nepodarilo otvorit");
        exit(1);
    }//otvorenie suoboru
    int riadky, stlpce;//deklaracia riadkov a stlpcov
    int *pozicie[26] = {NULL};//deklaracia pola 26 pointrov na int- cize pole indexov
    int pocet[26] = {0};//deklaracia pola kde budeme merat vyskyt kazdeho pismenka
    char **mapa;//deklaracia osemsmerovky
    int rvymen, svymen;//premenne do ktorych sa zapisu indexy z ulohy
    char c;//znak ktory chceme vymenit
    for (int k = 0; k < 26; k++) {
        pozicie[k] = (int *) malloc((N * 2) * sizeof(int));//mallocovanie osemsmerovky a vyplnenie  -1tkou
        for (int j = 0; j < N * 2; j++) {
            pozicie[k][j] = -1;
        }
    }
    mapa = nacitaj(mapa, &riadky, &stlpce, fr, pozicie, pocet);//nacítanie mapy
    char *slovo = (char *) malloc(100 * sizeof(char));//nacítanie slova pre najviac 100 charov
    while ((fscanf(fr, "%s", slovo)) != EOF) {
        zisti(mapa, pozicie, slovo, &riadky, &stlpce,pocet);//zistenie ci sa slovo nachadza v kazdom smere a ak ano tak ho vyškrtne
        for (int u = 0; u < strlen(slovo); u++) {
            slovo[u] = '\0';//vymazanie predchadzajuceho slova
        };
        for (int p = 0; p < riadky; p++) {//vyprintovanie doterajšej tajničky
            for (int q = 0; q < stlpce; q++) {
                printf("%c", mapa[p][q]);
            }
            printf("\n");
        }
        printf("        \n");
    }
    printf("Tajnicka je: ");
    tajnicka(mapa, &riadky, &stlpce);//vyprintovanie finalnej tajničky
    kontrolaSum(mapa, riadky, stlpce) == 1 ? printf("\nSucty su rovnake") : printf("\nSucty nie su rovnake\n");
    scanf("%d %d %c", &rvymen, &svymen, &c);
    zmenIndex(pozicie, mapa, rvymen, svymen, c, pocet);//uloha1
    vypisindex(pozicie, pocet);//uloha2
    uvolni(mapa, pozicie, slovo, fr, pocet);//uvolnenie všetkých dynamických blokov a ukoncenie suboru :)
    return 0;//hotovo <3
}