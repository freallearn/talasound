import sys
import numpy as np
import madmom
from aubio import source, pitch


#####################################################################################
# Function: detect_beat																#
# Author: Charles Paillot															#
# Brief: Detect the beats of a music track 											#
# Parameter: the mp3 file of the music track 										#
# Return: A (n,3)-array with all n beats: [time, 0.0, 100.0]    					#
#####################################################################################
def detect_beat(filename):
	proc = madmom.features.beats.BeatTrackingProcessor(fps=100)
	act = madmom.features.beats.RNNBeatProcessor()(filename)
	res = proc(act)

	length = len(res)
	tab = []
	for i in range(length):
		tab += [[res[i], 0.0, 100.0]]

	return(tab)


#####################################################################################
# Function: detect_pitch															#
# Author: Charles Paillot															#
# Brief: Detect the notes' pitches of a music track 								#
# Parameter: the mp3 file of the music track 										#
# Return: A (n,3) array with all n pitches: [time, pitch, 0.0]    					#
#####################################################################################
def detect_pitch(filename):
	downsample = 1
	samplerate = 600                  

	win_s = 4096 // downsample
	hop_s = 450  // downsample			

	s = source(filename, samplerate, hop_s)
	samplerate = s.samplerate
	tolerance = 0.8

	pitch_o = pitch("yin", win_s, hop_s, samplerate)
	pitch_o.set_unit("midi")
	pitch_o.set_tolerance(tolerance)

	tab = []

	pitch_min = 0.0
	pitch_max = 0.0

	total_frames = 0
	while True:
		samples, read = s()
		time = total_frames/float(samplerate)
		note = pitch_o(samples)[0]
		tab += [[time, note, 0.0]]

		if (total_frames == 0):
			pitch_min = note
			pitch_max = note
		else:
			if (pitch_min > note):
				pitch_min = note
			if (pitch_max < note):
				pitch_max = note

		total_frames += read
		if read < hop_s:
			break

	diff = pitch_max-pitch_min

	for line in tab:
		line[1] = (line[1]-pitch_min)*100.0/diff

	return(tab)


#####################################################################################
# Function: triTabs																	#
# Author: Charles Paillot															#
# Brief: fuse both array to obtain the full orders array 							#
# Parameters: Both pitches and beats (n,3)-arrays									#
# Return: A (n,3)-array	with all n orders (time, pitch, beat)						#
#####################################################################################
def triTabs(pitches, beats):
	x = 0
	y = 0
	xmax = len(pitches)
	ymax = len(beats)
	tab = []

	while ((x < xmax) or (y < ymax)):
		if ((x == xmax) or (y == ymax)):
			if (x == xmax):
				if (x > 0):
					beats[y][1] = pitches[x-1][1]
				tab += [beats[y]]
				y+=1
			else:
				tab += [pitches[x]]
				x+=1
		else:
			time_pitch = pitches[x][0]
			time_beat = beats[y][0]
			if (time_pitch == time_beat):
				tab += [[time_pitch, pitches[x][1], beats[y][2]]]
				x+=1
				y+=1
			elif (time_pitch < time_beat):
				tab += [pitches[x]]
				x+=1
			else:
				if (x == 0):
					beats[y][1] = pitches[x][1]*time_beat/time_pitch
				else:
					diffTime = (time_pitch-time_beat)/(time_pitch-pitches[x-1][0])
					diffPitch = pitches[x][1]-pitches[x-1][1]
					beats[y][1] = pitches[x][1]-diffPitch*diffTime 
				tab += [beats[y]]
				y+=1

	return(tab)


#####################################################################################
# File: musicProcessing.py															#
# Author: Charles Paillot															#
# Brief: Analyze a music track to make a orders file for a 2-motors system			#
# Parameter: the mp3 file of the music track										#
# Return: Write a CSV file with every orders (time, motor1, motor2)					#
#####################################################################################
if __name__ == "__main__":
	if (len(sys.argv) == 2):
		filename = sys.argv[1]

		tabPitch = detect_pitch(filename)[:]
		tabBeat = detect_beat(filename)[:]

		tabFinal = triTabs(tabPitch, tabBeat)[:]
		print(tabFinal)
		fic = open("orders.csv", "w")
		nbMoves = len(tabFinal)
		for i in range(nbMoves):
			fic.write(str(tabFinal[i][0])+";"+("%.2f"%tabFinal[i][1])+";"+str(tabFinal[i][2]))
			if (i < nbMoves-1):
				fic.write("\n")
		fic.close()

	else:
		print("Error: wrong number of arguments.")
