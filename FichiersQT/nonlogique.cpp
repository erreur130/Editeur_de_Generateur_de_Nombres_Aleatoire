#include "nonlogique.h"
#include "mainwindow.h" // dans le cpp pour évité la récusivité

NonLogique::NonLogique(QObject* parent)
    : Module(parent){}

void NonLogique::valeurSuivante(uint64_t (&val)[2]) const {
    // Non logique :
    val[0] = ~val[0];
    val[1] = ~val[1];
}

Module* NonLogique::copie() const{
    return new NonLogique(parent());
}

QWidget* NonLogique::creerPaneauParametres(){
    QWidget* paneau = new QWidget;
    QVBoxLayout* layoutVerticale = new QVBoxLayout(paneau);

    QLabel* labelTitre = new QLabel("Non logique");
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

void NonLogique::sauvegarder(QDataStream & out) const{
    out << QString("NonLogique");
}

void NonLogique::ecrireAlgo(QTextStream & out) const{
    out <<  "   {\n"
            "       // Non logique :\n"
            "       etat[0] = ~etat[0];\n"
            "       etat[1] = ~etat[1];\n"
            "   }\n";
}

