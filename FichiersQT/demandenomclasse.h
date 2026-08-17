#ifndef DEMANDENOMCLASSE_H
#define DEMANDENOMCLASSE_H

#include <QDialog>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

namespace Ui {
class DemandeNomClasse;
}

class DemandeNomClasse : public QDialog
{
    Q_OBJECT

public:
    explicit DemandeNomClasse(QWidget *parent = nullptr);
    ~DemandeNomClasse();

private slots:
    void on_pushButton_clicked();

signals:
    void envoyerNomClasse(QString);

private:
    Ui::DemandeNomClasse *ui;
};

#endif // DEMANDENOMCLASSE_H
