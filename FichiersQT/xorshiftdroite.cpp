#include "xorshiftdroite.h"
#include "mainwindow.h" // dans le cpp pour évité la récusivité

XorshiftDroite::XorshiftDroite(QObject* parent, uint8_t decalage_)
    : Module(parent), decalage(decalage_){
    // 0 < ---------------------- < 64
}

XorshiftDroite::XorshiftDroite(QObject* parent, QDataStream & in)
    : Module(parent), decalage(1){
    in >> decalage;
}

void XorshiftDroite::valeurSuivante(uint64_t (&val)[2]) const {
    // Xorshift vers la droite :

    uint64_t valTmp[2] = {val[0], val[1]};
    // on décale et xor de B bit à droite :
    // on décale valTmp
    uint64_t retenue = 0xFFFFFFFFFFFFFFFFULL >> (64-decalage);  // des 1 sur les bits qui seraient remplacé
    retenue &= valTmp[0]; // la retenue devient les bits qui vont disparaitre de la partie [0]
    valTmp[0] >>= decalage; // on décale la partie haute (trou à gauche dans la partie [0])
    valTmp[1] >>= decalage; // on décale la partie basse (trou à gauche dans la partie [1])
    valTmp[1] |= retenue << (64-decalage); // on remplis le trou à droite (car la retenue est sauvegardé à droite) par ce qui est sortit de l'autre partie de val
    // puis xor avec val et valTmp
    val[0] ^= valTmp[0];
    val[1] ^= valTmp[1];


}

Module* XorshiftDroite::copie() const{
    return new XorshiftDroite(parent(), decalage);
}

QWidget* XorshiftDroite::creerPaneauParametres(){
    QWidget* paneau = new QWidget;
    QVBoxLayout* layoutVerticale = new QVBoxLayout(paneau);
    QFormLayout* layoutParametre = new QFormLayout(); // rajouter plus tard dans layoutVerticale

    QLabel* labelTitre = new QLabel("Xorshift vers la droite");
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
    connect(nbDecalage, &QSpinBox::editingFinished, this, &XorshiftDroite::changerDecalage);
    // connect spinBox -> MainWindow
    connect(nbDecalage, &QSpinBox::editingFinished, qobject_cast<MainWindow*>(parent()), &MainWindow::miseAJourTout);
    layoutVerticale->setContentsMargins(5, 5, 5, 0);

    // on répartie les Wiget dans les layouts
    layoutParametre->addRow("Décalage :", nbDecalage);
    layoutVerticale->addWidget(labelTitre);
    layoutVerticale->addLayout(layoutParametre);
    layoutVerticale->addWidget(separateur);

    return paneau;
}

void XorshiftDroite::sauvegarder(QDataStream & out) const{
    out << QString("XorshiftDroite") << decalage;
}

void XorshiftDroite::ecrireAlgo(QTextStream & out) const{
    out <<  "   {\n"
            "       // Xorshift vers la droite\n"
            "       uint8_t decalage = " << decalage << ";\n\n"
            "       uint64_t valTmp[2] = {etat[0], etat[1]};\n"
            "       // on décale et xor de decalage bit à droite :\n"
            "       // on décale valTmp\n"
            "       uint64_t retenue = 0xFFFFFFFFFFFFFFFFULL >> (64-decalage);  // des 1 sur les bits qui seraient remplacé\n"
            "       retenue &= valTmp[0]; // la retenue devient les bits qui vont disparaitre de la partie [0]\n"
            "       valTmp[0] >>= decalage; // on décale la partie haute (trou à gauche dans la partie [0])\n"
            "       valTmp[1] >>= decalage; // on décale la partie basse (trou à gauche dans la partie [1])\n"
            "       valTmp[1] |= retenue << (64-decalage); // on remplis le trou à droite (car la retenue est sauvegardé à droite) par ce qui est sortit de l'autre partie de val\n"
            "       // puis xor avec etat et valTmp\n"
            "       etat[0] ^= valTmp[0];\n"
            "       etat[1] ^= valTmp[1];\n"
            "   }\n";
}

void XorshiftDroite::changerDecalage(){
    QSpinBox* spin = qobject_cast<QSpinBox*>(sender()); // sender() permet de récupérer le QObject qui a émis le signal
    if (spin) {
        decalage = static_cast<uint8_t>(spin->value());
    }
}
