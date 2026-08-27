#include "xorshift.h"
#include "mainwindow.h" // dans le cpp pour évité la récusivité

Xorshift::Xorshift(QObject* parent, uint8_t a_, uint8_t b_, uint8_t c_)
    : Module(parent), a(a_), b(b_), c(c_){
    // 0 < ---------------------- < 64
}

Xorshift::Xorshift(QObject* parent, QDataStream & in)
    : Module(parent), a(1), b(1), c(1){
    in >> a >> b >> c;
}

void Xorshift::valeurSuivante(uint64_t (&val)[2]) const {
    // Xorshift avec A,B,C
    // on copie la valeur actuel ----------------- A ----------------
    uint64_t valTmp[2] = {val[0], val[1]};
    // on décale et xor de A bit à gauche :
    // on décale valTmp
    uint64_t retenue = 0xFFFFFFFFFFFFFFFFULL << (64-a);  // des 1 sur les bits qui seraient remplacé
    retenue &= valTmp[1]; // la retenue devient les bits qui vont disparaitre de la partie [1]
    valTmp[0] <<= a; // on décale la partie haute (trou à droite dans la partie [0])
    valTmp[1] <<= a; // on décale la partie basse (trou à droite dans la partie [1])
    valTmp[0] |= retenue >> (64-a); // on remplis le trou à droite (car la retenue est sauvegardé à droite) par ce qui est sortit de l'autre partie de val
    // puis xor avec val et valTmp
    val[0] ^= valTmp[0];
    val[1] ^= valTmp[1];

    // on copie la valeur actuel ----------------- B ----------------
    valTmp[0] = val[0];
    valTmp[1] = val[1];
    // on décale et xor de B bit à droite :
    // on décale valTmp
    retenue = 0xFFFFFFFFFFFFFFFFULL >> (64-b);  // des 1 sur les bits qui seraient remplacé
    retenue &= valTmp[0]; // la retenue devient les bits qui vont disparaitre de la partie [0]
    valTmp[0] >>= b; // on décale la partie haute (trou à gauche dans la partie [0])
    valTmp[1] >>= b; // on décale la partie basse (trou à gauche dans la partie [1])
    valTmp[1] |= retenue << (64-b); // on remplis le trou à droite (car la retenue est sauvegardé à droite) par ce qui est sortit de l'autre partie de val
    // puis xor avec val et valTmp
    val[0] ^= valTmp[0];
    val[1] ^= valTmp[1];

    // on copie la valeur actuel ----------------- C ----------------
    valTmp[0] = val[0];
    valTmp[1] = val[1];
    // on décale et xor de C bit à gauche :
    // on décale valTmp
    retenue = 0xFFFFFFFFFFFFFFFFULL << (64-c);  // des 1 sur les bits qui seraient remplacé
    retenue &= valTmp[1]; // la retenue devient les bits qui vont disparaitre de la partie [1]
    valTmp[0] <<= c; // on décale la partie haute (trou à droite dans la partie [0])
    valTmp[1] <<= c; // on décale la partie basse (trou à droite dans la partie [1])
    valTmp[0] |= retenue >> (64-c); // on remplis le trou à droite (car la retenue est sauvegardé à droite) par ce qui est sortit de l'autre partie de val
    // puis xor avec val et valTmp
    val[0] ^= valTmp[0];
    val[1] ^= valTmp[1];
}

Module* Xorshift::copie() const{
    return new Xorshift(parent(), a, b, c);
}

QWidget* Xorshift::creerPaneauParametres(){
    QWidget* paneau = new QWidget;
    QVBoxLayout* layoutVerticale = new QVBoxLayout(paneau);
    QHBoxLayout* layoutParametre = new QHBoxLayout(); // rajouter plus tard dans layoutVerticale

    QLabel* labelTitre = new QLabel("Xorshift");
    QLabel* labelA = new QLabel("A : ");
    QLabel* labelB = new QLabel("B : ");
    QLabel* labelC = new QLabel("C : ");
    QSpinBox* nbA = new QSpinBox();
    QSpinBox* nbB = new QSpinBox();
    QSpinBox* nbC = new QSpinBox();
    QFrame* separateur = new QFrame();
    separateur->setFrameShape(QFrame::HLine);   // ligne horizontale
    separateur->setFrameShadow(QFrame::Sunken); // effet visuel "enfoncé"

    // paramètre du paneau :
    // pour A
    labelTitre->setAlignment(Qt::AlignHCenter);
    nbA->setMinimum(1);
    nbA->setMaximum(63);
    nbA->setValue(a);
    // connect spinBox -> Module
    connect(nbA, &QSpinBox::editingFinished, this, &Xorshift::changerA);
    // connect spinBox -> MainWindow
    connect(nbA, &QSpinBox::editingFinished, qobject_cast<MainWindow*>(parent()), &MainWindow::miseAJourTout);
    // pour B
    labelTitre->setAlignment(Qt::AlignHCenter);
    nbB->setMinimum(1);
    nbB->setMaximum(63);
    nbB->setValue(b);
    // connect spinBox -> Module
    connect(nbB, &QSpinBox::editingFinished, this, &Xorshift::changerB);
    // connect spinBox -> MainWindow
    connect(nbB, &QSpinBox::editingFinished, qobject_cast<MainWindow*>(parent()), &MainWindow::miseAJourTout);
    // pour C
    labelTitre->setAlignment(Qt::AlignHCenter);
    nbC->setMinimum(1);
    nbC->setMaximum(63);
    nbC->setValue(c);
    // connect spinBox -> Module
    connect(nbC, &QSpinBox::editingFinished, this, &Xorshift::changerC);
    // connect spinBox -> MainWindow
    connect(nbC, &QSpinBox::editingFinished, qobject_cast<MainWindow*>(parent()), &MainWindow::miseAJourTout);
    layoutVerticale->setContentsMargins(5, 5, 5, 0);

    // on répartie les Wiget dans les layouts
    layoutParametre->addWidget(labelA);
    layoutParametre->addWidget(nbA);
    layoutParametre->addWidget(labelB);
    layoutParametre->addWidget(nbB);
    layoutParametre->addWidget(labelC);
    layoutParametre->addWidget(nbC);
    layoutVerticale->addWidget(labelTitre);
    layoutVerticale->addLayout(layoutParametre);
    layoutVerticale->addWidget(separateur);

    return paneau;
}

void Xorshift::sauvegarder(QDataStream & out) const{
    out << QString("Xorshift") << a << b << c;
}

void Xorshift::ecrireAlgo(QTextStream & out) const{
    out <<  "   {\n"
            "       // Xorshift avec A,B,C\n"
            "       uint8_t a = " << a << ";\n"
            "       uint8_t b = " << b << ";\n"
            "       uint8_t c = " << c << ";\n\n"
            "       // on copie la valeur actuel ----------------- A ----------------\n"
            "       uint64_t valTmp[2] = {etat[0], etat[1]};\n"
            "       // on décale et xor de A bit à gauche :\n"
            "       // on décale valTmp\n"
            "       uint64_t retenue = 0xFFFFFFFFFFFFFFFFULL << (64-a);  // des 1 sur les bits qui seraient remplacé\n"
            "       retenue &= valTmp[1]; // la retenue devient les bits qui vont disparaitre de la partie [1]\n"
            "       valTmp[0] <<= a; // on décale la partie haute (trou à droite dans la partie [0])\n"
            "       valTmp[1] <<= a; // on décale la partie basse (trou à droite dans la partie [1])\n"
            "       valTmp[0] |= retenue >> (64-a); // on remplis le trou à droite (car la retenue est sauvegardé à droite) par ce qui est sortit de l'autre partie de val\n"
            "       // puis xor avec etat et valTmp\n"
            "       etat[0] ^= valTmp[0];\n"
            "       etat[1] ^= valTmp[1];\n\n"
            "       // on copie la valeur actuel ----------------- B ----------------\n"
            "       valTmp[0] = etat[0];\n"
            "       valTmp[1] = etat[1];\n"
            "       // on décale et xor de B bit à droite :\n"
            "       // on décale valTmp\n"
            "       retenue = 0xFFFFFFFFFFFFFFFFULL >> (64-b);  // des 1 sur les bits qui seraient remplacé\n"
            "       retenue &= valTmp[0]; // la retenue devient les bits qui vont disparaitre de la partie [0]\n"
            "       valTmp[0] >>= b; // on décale la partie haute (trou à gauche dans la partie [0])\n"
            "       valTmp[1] >>= b; // on décale la partie basse (trou à gauche dans la partie [1])\n"
            "       valTmp[1] |= retenue << (64-b); // on remplis le trou à droite (car la retenue est sauvegardé à droite) par ce qui est sortit de l'autre partie de val\n"
            "       // puis xor avec etat et valTmp\n"
            "       etat[0] ^= valTmp[0];\n"
            "       etat[1] ^= valTmp[1];\n\n"
            "       // on copie la valeur actuel ----------------- C ----------------\n"
            "       valTmp[0] = etat[0];\n"
            "       valTmp[1] = etat[1];\n"
            "       // on décale et xor de C bit à gauche :\n"
            "       // on décale valTmp\n"
            "       retenue = 0xFFFFFFFFFFFFFFFFULL << (64-c);  // des 1 sur les bits qui seraient remplacé\n"
            "       retenue &= valTmp[1]; // la retenue devient les bits qui vont disparaitre de la partie [1]\n"
            "       valTmp[0] <<= c; // on décale la partie haute (trou à droite dans la partie [0])\n"
            "       valTmp[1] <<= c; // on décale la partie basse (trou à droite dans la partie [1])\n"
            "       valTmp[0] |= retenue >> (64-c); // on remplis le trou à droite (car la retenue est sauvegardé à droite) par ce qui est sortit de l'autre partie de val\n"
            "       // puis xor avec etat et valTmp\n"
            "       etat[0] ^= valTmp[0];\n"
            "       etat[1] ^= valTmp[1];\n"
            "   }\n";
}

void Xorshift::changerA(){
    QSpinBox* spin = qobject_cast<QSpinBox*>(sender()); // sender() permet de récupérer le QObject qui a émis le signal
    if (spin) {
        a = static_cast<uint8_t>(spin->value());
    }
}

void Xorshift::changerB(){
    QSpinBox* spin = qobject_cast<QSpinBox*>(sender()); // sender() permet de récupérer le QObject qui a émis le signal
    if (spin) {
        b = static_cast<uint8_t>(spin->value());
    }
}

void Xorshift::changerC(){
    QSpinBox* spin = qobject_cast<QSpinBox*>(sender()); // sender() permet de récupérer le QObject qui a émis le signal
    if (spin) {
        c = static_cast<uint8_t>(spin->value());
    }
}
