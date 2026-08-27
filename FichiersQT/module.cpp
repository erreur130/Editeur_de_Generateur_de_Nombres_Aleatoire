#include "module.h"

#include "rotationbits.h"
#include "xorshift.h"
#include "additionconstante.h"
// Rajouter des #include si on rajoute des Modules -------------------------------------------------------------------------

Module::Module(QObject* parent)
    :QObject(parent){}

Module* Module::charger(QDataStream & in, QObject* parent){ // rajouter des conditions si on rajoute des Modules --------------------------------------
    QString type;
    in >> type;

    if (type == "RotationBits")
        return new RotationBits(parent, in);
    else if (type == "Xorshift")
        return new Xorshift(parent, in);
    else if (type == "AdditionConstante")
        return new AdditionConstante(parent, in);
    /*
    else if (type == "")
        return new XXX(parent, in);
    else if (type == "")
        return new XXX(parent, in);
    else if (type == "")
        return new XXX(parent, in);

    */
    qDebug() << "Type " << type << " est inconnu";
    return nullptr;
}
