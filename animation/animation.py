import pygame, sys
from pygame.locals import *
import time
import csv
import subprocess

#Creation d'un tableau contenant toutes les commandes
tabCommande = [[-0.1,0.,0.]]
with open('orders2.csv', 'r') as file:
    reader = csv.reader(file, delimiter=";")
    for row in reader:
        new_row = [float(row[0]), float(row[1]), float(row[2])]
        tabCommande+=[new_row]
file.close()

# set up de pygame
pygame.init()
FPS = 100
FPS10 = 10
fpsClock = pygame.time.Clock()

DISPLAYSURF = pygame.display.set_mode((900, 600), 0, 32)
pygame.display.set_caption('Animation')

WHITE = (255, 255, 255)
railImg = pygame.image.load('rail.png')
bouleImg = pygame.image.load('boule.png')
ligneImg = pygame.image.load('ligne.png')

# set up des variables de postion, pour aligner les images
constX = 100.
posx = 0.
difX = 0.
posy = 0.
difY = 0.
consty = 300.

# affiche les deplacements
for i in range(len(tabCommande)):
    #check le fichier commande
    with open('commande.txt','r') as fichier:
        commande = fichier.readline()
        fichier.close()
    vitesse = int(commande[2:4])/10
    # si le 1er chiffre est 0, met en pause
    while int(commande[0])==0:
        time.sleep(0.5)
        with open('commande.txt','r') as fichier:
            commande = fichier.readline()
            fichier.close()
    #recuperation de la position suivante
    difX = int((tabCommande[i][1]*7-posx)/FPS10)
    difY = int((tabCommande[i][2]-posy)/FPS10)
    #check des limites physiques des moteurs
    if abs(difX) > 90*7/(FPS*vitesse):
        difX = 90*7/(vitesse*FPS*abs(difX)/difX)
    if abs(difY) > 470/(FPS*vitesse):
        difY = 470/(vitesse*FPS*abs(difY)/difY)
    #affichage et passage a la position suivante
    for j in range(FPS10):
        DISPLAYSURF.fill(WHITE)
        DISPLAYSURF.blit(ligneImg, (int(constX), int(consty+190)))
        DISPLAYSURF.blit(railImg, (int(constX+posx), int(consty)))
        DISPLAYSURF.blit(bouleImg, (int(constX+posx)-7, int(consty-posy-140))) #corretifs pour aligner proprement
        posx+=difX
        posy+=difY
        pygame.display.update()
        fpsClock.tick(vitesse*FPS)

    #fermeture en cas de crash
    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            sys.exit()



time.sleep(3) #temps de pause a la fin de l'animation
sys.exit()