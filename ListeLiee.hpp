#pragma once
#include "iterateur.hpp"
#include "gsl/gsl_assert"
template<typename T> class Iterateur;
template<typename T>
class ListeLiee
{
public:
	//TODO: La construction par défaut doit créer une liste vide valide.
	ListeLiee() : taille_(0){}
	~ListeLiee()
	{
		//TODO: Enlever la tête à répétition jusqu'à ce qu'il ne reste aucun élément.
		// Pour enlever la tête, 
		// 1. La tête doit devenir le suivant de la tête actuelle.
		// 2. Ne pas oublier de désallouer le noeud de l'ancienne tête (si pas fait automatiquement).

		if (tete_ != nullptr)
		{
			Noeud<T>* noeud = tete_;
			tete_ = tete_->suivant_;
			delete noeud;
		}

	}

	bool estVide() const  { return taille_ == 0; }
	unsigned size() const { return taille_; }
	//NOTE: to_address permet que ce même code fonctionne que vous utilisiez des pointeurs bruts ou intelligents (ça prend le pointeur brut associé au pointeur intelligent, s'il est intelligent).
	Iterateur<T> begin()  { return {to_address(tete_)}; }
	Iterateur<T> end()    { return {to_address(queue_->suivant_)}; }

	// Ajoute à la fin de la liste.
	void push_back(const T& item)
	{
		//TODO: Vous devez créer un nouveau noeud en mémoire.
		//TODO: Si la liste était vide, ce nouveau noeud est la tête et la queue;
		// autrement, ajustez la queue et pointeur(s) adjacent(s) en conséquence.
		Noeud<T>* newNoeud = new Noeud(item);
		if (estVide())
		{ 
			tete_ = newNoeud;
			queue_ = tete_;
			newNoeud->precedent_ = nullptr;
		}
		else
		{
			queue_->suivant_ = newNoeud;
			newNoeud->precedent_ = queue_;
			queue_ = newNoeud;
		}
		++taille_;
		//cout << newNoeud << endl;
		queue_->suivant_ = Noeud<T>::past_end;
	}

	// Insère avant la position de l'itérateur.
	Iterateur<T> insert(Iterateur<T> it, const T& item)
	{
		//NOTE: Vous n'avez pas à supporter l'insertion à la fin (avant "past the end"),
		// ni l'insertion au début (avant la tête), dans cette méthode.
		//TODO: Pour commencer, vérifiez si l'itérateur est "past the end".
		// Si c'est le cas, on ajoute à la fin de la liste et on retourne
		// l'itérateur vers l'élément ajouté.
		// Autrement, procédez comme suit
		// 1. Créez un nouveau noeud initialisé avec item.
		// 2. Modifiez les attributs suivant_ et precedent_ du nouveau noeud
		//    afin que le nouveau noeud soit lié au noeud précédent et suivant
		//    à l'endroit où il est inséré selon notre itérateur.
		// 3. Modifiez l'attribut precedent_ du noeud après la position d'insertion
		//    (l'itérateur) afin qu'il pointe vers le noeud créé.
		// 4. Modifiez l'attribut suivant_ du noeud avant la position d'insertion
		//    (précédent de l'itérateur) afin qu'il point vers le noeud créé.
		// 5. Incrémentez la taille de la liste.
		// 6. Retournez un nouvel itérateur initialisé au nouveau noeud.
		if (it.position_ == Noeud<T>::past_end)
		{
			push_back(item);
			return Iterateur<T>(queue_);
		}
		else
		{
			Noeud<T>* apres = it.position_;
			Noeud<T>* avant = apres->precedent_;

			Noeud<T>* newNoeud = new Noeud<T>(item);

			newNoeud->suivant_ = apres;
			newNoeud->precedent_ = avant;

			apres->precedent_ = newNoeud;
			avant->suivant_ = newNoeud;

			++taille_;

			return Iterateur<T>(newNoeud);
		}
	}

	// Enlève l'élément à la position it et retourne un itérateur vers le suivant.
	Iterateur<T> erase(Iterateur<T> it)
	{
		//TODO: Enlever l'élément à la position de l'itérateur.
		//  1. Le pointeur vers le Noeud à effacer est celui dans l'itérateur.
		//  2. Modifiez l'attribut suivant_ du noeud précédent pour que celui-ci
		//     pointe vers le noeud suivant la position de l'itérateur (voir 1.).
		//  3. Modifiez l'attribut precedent_ du noeud suivant la position de
		//     l'itérateur pour que celui-ci pointe vers le noeud précédent
		//     de la position de l'itérateur (voir 1.).
		//  4. Désallouez le Noeud à effacer (voir 1.).
		//  5. Décrémentez la taille de la liste
		//  6. Retournez un itérateur vers le suivant du Noeud effacé.
		//TODO: On veut supporter d'enlever le premier élément de la liste,
		//  donc en 2. il se peut qu'il n'y ait pas de précédent et alors c'est
		//  la tête de liste qu'il faut ajuster.
		//NOTE: On ne demande pas de supporter d'effacer le dernier élément (c'est similaire au cas pour enlever le premier).
		if (tete_ == it.position_)
		{
			it.position_->suivant_->precedent_ = nullptr;
			tete_ = it.position_->suivant_;
		}
		else
		{
			it.position_->suivant_->precedent_ = it.position_->precedent_;
			it.position_->precedent_->suivant_ = it.position_->suivant_;
		}
		Noeud<T>* suivant = it.position_->suivant_;
		delete it.position_;
		--taille_;
		return Iterateur<T>(suivant);
	}

private:
	gsl::owner<Noeud<T>*> tete_ = Noeud<T>::past_end;
	Noeud<T>* queue_ = Noeud<T>::past_end;
	unsigned taille_;
	friend class Iterateur<T>;
};
