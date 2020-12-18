#include <stdio.h>   /* pour les entrées-sorties */
#include <string.h>  /* pour les manipulations de chaînes de caractères */
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>
#include "rep.h"

#define VERSION 3.0
#define SQUELET
/**************************************************************************/
/*   Nom : JUCHNIEWICZ             Prénom :	Pierre                        */
/**************************************************************************/

extern bool modif;


/**********************************************************************/
/*  Ajout d'un contact dans le répertoire stocké en mémoire           */
/**********************************************************************/

int ajouter_un_contact_dans_rep(Repertoire* rep, Enregistrement enr)
{
#ifdef IMPL_TAB
	int idx;

	if (rep->nb_elts < MAX_ENREG)
	{
		idx = rep->nb_elts;
		*((rep->tab) + idx) = enr;	//ajout dans la tab du répertoire l'enregistrement du nom/prénom/tél à la position idx
		(rep->nb_elts)++;			//à chaque ajout de contact le nombre d'éléments du répertoire augmente
		modif = true;				//on a modifié le tableau...
		rep->est_trie = false;		//...sans le trier

	}
	else {
		return(ERROR);
	}


#else
#ifdef IMPL_LIST

	bool inserted = false;
	if (rep->nb_elts == 0) {
		if (InsertElementAt(rep->liste, rep->liste->size, enr) != 0) {
			rep->nb_elts += 1;
			modif = true;
			rep->est_trie = true;
			return(OK);
		}

	}
	else {
		//on va trier directement
		SingleLinkedListElem* current = GetElementAt(rep->liste, 0);
		int pos = 0;
		while (!inserted && current != NULL) {
			if (!est_sup(enr, current->pers)) { // test si un contact se trouve après dans l'ordre alphabétique par rapport à un autre contact
				InsertElementAt(rep->liste, pos, enr);
				inserted = true;
				rep->nb_elts++;
				rep->est_trie = true;	//la chaine est bien triée
			}
			else
			{
				current = current->next; //on passe au suivant
				pos++;

			}

		}
		if (!inserted) {
			rep->nb_elts += 1;
			modif = true;
			rep->est_trie = true;
			return(OK);
		}


#endif

#endif


		return(OK);

	} /* fin ajout */

	  /**********************************************************************/
	  /* supprime du répertoire l'enregistrement dont l'indice est donné en */
	  /*   paramètre       et place modif = true                            */
	  /**********************************************************************/
#ifdef IMPL_TAB
	void supprimer_un_contact_dans_rep(Repertoire * rep, int indice) {

		if (rep->nb_elts >= 1) {		//il faut au moins un élément dans le tableau pour pouvoir supprimer					
			for (int i = indice; i < rep->nb_elts - 1; i++) {
				*(rep->tab + i) = *(rep->tab + i + 1);	//on remplace l'élément à supprimer par celui qui le suit
			}
			rep->nb_elts -= 1;		//on a un élément en moins
			modif = true;	//il y a eu modification
		}

		return;
	} /* fin supprimer */

#else
#ifdef IMPL_LIST
  /************************************************************************/
  /* supprime du répertoire l'enregistrement contenu dans le maillon elem */
  /*                   et fixe modif à vrai                              */
  /************************************************************************/
  // complet

	int supprimer_un_contact_dans_rep_liste(Repertoire * rep, SingleLinkedListElem * elem) {

		int ret = DeleteLinkedListElem(rep->liste, elem);
		if (ret == 1) {
			rep->nb_elts--;
			modif = true;
		}

		return (0);
	}
#endif
#endif


	/**********************************************************************/
	/*  fonction d'affichage d'un enregistrement sur une ligne à l'écran  */
	/* ex Dupont, Jean                 0320304050                         */
	/**********************************************************************/
	void affichage_enreg(Enregistrement enr)
	{
		printf("%s, %s          %s\n\n", enr.nom, enr.prenom, enr.tel);
	} /* fin affichage_enreg */




	  /**********************************************************************/
	  /*  fonction d'affichage d'un enregistrement avec alignement des mots */
	  /* pour les listes                                                    */
	  /* ex | Dupont                |Jean                  |0320304050      */
	  /**********************************************************************/
	void affichage_enreg_frmt(Enregistrement enr)
	{
		printf("\n|%-20s		|%-20s		|%s\n", enr.nom, enr.prenom, enr.tel);	//-20 permet d'alligner 
	} /* fin affichage_enreg */




	  /**********************************************************************/
	  /* test si dans l'ordre alphabetique, un enregistrement est apres     */
	  /* un autre                                                           */
	  /**********************************************************************/
	bool est_sup(Enregistrement enr1, Enregistrement enr2)
	{

		if (_strcmpi(enr1.nom, enr2.nom) < 0) return false; 					//on compare les noms 
		else if (_strcmpi(enr1.nom, enr2.nom) > 0) return true;
		else {
			if (_strcmpi(enr1.prenom, enr2.prenom) < 0) return false;			//on compare les prénoms
			else if (_strcmpi(enr1.prenom, enr2.prenom) > 0) return true;
			else return false;
		}
	}


	/*********************************************************************/
	/*   Tri Alphabetique du tableau d'enregistrements                   */
	/*********************************************************************/

	void trier(Repertoire * rep)
	{

#ifdef IMPL_TAB
		bool permute = true;
		Enregistrement tmp;
		if (rep->est_trie) return;
		int tabSize = rep->nb_elts;
		if (tabSize > 1) {
			for (int j = 0; j < tabSize; j++)
			{
				for (int i = 0; i < (tabSize - j - 1); i++)
				{
					permute = est_sup(*(rep->tab + i + 1), *(rep->tab + i));
					if (permute == false)
					{
						tmp = *(rep->tab + i);
						*(rep->tab + i) = *(rep->tab + i + 1);
						*(rep->tab + i + 1) = tmp;
					}
				}
			}
		}
		else permute = true;




#else
#ifdef IMPL_LIST
		// ajouter code ici pour Liste
		// rien à faire !
		// la liste est toujours triée
#endif
#endif


		rep->est_trie = true;

	} /* fin trier */

	  /**********************************************************************/
	  /* recherche dans le répertoire d'un enregistrement correspondant au  */
	  /*   nom à partir de l'indice ind                                     */
	  /*   retourne l'indice de l'enregistrement correspondant au critère ou*/
	  /*   un entier négatif si la recherche est négative				    */
	  /**********************************************************************/

	int rechercher_nom(Repertoire * rep, char nom[], int ind)
	{
		int i = ind;		    /* position (indice) de début de recherche dans tableau/liste rep */
		int ind_fin;			/* position (indice) de fin de tableau/liste rep */

		char tmp_nom[MAX_NOM];	/* 2 variables temporaires dans lesquelles */
		char tmp_nom2[MAX_NOM];	/* on place la chaine recherchee et la chaine lue dans le */
								/* tableau, afin de les convertir en majuscules et les comparer */
		bool trouve = false;


#ifdef IMPL_TAB
		ind_fin = rep->nb_elts;
		strcpy_s(tmp_nom, _countof(tmp_nom), nom);
		_strupr_s(tmp_nom, _countof(tmp_nom)); //On met le nom en majuscule
		while (trouve == false)
		{
			for (int i = 0; i < ind_fin; i++)
			{
				strcpy_s(tmp_nom2, _countof(tmp_nom2), rep->tab[i].nom);
				_strupr_s(tmp_nom2, _countof(tmp_nom2)); //On met le nom2 en majuscule

				if (strcmp(tmp_nom, tmp_nom2) == 0) //test d'égalité sur les deux noms
				{
					trouve = true;
					return i;
				}
			}
			return -1;
		}



#else
#ifdef IMPL_LIST
		//même schéma que précédemment
		ind_fin = rep->nb_elts;
		strcpy_s(tmp_nom, _countof(tmp_nom), nom);
		_strupr_s(tmp_nom, _countof(tmp_nom));
		while (trouve == false)
		{
			for (int i = 0; i < ind_fin; i++)
			{
				strcpy_s(tmp_nom2, _countof(tmp_nom2), GetElementAt(rep->liste, i)->pers.nom);
				_strupr_s(tmp_nom2, _countof(tmp_nom2));

				if (strcmp(tmp_nom, tmp_nom2) == 0)
				{
					trouve = true;
					return i;
				}
			}
			return -1;
		}

#endif
#endif

		return((trouve) ? i : -1);
	} /* fin rechercher_nom */

	  /*********************************************************************/
	  /* Supprimer tous les caracteres non numériques de la chaines        */
	  /*********************************************************************/
	void compact(char* s)
	{
		int size = strlen(s); //taille de s
		int i = 0;
		while (s[i] != '\0') {
			if (isdigit(s[i]) == 0) {
				for (int j = i; s[j + 1] != '\0'; j++) {
					s[j] = s[j + 1];
				}
				s[size - 1] = '\0'; //on réduit la taille
				size--;
			}
			else ++i;
		}
		return;
	}

	/**********************************************************************/
	/* sauvegarde le répertoire dans le fichier dont le nom est passé en  */
	/* argument                                                           */
	/* retourne OK si la sauvegarde a fonctionné ou ERROR sinon           */
	/**********************************************************************/
	int sauvegarder(Repertoire * rep, char nom_fichier[])
	{
		FILE* fic_rep;
#ifdef IMPL_TAB
		errno_t err;
		char buffer[sizeof(Enregistrement) + 1];
		err = fopen_s(&fic_rep, nom_fichier, "w");
		if (err != 0 || fic_rep == NULL) return ERROR;
		for (int i = 0; i < rep->nb_elts; i++)
		{
			sprintf_s(buffer, sizeof(buffer), "%s%c%s%c%s\n", rep->tab[i].nom, SEPARATEUR, rep->tab[i].prenom, SEPARATEUR, rep->tab[i].tel);
			fputs(buffer, fic_rep);
		}
		if (feof(fic_rep)) {
			fclose(fic_rep);
		}


#else
#ifdef IMPL_LIST
		err = fopen_s(&Fic_rep, nom_fichier, "w");
		if (err != 0 || Fic_rep == NULL) { return ERROR; }
		for (int i = 0; i < rep->nb_elts; i++)
		{
			sprintf_s(buffer, sizeof(buffer), "%s%c%s%c%s\n", GetElementAt(rep->liste, i)->pers.nom, SEPARATEUR, GetElementAt(rep->liste, i)->pers.prenom, SEPARATEUR, GetElementAt(rep->liste, i)->pers.tel); //Stock la serie de caractères dans le buffer
			fputs(buffer, Fic_rep);
		}
		if (feof(Fic_rep)) {
			fclose(Fic_rep);
		}
#endif
#endif

		return(OK);
	} /* fin sauvegarder */


	  /**********************************************************************/
	  /*   charge dans le répertoire le contenu du fichier dont le nom est  */
	  /*   passé en argument                                                */
	  /*   retourne OK si le chargement a fonctionné et ERROR sinon         */
	  /**********************************************************************/

	int charger(Repertoire * rep, char nom_fichier[])
	{
		FILE* fic_rep;					/* le fichier */
		errno_t err;
		int num_rec = 0;						/* index sur enregistrements */
		int long_max_rec = sizeof(Enregistrement);
		char buffer[sizeof(Enregistrement) + 1];
		int idx = 0;

		char* char_nw_line;

		_set_errno(0);
		if (((err = fopen_s(&fic_rep, nom_fichier, "r")) != 0) || (fic_rep == NULL))
		{
			return(err);
		}
		else
		{
			while (!feof(fic_rep) && (rep->nb_elts < MAX_ENREG))
			{
				if (fgets(buffer, long_max_rec, fic_rep) != NULL)
				{
					/* memorisation de l'enregistrement lu dans le tableau */
					buffer[long_max_rec] = 0;			/* en principe il y a deja un fin_de_chaine, cf fgets */

					if ((char_nw_line = strchr(buffer, '\n')) != NULL)
						*char_nw_line = '\0';			/* suppression du fin_de_ligne eventuel */

					idx = 0;								/* analyse depuis le debut de la ligne */
#ifdef IMPL_TAB
					if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].nom, MAX_NOM, SEPARATEUR) == OK)
					{
						idx++;							/* on saute le separateur */
						if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].prenom, MAX_NOM, SEPARATEUR) == OK)
						{
							idx++;
							if (lire_champ_suivant(buffer, &idx, rep->tab[num_rec].tel, MAX_TEL, SEPARATEUR) == OK)
								num_rec++;		/* element à priori correct, on le comptabilise */
						}
					}
#else
#ifdef IMPL_LIST
					Enregistrement enr;
					if (lire_champ_suivant(buffer, &idx, enr.nom, MAX_NOM, SEPARATEUR) == OK)
					{
						idx++;
						if (lire_champ_suivant(buffer, &idx, enr.prenom, MAX_NOM, SEPARATEUR) == OK)
						{
							idx++;
							if (lire_champ_suivant(buffer, &idx, enr.tel, MAX_TEL, SEPARATEUR) == OK)

								InsertElementAt(rep->liste, rep->liste->size, enr);

							num_rec++;

						}
					}
#endif
#endif




				}

			}
			rep->nb_elts = num_rec;
			fclose(fic_rep);
			return(OK);
		}


	}