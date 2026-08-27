#ifndef ROTATIONBITSDROITE_H
#define ROTATIONBITSDROITE_H

#include "module.h"

class RotationBitsDroite : public Module{
    Q_OBJECT
    uint8_t decalage;
public:
    RotationBitsDroite(QObject*, uint8_t = 1);
    RotationBitsDroite(QObject*, QDataStream &);
    void valeurSuivante(uint64_t (&val)[2]) const ;
    Module* copie() const;
    QWidget* creerPaneauParametres();
    void sauvegarder(QDataStream &) const;
    void ecrireAlgo(QTextStream &) const;

    inline uint8_t avoirDecalage() const {return decalage;};

public slots :
    void changerDecalage();
};

#endif // ROTATIONBITSDROITE_H
