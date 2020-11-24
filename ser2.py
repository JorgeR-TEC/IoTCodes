import serial


ser=serial.Serial("COM3", 9600);

while(1):
    try:
        lineBytes=ser.readline()
        line=lineBytes.decode("ascii")
        line=line.rstrip()
        partes=line.split(";")
        hr=int(partes[0].split(":")[1])
        milis=int(partes[1].split(":")[1])
        print(hr, "milis",milis)
    except e:
        continue