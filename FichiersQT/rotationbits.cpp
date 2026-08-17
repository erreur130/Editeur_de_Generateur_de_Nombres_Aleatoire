#include "rotationbits.h"
#include "mainwindow.h" // dans le cpp pour évité la récusivité

RotationBits::RotationBits(QObject* parent, uint8_t decalage_)
    : Module(parent), decalage(decalage_){
    // 0 < decalage < 64
}

RotationBits::RotationBits(QObject* parent, QDataStream & in)
    : Module(parent), decalage(1){
    in >> decalage;
}

void RotationBits::valeurSuivante(uint64_t (&val)[2]) const {
    uint64_t retenue0 = 0xFFFFFFFFFFFFFFFFULL >> (64-decalage);  // des 1 sur les bits qui seraient remplacé
    uint64_t retenue1 = 0xFFFFFFFFFFFFFFFFULL >> (64-decalage);  // des 1 sur les bits qui seraient remplacé
    retenue0 &= val[0]; // la retenue0 devient les bits qui vont disparaitre dans la partie [0]
    retenue1 &= val[1]; // la retenue1 devient les bits qui vont disparaitre dans la partie [1]

    val[0] >>= decalage; // on décale la partie haute (trou à gauche dans la partie [0])
    val[1] >>= decalage; // on décale la partie haute (trou à gauche dans la partie [1])

    val[1] |= retenue0 << (64 - decalage); // on remplis le trou à gauche (car la retenue est sauvegardé à droite) par ce qui est sortit de l'autre partie de val
    val[0] |= retenue1 << (64 - decalage); // on remplis le trou à gauche (car la retenue est sauvegardé à droite) par ce qui est sortit de l'autre partie de val
}

Module* RotationBits::copie() const{
    return new RotationBits(parent(),decalage);
}

QWidget* RotationBits::creerPaneauParametres(){
    QWidget* paneau = new QWidget;
    QVBoxLayout* layoutVerticale = new QVBoxLayout(paneau);
    QFormLayout* layoutParametre = new QFormLayout(); // rajouter plus tard dans layoutVerticale

    QLabel* labelTitre = new QLabel("Rotation Bits");
    QSpinBox* nbDecalage = new QSpinBox();

    // paramètre du paneau :
    labelTitre->setAlignment(Qt::AlignHCenter);
    nbDecalage->setMinimum(1);
    nbDecalage->setMaximum(63);
    nbDecalage->setValue(decalage);
    // connect spinBox -> Module
    connect(nbDecalage, &QSpinBox::editingFinished, this, &RotationBits::changerDecalage);
    // connect spinBox -> MainWindow
    connect(nbDecalage, &QSpinBox::editingFinished, qobject_cast<MainWindow*>(parent()), &MainWindow::miseAJourTout);

    // on répartie les Wiget dans les layouts
    layoutParametre->addRow("Décalage :", nbDecalage);
    layoutVerticale->addWidget(labelTitre);
    layoutVerticale->addLayout(layoutParametre);

    return paneau;
}

void RotationBits::sauvegarder(QDataStream & out) const{
    out << QString("RotationBits") << decalage;
}

void RotationBits::ecrireAlgo(QTextStream & out) const{
    out <<  "   {\n"
            "       uint8_t decalage = " << decalage << ";\n"
            "       uint64_t retenue0 = 0xFFFFFFFFFFFFFFFFULL >> (64-decalage);  // des 1 sur les bits qui seraient remplacé\n"
            "       uint64_t retenue1 = 0xFFFFFFFFFFFFFFFFULL >> (64-decalage);  // des 1 sur les bits qui seraient remplacé\n"
            "       retenue0 &= etat[0]; // la retenue0 devient les bits qui vont disparaitre dans la partie [0]\n"
            "       retenue1 &= etat[1]; // la retenue1 devient les bits qui vont disparaitre dans la partie [1]\n"
            "\n"
            "       etat[0] >>= decalage; // on décale la partie haute (trou à gauche dans la partie [0])\n"
            "       etat[1] >>= decalage; // on décale la partie haute (trou à gauche dans la partie [1])\n"
            "\n"
            "       etat[1] |= retenue0 << (64 - decalage); // on remplis le trou à gauche (car la retenue est sauvegardé à droite) par ce qui est sortit de l'autre partie de l'etat\n"
            "       etat[0] |= retenue1 << (64 - decalage); // on remplis le trou à gauche (car la retenue est sauvegardé à droite) par ce qui est sortit de l'autre partie de l'etat\n\n"
            "   }\n";
}

void RotationBits::changerDecalage(){
    QSpinBox* spin = qobject_cast<QSpinBox*>(sender());
    if (spin) {
        decalage = static_cast<uint8_t>(spin->value());
    }
}

