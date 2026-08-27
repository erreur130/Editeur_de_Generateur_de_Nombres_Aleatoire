#ifndef XORSHIFT_H
#define XORSHIFT_H

#include "module.h"

class Xorshift : public Module{
    Q_OBJECT
    uint8_t a;
    uint8_t b;
    uint8_t c;
public:
    Xorshift(QObject*, uint8_t = 1, uint8_t = 1, uint8_t = 1);
    Xorshift(QObject*, QDataStream &);
    void valeurSuivante(uint64_t (&val)[2]) const ;
    Module* copie() const;
    QWidget* creerPaneauParametres();
    void sauvegarder(QDataStream &) const;
    void ecrireAlgo(QTextStream &) const;

    inline uint8_t avoirA() const {return a;};
    inline uint8_t avoirB() const {return b;};
    inline uint8_t avoirC() const {return c;};

public slots :
    void changerA();
    void changerB();
    void changerC();
};

#endif // XORSHIFT_H
