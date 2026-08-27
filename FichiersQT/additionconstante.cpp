#include "additionconstante.h"
#include "mainwindow.h" // dans le cpp pour évité la récusivité

AdditionConstante::AdditionConstante(QObject* parent, uint64_t constante_)
    : Module(parent), constante(constante_){
    // 0 < constante < 2^64 - 1
}

AdditionConstante::AdditionConstante(QObject* parent, QDataStream & in)
    : Module(parent), constante(1){
    quint64 constante_;
    in >> constante_; // uint64_t non reconue
    constante = static_cast<uint64_t>(constante_);
}

void AdditionConstante::valeurSuivante(uint64_t (&val)[2]) const {
    uint64_t tmp = val[1];
    val[1] += constante; // on fait l'addition
    if (tmp > val[1]){ // alors overflow
        val[0]++;
    }
}

Module* AdditionConstante::copie() const{
    return new AdditionConstante(parent(),constante);
}

QWidget* AdditionConstante::creerPaneauParametres(){
    QWidget* paneau = new QWidget;
    QVBoxLayout* layoutVerticale = new QVBoxLayout(paneau);
    QFormLayout* layoutParametre = new QFormLayout(); // rajouter plus tard dans layoutVerticale

    QLabel* labelTitre = new QLabel("Addition Constante");
    QLineEdit* nbConstante = new QLineEdit();
    QFrame* separateur = new QFrame();
    separateur->setFrameShape(QFrame::HLine);   // ligne horizontale
    separateur->setFrameShadow(QFrame::Sunken); // effet visuel "enfoncé"

    // paramètre du paneau :
    labelTitre->setAlignment(Qt::AlignHCenter);
    nbConstante->setValidator(new QRegularExpressionValidator(QRegularExpression("^0x[0-9A-Fa-f]{1,16}$"), nbConstante)); // On limite les valeurs possible (hexadécimale)
    nbConstante->setText("0x" + QString::number(constante, 16)); // On met le bon visuel en hexadécimale
    // connect spinBox -> Module
    connect(nbConstante, &QLineEdit::editingFinished, this, &AdditionConstante::changerConstante);
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

void AdditionConstante::sauvegarder(QDataStream & out) const{
    quint64 constante_ = static_cast<quint64>(constante);
    out << QString("AdditionConstante") << constante_;
}

void AdditionConstante::ecrireAlgo(QTextStream & out) const{
    out <<  "   {\n"
            "       uint64_t constante = " << constante << ";\n"
            "       uint64_t tmp = etat[1];\n"
            "       etat[1] += constante; // on fait l'addition\n"
            "       if (tmp > etat[1]){ // alors overflow\n"
            "           etat[0]++;"
            "       }\n"
            "   }\n";
}

void AdditionConstante::changerConstante(){
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender()); // sender() permet de récupérer le QObject qui a émis le signal
    if (lineEdit) {
        bool ok;
        uint64_t valeur = lineEdit->text().toULongLong(&ok, 16); // base 16 (hexadécimale)
        if (ok)
            constante = static_cast<uint64_t>(valeur);
    }
}

