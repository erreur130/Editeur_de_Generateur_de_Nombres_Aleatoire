#include "inversementbits.h"
#include "mainwindow.h" // dans le cpp pour évité la récusivité

InversementBits::InversementBits(QObject* parent)
    : Module(parent){}

void InversementBits::valeurSuivante(uint64_t (&val)[2]) const {
    // Inversement des Bits :
    // On inverse chaque paries pour ensuite les permuter
    // "parallel bit reversal" : On échange des blocs de bits adjacents entre eux, en doublant la taille des blocs à chaque étape (1, 2, 4, 8, 16, 32)
    val[0] = ((val[0] >> 1)  & 0x5555555555555555ULL) | ((val[0] & 0x5555555555555555ULL) << 1);
    val[0] = ((val[0] >> 2)  & 0x3333333333333333ULL) | ((val[0] & 0x3333333333333333ULL) << 2);
    val[0] = ((val[0] >> 4)  & 0x0F0F0F0F0F0F0F0FULL) | ((val[0] & 0x0F0F0F0F0F0F0F0FULL) << 4);
    val[0] = ((val[0] >> 8)  & 0x00FF00FF00FF00FFULL) | ((val[0] & 0x00FF00FF00FF00FFULL) << 8);
    val[0] = ((val[0] >> 16) & 0x0000FFFF0000FFFFULL) | ((val[0] & 0x0000FFFF0000FFFFULL) << 16);
    val[0] = (val[0] >> 32) | (val[0] << 32);

    val[1] = ((val[1] >> 1)  & 0x5555555555555555ULL) | ((val[1] & 0x5555555555555555ULL) << 1);
    val[1] = ((val[1] >> 2)  & 0x3333333333333333ULL) | ((val[1] & 0x3333333333333333ULL) << 2);
    val[1] = ((val[1] >> 4)  & 0x0F0F0F0F0F0F0F0FULL) | ((val[1] & 0x0F0F0F0F0F0F0F0FULL) << 4);
    val[1] = ((val[1] >> 8)  & 0x00FF00FF00FF00FFULL) | ((val[1] & 0x00FF00FF00FF00FFULL) << 8);
    val[1] = ((val[1] >> 16) & 0x0000FFFF0000FFFFULL) | ((val[1] & 0x0000FFFF0000FFFFULL) << 16);
    val[1] = (val[1] >> 32) | (val[1] << 32);

    uint64_t tmp = val[0];
    val[0] = val[1];
    val[1] = tmp;
}

Module* InversementBits::copie() const{
    return new InversementBits(parent());
}

QWidget* InversementBits::creerPaneauParametres(){
    QWidget* paneau = new QWidget;
    QVBoxLayout* layoutVerticale = new QVBoxLayout(paneau);

    QLabel* labelTitre = new QLabel("Inversement des bits");
    QFrame* separateur = new QFrame();
    separateur->setFrameShape(QFrame::HLine);   // ligne horizontale
    separateur->setFrameShadow(QFrame::Sunken); // effet visuel "enfoncé"

    // paramètre du paneau :
    labelTitre->setAlignment(Qt::AlignHCenter);
    layoutVerticale->setContentsMargins(5, 5, 5, 0);

    // on répartie les Wiget dans les layouts
    layoutVerticale->addWidget(labelTitre);
    layoutVerticale->addWidget(separateur);

    return paneau;
}

void InversementBits::sauvegarder(QDataStream & out) const{
    out << QString("InversementBits");
}

void InversementBits::ecrireAlgo(QTextStream & out) const{
    out <<  "   {\n"
            "       // Inversement des Bits :\n"
            "       // On inverse chaque paries pour ensuite les permuter\n"
            "       // \"parallel bit reversal\" : On échange des blocs de bits adjacents entre eux, en doublant la taille des blocs à chaque étape (1, 2, 4, 8, 16, 32)\n"
            "       etat[0] = ((etat[0] >> 1)  & 0x5555555555555555ULL) | ((etat[0] & 0x5555555555555555ULL) << 1);\n"
            "       etat[0] = ((etat[0] >> 2)  & 0x3333333333333333ULL) | ((etat[0] & 0x3333333333333333ULL) << 2);\n"
            "       etat[0] = ((etat[0] >> 4)  & 0x0F0F0F0F0F0F0F0FULL) | ((etat[0] & 0x0F0F0F0F0F0F0F0FULL) << 4);\n"
            "       etat[0] = ((etat[0] >> 8)  & 0x00FF00FF00FF00FFULL) | ((etat[0] & 0x00FF00FF00FF00FFULL) << 8);\n"
            "       etat[0] = ((etat[0] >> 16) & 0x0000FFFF0000FFFFULL) | ((etat[0] & 0x0000FFFF0000FFFFULL) << 16);\n"
            "       etat[0] = (etat[0] >> 32) | (etat[0] << 32);\n\n"
            "       etat[1] = ((etat[1] >> 1)  & 0x5555555555555555ULL) | ((etat[1] & 0x5555555555555555ULL) << 1);\n"
            "       etat[1] = ((etat[1] >> 2)  & 0x3333333333333333ULL) | ((etat[1] & 0x3333333333333333ULL) << 2);\n"
            "       etat[1] = ((etat[1] >> 4)  & 0x0F0F0F0F0F0F0F0FULL) | ((etat[1] & 0x0F0F0F0F0F0F0F0FULL) << 4);\n"
            "       etat[1] = ((etat[1] >> 8)  & 0x00FF00FF00FF00FFULL) | ((etat[1] & 0x00FF00FF00FF00FFULL) << 8);\n"
            "       etat[1] = ((etat[1] >> 16) & 0x0000FFFF0000FFFFULL) | ((etat[1] & 0x0000FFFF0000FFFFULL) << 16);\n"
            "       etat[1] = (etat[1] >> 32) | (etat[1] << 32);\n\n"
            "       uint64_t tmp = etat[0];\n"
            "       etat[0] = etat[1];\n"
            "       etat[1] = tmp;\n"
            "   }\n";
}
