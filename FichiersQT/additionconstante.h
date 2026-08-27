#ifndef ADDITIONCONSTANTE_H
#define ADDITIONCONSTANTE_H

#include "module.h"
#include <QLineEdit>

class AdditionConstante : public Module{
    Q_OBJECT
    uint64_t constante;
public:
    AdditionConstante(QObject*, uint64_t = 1);
    AdditionConstante(QObject*, QDataStream &);
    void valeurSuivante(uint64_t (&val)[2]) const ;
    Module* copie() const;
    QWidget* creerPaneauParametres();
    void sauvegarder(QDataStream &) const;
    void ecrireAlgo(QTextStream &) const;

    inline uint64_t avoirConstante() const {return constante;};

public slots :
    void changerConstante();
};

#endif // ADDITIONCONSTANTE_H
