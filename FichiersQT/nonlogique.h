#ifndef NONLOGIQUE_H
#define NONLOGIQUE_H

#include "module.h"

class NonLogique : public Module{
    Q_OBJECT
public:
    NonLogique(QObject*);
    void valeurSuivante(uint64_t (&val)[2]) const ;
    Module* copie() const;
    QWidget* creerPaneauParametres();
    void sauvegarder(QDataStream &) const;
    void ecrireAlgo(QTextStream &) const;
};

#endif // NONLOGIQUE_H
