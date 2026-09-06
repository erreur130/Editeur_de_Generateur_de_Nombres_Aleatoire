#include "multiplicationconstante.h"
#include "mainwindow.h" // dans le cpp pour évité la récusivité

MultiplicationConstante::MultiplicationConstante(QObject* parent, uint64_t constante_)
    : Module(parent), constante(constante_){
    // 0 < constante < 2^64 - 1
}

MultiplicationConstante::MultiplicationConstante(QObject* parent, QDataStream & in)
    : Module(parent), constante(1){
    quint64 constante_;
    in >> constante_; // uint64_t non reconue
    constante = static_cast<uint64_t>(constante_);
}

void MultiplicationConstante::valeurSuivante(uint64_t (&val)[2]) const {
    // multiplication d'une constante
    // val × mult   = (valHaut x 2³² + valBas) × (multHaut x 2³² + multBas)
    //              = valHaut x multHaut x 2⁶⁴ + valHaut x multBas x 2³² + valBas x multHaut x 2³² + valBas x multBas
    //              =         haut x 2⁶⁴       +         m1 x 2³²        +        m2 x 2³²         +      bas

    uint64_t valHaut = val[1] >> 32;
    uint64_t valBas = val[1] & 0xFFFFFFFFULL;
    uint64_t multHaut = constante >> 32;
    uint64_t multBas = constante & 0xFFFFFFFFULL;

    uint64_t haut = valHaut * multHaut; // x 2⁶⁴
    uint64_t m1 = valHaut * multBas; // x 2³²
    uint64_t m2 = valBas * multHaut; // x 2³²
    uint64_t bas = valBas * multBas;

    uint64_t retenue = ((bas >> 32) + (m1 & 0xFFFFFFFFULL) + (m2 & 0xFFFFFFFFULL)) >> 32;
    val[1] = bas + (m1 << 32) + (m2 << 32); // les 64 premier bits
    val[0] = (val[0] * constante) + (haut + (m1 >> 32) + (m2 >> 32) + retenue); // les 64 dernier bits, + on addtitionne la multiplication de la partie haute et sur surplus de celle du bas

}

Module* MultiplicationConstante::copie() const{
    return new MultiplicationConstante(parent(),constante);
}

QWidget* MultiplicationConstante::creerPaneauParametres(){
    QWidget* paneau = new QWidget;
    QVBoxLayout* layoutVerticale = new QVBoxLayout(paneau);
    QFormLayout* layoutParametre = new QFormLayout(); // rajouter plus tard dans layoutVerticale

    QLabel* labelTitre = new QLabel("Multiplication Constante");
    QLineEdit* nbConstante = new QLineEdit();
    QFrame* separateur = new QFrame();
    separateur->setFrameShape(QFrame::HLine);   // ligne horizontale
    separateur->setFrameShadow(QFrame::Sunken); // effet visuel "enfoncé"

    // paramètre du paneau :
    labelTitre->setAlignment(Qt::AlignHCenter);
    nbConstante->setValidator(new QRegularExpressionValidator(QRegularExpression("^0x[0-9A-Fa-f]{1,16}$"), nbConstante)); // On limite les valeurs possible (hexadécimale)
    nbConstante->setText("0x" + QString::number(constante, 16)); // On met le bon visuel en hexadécimale
    // connect spinBox -> Module
    connect(nbConstante, &QLineEdit::editingFinished, this, &MultiplicationConstante::changerConstante);
    // connect spinBox -> MainWindow
    connect(nbConstante, &QLineEdit::editingFinished, qobject_cast<MainWindow*>(parent()), &MainWindow::miseAJourTout);
    layoutVerticale->setContentsMargins(5, 5, 5, 0);

    // on répartie les Wiget dans les layouts
    layoutParametre->addRow("Constante :", nbConstante);
    layoutVerticale->addWidget(labelTitre);
    layoutVerticale->addLayout(layoutParametre);
    layoutVerticale->addWidget(separateur);

    return paneau;
}

void MultiplicationConstante::sauvegarder(QDataStream & out) const{
    quint64 constante_ = static_cast<quint64>(constante);
    out << QString("MultiplicationConstante") << constante_;
}

void MultiplicationConstante::ecrireAlgo(QTextStream & out) const{
    out <<  "   {\n"
            "       uint64_t constante = " << constante << ";\n"
            "       // multiplication d'une constante\n"
            "       // etat × mult   = (etatHaut x 2³² + etatBas) × (multHaut x 2³² + multBas)\n"
            "       //              = etatHaut x multHaut x 2⁶⁴ + etatHaut x multBas x 2³² + etatBas x multHaut x 2³² + etatBas x multBas\n"
            "       //              =         haut x 2⁶⁴       +         m1 x 2³²        +        m2 x 2³²         +      bas\n\n"
            "       uint64_t etatHaut = etat[1] >> 32;\n"
            "       uint64_t etatBas = etat[1] & 0xFFFFFFFFULL;\n"
            "       uint64_t multHaut = constante >> 32;\n"
            "       uint64_t multBas = constante & 0xFFFFFFFFULL;\n\n"
            "       uint64_t haut = etatHaut * multHaut; // x 2⁶⁴\n"
            "       uint64_t m1 = etatHaut * multBas; // x 2³²\n"
            "       uint64_t m2 = etatBas * multHaut; // x 2³²\n"
            "       uint64_t bas = etatBas * multBas;\n\n"
            "       uint64_t retenue = ((bas >> 32) + (m1 & 0xFFFFFFFFULL) + (m2 & 0xFFFFFFFFULL)) >> 32;\n"
            "       etat[1] = bas + (m1 << 32) + (m2 << 32); // les 64 premier bits\n"
            "       etat[0] = (etat[0] * constante) + (haut + (m1 >> 32) + (m2 >> 32) + retenue); // les 64 dernier bits, + on addtitionne la multiplication de la partie haute et sur surplus de celle du bas\n"
            "   }\n";
}

void MultiplicationConstante::changerConstante(){
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender()); // sender() permet de récupérer le QObject qui a émis le signal
    if (lineEdit) {
        bool ok;
        uint64_t valeur = lineEdit->text().toULongLong(&ok, 16); // base 16 (hexadécimale)
        if (ok)
            constante = static_cast<uint64_t>(valeur);
    }
}