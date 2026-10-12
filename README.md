# Editeur_de_Générateur_de_Nombres_Aleatoire
Un éditeur QT qui permet de créer des classes C++ personnalisées pour faire de l'aléatoire.



L'éditeur gère les sauvegrades et peut générer 3 fichiers avec un nom personaliser suite à la sauvegarde d'un projet :

- nom_classe.cpp et nom_class.hpp qui représentent la Classe C++ éditer, qui a les fonction décrite dans le diagramme UML
Attention ! la classe C++ créée doit être compiler avec -std=c++20

- nom_classe.egna est le fichier que l'éditeur à besoin pour recharger un projet


L'éditeur à été pensé pour être modulable, c'est à dire que les modules existant peuvent être facilement :
suprimer / modifier
D'autre modules peuvent aussi être créé mais pour marcher il faudras avoir c'est points :
- Avoir la même structure que les autre Modules filles (voir un .cpp .hpp ou le diagramme UML)
- #include "mainwindow.h" // dans Nom_du_module.cpp et module.cpp pour évité la récusivité
- #include "Nom_du_module.h" // dans mainwindow.h
- modulesParDefaut.push_back(new Nom_du_module(this)); // à la fin du constructeur de Mainwindow et avant afficherListeModules();
- else if (type == "Nom_du_module") return new Nom_du_module(parent, in); // à la suite de ceux dans Module::charger() dans module.cpp
