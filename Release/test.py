import subprocess


def run(dim,c,reserve,output):
    res=subprocess.Popen("./FastHash "+str(dim)+" "+str(c)+" "+str(reserve)+" 1", shell=True, stdout=subprocess.PIPE).stdout.read().decode()
    if "ERROR" in res:
        print("ERROR")
        exit(0)
    print("DIM:"+str(dim)+"\tC:"+str(c)+"\tR:"+str(reserve))
    res=res.replace("\n","\n\t")
    print("\t"+res)

DIM=15000000
chars=[4,
       6,8]
resultNoReserve={}
resultReserve={}
for c in chars:
    run(DIM,c,0,resultNoReserve)
    run(DIM,c,1,resultReserve)

header=""
for k,v in resultNoReserve.items():
    header=header+k+"\t"
    
