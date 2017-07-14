import os

path_ = '/store/t0streamer/Data/PhysicsCommissioning/000/298/997/'
file_save = open("../python/AutoGenerate_cff.py",'w') 

asps = []
print>>file_save, "readFiles=["
for root, dirs, files in os.walk(r'/eos/cms'+path_):
    for file in files:
        if file.endswith('.dat'):
            asps.append(file)
            print>>file_save, "'"+path_ + file + "',"
print>>file_save, "]"
file_save.close()
