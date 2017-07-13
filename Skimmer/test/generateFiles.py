import os

file_save = open("testfile.txt",'w') 

asps = []
print>>file_save, "readfiles=["
for root, dirs, files in os.walk(r'/eos/cms/store/t0streamer/Data/Physics/000/298/597'):
    for file in files:
        if file.endswith('.dat'):
            asps.append(file)
            print>>file_save, file+"',"

file_save.close()
