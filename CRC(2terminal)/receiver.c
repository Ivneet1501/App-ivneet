#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* ================= CRC-8 (Polynomial 0x07) ================= */
uint8_t calculate_crc8(const uint8_t *data, size_t length)
{
    uint8_t crc = 0x00;

    for (size_t i = 0; i < length; i++)
    {
        crc ^= data[i];

        for (int bit = 0; bit < 8; bit++)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x07;
            else
                crc <<= 1;
        }
    }
    return crc;
}

int main(void)
{
    uint8_t packet[12];

    // Open packet file created by sender
    FILE *fp = fopen("packet.bin", "rb");
    if (!fp)
    {
        printf("No packet found! Please run sender first.\n");
        return 1;
    }

    // Read 12 bytes from file
    size_t read_bytes = fread(packet, 1, 12, fp);
    fclose(fp);

    if (read_bytes != 12)
    {
        printf("Error reading packet! File size mismatch.\n");
        return 1;
    }

    // Extract received CRC (last byte)
    uint8_t received_crc = packet[11];

    // Calculate CRC from first 11 bytes
    uint8_t calculated_crc = calculate_crc8(packet, 11);

    // Print packet bytes
    printf("\nReceived Packet Bytes:\n");
    for (int i = 0; i < 12; i++)
        printf("Byte %2d : 0x%02X\n", i, packet[i]);

    // CRC verification
    printf("\nCRC Verification:\n");
    printf("Received CRC   : 0x%02X\n", received_crc);
    printf("Calculated CRC : 0x%02X\n", calculated_crc);

    if (received_crc == calculated_crc)
        printf("Packet VALID (CRC matched)\n");
    else
        printf("Packet INVALID (CRC mismatch)\n");

    return -1;
}
