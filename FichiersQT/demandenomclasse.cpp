#include "demandenomclasse.h"
#include "ui_demandenomclasse.h"

DemandeNomClasse::DemandeNomClasse(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DemandeNomClasse)
{
    ui->setupUi(this);

    // On limite les valeurs possible à mettre dans le QLineEdite pour textNom avec toutes les lettre, chiffres , _ et -
    ui->textNom->setValidator(new QRegularExpressionValidator(QRegularExpression("^[A-Za-z0-9_-]+$"), ui->textNom));
}

DemandeNomClasse::~DemandeNomClasse()
{
    delete ui;
}

void DemandeNomClasse::on_pushButton_clicked(){
    emit envoyerNomClasse(ui->textNom->text());
    close();
}

