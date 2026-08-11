#include "egna.h"

EGNA::EGNA(QVector<Module*> *modules_)
    : graine(uint64_t()), etat{uint64_t()}, etatTmp{uint64_t()}, modules(modules_), moyenne(0.0), M2(0.0), cv(0.0), histogramme(QVector<unsigned int>(100,0)), autoCorrelation(0.0), equilibreBits(0.0), nbValeursActuel(0.0), nbValeursTotale(0){

    using namespace std::chrono;
    time_point<system_clock> now = system_clock::now();
    system_clock::duration temps = now.time_since_epoch();
    // Conversion duration -> nanoseconds -> uint64_t
    nanoseconds tempsNano = duration_cast<nanoseconds>(temps);
    graine = static_cast<uint64_t>(tempsNano.count());
}

EGNA::~EGNA(){}

bool EGNA::charger(QString nomFichier){
    QFile fichier(nomFichier);

    if(fichier.open(QFile::ReadOnly | QFile::Text)){ // Si on arrive pas à ouvrir
        qDebug() << "Erreur" << fichier.errorString();
        return false;
    }

    QString text = fichier.readAll();

    /*truc pour charger et pour rénitialiser--------------------------------------------------------*/

    fichier.close();
    return true;
}

bool EGNA::sauvegarder(QString nomFichier) const {
    QFile fichier(nomFichier);
    if(!fichier.open(QFile::WriteOnly | QFile::Text)){ // Si on arrive pas à ouvrir
        qDebug() << "Erreur" << fichier.errorString();
        return false;
    }

    /*fichier.write(...); -------------------------------------------------------------------------------*/

    fichier.close();
    return true;
}

void EGNA::renitialiserEtat(){
    nbValeursActuel = 0; // renitialise le compteur
    // cas vide
    if (modules->size() == 0){
        etat[0] = etat[1] = 255;
        return;
    }
    // cas non vide
    // on initalise l'etat avec la graine
    etat[0] = graine;
    etat[1] = -graine; // -graine est une valeur non signé, c'est juste pour faire une valeur différente
    for (qsizetype indice = 0; indice < modules->size(); indice++){ // On parcourt tout les modules
        modules->at(indice)->valeurSuivante(etat);
    }
}

uint8_t EGNA::suivantPixelBruit(){
    // calcule de l'état
    for (qsizetype indice = 0; indice < modules->size(); indice++){ // On parcourt tout les modules
        modules->at(indice)->valeurSuivante(etat);
    }

    // calcule des stats sur l'état
    nbValeursActuel++; // fait commencé à 1
    calculeMoyenne();
    calculeUniformite();
    calculeAutocorrelation();
    calculeEquilibreBits();

    return static_cast<uint8_t>(etat[0]); // le cast ne prend que les 8 derniers bits ce qui correspond à faire modulo 256
}

void EGNA::calculeMoyenne(){
    // nbValeursActuel commence à 1
    if (nbValeursActuel == 1){ // pour initialiser
        // prend les 53 bits de poids fort de etat[0] et les divise par 2^53
        moyenne = static_cast<double>(etat[0] >> 11) / (1ULL << 53);
        M2 = 0.0; // renitialise la valeur pour le calcule de CV
    } else {
        double valeur = static_cast<double>(etat[0] >> 11) / (1ULL << 53); // pour évité la redondance / des calcules inutiles

        // calcule pour CV
        double delta1 = valeur - moyenne;

        // moyene = ( (MoyTotal*nbValsTotalAvant) + (1autreMoy*1) ) / (nbVals)
        moyenne = ( (moyenne*(nbValeursActuel-1)) + valeur ) / nbValeursActuel;

        // calcule pour CV
        double delta2 = valeur - moyenne; // Nouvelle différence avec la moyenne mise à jour
        M2 += delta1 * delta2;
    }
}

double EGNA::avoirCV() const { // ne s'exécute que à la fin
    if (moyenne != 0.0){
        // variance -> algo de Welford
        double variance = M2 / nbValeursActuel;
        return std::sqrt(variance) / moyenne;
    } else
        qDebug() << "moyenne == 0 !";
    return 0.0;
}

void EGNA::calculeUniformite(){
    if (nbValeursActuel == 1) // pour initialiser
        histogramme = QVector<unsigned int>(100,0); // 100 valeurs de 0

    double valeur = static_cast<double>(etat[0] >> 11) / static_cast<double>(1ULL << 53); // même calcule que pour la moyenne

    int quartier = static_cast<int>(valeur * 100); // permet de savoir à qu'elle quartier la valeur apartient
    if (quartier >= 100)
        quartier = 99; // sécurité si valeur == 1.0 exactement

    histogramme[quartier]++;
}

double EGNA::avoirUniformite() const{
    double attendu = static_cast<double>(nbValeursActuel) / 100.0;
    double chiCarre = 0.0;

    for (uint64_t compte : histogramme) {
        double ecart = static_cast<double>(compte) - attendu;
        chiCarre += (ecart * ecart) / attendu;
    }

    return chiCarre;
}

void EGNA::calculeAutocorrelation(){
    if (nbValeursActuel == 1){ // pour initialiser
        // on sauvegrade l'état
        etatTmp[0] = etat[0];
        etatTmp[1] = etat[1];
        autoCorrelation = 0.0;
    } else {
        // On utiliser ne NXOR pour touvé des similarité c'est à dire si 0 et 0 ou si 1 et 1 puis on récumpère tout les bit à 1
        double valeur = ( std::popcount(~(etat[0]^etatTmp[0])) + std::popcount(~(etat[1]^etatTmp[1])) ) / 128.0;

        // moyene = ( (MoyTotal*nbValsTotalAvant) + (1autreMoy*1) ) / (nbVals)
        autoCorrelation = ( (autoCorrelation*(nbValeursActuel-2)) + valeur ) / (nbValeursActuel - 1); // -1 et -2 car la première valeur ne peut pas être compté ici

        // on sauvegrade l'état
        etatTmp[0] = etat[0];
        etatTmp[1] = etat[1];
    }
}

void EGNA::calculeEquilibreBits(){
    double valeur = (std::popcount(etat[0]) + std::popcount(etat[1])) / 128.0; // pour évité la redondance
    if (nbValeursActuel == 1){ // pour initialiser
        // on compte tout les bits à 1 et on divise tout pas le nombre de bits total (128)
        equilibreBits = valeur;
    } else {
        // moyene = ( (MoyTotal*nbValsTotalAvant) + (1autreMoy*1) ) / (nbVals)
        equilibreBits = ( (equilibreBits*(nbValeursActuel-1)) + valeur ) / nbValeursActuel;
    }
}