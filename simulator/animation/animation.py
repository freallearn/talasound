import pygame, sys
from pygame.locals import *
import time
import csv
import shutil

# import subprocess
# import os



class Animation:

    def __init__(self):
        # set up de pygame
        pygame.init()

        self.fpsClock = pygame.time.Clock()
        self.FPS = 100
        self.FPS10 = 10

        self.DISPLAYSURF = pygame.display.set_mode((900, 600), 0, 32)
        pygame.display.set_caption('Animation')

        self.WHITE = (255, 255, 255)
        self.railImg = pygame.image.load('rail.png')
        self.bouleImg = pygame.image.load('boule.png')
        self.ligneImg = pygame.image.load('ligne.png')

        # set up des variables de postion, pour aligner les images
        self.constX = 100.
        self.posx = 0.
        self.difX = 0.
        self.posy = 0.
        self.difY = 0.
        self.consty = 300.

    def animate(self):

        # Creation d'un tableau contenant toutes les commandes
        tabCommande = [[-0.1, 0., 0.]]
        with open('./client/orders.txt', 'r') as file:
            reader = csv.reader(file, delimiter=";")
            for row in reader:
                new_row = [float(row[0]), float(row[1]), float(row[2])]
                tabCommande += [new_row]
        file.close()

        # affiche les deplacements
        for i in range(len(tabCommande)):
            # check le fichier commande
            with open('commande.txt', 'r') as fichier:
                commande = fichier.readline()
                fichier.close()
            vitesse = int(commande[2:4]) / 10
            # si le 1er chiffre est 0, met en pause
            while int(commande[0]) == 0:
                time.sleep(0.5)
                with open('commande.txt', 'r') as fichier:
                    commande = fichier.readline()
                    fichier.close()
            # recuperation de la position suivante
            self.difX = int((tabCommande[i][1] * 7 - self.posx) / self.FPS10)
            self.difY = int((tabCommande[i][2] - self.posy) / self.FPS10)
            # check des limites physiques des moteurs
            self.correct_limits_of_motors(vitesse)

            self.update_graphics(vitesse)

            # fermeture en cas de crash
            for event in pygame.event.get():
                if event.type == QUIT:
                    pygame.quit()
                    sys.exit()

        time.sleep(3)  # temps de pause a la fin de l'animation

    def correct_limits_of_motors(self, vitesse):
        """
        Correct limits of motor if necessary
        :param vitesse:
        :return:
        """
        if abs(self.difX) > 90 * 7 / (self.FPS * vitesse):
            self.difX = 90 * 7 / (vitesse * self.FPS * abs(self.difX) / self.difX)
        if abs(self.difY) > 470 / (self.FPS * vitesse):
            self.difY = 470 / (vitesse * self.FPS * abs(self.difY) / self.difY)

    def update_graphics(self, vitesse):
        """
        update the graphics and print the next position
        :param vitesse: the vitesse
        :return:
        """
        # affichage et passage a la position suivante
        for j in range(self.FPS10):
            self.DISPLAYSURF.fill(self.WHITE)
            self.DISPLAYSURF.blit(self.ligneImg, (int(self.constX), int(self.consty + 190)))
            self.DISPLAYSURF.blit(self.railImg, (int(self.constX + self.posx), int(self.consty)))
            self.DISPLAYSURF.blit(self.bouleImg,
                                  (
                                      int(self.constX + self.posx) - 7,
                                      int(self.consty - self.posy - 140)))  # corretifs pour aligner proprement
            self.posx += self.difX
            self.posy += self.difY
            # print(self.posx," ",self.posy)
            pygame.display.update()
            self.fpsClock.tick(vitesse * self.FPS)
