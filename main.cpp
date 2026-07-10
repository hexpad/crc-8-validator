#include <iostream>
#include <vector>
#include <cstdint>

struct TelemetryPacket {
    uint8_t x;
    uint8_t y;
    uint8_t z;
    uint8_t crc;
};

uint8_t calculateCRC8(const uint8_t* data, size_t length) {
    uint8_t crc = 0x00;
    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

int main() {
    TelemetryPacket packet = {10, 20, 30, 0};

    packet.crc = calculateCRC8((uint8_t*)&packet, 3);

    std::cout << "UAV sent packet. CRC Signature: 0x" << std::hex << (int)packet.crc << std::endl;

    uint8_t receivedCRC = calculateCRC8((uint8_t*)&packet, 3);

    if (receivedCRC == packet.crc) {
        std::cout << "Data verified successfully: UAV Pos: "
                  << (int)packet.x << "," << (int)packet.y << std::endl;
    } else {
        std::cout << "ERROR: Data corruption detected! Packet rejected." << std::endl;
    }

    return 0;
}
