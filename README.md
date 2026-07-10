# UAV Telemetry CRC-8 Check

Small C++ program that checks if a telemetry packet got corrupted during transfer. It uses a CRC-8 checksum. The idea is simple: a UAV sends its position, adds a small check byte, and the receiver checks that byte to make sure nothing broke on the way.

## What is a CRC?

When you send data over radio or serial, sometimes a bit flips and the data changes. A CRC is just a number you calculate from the data. You send it with the data, then calculate it again on the other side. If the two numbers match, the data is fine. If not, something went wrong.

Here I use CRC-8 with the polynomial `0x07`. It gives back one byte (0 to 255).

## How to run

Compile it:

```bash
g++ -o crc main.cpp
```

Then run it:

```bash
./crc
```

## What you get

With the position `{10, 20, 30}` the output looks like this:

```
UAV sent packet. CRC Signature: 0xde
Data verified successfully: UAV Pos: 10,20,30
```

So the checksum for those three numbers comes out to `0xde` (222 in decimal).

## The parts

The `TelemetryPacket` struct holds the x, y, z position and one crc byte.

The `calculateCRC8` function goes through the bytes one by one and builds the checksum. For every bit it either shifts and XORs with the polynomial, or just shifts. Nothing fancy, just a loop.

The `main` function builds a packet, calculates the crc, then calculates it again and compares. If they match it prints the position, if not it prints an error.

## A couple of things to know

Right now the program always says "verified" because it checks the same data it just made. Nothing actually gets corrupted. If you want to see the error message, change one of the values before the second check, like this:

```cpp
packet.x ^= 0x01;  // pretend a bit flipped
uint8_t receivedCRC = calculateCRC8((uint8_t*)&packet, 3);
```

Now the two checksums won't match and you'll get the error line.
