#ifndef MULTIPLICATIONCONSTANTE_H
#define MULTIPLICATIONCONSTANTE_H

#include "module.h"

class MultiplicationConstante : public Module{
    Q_OBJECT
    uint64_t constante;
public:
    MultiplicationConstante(QObject*, uint64_t = 1);
    MultiplicationConstante(QObject*, QDataStream &);
    void valeurSuivante(uint64_t (&val)[2]) const ;
    Module* copie() const;
    QWidget* creerPaneauParametres();
    void sauvegarder(QDataStream &) const;
    void ecrireAlgo(QTextStream &) const;

    inline uint64_t avoirConstante() const {return constante;};

public slots :
    void changerConstante();
};

#endif // MULTIPLICATIONCONSTANTE_H
