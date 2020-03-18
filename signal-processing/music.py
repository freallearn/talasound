import numpy as np
import madmom

proc = madmom.features.notes.NotePeakPickingProcessor(fps=100)
act = madmom.features.notes.RNNPianoNoteProcessor()('imperialdragon.mp3')
res = proc(act)
print(res)