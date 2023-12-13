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
#include <algorithm>

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
        string message = "Exception pour précondition violée : Dans getCountryCodeFromName, le pays " + countryName +
                         " n'existe pas !";
        throw PrecondVioleeExcep(message);
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
        return stoi(country->LastChildElement("population")->GetText());
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
        cout << "Le pays : " << countryName << endl;
        XMLElement *currentBorder = country->FirstChildElement("border");
        while (currentBorder != nullptr) {
            cout << " est frontalier avec : ";
            // on récupère le code du pays frontalier
            string currentBorderCountryCode = currentBorder->Attribute("country");
            // on récupère le <country> element du pays frontalier
            XMLElement *currentBorderCountry = getCountryXmlelementFromCode(currentBorderCountryCode);
            cout << currentBorderCountry->FirstChildElement("name")->GetText();
            cout << ", la longueur de sa frontière avec celui-ci est : " << currentBorder->Attribute("length") << endl;

            currentBorder = currentBorder->NextSiblingElement("border");
        }
    }
}


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
                XMLElement *country = getCountryXmlelementFromCode(currentLocatedCountry->Attribute("country"));

                cout << country->FirstChildElement("name")->GetText() << ", ";
            }
            currentLocatedCountry = currentLocatedCountry->NextSiblingElement("located");
        }
        cout << " ; il a la longueur suivante : " << river->FirstChildElement("length")->GetText() << endl;
    }

}


void Mondial::printCountriesAndProvincesCrossedByRiver(string riverName) const {

    // on accède a <river> avec comme <name> riverName
    XMLElement *river = getRiverXmlelementFromNameIter(riverName);
    // on vérifie si le fleuve existe, si non on affiche un message pour le dire à l'utilisateur.
    if (river == nullptr) {
        cout << "Le fleuve : " << riverName << ", n'existe pas !" << endl;
    } else {
        // le fleuve existe / est présente
        cout << "Le fleuve : " << riverName << " de longueur " << river->FirstChildElement("length")->GetText()
             << " traverse les pays suivants :  " << endl;
        // on récupère l'attribut country de <river> qui est sous la forme "COUNTRY_CODE COUNTRY_CODE ..."
        string countriesString = river->Attribute("country");
        // on crée une liste des codes des pays traversés par le fleuve,
        // elle nous servira ensuite à vérifier si le pays est présent dans la map et à afficher le nom du pays
        vector<string> countriesCarCode = split(countriesString, ' ');
        // on crée une map de la forme <car_code, liste des provinces traversées>
        // elle nous servira à récupérer les elements provinces pour afficher les noms des provinces
        map<string, string> countryCarCode_Provinces;
        // on récupère le premier <located> de river = le premier pays que traverse la rivière
        XMLElement *currentLocatedCountry = river->FirstChildElement("located");
        // pour chaque <located> on ajoute à la map la paire clef-valeur <car_code, liste des provinces traversées> correspondante
        // si le pays est dans <located> cela signifie qu'il a des provinces , la map ne contiendra donc que les pays avec des provinces
        while (currentLocatedCountry != nullptr) {
            // nouvelle paire entrée dans la map
            countryCarCode_Provinces.insert(
                    {(currentLocatedCountry->Attribute("country")), (currentLocatedCountry->Attribute("province"))});
            // on avance au prochain pays
            currentLocatedCountry = currentLocatedCountry->NextSiblingElement("located");
        }
        // on parcourt countriesCarCode
        // on affiche le nom du pays et le nom des provinces traversées du pays si il en a
        // currentCountry correspond a l'element <country> correspondant à un  car_code dans countriesCarCode
        XMLElement *currentCountry;
        for (int i = 0; i < countriesCarCode.size(); i++) {
            currentCountry = getCountryXmlelementFromCode(countriesCarCode[i]);
            cout << " - " << currentCountry->FirstChildElement("name")->GetText();
            // on vérifie si le fleuve traverse bien des provinces (normalement, il en traverse forcément)
            if (!countryCarCode_Provinces.empty()) {
                // on crée un itérateur sur la map à l'emplacement de la paire correspondante à currentCountry pour pouvoir manipuler les clefs et les valeurs de la map
                auto it = countryCarCode_Provinces.find(countriesCarCode[i]);
                // on vérifie si le pays courant a des provinces
                // find() renvoie un itérateur sur la fin de la map  = end() si il n'a pas trouvé l'élément passé en paramètre
                if (it != countryCarCode_Provinces.end()) {
                    cout << ", où il traverse les divisions administratives suivantes : " << endl;
                    // on crée une liste de tous les id des provinces traversées
                    vector<string> provincesId = split(it->second, ' ');
                    // on parcourt la liste des provinces et on essaye de retrouver chacune dans currentCountry
                    for (string id: provincesId) {
                        // on va à la première province
                        XMLElement *currentProvince = currentCountry->FirstChildElement("province");
                        while (currentProvince != nullptr && id != currentProvince->Attribute("id")) {
                            currentProvince = currentProvince->NextSiblingElement("province");
                        }
                        if (currentProvince) {
                            // on est sorti parce que id = currentProvince->Attribute("id")
                            cout << "  * " << currentProvince->FirstChildElement("name")->GetText() << endl;
                        } else {
                            // on est sorti parce qu'on est arrivé au bout des provinces (currentProvince == nullptr) mais ce n'est pas censé être possible puisque qu'on devrait tout le temps trouver le nom de la province
                            cout << " problème de cohérence dans les données..." << endl;
                        }

                    }
                } else {
                    // le fleuve ne traverse pas de province dans le pays courant, on passe au pays suivant en sautant une ligne pour un meilleur affichage
                    cout << endl;
                }
            }
        }
    }
}

/*
 * A COMPLETER
 */
void Mondial::printCityInformation(string cityName) const {
    // on doit parcourir <country> en profondeur pour trouver toutes les villes renseignées
    // et trouveur si leur nom est le même que cityName,
    // auquel cas, on a trouvé la ville que l'on cherchait et on peut afficher ses infos.
    // si le pays n'a pas de provinces <city> est un fils direct de <country>
    // si le pays a une/des province(s) <city> est un fils de <province> qui est un fils de <country>
    // chemin pour un pays sans province (ici, "->" représente un lien du père vers un fils) : <country> -> <city>
    // chemin pour un pays avec province : <country> -> <province> -> <city>

    // on crée une variable booléenne initialisée à faux et qui passera à vrai si on a trouvé la ville
    // c'est elle qui permet à la boucle de savoir quand s'arrêter
    bool cityIsFound = false;
    // on initialise les elements XML qui seront les villes, provinces et pays dans lesquels on itère.
    XMLElement *currentCity;
    XMLElement *currentProvince;
    XMLElement *currentCountry;

    // on accède à <countriescategory>, fils de <racineMondial>
    XMLElement *countriesCategory = racineMondial->FirstChildElement("countriescategory");
    // on accède au premier pays dans <countriescategories>
    currentCountry = countriesCategory->FirstChildElement();
    // on parcourt les pays tant que la ville n'est pas trouvé
    while ((currentCountry != nullptr) && !cityIsFound) {
        // on parcourt les <province> du <country>
        // on accède à la première <province> de currentCountry
        currentProvince = currentCountry->FirstChildElement("province");
        // on vérifie si le pays à des provinces
        if (currentProvince == nullptr) {
            // le pays n'a pas de province
            currentCity = currentCountry->FirstChildElement("city");
            // on parcourt les <city> tant que l'on ne trouve pas la <city>
            // recherchée où que l'on n'est pas au bout des elements <city>
            while ((currentCity != nullptr) && !cityIsFound) {
                if (currentCity->FirstChildElement("name")->GetText() == cityName) {
                    cityIsFound = true;
                }
                if (!cityIsFound) {
                    // on n'a pas trouvé la ville, on passe au frère suivant.
                    currentCity = currentCity->NextSiblingElement("city");
                }
            }
        } else {
            // le pays a des provinces
            while ((currentProvince != nullptr) && !cityIsFound) {
                currentCity = currentProvince->FirstChildElement("city");
                while ((currentCity != nullptr) && !cityIsFound) {
                    if (currentCity->FirstChildElement("name")->GetText() == cityName) {
                        cityIsFound = true;
                    }
                    if (!cityIsFound) {
                        // currentCity n'est pas la ville que l'on cherche
                        // on passe au frère suivant
                        currentCity = currentCity->NextSiblingElement("city");
                    }
                }
                if (!cityIsFound) {
                    // on est sorti de la boucle car on est arrivé au bout des <city> de cette <province>
                    //on va au frère suivant de currentProvince
                    currentProvince = currentProvince->NextSiblingElement("province");
                }
            }
        }
        if (!cityIsFound) {
            // on est sorti de la boucle car on est arrivé au bout des <provinces> de currentCountry
            // ou des <city> si le pays n'a pas de provinces
            //on va au frère suivant de currentCountry
            currentCountry = currentCountry->NextSiblingElement("country");
        }
    }
    // on sort soit parce que
    // cas 1 - currentCountry == nullptr (donc cityIsFound est toujours à False)
    // cela veut dire que l'on n'a pas trouvé la ville
    // cas 2 - cityIsFound = true
    // on a trouvé la ville, on peut affiche ses informations
    if (!cityIsFound) {
        cout << "La ville " << cityName << ", n'existe pas !";
    } else {
        // on affiche les informations sur la ville
        cout << "La ville " << cityName << endl
             << " - se trouve dans le pays : " << currentCountry->FirstChildElement("name")->GetText() << endl;
        // si le pays a des provinces on affiche la province dans laquelle est la ville
        if (currentProvince) {
            cout << " - dans la division adminstrative : " << currentProvince->FirstChildElement("name")->GetText()
                 << endl;
        }
        cout << " - sa latitude est : " << currentCity->FirstChildElement("latitude")->GetText() << endl
             << " - sa longitude est : " << currentCity->FirstChildElement("longitude")->GetText() << endl
             << " - son altitude est : " << currentCity->FirstChildElement("elevation")->GetText() << endl
             << " - sa population est : " << currentCity->LastChildElement("population")->GetText() << endl;
    }


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

void Mondial::printPopulationRanking(int numberOfRanks) const {
    vector<int> PopulationRanking;
    XMLElement *currentCountry;

    // on accède à <countriescategory>, fils de <racineMondial>
    XMLElement *countriesCategory = racineMondial->FirstChildElement("countriescategory");
    // on accède au premier pays dans <countriescategories>
    currentCountry = countriesCategory->FirstChildElement();
    vector<int> populations;
    while (currentCountry != nullptr){
        for (int i = 0; i < numberOfRanks; i++) {
            populations.push_back(stoi(currentCountry->FirstChildElement("population")->GetText()));
            currentCountry = currentCountry->NextSiblingElement("country");
        }
        sort(populations.begin(), populations.end());
        int currentPopulation = stoi(currentCountry->FirstChildElement("population")->GetText());
        if (currentPopulation > populations.front()){
            populations.erase(populations.begin());
            populations.push_back(currentPopulation);
            sort(populations.begin(), populations.end());
        }
        currentCountry = currentCountry->NextSiblingElement("country");
    }
    for (auto population:populations) {
        cout << population << endl;
    }
}

/**
 * fonction permettant de comparer des valeurs de paires de string entres elles
 * @param pairA première paire à comparer
 * @param pairB deuxième paire à comparer
 * @return true si pairA est lexicographiquement inférieur à pairB, renvoie false sinon ( si lexicographiquement : pairB<=pairA)
 */
bool stringPairSort(const pair<string,string>& pairA, const pair<string, string>& pairB) {
    return pairA.second.compare(pairB.second) < 0;
}

/**
 * affiche tous les aéroports qui se situent dans la zone gmt+ la valeur rentrée en argument et le pays de ceux-ci.
 * @param gmt : l'heure de décalage
 */
void Mondial::printAirportsWithGmt(int gmtOffset) const {
    // on accède à <airportscategories>, fils de <racineMondial>
    XMLElement *airportCategory = racineMondial->FirstChildElement("airportscategory");
    // on accède au premier aéroport dans <airportscategory>
    XMLElement *currentAirport = airportCategory->FirstChildElement("airport");
    // on crée une map qui stockera les aéroports et le pays associé à afficher
    map<string,string> airport_country;
    // on parcourt tous les aéroports et on ajoute à la map unqiuement ceux avec comme element fils <gmtOffset> gmtOffset donné en argument
    while (currentAirport != nullptr) {
        if (stoi(currentAirport->FirstChildElement("gmtOffset")->GetText()) == gmtOffset) {
            XMLElement* country = getCountryXmlelementFromCode(currentAirport->Attribute("country"));
            airport_country.insert({currentAirport->FirstChildElement("name")->GetText(),country->FirstChildElement("name")->GetText()});
        }
        currentAirport = currentAirport->NextSiblingElement("airport");
    }
    // si aucun aéroport n'est dans cette zone horaire ou si la zone horaire n'existe pas
    if (airport_country.empty()) {
        cout << "aucun aéroport n'est présent dans une zone sur la plage horaire gmt+" << gmtOffset << " ou la plage n'existe pas" << endl;
    } else {
        // on va trier toute les paires en fonction des pays, pour cela, on doit d'abord mettre les paires dans un vecteur car ce n'est pas possible avec les Maps
        vector<pair<string,string>> sortedAirport_Country(airport_country.begin(), airport_country.end());
        // le troisième argument donne la méthode de comparaison, ici stringPairSort qui est une méthode créé au dessus dans la classe
        sort(sortedAirport_Country.begin(), sortedAirport_Country.end(), stringPairSort);

        // on va utiliser un booléen pour vérifier si la paire suivante change de pays, comme ça on peut afficher uniquement le nom de l'aéroport si ce n'est pas le cas.
        bool afficherPays = false;
        cout << "========================================================" << endl;
        cout << "liste des aéroports avec un décalage horaire de gmt+" << gmtOffset << " : " << endl;
        cout << "========================================================" << endl;
        for (auto it = sortedAirport_Country.begin(); it != sortedAirport_Country.end(); it++) {

            if (it->second == sortedAirport_Country.front().second){
                afficherPays = true;
            }
            if (afficherPays){
                cout << " Aéroports dans le Pays : " << it->second << endl;
                cout << " ------------------------------------------" << endl << endl;
                cout << "    * " << it->first << " ( verification pays : " << it->second << ")" << endl;
                afficherPays = false;
            }else {
                cout << "    *" << it->first << " ( verification pays : " << it->second << ")" << endl;
            }
            if (it->second != next(it)->second){
                afficherPays = true;
                cout << endl;
            }

        }


    }
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