import socket
import sys
import struct

buffer_size = 255

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('', 7777))
while True:
    data, addr = sock.recvfrom(buffer_size)
    x, y = struct.unpack(">HH", data)
    print(x, y)
