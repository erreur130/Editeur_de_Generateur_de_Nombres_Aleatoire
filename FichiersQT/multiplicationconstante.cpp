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

    /*---------------------------------------------------------------------------------------------------------------------------*/

}

Module* MultiplicationConstante::copie() const{
    return new MultiplicationConstante(parent(),constante);
}

QWidget* MultiplicationConstante::creerPaneauParametres(){
    QWidget* paneau = new QWidget;
    QVBoxLayout* layoutVerticale = new QVBoxLayout(paneau);
    QFormLayout* layoutParametre = new QFormLayout(); // rajouter plus tard dans layoutVerticale

    QLabel* labelTitre = new QLabel("Multiplisation Constante");
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
           /*---------------------------------------------------------------------------------------------------------------------------*/

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