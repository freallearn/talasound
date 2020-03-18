import numpy as np
import madmom

proc = madmom.features.beats.BeatTrackingProcessor(fps=100)
act = madmom.features.beats.RNNBeatProcessor()('imperialdragon.mp3')
res = proc(act)

length = len(res)
tab = []
for i in range(length):
	tab += [[res[i], 0.0, 100.0]]

print(tab)