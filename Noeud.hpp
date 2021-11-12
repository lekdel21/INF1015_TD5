#pragma once
#include <gsl/pointers>
template<typename T> class ListeLiee;
template<typename T> class Iterateur;
template<typename T>
class Noeud
{
public:
	//TODO: Constructeur(s).
	Noeud(const T& donnee) : donnee_(donnee){}
private:
	//TODO: Attributs d'un noeud.
	Noeud<T>* suivant_ = past_end;
	Noeud<T>* precedent_ = past_end;
	T donnee_;

	inline static constexpr Noeud* past_end = nullptr;

	friend class ListeLiee<T>;
	friend class Iterateur<T>;
};
