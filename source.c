/**
 * \file main.c
 * \brief Programme du jeu de YAMS
 * \author Enzo MAROS
 * \version 0.1
 * \date 10 Novembre 2021
 * 
 * Ce programme permet à deux joueurs de se battre sur une partie
 * de YAMS. Le programme gère l'affichage des feuilles de score,
 * des dés, et calcule les points à chaque tour.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

/** 
 * \def TAILLE_FEUILLE 
 * \brief Taille d'une feuille de score 
 */
/** 
 * \def TAILLE_COMBI
 * \brief Taille du nom d'une combinaison 
 */
/** 
 * \def TAILLE_NOM
 * o\brief Taille du nom d'un joueur (\0 compris)
 */
/**
 * \def CASE_VIDE
 * \brief Valeur d'une case vide
 */
/** 
 * \def NB_DES
 * \brief Nombre de dés 
 */
#define TAILLE_FEUILLE 13   
#define TAILLE_COMBI 13
#define TAILLE_NOM 14
#define CASE_VIDE -1
#define NB_DES 5

/** 
 * \def I_BRELAN
 * \brief Indice du brelan dans le tableau 
 */
/** 
 * \def I_CARRE
 * \brief Indice du carré dans le tableau 
 */
/** 
 * \def I_FULL_H
 * \brief Indice du full house dans le tableau 
 */
/** 
 * \def I_P_SUITE
 * \brief Indice de la petite suite dans le tableau 
 */
/** 
 * \def I_G_SUITE
 * \brief Indice de la grande suite dans le tableau 
 */
/** 
 * \def I_YAMS
 * \brief Indice du yams dans le tableau 
 */
/** 
 * \def I_CHANCE
 * \brief Indice de la chance dans le tableau 
 */
#define I_BRELAN 6
#define I_CARRE 7
#define I_FULL_H 8
#define I_P_SUITE 9
#define I_G_SUITE 10
#define I_YAMS 11
#define I_CHANCE 12


/** 
 * \typedef int t_feuille[TAILLE_FEUILLE]
 * \brief Feuille de marque 
 */
/** 
 * \typedef int t_des[NB_DES]
 * \brief Tableau de dés
 */
/**
 * \typedef char nom[TAILLE_NOM]
 * \brief Nom d'un joueur
 */
/** 
 * \typedef char combi[TAILLE_COMBI]
 * \brief Nom d'une combinaison
 */
typedef int t_feuille[TAILLE_FEUILLE];
typedef int t_des[NB_DES];
typedef char nom[TAILLE_NOM];
typedef char combi[TAILLE_COMBI];

/**
 * \fn void lire_clavier(char entree[])
 * \brief Lit une entrée au clavier, espaces compris. Retourne une chaine vide si rien est entré
 * \param entree La chaine de caractère entrée
 */
void lire_clavier(char entree[]) {
    entree[0] = '\0';
    scanf("%[^\n]", entree);
    getchar();
}

/**
 * \fn initialiser_feuilles(t_feuille t1, t_feuille t2)
 * \brief Met toutes les cases des deux feuilles à CASE_VIDE
 * \param t1 La première feuille
 * \param t2 La seconde feuille
 */
void initialiser_feuilles(t_feuille t1, t_feuille t2) {
    for (int i = 0; i < TAILLE_FEUILLE; i++) {
        t1[i] = CASE_VIDE;
        t2[i] = CASE_VIDE;
    }
}

/**
 * \fn nom_est_valide(const char nom_joueur[100])
 * \brief Vérifie qu'un nom de joueur est valide
 * \param nom_joueur La chaine contenant le nom du joueur
 * \return Vrai si le nom est valide, faux sinon
 */
bool nom_est_valide(const char nom_joueur[100]) {
    bool resultat;
    resultat = true;
    // vérifier si le nom est vide
    if (nom_joueur[0] == '\0') {
        resultat = false;
        printf("Nom vide\n");
    }
    // vérifier si le nom ne dépasse pas la taille
    if (strlen(nom_joueur) >= TAILLE_NOM) {
        printf("Nom de 13 caractères maximum\n");
        resultat = false;
    }

    return resultat;
}

/**
 * \fn recuperer_nom_joueur(int numero_joueur, nom nom_joueur)
 * \brief Demande la saisie d'un nom par l'utilisateur
 * \param numero_joueur Le numéro du joueur
 * \param nom_joueur Le nom du joueur renvoyé
 */
void recuperer_nom_joueur(int numero_joueur, nom nom_joueur) {
    char entree[100];   // chaine de caractères plus grande que le nom

    do {
        printf("Nom du joueur %d (sans accent) : ", numero_joueur);
        lire_clavier(entree);
    } while(!nom_est_valide(entree));

    // On copie les caractères dans le nom final
    strcpy(nom_joueur, entree);
}

/**
 * \fn void formater_nom_joueur(const nom nom_joueur, char resultat[])
 * \brief Centre et rajoute les bordures au nom du joueur
 * \param nom_joueur Le nom du joueur à formater
 * \param resultat Le texte une fois formaté
 */
void formater_nom_joueur(const nom nom_joueur, char resultat[]) {
    const int TAILLE_COL = TAILLE_NOM + 1;  // taille de la colonne
    const int MARGE = 1;                    // taille d'une marge
    const char C_BORD[] = "─";
    const char C_MARGE[] = " ";

    int taille_total;                       // taille totale à répartir entre les bords
    int taille_bord_g, taille_bord_d;       // taille du bord gauche et du bord droit
    size_t taille_joueur;                   // taille du nom du joueur
    int i;

    taille_joueur = strlen(nom_joueur);

    // les bords remplissent l'espace qui n'est pris
    // ni par les 2 marges ni par le nom du joueur
    taille_total = TAILLE_COL - taille_joueur - 2*MARGE;
    // le bord gauche est plus long d'un caractère si 
    // texte ne peut être centré correctement
    taille_bord_d = taille_total / 2;
    taille_bord_g = taille_bord_d + (taille_total % 2);

    strcpy(resultat, "");
    // on insère le bord gauche
    for (i = 0; i < taille_bord_g; i++) {
        strcat(resultat, C_BORD);
    }
    // puis le nom avec ses marges
    strcat(resultat, C_MARGE);
    strcat(resultat, nom_joueur);
    strcat(resultat, C_MARGE);
    // puis le bord droit
    for (i = 0; i < taille_bord_d; i++) {
        strcat(resultat, C_BORD);
    }
}

/**
 * \fn void formater_valeur(int valeur, char resultat[]) 
 * \brief Transforme une valeur chiffrée d'une case du tableau en texte
 * \param valeur La valeur chiffrée
 * \param resultat Le texte retourné 
 */
void formater_valeur(int valeur, char resultat[]) {
    strcpy(resultat, "   ");
    if (valeur != CASE_VIDE) {
        sprintf(resultat, "%3d", valeur);
    }
}

/**
 * \fn void calculer_totaux(const t_feuille feuille_joueur, int *t_sup, int *t_inf, int *t, int *bonus)
 * \brief Calcule les totaux et le bonus à partir d'une feuille de marque
 * \param feuille_joueur La feuille de marque du joueur
 * \param t_sup Le total supérieur. Vaut CASE_VIDE si les cases supérieures sont vides
 * \param t_inf Le total inférieur. Vaut CASE_VIDE si les cases inférieures sont vides
 * \param t Le total des deux. Vaut CASE_VIDE si les deux totaux sont vides
 * \param bonus Le bonus. Vaut CASE_VIDE si pas de bonus
 */
void calculer_totaux(const t_feuille feuille_joueur, int *t_sup, int *t_inf, int *t, int *bonus) {
    const int BONUS = 35;
    const int BORNE_BONUS = 62;
    // le dernier indice du tableau de la partie supérieure
    const int BORNE_SUP = 6;
    // si toutes les cases sont vides pour le supérieur et l'inférieur
    bool vide_sup, vide_inf;
    int i;

    // total supérieur
    *t_sup = 0;
    vide_sup = true;
    for (i = 0; i < BORNE_SUP; i++) {
        if (feuille_joueur[i] != CASE_VIDE) {
            vide_sup = false;
            *t_sup = *t_sup + feuille_joueur[i];
        }
    }

    // total inférieur
    *t_inf = 0;
    vide_inf = true;
    for (i = BORNE_SUP; i < TAILLE_FEUILLE; i++) {
        if (feuille_joueur[i] != CASE_VIDE) {
            vide_inf = false;
            *t_inf = *t_inf + feuille_joueur[i];
        }
    }

    // bonus
    *bonus = CASE_VIDE;
    if (*t_sup > BORNE_BONUS) {
        *bonus = BONUS;
        *t_sup = *t_sup + BONUS;
    }

    // on n'affiche pas de total quand les cases sont vides
    if (vide_sup && vide_inf) {
        *t = CASE_VIDE;
        *t_inf = CASE_VIDE;
        *t_sup = CASE_VIDE;
    }
    else if (vide_sup) {
        *t_sup = CASE_VIDE;
        *t = *t_inf;
    }
    else if (vide_inf) {
        *t_inf = CASE_VIDE;
        *t = *t_sup;
    }
    else {
        *t = *t_sup + *t_inf;
    }
}

/**
 * \fn void afficher_ligne(const char texte[], int valeur)
 * \brief Affiche une ligne de la feuille de marque
 * \param texte Le texte à afficher dans la colonne de gauche
 * \param valeur La valeur à formater et afficher dans la colonne de droite 
 */
void afficher_ligne(const char texte[], int valeur) {
    char case_txt[4];

    formater_valeur(valeur, case_txt);
    printf("│ %-20s │      %s      │\n", texte, case_txt);
}

/**
 * \fn void afficher_feuille_marque(const nom nom_joueur, const t_feuille feuille_joueur)
 * \brief Affiche la feuille de marque d'un joueur
 * \param nom_joueur Le nom du joueur à afficher en haut de la colonne de droite
 * \param feuille_joueur Les scores du joueur
 */ 
void afficher_feuille_marque(const nom nom_joueur, const t_feuille feuille_joueur) {
    // Un caractère de bordure est un caractère composé,
    // (c-a-d un caractère s'étalant sur 3 octets)
    // On calcule ici la taille maximum du nom formaté maximum
    const int T_NOM_FORMATE = 3 * TAILLE_NOM;

    char nom_formate[T_NOM_FORMATE];    // le nom avec les bordures
    int total_sup, total_inf, total;    // les totaux
    int bonus;                          // la valeur du bonus
    char texte[20];                     // texte de la colonne de gauche

    calculer_totaux(feuille_joueur, &total_sup, &total_inf, &total, &bonus);

    /* PARTIE HAUTE TABLEAU */
    formater_nom_joueur(nom_joueur, nom_formate);
    printf("┌──────────────────────┬%s┐\n", nom_formate);

    // afficher les 6 première cases
    for (int i = 0; i < 6; i++) {
        sprintf(texte, "%1$d [total de %1$d]", i+1);    // ex: 5 [total de 5]
        afficher_ligne(texte, feuille_joueur[i]);
    }

    afficher_ligne("Bonus si > à 62 [35]", bonus);
    afficher_ligne("Total superieur", total_sup);
    printf("└──────────────────────┴───────────────┘\n\n");

    /* PARTIE BASSE TABLEAU */
    printf("┌──────────────────────┬───────────────┐\n");
    afficher_ligne("Brelan       [total]", feuille_joueur[I_BRELAN]);
    afficher_ligne("Carré        [total]", feuille_joueur[I_CARRE]);
    afficher_ligne("Full House   [25]", feuille_joueur[I_FULL_H]);
    afficher_ligne("Petite suite [30]", feuille_joueur[I_P_SUITE]);
    afficher_ligne("Grande suite [40]", feuille_joueur[I_G_SUITE]);
    afficher_ligne("Yams         [50]", feuille_joueur[I_YAMS]);
    afficher_ligne("Chance       [total]", feuille_joueur[I_CHANCE]);
    afficher_ligne("Total inferieur ", total_inf);
    printf("└──────────────────────┴───────────────┘\n\n");

    /* PARTIE TOTAL TABLEAU */
    printf("┌──────────────────────┬───────────────┐\n");
    afficher_ligne("Total", total);
    printf("└──────────────────────┴───────────────┘\n");
}

/**
 * \fn bool oui_non(const char texte[]) 
 * \brief Demande à l'utilisateur de saisir oui ou non, et redemande si besoin
 * \param texte Texte accompagnant la demande
 * \return true si l'utilisateur rentre oui, false s'il rentre non
 */
bool oui_non(const char texte[]) {
    bool correct, resultat;
    char entree[100];   // une entrée plus grande que "oui" et "non"

    do {
        // On récupère l'entrée utilisateur
        printf(texte);
        lire_clavier(entree);
        // on met le texte en minuscule pour la comparaison, ce qui permet
        // d'accepter les mots oui et non quelque soit la casse (OUI, Oui...)
        for (int i = 0; i < strlen(entree); i++) {
            entree[i] = tolower(entree[i]);
        }

        // Début des tests
        correct = true;
        if (strcmp(entree, "oui") == 0) {
            resultat = true;
        } 
        else if (strcmp(entree, "non") == 0) {
            resultat = false;
        } 
        else if (entree[0] == '\0') {
            // on vérifie si le choix est vide
            printf("Choix vide\n\n");
            correct = false;
        }
        else {
            printf("Veuillez saisir \"oui\" ou \"non\"\n\n");
            correct = false;
        }
    } while (!correct);

    return resultat;
}

/**
 * \fn void trier_des(t_des liste_des)
 * \brief Tries dans l'ordre croissant les 5 dés pour faciliter les calculs de combinaisons
 * \param liste_des La liste des dés à trier
 */
void trier_des(t_des liste_des) {
    // Implémentation du tri par sélection
    int ind_min, i, j;
    int temp;       // variable temporaire pour l'échange

    for (i = 0; i < NB_DES - 1; i++) {
        ind_min = i;
        for (j = i+1; j < NB_DES; j++) {
            if (liste_des[ind_min] > liste_des[j])
                ind_min = j;
        }

        // on échange le minimum avec la case i
        temp = liste_des[i];
        liste_des[i] = liste_des[ind_min];
        liste_des[ind_min] = temp;
    }
}

/**
 * \fn afficher_des(t_des liste_des)
 * \brief Affiche la liste des dés, chaque dé ayant une bordure
 * \param liste_des La liste des dés à afficher
 */
void afficher_des(t_des liste_des) {
    printf("    ╭───╮  ╭───╮  ╭───╮  ╭───╮  ╭───╮\n    ");
    for (int i = 0; i < NB_DES; i++) {
        printf("│ %d │  ", liste_des[i]);
    }
    printf("\n");
    printf("    ╰───╯  ╰───╯  ╰───╯  ╰───╯  ╰───╯\n");
    printf(" n°   1      2      3      4      5  \n\n");
}

/**
 * \fn choix_des(t_des a_lancer)
 * \brief Demande à l'utilisateur les dés qu'il souhaite relancer
 * \param a_lancer Tableau en sortie contenant 1 si le dé doit être relancé, 0 sinon
 */
void choix_des(t_des a_lancer) {
    const char FIN[] = "fin";
    char entree[100];       // réponse entrée par l'utilisateur (volontairement plus grande)
    int indice_de;          // L'indice du dé une fois transformé en nombre

    // on initialise les dés à relancer à 0
    for (int i = 0; i < NB_DES; i++)
        a_lancer[i] = 0;

    printf("Veuillez entrer les dés à relancer, un\n");
    printf("dé par ligne, puis terminez par \"fin\"\n");

    printf("(1, 2...) ");
    lire_clavier(entree);

    // on met le texte en minuscules pour autoriser différentes écritures de fin (Fin, FIN...)
    for (int i = 0; i < strlen(entree); i++)
        entree[i] = tolower(entree[i]);

    while (strcmp(FIN, entree) != 0) {
        if (entree[0] == '\0') {
            printf("Choix vide\n");
        }
        // vérifier qu'il s'agit d'un chiffre
        else if (strlen(entree) != 1 || !isdigit(entree[0])) {
            printf("Nom de dé incorrect\n");
        }
        else {
            // dans le cas où il s'agit bien d'un chiffre, on l'extrait
            // et on vérifie qu'il est compris entre 1 et 5 et qu'il n'a
            // pas déjà été entré
            sscanf(entree, "%d", &indice_de);
            if (indice_de > NB_DES || indice_de < 1) {
                printf("Nom de dé incorrect\n");
            }
            else if (a_lancer[indice_de - 1] == 1) {
                printf("Dé déjà saisi\n");
            }
            else {
                a_lancer[indice_de - 1] = 1;
            }
        }

        printf("(1, 2...) ");
        lire_clavier(entree);

        // on met le texte en minuscules pour autoriser différentes écritures de fin (Fin, FIN...)
        for (int i = 0; i < strlen(entree); i++)
            entree[i] = tolower(entree[i]);
    }
}

/**
 * \fn lancer_des(t_des liste_des)
 * \brief Gère le lancement aléatoire des dés
 * \param liste_des Les dés triés définitifs triés dans l'ordre croissant
 */
void lancer_des(t_des liste_des) {
    const int MAX_TOURS = 3;
    t_des a_lancer = {1, 1, 1, 1, 1};   // les dés à lancer (1 = lancer, 0 = garder)
    bool relancer;                      // s'il faut relancer les dés
    int tours, i;

    printf("Appuyez sur entrée pour lancer les dés... ");
    getchar();
    printf("\n");

    relancer = true;
    tours = 1;

    while (relancer && tours <= MAX_TOURS) {
        // lancer les dés nécessaires
        for (i = 0; i < NB_DES; i++) {
            if (a_lancer[i] == 1)
                liste_des[i] = rand() % 6 + 1;
        }

        printf("Les dés sont lancés, vous obtenez : \n");
        afficher_des(liste_des);

        // Ne pas demander à relancer au dernier tour
        if (tours != MAX_TOURS) {
            relancer = oui_non("Souhaitez-vous relancer les dés ?\n(oui / non) : ");
            printf("\n");

            if (relancer)
                choix_des(a_lancer);
            
            printf("\n");
        }

        tours++;
    }

    // à ce stade, les dés sont définitifs et ne sont plus affichés
    // on les trie pour faciliter l'utilisation dans les fonctions
    trier_des(liste_des);
}

/**
 * \fn int somme_des(t_des liste_des)
 * \brief Calcule la somme de toutes les valeurs des dés
 * \param liste_des La liste des dés
 * \return La somme des valeurs des dés 
 */
int somme_des(t_des liste_des) {
    int somme;

    somme = 0;
    for (int i = 0; i < NB_DES; i++)
        somme = somme + liste_des[i];

    return somme;
}

/**
 * \fn bool suite_identique(t_des liste_des, int taille_suite)
 * \brief Vérifie si un certain nombre de dés sont identiques
 * \param liste_des La liste des dés triée
 * \param taille_suite Le nombre de dés identiques à rechercher
 * \return Vrai si la suite de dés est présente, faux sinon
 */
bool suite_identique(t_des liste_des, int taille_suite) {
    int occurence, i, j;

    i = 0;
    do {
        // on vérifie le nombre de fois que le dé apparait dans les cases suivantes
        occurence = 1;
        for (j = 1; j < taille_suite; j++)
            occurence += (liste_des[i] == liste_des[i+j]);

        i++;
    } while(occurence != taille_suite && i <= NB_DES - taille_suite);

    // le nombre attendu de dé à suivre est trouvé ?
    return occurence == taille_suite;
}

/**
 * \fn int brelan(t_des liste_des)
 * \brief Vérifie si un brelan est présent et calcule le nombre de points
 * \param liste_des La liste des dés triée
 * \return 0 si pas de brelan, ou le nombre de points
 */
int brelan(t_des liste_des) {
    const int TAILLE_BRELAN = 3;
    int resultat = 0;

    if (suite_identique(liste_des, TAILLE_BRELAN))
        resultat = somme_des(liste_des);

    return resultat;
}

/**
 * \fn int carre(t_des liste_des)
 * \brief Vérifie si un carre est présent et calcule le nombre de points
 * \param liste_des La liste des dés triée
 * \return 0 si pas de carré, ou le nombre de points
 */
int carre(t_des liste_des) {
    const int TAILLE_CARRE = 4;
    int resultat = 0;

    if (suite_identique(liste_des, TAILLE_CARRE))
        resultat = somme_des(liste_des);

    return resultat;
}

/**
 * \fn int yams(t_des liste_des)
 * \brief Vérifie si un yams est présent et calcule le nombre de points
 * \param liste_des La liste des dés triée
 * \return 0 si pas de yams, ou 50 points
 */
int yams(t_des liste_des) {
    const int TAILLE_YAMS = 5;
    const int POINTS = 50;

    // Pas de point si la suite n'est pas trouvée, et 50 si elle l'est,
    // un booléen valant 0 ou 1
    return suite_identique(liste_des, TAILLE_YAMS) * POINTS;
}

/**
 * \fn int full_house(t_des liste_des)
 * \brief Vérifie si un full house est présent et calcule le nombre de points
 * \param liste_des La liste des dés triée
 * \return 0 si pas de full house, ou 25 points
 */
int full_house(t_des liste_des) {
    const int POINTS = 25;
    bool trouve = false;

    // dans un full house, les extrémités sont égales deux par deux,
    // et la case centrale est égale à l'une des deux extrémités
    trouve = (liste_des[0] == liste_des[1]) && (liste_des[3] == liste_des[4]);
    trouve = trouve && (liste_des[2] == liste_des[0] || liste_des[2] == liste_des[4]);

    // Pas de point si le full house n'est pas trouvée, et 25 si il l'est,
    // un booléen valant 0 ou 1
    return trouve * POINTS;
}

void dedoublonner(t_des liste_des, t_des des_uniques) {
    int ind_unique; // l'indice du dernier élément unique

    ind_unique = 0;
    des_uniques[0] = liste_des[0];

    // comme les dés sont triés, l'élément pointé dans le tableau
    // des dés est égal ou supérieur à celui pointé dans le tableau
    // des dés dédoublonnés
    for (int i = 1; i < NB_DES; i++) {
        des_uniques[i] = 0;
        if (des_uniques[ind_unique] != liste_des[i]) {
            ind_unique++;
            des_uniques[ind_unique] = liste_des[i];
        }
    }
}

/**
 * \fn int petite_suite(t_des liste_des)
 * \brief Vérifie si une petite suite est présent et calcule le nombre de points
 * \param liste_des La liste des dés triée
 * \return 0 si pas de petite suite, ou 30 points
 */
int petite_suite(t_des liste_des) {
    const int POINTS = 30;

    t_des des_uniques;  // la liste des dés sans doublons
    bool trouve;

    // on récupère les dés uniques
    dedoublonner(liste_des, des_uniques);

    // Deux cas possibles seulement
    trouve = (des_uniques[0] == des_uniques[3] - 3);
    trouve = trouve || (des_uniques[1] == des_uniques[4] - 3);

    // Pas de point si la suite n'est pas trouvée, et 30 si elle l'est,
    // un booléen valant 0 ou 1
    return trouve * POINTS;
}

/**
 * \fn int grande_suite(t_des liste_des)
 * \brief Vérifie si une grande suite est présent et calcule le nombre de points
 * \param liste_des La liste des dés triée
 * \return 0 si pas de grande suite, ou 40 points
 */
int grande_suite(t_des liste_des) {
    const int POINTS = 40;
    
    t_des des_uniques;  // la liste des dés sans doublons
    bool trouve;

    // on récupère les dés uniques
    dedoublonner(liste_des, des_uniques);

    // Un seul cas possible
    trouve = (des_uniques[0] == des_uniques[4] - 4);

    // Pas de point si la suite n'est pas trouvée, et 40 si elle l'est,
    // un booléen valant 0 ou 1
    return trouve * POINTS;
}

/**
 * \fn int chance(t_des liste_des)
 * \brief Calcule la somme des dés
 * \param liste_des La liste des dés triée
 * \return La somme des valeurs de tous les dés 
 */
int chance(t_des liste_des) {
    return somme_des(liste_des);
}

/**
 * \fn int total_de(t_des liste_des, int valeur)
 * \brief Calcule la somme des dés ayant la même valeur
 * \param liste_des La liste des dés triée
 * \param valeur La valeur du dé à additionner
 * \return La somme de tous les dés de cette valeur
 */
int total_de(t_des liste_des, int valeur) {
    int somme;

    somme = 0;
    for (int i = 0; i < NB_DES; i++) {
        if (liste_des[i] == valeur)
            somme = somme + liste_des[i];
    }

    return somme;
}

/**
 * \fn void rechercher_combinaisons(t_feuille feuille_joueur, t_des liste_des, t_feuille combi_possible)
 * \brief Recherche l'ensemble des combinaisons jouables et les points rapportés par chaque
 * \param feuille_joueur La feuille du marque du joueur, pour ne pas proposer des combinaisons déjà prises
 * \param liste_des La liste des dés triée
 * \param combi_possible La liste des points par combinaison
 */
void rechercher_combinaisons(t_feuille feuille_joueur, t_des liste_des, t_feuille combi_possible) {
    int points;
    
    // on calcule le nombre de points pour chaque combinaison si elle était choisie
    for (int i = 0; i < TAILLE_FEUILLE; i++) {
        combi_possible[i] = CASE_VIDE;

        if (feuille_joueur[i] == CASE_VIDE) {
            // si la case de la feuille de marque est déjà remplie, on ne calcule pas,
            // le choix de la combinaison étant impossible
            if (i < 6) {
                points = total_de(liste_des, i+1);
            }
            else {
                switch (i) {
                    case I_BRELAN:
                        points = brelan(liste_des);
                        break;

                    case I_CARRE:
                        points = carre(liste_des);
                        break;

                    case I_FULL_H:
                        points = full_house(liste_des);
                        break;

                    case I_P_SUITE:
                        points = petite_suite(liste_des);
                        break;

                    case I_G_SUITE:
                        points = grande_suite(liste_des);
                        break;

                    case I_YAMS:
                        points = yams(liste_des);
                        break;
                    
                    case I_CHANCE:
                        points = chance(liste_des);
                        break;
                    
                    default:
                        printf("Erreur dans l'indice de la combinaison !\n");
                        break;
                }
            }

            combi_possible[i] = points;
        }
    }
}

/**
 * \fn void indice_vers_combi(int indice, combi nom_combi)
 * \brief Transforme l'indice d'une combinaison dans la feuille de marque en texte 
 */
void indice_vers_combi(int indice, combi nom_combi) {
    if (indice >= 0 && indice < 6) {
        sprintf(nom_combi, "%d", indice+1);
    } 
    else {
        switch(indice) {
            case I_BRELAN:
                strcpy(nom_combi, "Brelan");
                break;
            case I_CARRE:
                strcpy(nom_combi, "Carré");
                break;
            case I_CHANCE:
                strcpy(nom_combi, "Chance");
                break;
            case I_FULL_H:
                strcpy(nom_combi, "Full House");
                break;
            case I_G_SUITE:
                strcpy(nom_combi, "Grande Suite");
                break;
            case I_P_SUITE:
                strcpy(nom_combi, "Petite Suite");
                break;
            case I_YAMS:
                strcpy(nom_combi, "Yams");
                break;
            default:
                printf("Erreur dans l'indice de la combinaison\n");
        }
    }
}

/**
 * \fn int combi_vers_indice(combi nom_combi)
 * \brief Récupère l'indice correspondant au texte nom_combi dans la feuille de marque
 * \param nom_combi Le nom de la combinaison
 * \return L'indice de la case, ou -1 si elle n'existe pas
 */
int combi_vers_indice(combi nom_combi) {
    const combi LISTE_COMBI[] = {"1", "2", "3", "4", "5", "6", "brelan", "carre", "full house", 
        "petite suite", "grande suite", "yams", "chance"};
    
    // on passe le nom en minuscules pour prendre en compte toutes les écritures
    for (int i = 0; i < strlen(nom_combi); i++) {
        nom_combi[i] = tolower(nom_combi[i]);
    }

    int indice;
    bool trouve;
    
    trouve = false;
    indice = 0;
    while(!trouve && indice < TAILLE_FEUILLE) {
        trouve = (strcmp(nom_combi, LISTE_COMBI[indice]) == 0);
        if (!trouve)
            indice++;
    }

    if (!trouve)
        indice = -1;    // en cas d'erreur

    return indice;
}

/**
 * \fn void choisir_combinaison(t_feuille feuille_joueur, t_des liste_des)
 * \brief Affiche à l'utilisateur les combinaisons qui lui rapportent des point, puis demande quelle combinaison choisir
 * \param feuille_joueur La feuille de marque du joueur
 * \param liste_des Les dés triés
 */
void choisir_combinaison(t_feuille feuille_joueur, t_des liste_des) {
    t_feuille combi_possibles;  // les points rapportés par chaque combinaison si jouée
    bool zeros;                 // tous les dés sont à zéro (ou CASE_VIDE)
    char entree[100];           // l'entrée utilisateur volontairement plus grande
    bool correct;               // l'entrée est correcte
    int indice;                 // l'indice de la case à compléter
    combi nom_combinaison;

    // Etape 1 : Afficher les combinaisons possibles
    zeros = true;
    rechercher_combinaisons(feuille_joueur, liste_des, combi_possibles);

    for (int i = 0; i < TAILLE_FEUILLE; i++) {
        if (combi_possibles[i] > 0) {
            if (zeros) {
                // si aucune combinaison n'a été affichée
                zeros = false;
                printf("Vous pouvez remplir les cases suivantes : \n");
            }

            indice_vers_combi(i, nom_combinaison);
            printf(" - %s (%d points)\n", nom_combinaison, combi_possibles[i]);
        }
    }

    if (zeros) {
        // si aucune combinaison n'a été affichée
        printf("Aucune combinaison ne vous rapporte de points\n");
    }

    // Etape 2 : Choisir la combinaison
    do {
        printf("Quel est votre choix : ");
        lire_clavier(entree);

        correct = false;
        if (strlen(entree) == 0) {
            printf("Choix vide\n");
        }
        else if (strlen(entree) > TAILLE_COMBI-1) {
            printf("Case inexistante\n");
        }
        else {
            indice = combi_vers_indice(entree);
            if (indice == -1) {
                printf("Case inexistante\n");
            }
            else if (combi_possibles[indice] == CASE_VIDE) {
                printf("Vous ne pouvez pas jouer cette case\n");
            }
            else {
                if (combi_possibles[indice] == 0) {
                    printf("Attention, la combinaison ne\n");
                    printf("rapporte rien\n");
                }
                correct = true;
                feuille_joueur[indice] = combi_possibles[indice];
            }
        }
    } while(!correct);
}

/**
 * \fn void afficher_gagnant(nom nom1, nom nom2, t_feuille feuille1, t_feuille feuille2)
 * \brief Affiche les totaux finaux de chaque joueur sous forme de petit tableau
 * \param nom1 Le nom du joueur 1
 * \param nom2 Le nom du joueur 2
 * \param feuille1 La feuille de marque du joueur 1
 * \param feuille2 La feuille de marque du joueur 2
 */
void afficher_gagnant(nom nom1, nom nom2, t_feuille feuille1, t_feuille feuille2) {
    int total_j1, total_sup_j1, total_inf_j1;   // les totaux du joueur 1
    int total_j2, total_sup_j2, total_inf_j2;   // les totaux du joueur 2
    int bonus_temp;                             // le bonus temporaire car non compté

    // calcul des totaux
    calculer_totaux(feuille1, &total_sup_j1, &total_inf_j1, &total_j1, &bonus_temp);
    calculer_totaux(feuille2, &total_sup_j2, &total_inf_j2, &total_j2, &bonus_temp);

    printf("\n");
    printf("┌────────────────┬─────── Totaux ──────┐\n");
    printf("│ %-14s │ %3d + %3d = %3d     │\n", nom1, total_sup_j1, total_inf_j1, total_j1);
    printf("│ %-14s │ %3d + %3d = %3d     │\n", nom2, total_sup_j2, total_inf_j2, total_j2);
    printf("└────────────────┴─────────────────────┘\n");
}

/**
 * \fn int main()
 * \brief Fonction principale
 * \return EXIT_SUCCESS
 */
int main() {
    t_feuille feuille_j1, feuille_j2;
    nom joueur1, joueur2;
    t_des des;

    initialiser_feuilles(feuille_j1, feuille_j2);
    recuperer_nom_joueur(1, joueur1);
    do {
        recuperer_nom_joueur(2, joueur2);
        if (strcmp(joueur1, joueur2) == 0) {
            printf("Nom de joueur déjà pris\n");
        }
    } while(strcmp(joueur1, joueur2) == 0);

    // On initialise l'horloge pour l'aléatoire
    srand(time(NULL));

    for (int i = 0; i < TAILLE_FEUILLE; i++) {
        // Chaque case de la feuille de marque doit être remplie
        
        // Joueur 1
        afficher_feuille_marque(joueur1, feuille_j1);
        lancer_des(des);
        choisir_combinaison(feuille_j1, des);

        // Joueur 2
        afficher_feuille_marque(joueur2, feuille_j2);
        lancer_des(des);
        choisir_combinaison(feuille_j2, des);
    }

    afficher_feuille_marque(joueur1, feuille_j1);
    afficher_feuille_marque(joueur2, feuille_j2);
    printf("\n");
    afficher_gagnant(joueur1, joueur2, feuille_j1, feuille_j2);

    return EXIT_SUCCESS;
}
