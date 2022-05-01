import random
def pontosIguais(a,b):
    if(a[0]==b[0] and a[1]==b[1] and a[2]==b[2]):
        return True
    return False

f = open("comet.patch", "r")
temp = f.read().splitlines()
count = 0

patches = int(temp[count])
count+=1


lines = []
for x in range(0,patches):
    colunas = temp[count].split(",")
    count+=1
    for col in colunas:
        lines.append(int(col))

npoints = int(temp[count])
count+=1
points = []
for x in range(0,npoints):
    ponto = temp[count].split(",")
    count+=1
    points.append((float(ponto[0]),float(ponto[1]),float(ponto[2])))

pares = []
for x in range(0,npoints):
    pares.append((lines[x],points[x]))
changed = []

removed = 0
for ord in range(0,len(pares)-1):
    if(ord not in changed):
        for rec in range(ord+1 , len(pares)):
            if(rec not in changed and pontosIguais(pares[ord][1],pares[rec][1])):
                removed += 1
                changed.append(rec)
                pares[rec]= (pares[ord][0],"")
                for y in range(rec+1,len(lines)):
                    if(y not in changed):
                        pares[y]= (int(pares[y][0])-1,pares[y][1])
        changed.append(ord)
    

with open('randomizedPatch.txt', 'w') as newFile:
    newFile.write(str(patches)+"\n")
    for x in range(0,patches):
        for x1 in range(0,16):
            if(x1==15):
                newFile.write(str(pares[x*16+x1][0])+"\n")
            else: newFile.write(str(pares[x*16+x1][0])+",")
    newFile.write(str(npoints-removed)+"\n")
    for par in pares:
        if(par[1]!=""):
            reduction = random.randint(45,100)/100
            newFile.write(" "+str(par[1][0]*reduction)+" ,"+str(par[1][1]*reduction)+" ,"+str(par[1][2]*reduction)+"\n")