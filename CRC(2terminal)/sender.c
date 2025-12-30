#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* CRC-8 Function */
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

/* Serialization Function */
void serialize_sensor_data(uint8_t sensor_id, float temperature,
                           uint16_t humidity, uint32_t timestamp,
                           uint8_t *packet)
{
    packet[0] = sensor_id;
    memcpy(&packet[1], &temperature, sizeof(float));
    packet[5] = (uint8_t)(humidity & 0xFF);
    packet[6] = (uint8_t)((humidity >> 8) & 0xFF);
    packet[7]  = (uint8_t)(timestamp & 0xFF);
    packet[8]  = (uint8_t)((timestamp >> 8) & 0xFF);
    packet[9]  = (uint8_t)((timestamp >> 16) & 0xFF);
    packet[10] = (uint8_t)((timestamp >> 24) & 0xFF);
}

/* Full Packet Creation */
void create_full_sensor_packet(uint8_t sensor_id, float temperature,
                               uint16_t humidity, uint32_t timestamp,
                               uint8_t *packet)
{
    serialize_sensor_data(sensor_id, temperature, humidity, timestamp, packet);
    packet[11] = calculate_crc8(packet, 11);
}

/* MAIN FUNCTION */
int main()
{
    uint8_t packet[12];
    unsigned int sensor_id_input;
    int humidity_input;
    float temperature;
    uint32_t timestamp;

    printf("Enter Sensor ID: ");
    scanf("%u", &sensor_id_input);
    printf("Enter Temperature: ");
    scanf("%f", &temperature);
    printf("Enter Humidity: ");
    scanf("%d", &humidity_input);
    printf("Enter Timestamp: ");
    scanf("%u", &timestamp);

    create_full_sensor_packet((uint8_t)sensor_id_input, temperature,
                              (uint16_t)humidity_input, timestamp, packet);

    // Print packet bytes
    printf("\nPacket Bytes:\n");
    for(int i=0;i<12;i++)
        printf("0x%02X ", packet[i]);
    printf("\n");

    // Save packet to file for receiver
    FILE *fp = fopen("packet.bin", "wb");
    if (!fp)
    {
        printf("Error creating packet file!\n");
        return 1;
    }
    fwrite(packet, 1, 12, fp);
    fclose(fp);

    printf("Packet saved to packet.bin\n");

    return 0;
}
