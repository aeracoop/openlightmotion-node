# OpenLightMotion
Tracking drones in infrared

A project to capture 3D position from passive IR markers.

## Open
This code is licensed under the General Public License v3.0

## Light
We use infrared light, invisible to humans, to track high speed drones.

## Motion
With two Pixy smart cameras we capture motion and send the information to a base station.

## System Architecture

### Nodes
Each node is composed of a Pixy camera connected via SPI bus to an ESP8266 microcontroller.
The camera tracks the target in its field of vision, and produces a stream of 2D coordinates as the target moves. These coordinates are picked up by the ESP8266
and sent via UDP to the base station.
Additionally each node exposes its own individual WiFi access point to allow configuration and firmware updates.
The code in this repository is the firmware for the ESP8266 in the nodes.

### Base station
The base station is a central machine which receives the 2D coordinates of the target from all the nodes, and calculates the real 3D position.

## Build and Install
To build this firmware you need to use the platformio framework.
The simplest way, if you don't want to use the platformio IDE, is to install the command line version (platformio core) following the instructions here:
http://docs.platformio.org/en/stable/core.html#core
http://docs.platformio.org/en/stable/installation.html#installation-methods

Then you can build with:
`$ platformio run`

And you can upload to the device with:
`$ platformio run --target upload`

If platformio fails to auto-detect your serial connection, you can add
`upload_port = /dev/yourSerialDevice`
To the platformio.ini file.

## Node Configuration

By default all nodes start by bringing up an AP called `OpenLightMotionNode`, with a web server listening on 192.168.0.1

Start one node at a time and connect to its AP with your machine.
Access http://192.168.0.1/ to view the current configuration (which will be empty right now). Set configuration options by accessing http://192.168.0.1/config?key=value

The options currently available are:
* base_ssid : the SSID of the base station (or router) we need to connect to
* base_password : the password for the base station
* base_ip : the IP address of the base station (where to send UDP packets)
* node_number: the unique ID of this node (see below)

All the settings are applied the next time you reboot the node.

When base_ssid is set, the node will try to connect to this AP at startup, using the base_password (which can be empty for open networks). The node will assume
that a DHCP server is running on this network and ask for an IP to it.

If it fails to connect, authenticate or get an IP address within 10 seconds, the node will give up. You will have to reboot it to make it try again to connect to the base station.

In any case, whether or not it manages to connect to the base AP, the node will start its own AP so you can fix the configuration.

If you don't provide a base_ip, when the node connects to the base AP, it will try to send the UDP packets to the gateway IP that it is given by DHCP. This usually works if the base is also the access point. But if the access point is a separate router, then you need to provide a base_ip.

Finally, once you set a node_number (higher than zero) the node's access point will be named `OpenLightMotionNode_N` (where N is the node_number) on the next reboot. This way each node has its own SSID and you can access all of them without conflicts.

## No Camera Mode

If you don't have a Pixy camera connected to the ESP8266, when you try to read data from the camera the SPI interface will block indefinitely.
The web server will not work and you will not be able to use the node.

If you want to test the rest of the system without the camera, you can enable a fake reader that will generate random coordinates instead of trying to read them from the camera.

To do this, uncomment `#define USE_FAKE_READER` from the top of `main.ino`, rebuild and upload the firmware.

## Statistics

You can access a page displaying various statistics about the node by accessing the /stats page.

## Networking

Please note that once you configure the node to connect to an access point, you will be able to access the web server also from a machine connected to that network, without having to connect to the node's AP directly.

The only problem with this is that you don't know the IP address of the node on the network, because it is assigned by DHCP.

If you control the DHCP server, then you can make sure that each node always gets the same IP (ideally with the last digit equal to the node_number).

Otherwise you can connect once to the node's AP (OpenLightMotionNode_N) and check the /stats page. This page will display the IP.

You usually need to do this only a few times, because DHCP servers tend to re-assign the same address to the same client for a while.

## Authors and Contributors
This project is mainly developped by @lotelx, @nerochiaro, wiht some collaborators.
This software is based on previous works like VRTraker.xyz, and the IR-Lock precision landing system.
