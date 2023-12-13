
import sys 

file1 = open(sys.argv[1], "r")

wk = 0
wo = 0
wq = 0
wv = 0
w1 = 0
w2 = 0
w3 = 0

layerDict = {"wk":0, "wo":0, "wq":0, "wv":0, "w1":0, "w2":0, "w3":0}
count = {"wk":0, "wo":0, "wq":0, "wv":0, "w1":0, "w2":0, "w3":0}

for line in file1.readlines():
    line = line.split(" ")
    cycles = int(line[3])
    name = line[0].split(".")[3]
    layerDict[name] = layerDict[name] + cycles 
    count[name] = count[name] + 1
file1.close()

for key in layerDict.keys():
    print(key, layerDict[key] / count[name])   

