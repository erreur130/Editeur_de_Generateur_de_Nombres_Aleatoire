#ifndef EGNA_H
#define EGNA_H

#include "module.h"
#include <QFile>
#include <bit>
#include <QVector>

class EGNA{
    uint64_t graine;
    uint64_t etat[2];
    uint64_t etatTmp[2];
    QVector<Module*> *modules;
    double moyenne;
    double M2; // somme des carrés des écarts (pour le calcule de CV)
    double cv;
    QVector<unsigned int> histogramme;
    double autoCorrelation;
    double equilibreBits;
    int nbValeursActuel;
    int nbValeursTotale;

public:
    EGNA(QVector<Module*>*);
    ~EGNA();

    bool charger(QString nomFichier);
    bool sauvegarder(QString nomFichier) const;
    void renitialiserEtat();
    inline uint64_t avoirGraine() const {return graine;};
    inline void changerGraine(uint64_t graine_){graine = graine_;};
    void changerGraine();
    inline void changerNbValeursTotale(int nbValeursTotale_){nbValeursTotale = nbValeursTotale_;};

    uint8_t suivantPixelBruit();
    inline double avoirMoyenne() const {return moyenne;};
    double avoirCV() const;
    double avoirUniformite() const;
    inline double avoirAutocorrelation() const {return autoCorrelation;};
    inline double avoirEquilibreBits() const {return equilibreBits;};

private :
    void calculeMoyenne();
    void calculeUniformite();
    void calculeAutocorrelation();
    void calculeEquilibreBits();
};

#endif // EGNA_H
