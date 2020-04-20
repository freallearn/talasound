talasound
==============


# Configuration requise
  - cmake
  - qt5
  - python version 3
  

# Interface graphique

Dans le dossier talasound-ui, l'interface graphique a été développée en Qt5 avec le langage C++

L'interface graphique communique via le protocole TCP, elle transmettra différents ordres d'exécutions à la tala box.

Ordre :
  - start:'nommusique' => envoie l'ordre démarrer la musique
  - stop => arrête la musique
  - pause => met en pause la musique
  - envoi => envoi le fichier d'ordre
  

# Signal Processing

Contient tout le traitement de la musique.
