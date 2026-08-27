#include "module.h"

#include "rotationbitsdroite.h"
#include "rotationbitsgauche.h"
#include "xorshiftdroite.h"
#include "xorshiftgauche.h"
#include "additionconstante.h"
#include "multiplicationconstante.h"
// Rajouter des #include si on rajoute des Modules -------------------------------------------------------------------------

Module::Module(QObject* parent)
    :QObject(parent){}

Module* Module::charger(QDataStream & in, QObject* parent){ // rajouter des conditions si on rajoute des Modules --------------------------------------
    QString type;
    in >> type;

    if (type == "RotationBitsDroite")
        return new RotationBitsDroite(parent, in);

    else if (type == "RotationBitsGauche")
        return new RotationBitsGauche(parent, in);

    else if (type == "XorshiftDroite")
        return new XorshiftDroite(parent, in);

    else if (type == "XorshiftGauche")
        return new XorshiftGauche(parent, in);

    else if (type == "AdditionConstante")
        return new AdditionConstante(parent, in);

    else if (type == "MultiplicationConstante")
        return new MultiplicationConstante(parent, in);

    /*
    else if (type == "")
        return new XXX(parent, in);
    else if (type == "")
        return new XXX(parent, in);

    */
    qDebug() << "Type " << type << " est inconnu";
    return nullptr;
}
