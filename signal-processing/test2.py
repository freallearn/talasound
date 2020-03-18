from aubio import source, pitch

filename = "imperialdragon.mp3"

downsample = 1
samplerate = 600                   #Nombre de frames par seconde

win_s = 4096 // downsample
hop_s = 450  // downsample         #Saut de frames

s = source(filename, samplerate, hop_s)
samplerate = s.samplerate

tolerance = 0.8

pitch_o = pitch("yin", win_s, hop_s, samplerate)
pitch_o.set_unit("midi")
pitch_o.set_tolerance(tolerance)

pitches = []

pitch_min = 0
pitch_max = 0

total_frames = 0
while True:
    samples, read = s()
    time = total_frames/float(samplerate)
    pitch = pitch_o(samples)[0]
    pitches += [[time, pitch, 0.0]]

    if (total_frames == 0):
        pitch_min = pitch
        pitch_max = pitch
    else:
        if (pitch_min > pitch):
            pitch_min = pitch
        if (pitch_max < pitch):
            pitch_max = pitch

    total_frames += read
    if read < hop_s: 
        break

print("Valeur min : "+str(pitch_min))
print("Valeur max : "+str(pitch_max))

diff = pitch_max-pitch_min

for pitch in pitches:
    pitch[1] = (pitch[1]-pitch_min)*100.0/diff

print(pitches)