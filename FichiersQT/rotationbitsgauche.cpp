#include "rotationbitsgauche.h"
#include "mainwindow.h" // dans le cpp pour évité la récusivité

RotationBitsGauche::RotationBitsGauche(QObject* parent, uint8_t decalage_)
    : Module(parent), decalage(decalage_){
    // 0 < decalage < 64
}

RotationBitsGauche::RotationBitsGauche(QObject* parent, QDataStream & in)
    : Module(parent), decalage(1){
    in >> decalage;
}

void RotationBitsGauche::valeurSuivante(uint64_t (&val)[2]) const {
    // rotation bits à gauche
    uint64_t retenue0 = 0xFFFFFFFFFFFFFFFFULL << (64-decalage);  // des 1 sur les bits qui seraient remplacé
    uint64_t retenue1 = 0xFFFFFFFFFFFFFFFFULL << (64-decalage);  // des 1 sur les bits qui seraient remplacé
    retenue0 &= val[0]; // la retenue0 devient les bits qui vont disparaitre dans la partie [0]
    retenue1 &= val[1]; // la retenue1 devient les bits qui vont disparaitre dans la partie [1]

    val[0] <<= decalage; // on décale la partie haute (trou à gauche dans la partie [0])
    val[1] <<= decalage; // on décale la partie basse (trou à gauche dans la partie [1])

    val[1] |= retenue0 >> (64 - decalage); // on remplis le trou à droite (car la retenue est sauvegardé à gauche) par ce qui est sortit de l'autre partie de val
    val[0] |= retenue1 >> (64 - decalage); // on remplis le trou à droite (car la retenue est sauvegardé à gauche) par ce qui est sortit de l'autre partie de val
}

Module* RotationBitsGauche::copie() const{
    return new RotationBitsGauche(parent(),decalage);
}

QWidget* RotationBitsGauche::creerPaneauParametres(){
    QWidget* paneau = new QWidget;
    QVBoxLayout* layoutVerticale = new QVBoxLayout(paneau);
    QFormLayout* layoutParametre = new QFormLayout(); // rajouter plus tard dans layoutVerticale

    QLabel* labelTitre = new QLabel("Rotation Bits à Gauche");
    QSpinBox* nbDecalage = new QSpinBox();
    QFrame* separateur = new QFrame();
    separateur->setFrameShape(QFrame::HLine);   // ligne horizontale
    separateur->setFrameShadow(QFrame::Sunken); // effet visuel "enfoncé"

    // paramètre du paneau :
    labelTitre->setAlignment(Qt::AlignHCenter);
    nbDecalage->setMinimum(1);
    nbDecalage->setMaximum(63);
    nbDecalage->setValue(decalage);
    nbDecalage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // connect spinBox -> Module
    connect(nbDecalage, &QSpinBox::editingFinished, this, &RotationBitsGauche::changerDecalage);
    // connect spinBox -> MainWindow
    connect(nbDecalage, &QSpinBox::editingFinished, qobject_cast<MainWindow*>(parent()), &MainWindow::miseAJourTout);
    connect(nbDecalage, &QSpinBox::editingFinished, qobject_cast<MainWindow*>(parent()), &MainWindow::signalerModification);
    layoutVerticale->setContentsMargins(5, 5, 5, 0);

    // on répartie les Wiget dans les layouts
    layoutParametre->addRow("Décalage :", nbDecalage);
    layoutVerticale->addWidget(labelTitre);
    layoutVerticale->addLayout(layoutParametre);
    layoutVerticale->addWidget(separateur);

    return paneau;
}

void RotationBitsGauche::sauvegarder(QDataStream & out) const{
    out << QString("RotationBitsGauche") << decalage;
}

void RotationBitsGauche::ecrireAlgo(QTextStream & out) const{
    out <<  "   {\n"
            "       // rotation bits à gauche\n"
            "       uint8_t decalage = " << decalage << ";\n"
            "       uint64_t retenue0 = 0xFFFFFFFFFFFFFFFFULL << (64-decalage);  // des 1 sur les bits qui seraient remplacé\n"
            "       uint64_t retenue1 = 0xFFFFFFFFFFFFFFFFULL << (64-decalage);  // des 1 sur les bits qui seraient remplacé\n"
            "       retenue0 &= etat[0]; // la retenue0 devient les bits qui vont disparaitre dans la partie [0]\n"
            "       retenue1 &= etat[1]; // la retenue1 devient les bits qui vont disparaitre dans la partie [1]\n\n"
            "       etat[0] <<= decalage; // on décale la partie haute (trou à gauche dans la partie [0])\n"
            "       etat[1] <<= decalage; // on décale la partie basse (trou à gauche dans la partie [1])\n\n"
            "       etat[1] |= retenue0 >> (64 - decalage); // on remplis le trou à droite (car la retenue est sauvegardé à gauche) par ce qui est sortit de l'autre partie de val\n"
            "       etat[0] |= retenue1 >> (64 - decalage); // on remplis le trou à droite (car la retenue est sauvegardé à gauche) par ce qui est sortit de l'autre partie de val\n"
            "   }\n";
}

void RotationBitsGauche::changerDecalage(){
    QSpinBox* spin = qobject_cast<QSpinBox*>(sender()); // sender() permet de récupérer le QObject qui a émis le signal
    if (spin) {
        decalage = static_cast<uint8_t>(spin->value());
    }
}
