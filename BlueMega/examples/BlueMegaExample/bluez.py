import bluetooth
import sys
import time
bd_addr = "00:13:EF:12:16:49" #itade address

port = 1
sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )

try:
    sock.connect((bd_addr, port))
    print 'Connected'
    sock.settimeout(10.0)
    sock.send("#ol#")
    time.sleep(2)
    sock.send("a")
    time.sleep(0.5)
    #sock.send("\r\n+INQ=1\r\n")
    print 'Sent data'
    data = sock.recv(1)
    while '\r\n' not in data:
        data += sock.recv(1)
    print repr(data)
    sock.send("FASE 2")
    time.sleep(0.5)
    sock.send("a")
    print 'Sent again'
    time.sleep(1)
    data = sock.recv(1)
    while '\r\n' not in data:
        data += sock.recv(1)
    print repr(data)
    print "fim fase 2"
except Exception as e:
    print e
finally:
    sock.close()
    print 'bye'
