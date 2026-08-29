#ifndef INVERSEMENTBITS_H
#define INVERSEMENTBITS_H

#include "module.h"

class InversementBits : public Module{
    Q_OBJECT
public:
    InversementBits(QObject*);
    void valeurSuivante(uint64_t (&val)[2]) const ;
    Module* copie() const;
    QWidget* creerPaneauParametres();
    void sauvegarder(QDataStream &) const;
    void ecrireAlgo(QTextStream &) const;
};
#endif // INVERSEMENTBITS_H
