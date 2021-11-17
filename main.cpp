#include "Personnage.hpp"
#include "Heros.hpp"
#include "Vilain.hpp"
#include "VilainHeros.hpp"
#include "ListeLiee.hpp"
#include <fstream>
#include <vector>
#include <functional>
#include <map>
#include "cppitertools/range.hpp"
#include "bibliotheque_cours.hpp"
using namespace std;
using namespace iter;

using UInt8  = uint8_t;
using UInt16 = uint16_t;

UInt8 lireUint8(istream& fichier)
{
	UInt8 valeur = 0;
	fichier.read(reinterpret_cast<char*>(&valeur), sizeof(valeur));
	return valeur;
}

UInt16 lireUint16(istream& fichier)
{
	UInt16 valeur = 0;
	fichier.read(reinterpret_cast<char*>(&valeur), sizeof(valeur));
	return valeur;
}

string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUint16(fichier));
	fichier.read(reinterpret_cast<char*>(&texte[0]), streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

template <typename T>
vector<T> lireFichier(istream& fichier)
{
	vector<T> elements;
	for ([[maybe_unused]] int i : range(lireUint16(fichier)))
		elements.push_back(T(fichier));
	return elements;
}

ifstream ouvrirLectureFichierBinaire(const string& nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	return fichier;
}

// Permet d'avoir une référence non-const à un objet temporaire.
template <typename T> T& temporaireModifiable(T&& objet) { return objet; }

template <typename T>
vector<T> lireFichier(const string& nomFichier)
{
	return lireFichier<T>(temporaireModifiable(
		ouvrirLectureFichierBinaire(nomFichier)));
}

template <typename T>
Iterateur<T> trouverParNom(ListeLiee<T>& liste, const string& nom)
{
	Iterateur<T> fin = liste.end();
	for (Iterateur<T> pos = liste.begin(); pos != fin; pos.avancer()) {
		if ((*pos).getNom() == nom)
			return pos;
	}
	return fin;
}

int main()
{
	#pragma region "Bibliothèque du cours"
	// Permet sous Windows les "ANSI escape code" pour changer de couleur
	// https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac
	// les supportent normalement par défaut.
	bibliotheque_cours::activerCouleursAnsi();
	#pragma endregion
	
	// Trait de separation
	static const string trait =
		"═════════════════════════════════════════════════════════════════════════";

	static const string separateurSections = "\033[95m" + trait + "\033[0m\n";
	static const string separateurElements = "\033[33m" + trait + "\033[0m\n";

	//{ Solutionnaire du TD4:
	vector<Heros> heros = lireFichier<Heros>("heros.bin");
	vector<Vilain> vilains = lireFichier<Vilain>("vilains.bin");
	vector<unique_ptr<Personnage>> peronnages;  // Doit être des pointeurs pour le polymorphisme, l'énoncé ne force pas les unique_ptr.

	//TODO: Transférez les héros du vecteur heros dans une ListeLiee.
	ListeLiee<Heros> herosListe;
	for (auto&& hero : heros)
	{
		herosListe.push_back(hero);
	}

	cout << "Partie 1: " << endl << endl;
	//TODO: Créez un itérateur sur la liste liée à la position du héros Alucard
	// Servez-vous de la fonction trouverParNom définie plus haut
	cout << separateurSections << endl;
	Iterateur<Heros> pos = trouverParNom(herosListe, "Alucard");
	cout << "Iterateur trouve : " << (*pos).getNom() << endl << endl;

	//TODO: Servez-vous de l'itérateur créé précédemment pour trouver l'héroine Aya Brea,
	// en sachant qu'elle se trouve plus loin dans la liste.
	cout << separateurSections << endl;
	Iterateur<Heros> fin = herosListe.end();
	Iterateur<Heros> newPos = nullptr;
	for (pos; pos != fin; pos.avancer())
	{
		if ((*pos).getNom() == "Aya Brea")
		{
			break;
		}
	}
	cout << "Iterateur trouve: " << (*pos).getNom() << endl << endl;

	//TODO: Ajouter un hero bidon à la liste avant Aya Brea en vous servant de l'itérateur.
	//TODO: Assurez-vous que la taille de la liste est correcte après l'ajout.
	cout << separateurSections << endl;
	cout << "Ancienne taille: " << herosListe.size() << endl;
	Heros newHeros("Spider-man", "Spider-man the game", "Venom");
	cout << "Nouvel iterateur bidon : " << (*herosListe.insert(pos, newHeros)).getNom() << endl;
	cout << "Nouvelle taille: " << herosListe.size() << endl << endl;

	//TODO: Reculez votre itérateur jusqu'au héros Mario et effacez-le en utilisant l'itérateur, puis affichez le héros suivant dans la liste (devrait êter "Naked Snake/John").
	//TODO: Assurez-vous que la taille de la liste est correcte après le retrait.
	cout << separateurSections << endl;

	for (pos; pos != fin; pos.reculer())
	{
		if ((*pos).getNom() == "Mario")
		{
			break;
		}
	}

	cout << "Iterateur retire: " << (*herosListe.erase(pos)).getNom() << endl << endl;

	//TODO: Effacez le premier élément de la liste.
	cout << separateurSections << endl;
	cout << "Iterateur retire: " << (*herosListe.erase(herosListe.begin())).getNom() << endl << endl;

	//TODO: Affichez votre liste de héros en utilisant un itérateur. La liste débute
	// avec le héros Randi et n'a pas Mario.
	// Servez-vous des methodes begin et end de la liste...
	cout << separateurSections << endl;
	cout << "Affichage de la liste de heros en utilisant un iterateur: " << endl << endl;
	for (Iterateur<Heros> it = herosListe.begin(); it != fin; it.avancer())
	{
		cout << separateurElements << endl;
		(*it).afficher(cout);
		cout << endl;
	}

	//TODO: Refaite le même affichage mais en utilisant une simple boucle "for" sur intervalle.
	cout << separateurSections << endl;
	cout << "Affichage de la liste de heros en utilisant une boucle sur intervalle: " << endl << endl;
	for (Heros hero1 : herosListe)
	{
		cout << separateurElements << endl;
		hero1.afficher(cout);
		cout << endl;
	}
	cout << separateurSections << endl;

	cout << "Partie 2: " << endl << endl;
	//TODO: Utilisez un conteneur pour avoir les héros en ordre alphabétique (voir point 2 de l'énoncé).
	// Creer conteneur MAP.

	//surement pas le best way to do it.

	map<string, Heros> conteneurHeros = {};
	for (Iterateur<Heros> it = herosListe.begin(); it != herosListe.end(); it.avancer())
	{
		conteneurHeros[(*it).getNom()] = *it;
	}
	for (auto&& [nom, hero1] : conteneurHeros)
		cout << nom << endl;
	/*string max = "ZZ";
	Iterateur<Heros> itMax = herosListe.begin();
	map<string, Heros> conteneurHeros = {};
	unsigned int size = herosListe.size();
	for (unsigned int i = 0; i < size; i++)
	{
		for (Iterateur<Heros> it = herosListe.begin(); it != herosListe.end(); it.avancer())
		{
			if (int((*it).getNom()[0]) == max[0] && int((*it).getNom()[0]) + int((*it).getNom()[1]) < max[0] + max[1]  || int((*it).getNom()[0]) < max[0])
			{
				max = (*it).getNom();
				itMax = it;
			}
		}
		conteneurHeros[(*itMax).getNom()] = *itMax;
		if (herosListe.size() > 1)
			herosListe.erase(itMax);
		max = "ZZ";
	}*/ // INUTILE CA SE CLASSE EN ORDRE ALPHABETIQUE AUTOMATIQUEMENT!!!!
	

	//2.2: L'indice de complexité de la map est de O(log(n)). En effet, Les élements d'une map sont disposés sous forme d'un "Tree structure", à chaque fois que l'on examine un noeud du circuit, on détermine si l'élement que l'on cherche est plus grand ou plus petit que le noeud lui-meme. Donc, normalement il serait nécessaire de faire cette démarche log2(n), car chaque comparaison enlève la moitier des possibilités.

	//2.3: La liste cherchera avec une complexité de O(n), car il doit passer chaque élément un par un (n*O(1)). Donc, pour une liste très courte, la liste serait plus rapide que la map, par contre le reste du temps la map sera plus rapide avec un indice de complexité de O(log(n)). Bref, pour un nombre d'élements moyen, il serait préférable d'y aller avec une map.


	//TODO: Assurez-vous de n'avoir aucune ligne non couverte dans les classes pour la liste liée.  Il peut y avoir des lignes non couvertes dans les personnages...
	for (unsigned int i = 0; i < herosListe.size(); i++)
	{
		herosListe.~ListeLiee();
	}
}
