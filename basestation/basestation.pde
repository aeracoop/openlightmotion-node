import hypermedia.net.*;
import java.nio.*;

int PORT_RX = 7777;
String HOST_IP = "0.0.0.0";

UDP udp;

void setup(){
    udp = new UDP(this, PORT_RX, HOST_IP);
    udp.listen(true);
    print("Listening on ", HOST_IP, ":", PORT_RX);
    noLoop();
}

void draw(){

}

void receive(byte[] data, String HOST_IP, int PORT_RX) {
    ByteBuffer buf = ByteBuffer.wrap(data).order(ByteOrder.BIG_ENDIAN);
    short x = buf.getShort();
    short y = buf.getShort();
    println("Packet:", x, ",", y);
}