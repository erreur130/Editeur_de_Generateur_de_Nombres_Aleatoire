#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_aideModules.h"
#include "ui_aideTests.h"
#include "ui_infoProjet.h"
#include "demandenomclasse.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , modulesActif(new QVector<Module*>), modulesParDefaut(QVector<Module*>()), editeur(EGNA(modulesActif)), ui(new Ui::MainWindow){
    ui->setupUi(this);

    //connect ListeModulesActifs -> MainWindow
    connect(ui->listeModulesActifs, &ListeModulesActifs::envoyerIdModule, this, &MainWindow::recevoirIdModule);
    //connect ListeModulesTemplate -> MainWindow
    connect(ui->listeModulesTemplate, &ListeModulesTemplate::envoyerSuprimerModule, this, &MainWindow::recevoirSuprimerModule);

    // Initialiser l'image avec 1px blanc
    QImage image(1, 1, QImage::Format_Indexed8);
    QVector<QRgb> colorTable(255);
    colorTable[0] = qRgb(255, 255, 255);
    image.fill(0);
    image.setColorTable(colorTable);
    ui->imageBruit->setPixmap(QPixmap::fromImage(image));

    // On limite les valeurs possible à mettre dans le QLineEdite pour la graine (hexadécimale)
    ui->textGraine->setValidator(new QRegularExpressionValidator(QRegularExpression("^0x[0-9A-Fa-f]{1,16}$"), ui->textGraine));
    // On met la graine en visuel
    ui->textGraine->setText("0x" + QString::number(editeur.avoirGraine(), 16)); // hexadécimale

    // On met le nb valeur en visuel (par défaut c'est 16*16)
    ui->labelNbValeurs->setText(QString::number(ui->spinBoxResolution->value() * ui->spinBoxResolution->value()));
    // On change directement le NbValeursTotale pour qu'il soit racore avec le spinbox actuel
    editeur.changerNbValeursTotale(ui->spinBoxResolution->value() * ui->spinBoxResolution->value());

    // On remplie la liste des templates
    modulesParDefaut.push_back(new RotationBits(this, 63));
    modulesParDefaut.push_back(new RotationBits(this, 2)); // pour des tests
    /* ...----------------------------------------------------------------------------------------------------------------------------------------------------------*/
    afficherListeModules();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event){
    QMainWindow::resizeEvent(event); //pour conserver le comportement par défaut

    // le rezise de l'image carré / singleShot() pour que le layout ait le temps de s'update
    QTimer::singleShot(0, this, [this](){
        int tailleMin = qMin(ui->layoutImage->geometry().height(),ui->layoutImage->geometry().width());
        ui->imageBruit->setFixedSize(QSize(tailleMin - 10, tailleMin - 10));
    });
}

void MainWindow::afficherStats() const {
    if (modulesActif->size() == 0){ // affichage par défaut
        ui->labelAquilibreBits->setText("---");
        ui->labelMoyenne->setText("---");
        ui->labelCV->setText("---");
        ui->labelUniformite->setText("---");
        ui->labelCorrelation->setText("---");
    } else {
        ui->labelAquilibreBits->setText(QString::number(editeur.avoirEquilibreBits()*100.0));
        ui->labelMoyenne->setText(QString::number(editeur.avoirMoyenne()*100.0));
        ui->labelCV->setText(QString::number(editeur.avoirCV()*100.0));
        ui->labelUniformite->setText(QString::number(editeur.avoirUniformite())); // pas de * 100.0 ici, la valeurest déjà bonne
        ui->labelCorrelation->setText(QString::number(editeur.avoirAutocorrelation()*100.0));
    }
}

void MainWindow::afficherListeModules() const {
    ui->listeModulesTemplate->clear();
    for (qsizetype indice = 0; indice < modulesParDefaut.size(); indice++){
        QWidget *widget = modulesParDefaut[indice]->creerPaneauParametres(); // on créé le widget (le new est dans la fonction)

        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(widget->sizeHint()); // On définit la taille qu'il prendra dans la liste

        ui->listeModulesTemplate->addItem(item); // on met la taille
        ui->listeModulesTemplate->setItemWidget(item, widget); // on met le widget
    }
}

void MainWindow::miseAJourMethodesActives() const {
    ui->listeModulesActifs->clear();
    for (Module* module : *modulesActif){
        QWidget *widget = module->creerPaneauParametres(); // on créé le widget

        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(widget->sizeHint()); // On définit la taille qu'il prendra dans la liste

        ui->listeModulesActifs->addItem(item); // on met la taille
        ui->listeModulesActifs->setItemWidget(item, widget); // on met le widget
    }
}

void MainWindow::miseAJourTout(){
    editeur.renitialiserEtat();
    afficherBruit();
    afficherStats();
}

void MainWindow::afficherBruit(){
    ui->imageBruit->clear();

    if (modulesActif->size()){
        int resolution = ui->spinBoxResolution->value();
        QImage image(resolution, resolution, QImage::Format_Indexed8);

        // Initialiser la palette
        QVector<QRgb> colorTable(256);
        for (int i = 0; i < 256; ++i) colorTable[i] = qRgb(i, i, i);
        image.setColorTable(colorTable);

        // création de l'image
        for (int y = 0; y < resolution; ++y) {
            for (int x = 0; x < resolution; ++x) {
                unsigned char gris = editeur.suivantPixelBruit(); // nouvelle couleur généré
                image.setPixel(x, y, gris);
            }
        }

        // Affichage de l'image dans le label
        ui->imageBruit->setPixmap(QPixmap::fromImage(image));

    } else { // Sinon on met 1px blanc
        QImage image(1, 1, QImage::Format_Indexed8);
        QVector<QRgb> colorTable(255);
        colorTable[0] = qRgb(255, 255, 255);
        image.fill(0);
        image.setColorTable(colorTable);
        ui->imageBruit->setPixmap(QPixmap::fromImage(image));
    }
}

void MainWindow::on_actionModules_triggered(){
    QDialog *fenetre = new QDialog();
    Ui::AideModules ui;
    ui.setupUi(fenetre);
    fenetre->setWindowFlags(Qt::Window);  // force que ce soit une fenêtre indépendante
    fenetre->setWindowTitle(" ");
    fenetre->show();
}

void MainWindow::on_actionTests_triggered(){
    QDialog *fenetre = new QDialog();
    Ui::AideTests ui;
    ui.setupUi(fenetre);
    fenetre->setWindowFlags(Qt::Window);  // force que ce soit une fenêtre indépendante
    fenetre->setWindowTitle(" ");
    fenetre->show();
}

void MainWindow::on_actionInfo_Projet_triggered(){
    QDialog *fenetre = new QDialog();
    Ui::InfoProjet ui;
    ui.setupUi(fenetre);
    fenetre->setWindowFlags(Qt::Window);  // force que ce soit une fenêtre indépendante
    fenetre->setWindowTitle(" ");
    fenetre->show();
}

void MainWindow::on_actionSauvegarder_sous_triggered(){
    QString nomFichier;

    // demande à l'utilisateur le nom du ficher à créé
    DemandeNomClasse *fenetre = new DemandeNomClasse(this);
    connect(fenetre, SIGNAL(envoyerNomClasse(QString)), this, SLOT(recevoirNomClasse(QString)));
    fenetre->setWindowTitle(" ");
    fenetre->exec(); // bloque la fenetre

    // en attente d'une réponse ou non (recevoirNomClasse() est la suite)
}

void MainWindow::on_actionCharger_triggered(){
    QString nomFichier = QFileDialog::getOpenFileName( // permet de chercher un fichier
        0,
        "Gestionaire des fichiers",
        "/home",
        "Fichiers EGNA (*.egna)");

    if(nomFichier != ""){ // Si il existe on charge les données, puis le visuel
        /* -------------------------------------------- chargement ---------------------------------------------------------------- */
        QFile fichier(nomFichier);
        if(not(fichier.open(QFile::ReadOnly | QFile::Text))){ // Si on arrive pas à ouvrir
            qDebug() << "Erreur" << fichier.errorString() << " -> " << nomFichier;
            return;
        }

        modulesActif->clear();
        QDataStream in(&fichier); // fait une référence au fichier et permet de le manipuler comme un flux

        while (not(in.atEnd())){
            Module* module = Module::charger(in, this); // charger() fait le new
            if (module != nullptr)
                modulesActif->push_back(module);
        }

        fichier.close();

        // update
        miseAJourMethodesActives();
        miseAJourTout();
    }
}

void MainWindow::on_spinBoxResolution_editingFinished(){
    int val = ui->spinBoxResolution->value() * ui->spinBoxResolution->value();
    ui->labelNbValeurs->setText(QString::number(val));
    // update
    editeur.changerNbValeursTotale(val);
    miseAJourTout();
}

void MainWindow::on_textGraine_editingFinished(){
    // change la graine
    bool ok;
    uint64_t valeur = ui->textGraine->text().toULongLong(&ok, 16); // base 16 (hexadécimale)
    if (ok){
        editeur.changerGraine(valeur);
        // update
        miseAJourTout();

    } else
        qDebug() << "Graine non changer";
}

//------------------------------------- public slots -------------------------------------------------

void MainWindow::recevoirNomClasse(QString nomClasse){
    if(nomClasse != ""){
        // --------------------------------- Sauvegarde ----------------------------
        QString nomClasseMin = nomClasse.toLower(); // on prend le nom de la classe en minuscule pour le nom des fichiers
        { // ---------------------------------- Fichier .egna -----------------------------
            QFile fichierEGNA(nomClasseMin + ".egna");
            if(not(fichierEGNA.open(QFile::WriteOnly | QFile::Text))){ // Si on arrive pas à ouvrir
                qDebug() << "Erreur" << fichierEGNA.errorString();
                return;
            }
            QDataStream out(&fichierEGNA); // fait une référence au fichier et permet de le manipuler comme un flux

            for (Module* module : *modulesActif){
                module->sauvegarder(out);
            }

            fichierEGNA.close();
        }
        { // ---------------------------------- Fichier .hpp -----------------------------
            QFile fichierHPP(nomClasseMin + ".hpp");
            if(not(fichierHPP.open(QFile::WriteOnly | QFile::Text))){ // Si on arrive pas à ouvrir
                qDebug() << "Erreur" << fichierHPP.errorString();
                return;
            }
            QTextStream out(&fichierHPP); // fait une référence au fichier et permet de le manipuler comme un flux

             out << "// Cette classe à était créé grace à un éditeur disponible sur : https://github.com/erreur130/Editeur_Nombres_Aleatoire\n\n"
                    "#include <cmath>       // std::fmod\n"
                    "#include <bit>         // std::bit_cast\n"
                    "#include <type_traits> // std::is_integral_v, std::is_arithmetic_v\n"
                    "#include <iterator>    // std::distance, std::iter_swap\n"
                    "#include <stdexcept>   // std::invalid_argument, pour le throw\n"
                    "#include <cstdint>     // uint64_t\n\n"
                    "class " << nomClasse << " {\n" /* ------------ fonctions privées : ----------*/
                    "   void renitialiserEtat();\n" /*renitialiserEtat*/
                    "   void etatSuivant();\n\n" /*etatSuivant*/
                    "public :\n"                    /* ------------ fonctions publics : ----------*/
                    "   " << nomClasse << "();\n" /*constructeur par défaut*/
                    "   " << nomClasse << "(uint64_t graine);\n" /*constructeur avec uint64_t*/
                    "   ~" << nomClasse << "();\n" /*destructeur*/
                    "   inline void changerGraine(uint64_t graine_){graine = graine_;};\n" /*changerGraine*/
                    "   void regenererGraine();\n" /*regenererGraine*/
                    "   inline uint64_t avoirGraine() const{return graine;};\n\n" /*avoirGraine*/
                    "   template <typename T> T alea(T min, T max) const {\n" /*alea(T min, T max)*/
                    "       if (min > max)\n"
                    "           static_assert(false, \"alea() : Valeur minimale est supérieur à celle maximale!\"); // plante à la compilation\n"
                    "       if constexpr (std::is_integral_v<T>){ // logique entière\n"
                    "           uint64_t plage = static_cast<uint64_t>(max) - static_cast<uint64_t>(min) + 1;\n"
                    "           return min + static_cast<T>(etat[1] % plage);\n"
                    "       } else if constexpr (std::is_floating_point_v<T>){ // logique flottante\n"
                    "           uint64_t bitsSecurises = etat[1] & 0x7FEFFFFFFFFFFFFFULL; // 0x7FF enlève le signe et NaN/inf\n"
                    "           double valeurBrute = std::bit_cast<double>(bitsSecurises); // double et uint64_t on tout les deux 8 octets\n"
                    "           T plage = max - min;\n"
                    "           return min + std::fmod(static_cast<T>(valeurBrute), plage); // std::fmod() = % mais pour les nb à virgules flotantes\n"
                    "       } else {\n"
                    "            static_assert(false, \"alea() : Type non supporté!\");"
                    "       }\n"
                    "       etatSuivant();\n"
                    "   }\n\n"
                    "   template <typename T> T alea(T max) const {\n" /*alea(T max)*/
                    "       return alea(static_cast<T>(0), max);\n"
                    "   }\n\n"
                    "   bool aleaBool() const {\n" /*aleaBool*/
                    "       return static_cast<bool>(etat[1]);"
                    "   }\n\n"
                    "   template <std::random_access_iterator Iterator> void melanger(Iterator debut, Iterator fin) const {\n" /*melanger*/
                    "       if (fin < debut)\n"
                    "           throw std::invalid_argument(\"melanger() : l'iterateur fin est avant debut, plage invalide!\")\n"
                    "       else if (debut == fin)\n"
                    "           return; // rien à mélanger\n\n"
                    "       // algo de Fisher-Yates ou algorithme de Knuth\n"
                    "       for (auto it = fin - 1; it > debut; --it){\n"
                    "           size_t distance = std::distance(debut, it);\n"
                    "           size_t indexAleatoire = alea<size_t>(distance);\n"
                    "           std::iter_swap(it, debut + indexAleatoire);\n"
                    "       }"
                    "   }\n\n"
                    "};";
            fichierHPP.close();
        }
        { // ---------------------------------- Fichier .cpp -----------------------------
            QFile fichierCPP(nomClasseMin + ".cpp");
            if(not(fichierCPP.open(QFile::WriteOnly | QFile::Text))){ // Si on arrive pas à ouvrir
                qDebug() << "Erreur" << fichierCPP.errorString();
                return;
            }
            QTextStream out(&fichierCPP); // fait une référence au fichier et permet de le manipuler comme un flux

            out <<  "// Cette classe à était créé grace à un éditeur disponible sur : https://github.com/erreur130/Editeur_Nombres_Aleatoire\n\n"
                    "#include \"" << nomClasse << ".hpp\"\n\n"
                    << nomClasse << "::" << nomClasse << "()\n" /*constructeur par défaut*/
                    ": graine(0), etat{uint64_t()}{\n"
                    "   regenererGraine();\n"
                    "}\n\n"
                    << nomClasse << "::" << nomClasse << "(uint64_t graine_)\n" /*constructeur avec un uint64_t*/
                    ": graine(graine_), etat{uint64_t()}{}\n\n"
                    << nomClasse << "::~" << nomClasse << "(){}\n\n" /*destructeur*/
                    "void " << nomClasse << "::renitialiserEtat(){\n" /*renitialiserEtat*/
                    "   etat[0] = graine;\n"
                    "   etat[1] = -graine; // -graine est une valeur non signé, c'est juste pour faire une valeur différente\n"
                    "   etatSuivant();\n"
                    "}\n\n"
                    "void " << nomClasse << "::etatSuivant(){\n" /*etatSuivant()*/
                    "   // suite de calcules qui change l'état\n\n";
            for (Module* module: *modulesActif)
                module->ecrireAlgo(out);
            out <<  "}\n\n"
                    "void " << nomClasse << "::regenererGraine(){\n" /*regenererGraine*/
                    "   // Pour ne pas dépendre d'un autre algo d'aléatoire on initialise la graine avec le temps\n"
                    "   using namespace std::chrono;\n"
                    "   time_point<system_clock> now = system_clock::now();\n"
                    "   system_clock::duration temps = now.time_since_epoch();\n"
                    "   // Conversion duration -> nanoseconds -> uint64_t\n"
                    "   nanoseconds tempsNano = duration_cast<nanoseconds>(temps);\n"
                    "   graine = static_cast<uint64_t>(tempsNano.count());\n"
                    "   // Puis on change l'état pour qui soit conforme aux changement\n"
                    "   renitialiserEtat();\n"
                    "}\n";
            fichierCPP.close();
        }
    }
}

void MainWindow::recevoirIdModule(int idOrigine, int idCible, bool vientDeTemplate){
    if (vientDeTemplate)
        modulesActif->insert(idCible, modulesParDefaut[idOrigine]->copie()); // copie() fait un new
    else { // vient des modules actifs
        Module* tmp = (*modulesActif)[idOrigine]; // On sauvegarde pour pas perdre le pointeur
        modulesActif->removeAt(idOrigine); // On suprime ce qu'on vient de déplacer
        modulesActif->insert(idCible + ((idCible > idOrigine)?-1:0), tmp); // déplace le pointeur / si idCible > idOrigine alors il y auras un décalage de trop car remouveAT avant donc -1
    }
    // update
    miseAJourMethodesActives();
    miseAJourTout();
}

void MainWindow::recevoirSuprimerModule(int idOrigine, bool vientDeTemplate){
    if (not(vientDeTemplate)){ // si Actifs -> Template, alors on suprime l'élément
        modulesActif->removeAt(idOrigine); // On suprime ce qu'on vient de déplacer
        // update
        miseAJourMethodesActives();
        miseAJourTout();
    }
}
void MainWindow::on_boutonChangerGraine_clicked(){
    editeur.changerGraine();
    // On met la nouvelle graine en visuel et le reste
    ui->textGraine->setText("0x" + QString::number(editeur.avoirGraine(), 16)); // hexadécimale
    miseAJourTout();
}

