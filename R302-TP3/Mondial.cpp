/* 
 * File:   Mondial.cpp
 * Author: hb
 * 
 * Created on 22 novembre 2018, 16:05
 */

#include "Mondial.h"


#include <iostream>     // pour cout
#include <iomanip>      // pour setw()
#include <sstream>
#include <iterator>

Mondial::Mondial(const char *filename) {
    // Chargement du fichier XML en mémoire
    imageMondial.LoadFile(filename);
    // Initialisation de l'attribut racineMondial avec la racine (élément <mondial>)
    racineMondial = imageMondial.FirstChildElement();
}

void Mondial::Print() {
    imageMondial.Print();
}

/*
 * FOURNIE
 */
int Mondial::getNbAirports() const {
    // initialisation du nombre d’aéroports
    int nb = 0;
    // accéder à <airportscategory>, c’est un fils de l'élément <racineMondial>
    XMLElement *airportsCategory = racineMondial->FirstChildElement("airportscategory");
    // parcours complet des fils de <airportscategory> en les comptants
    // 1) accéder au premier fils <airport> de <airportscategory>
    XMLElement *currentAirport = airportsCategory->FirstChildElement();
    // 2) parcourir tous les <airport> qui sont des frères
    while (currentAirport != nullptr) {
        // un aéroport supplémentaire
        nb = nb + 1;
        // avancer au frère <airport> suivant de currentAirport
        currentAirport = currentAirport->NextSiblingElement();
    }
    // currentAirport n’a plus de frère {currentAirport == nullptr}, c’est le dernier
    return nb;
}

/*
 * FOURNIE
 */
void Mondial::printCountriesCode() const {
    int rank = 1; // rang du pays
    string carcodeValue; // valeur de l'attribut "car_cod" du pays courant
    // accéder à <countriescategory>, c’est un fils de l'élément <racineMondial>)
    XMLElement *countriesCategory = racineMondial->FirstChildElement("countriescategory");
    // parcours complet des fils de <countriescategory> en affichant le rang et le code
    // 1) accéder au premier fils <country> de <countriescategory>
    XMLElement *currentCountry = countriesCategory->FirstChildElement();
    // 2) parcourir tous les <country> qui sont des frères
    while (currentCountry != nullptr) {
        // traiter le pays courant
        //      1) récupérer la valeur de l’attribut "car_code"
        carcodeValue = currentCountry->Attribute("car_code");
        //      2) faire l’affichage
        cout << setw(5) << rank << " : " << carcodeValue << endl;
        // avancer au frère <country> suivant de currentCountry
        currentCountry = currentCountry->NextSiblingElement();
        // mettre à jour le rang
        rank = rank + 1;
    }
    // currentCountry n’a pas de frère {currentCountry == nullptr}, c’est fini
}


/*
 * A COMPLETER
 */
int Mondial::getNbDeserts() const {
    /*
     * A COMPLETER
     */
    //initialisation du nombre de déserts
    int nbDeserts = 0;
    // on accède à <desertsCategory>, fils de l'élément <racineMondial>
    XMLElement *desertsCategory = racineMondial->FirstChildElement("desertscategory");
    // parcours complet des fils de <desertsCategory> en les comptant
    //on accède au premier fils <desert> de <desertsCategory>
    XMLElement *currentDesert = desertsCategory->FirstChildElement();
    // on parcourt tous les <desert> qui sont des frères
    while (currentDesert != nullptr) {
        // +1 désert supplémentaire.
        nbDeserts++;
        // on avance au frère suivant de currentDesert
        currentDesert = currentDesert->NextSiblingElement();
    } // currentDesert == nullptr
    // currentDesert n’a plus de frère {}, c’est le dernier
    return nbDeserts;
}

/*
 * A COMPLETER
 */
int Mondial::getNbElemCat(const string categoryName) {
    // on initialise le nombre d'éléments de la catégorie donnnée
    int nbElemCat = 0;
    // accéder à "<elemsCategory>", c’est un fils de l'élément <racineMondial>
    // decod_category est une map dont la valeur associé est le nom du père de la catégorie chosie ( ex : desert -> desertscategory )
    // on utilise c_str pour transformer categoryName en char* ( FirstChildElement() ne peut pas convertir un string en char*)
    XMLElement *elemsCategory = racineMondial->FirstChildElement(decod_category[categoryName].c_str());
    // parcours complet des fils de "<elemsCategory>" en les comptant
    //on accède au premier fils <elem> de <elemsCategory>
    XMLElement *currentElem = elemsCategory->FirstChildElement();
    // on parcourt tous les <elem> qui sont des frères
    while (currentElem != nullptr) {
        // +1 <elem> supplémentaire.
        nbElemCat++;
        // on avance au frère suivant de currentElem
        currentElem = currentElem->NextSiblingElement();
    } // currentElem == nullptr
    // currentElem n’a plus de frère {}, c’est le dernier
    return nbElemCat;
}

/*
 * A COMPLETER
 */
XMLElement *Mondial::getCountryXmlelementFromNameRec(string countryName) const {
    // accéder à <countriescategory>, un fils de l'élément <racineMondial>
    XMLElement *countriesCategory = racineMondial->FirstChildElement("countriescategory");
    // on récupère le premier fils de <countriescategory> pour le passer au worker
    XMLElement *currentCountry = countriesCategory->FirstChildElement();

    return getCountryXmlelementFromNameRecWorker(currentCountry, countryName);
}


XMLElement *
Mondial::getCountryXmlelementFromNameRecWorker(XMLElement *currentCountryElement, string countryName) const {
    // si le pays que l'on cherche n'est pas présent
    if (currentCountryElement == nullptr) {
        return nullptr;
    } else if (currentCountryElement->FirstChildElement("name")->GetText() == countryName) {
        // si on trouve le pays c'est fini
        return currentCountryElement;
    } else {
        // si le pays courant n'est pas pareil que countryName on relance la fonction
        return getCountryXmlelementFromNameRecWorker(currentCountryElement->NextSiblingElement(), countryName);
    }
}

/*
 * A COMPLETER
 */
string Mondial::getCountryCodeFromName(string countryName) const throw(PrecondVioleeExcep) {
    // on met le résultat de la fonction récursive dans une variable pour pouvoir vérifier que le résultat n'est pas nullptr
    XMLElement *country = getCountryXmlelementFromNameRec(countryName);
    if (country == nullptr) {
        throw PrecondVioleeExcep("le pays n'existe pas ou n'est pas présent dans la liste");
    } else {
        return country->Attribute("car_code");
    }

}

/*
 * A COMPLETER
 */
/**
 * élément <country> d'un pays identifié par son nom countryName
 * @param countryName
 * @return pointeur sur l'élément <country> dont la valeur du fils <name> est égal à countryName, nullprt sinon
 */
XMLElement *Mondial::getCountryXmlelementFromNameIter(string countryName) const {
    // accéder à <countriescategory>, c’est un fils de l'élément <racineMondial>)
    XMLElement *countriesCategory = racineMondial->FirstChildElement("countriescategory");
    // accéder au premier pays : premier fils <country> de <countriescategory>
    XMLElement *currentCountry = countriesCategory->FirstChildElement();
    // parcourir (partiel) les <country> tq le fils <name> ≠ de countryName
    while ((currentCountry != nullptr) && (currentCountry->FirstChildElement("name")->GetText() != countryName)) {
        // avancer au frère <country> suivant de currentCountry
        currentCountry = currentCountry->NextSiblingElement();
    }
    // (currentCountry == nullptr) || (currentCountry->FirstChildElement("name")->GetText() == countryName)
    // on retourne l'élément courant dans tous les cas
    return currentCountry;
}

/*
 * A COMPLETER
 */
int Mondial::getCountryPopulationFromName(string countryName) const {
    XMLElement *country = getCountryXmlelementFromNameIter(countryName);
    // si le pays n'existe pas
    if (country == nullptr) {
        return -1;
    } else if (country->LastChildElement("population")->GetText() == nullptr) {
        return 0;
    } else {
        return atoi(country->LastChildElement("population")->GetText());
    }
}

// cette fonction permet de trouver le pays correspondant au code donné
XMLElement *Mondial::getCountryXmlelementFromCodeIter(string countryCode) const {
    // accéder à <countriescategory>, c’est un fils de l'élément <racineMondial>)
    XMLElement *countriesCategory = racineMondial->FirstChildElement("countriescategory");
    // accéder au premier pays : premier fils <country> de <countriescategory>
    XMLElement *currentCountry = countriesCategory->FirstChildElement();
    // parcourir (partiel) les <country> tq l'attribut car_code ≠ de countryCode
    while ((currentCountry != nullptr) && (currentCountry->Attribute("car_code") != countryCode)) {
        // avancer au frère <country> suivant de currentCountry
        currentCountry = currentCountry->NextSiblingElement();
    }
    // (currentCountry == nullptr) || (currentCountry->FirstChildElement("name")->GetText() == countryName)
    // on retourne l'élément courant dans tous les cas
    if (currentCountry == nullptr) {
        return nullptr;
    } else {
        return currentCountry;
    }
}

XMLElement *Mondial::getCountryXmlelementFromCode(string countryCode) const {
    // on met le résultat de la fonction itérative dans une variable pour pouvoir vérifier que le résultat n'est pas nullptr
    XMLElement *resultat = getCountryXmlelementFromCodeIter(countryCode);
    return resultat;
    // supprimer à partir d'ici après complétion
    return nullptr;
}


/*
 * A COMPLETER
 */
void Mondial::printCountryBorders(string countryName) const {
    XMLElement *country = getCountryXmlelementFromNameRec(countryName);
    if (country == nullptr) {
        cout << "le pays : " << countryName << " n'existe pas." << endl;
    } else if (country->FirstChildElement("border") == nullptr) {
        cout << "le pays : " << countryName << " n'a pas de frontières" << endl;
    } else {
        cout << "frontière(s) du pays : " << countryName << endl;
        XMLElement *currentBorder = country->FirstChildElement("border");
        while (currentBorder != nullptr) {
            // on récupère le code du pays frontalier
            string currentBorderCountryCode = currentBorder->Attribute("country");
            cout << currentBorderCountryCode << endl;
            // on récupère le <country> element du pays frontalier
            XMLElement *currentBorderCountry = getCountryXmlelementFromCode(currentBorderCountryCode);
            cout << currentBorderCountry->FirstChildElement("name")->GetText();
            cout << "longueur de la frontière avec ce pays : " << currentBorder->Attribute("length") << endl;

            currentBorder = currentBorder->NextSiblingElement();
        }
    }
    // supprimer à partir d'ici après complétion
}

/*
 * A COMPLETER
 */
XMLElement *Mondial::getRiverXmlelementFromNameIter(string riverName) const {
    // on accède à <riverscategory> qui est un fils de <racineMondial>
    XMLElement *riversCategory = racineMondial->FirstChildElement("riverscategory");
    // on accède à la première rivière
    XMLElement *currentRiver = riversCategory->FirstChildElement();
    // on parcourt tous les <river> tant que le fils <name> n'est pas pareil que riverName
    while ((currentRiver != nullptr) && (currentRiver->FirstChildElement("name")->GetText() != riverName)) {
        // on passe au frère suivant de currentCountry
        currentRiver = currentRiver->NextSiblingElement();
    }
    // currentRiver == nullptr ou currentRiver->FirstChildElement("name")->GetText() == riverName
    return currentRiver;
}

/*
 * A COMPLETER
 */
void Mondial::printAllCountriesCrossedByRiver(string riverName) const {
    // on accède a <river> avec comme <name> riverName
    XMLElement *river = getRiverXmlelementFromNameIter(riverName);
    // on check la présence de la rivière avant de faire la suite
    if (river) {
        // la rivière existe / est présente
        cout << "Le fleuve : " << riverName << endl;
        // on accède à l'attribut country de <river>
        string countriesString = river->Attribute("country");
        // on créé une liste des codes des pays traversés par la rivière
        vector<string> countryCodes = split(countriesString, ' ');
        // on parcourt la liste , on récupère le nom des pays et on les affichent
        XMLElement *currentCountry;
        cout << " traverse les pays suivants : ";
        for (int i = 0; i != countryCodes.size(); i++) {
            currentCountry = getCountryXmlelementFromCode(countryCodes[i]);
            if (currentCountry) {
                cout << currentCountry->FirstChildElement("name")->GetText() << ", ";
            }
        }
        cout << " ; il a la longueur suivante : " << river->FirstChildElement("length")->GetText() << endl;
    } else {
        cout << "la rivière : " << riverName << ", n'existe pas !" << endl;
    }
}

/*
 * A COMPLETER
 */
void Mondial::printCountriesWithProvincesCrossedByRiver(string riverName) const {
    XMLElement *river = getRiverXmlelementFromNameIter(riverName);
    // on vérifie si le fleuve existe, si non on affiche un message pour le dire à l'utilisateur.
    if (river == nullptr) {
        cout << "la rivière : " << riverName << ", n'existe pas !" << endl;
    } else {
        // la rivière existe / est présente
        cout << "Le fleuve : " << riverName << endl;
        cout << " traverse les pays suivants : ";
        // on récupère le premier pays traversé par le fleuve
        XMLElement *currentLocatedCountry = river->FirstChildElement("located");
        while (currentLocatedCountry != nullptr) {
            // on vérifie si le pays a des provinces , si non on ne l'affiche pas
            if (currentLocatedCountry->Attribute("province") != "") {
                XMLElement* country = getCountryXmlelementFromCode(currentLocatedCountry->Attribute("country"));

                cout << country->FirstChildElement("name")->GetText() << ", ";
            }
            currentLocatedCountry = currentLocatedCountry->NextSiblingElement("located");
        }
        cout << " ; il a la longueur suivante : " << river->FirstChildElement("length")->GetText() << endl;
    }

}


/*
 * A COMPLETER
 */
void Mondial::printCountriesAndProvincesCrossedByRiver(string riverName) const {

    // on accède a <river> avec comme <name> riverName
    XMLElement *river = getRiverXmlelementFromNameIter(riverName);
    // on vérifie si le fleuve existe, si non on affiche un message pour le dire à l'utilisateur.
    if (river == nullptr) {
        cout << "la rivière : " << riverName << ", n'existe pas !" << endl;
    } else {
        // la rivière existe / est présente
        cout << "Le fleuve : " << riverName << endl;
        // on
        XMLElement *currentLocatedCountry = river->FirstChildElement("located country");
        while (currentLocatedCountry->NextSiblingElement("locatedCountry") != nullptr) {
            cout <<
                 string provinces = currentLocatedCountry->Attribute("country");
            // on créé une liste de toutes les provinces
            vector<string> currentLocatedCountryprovinces = split(provinces, ' ');
        }
    }
}

/*
 * A COMPLETER
 */
void Mondial::printCityInformation(string cityName) const {
    /*
     * A COMPLETER
     */
}

/**
 * Exemple de question additionnelle pour l'exercice 9 afficher toutes les informations disponibles
 * dans Mondial concernant toutes les îles.
 * On peut commencer par une île en particulier à partir de son nom
 */
void Mondial::printIslandsInformations() const {
    /*
     * A COMPLETER
     */
}

/*
 * Méthodes de service fournies
 */

template<typename Out>
void Mondial::split(string &s, char delim, Out result) const {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        *(result++) = item;
    }
}

/**
 * Méthode à utiliser pour découper les mots d'une chaîne dans un vecteur
 * Utile pour la gestion des attributs à valeurs multiples
 * @param s chaîne à découper
 * @param delim délimiteur des mots de la chaîne à découper
 * @return vecteur contenant les mots de la chaîne découpée
 */
vector<std::string> Mondial::split(string &s, char delim) const {
    vector<std::string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

Mondial::~Mondial() {
}