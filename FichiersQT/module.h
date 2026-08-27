#ifndef MODULE_H
#define MODULE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QFormLayout>
#include <QFile>
#include <QLineEdit>


class Module : public QObject {
public:
    explicit Module(QObject* parent);
    virtual void valeurSuivante(uint64_t (&val)[2]) const = 0; // prend par référence
    virtual Module* copie() const = 0;
    virtual QWidget* creerPaneauParametres() = 0;
    virtual void sauvegarder(QDataStream &) const = 0;
    virtual void ecrireAlgo(QTextStream &) const = 0;

    static Module* charger(QDataStream & in, QObject* parent);
};

#endif // MODULE_H
