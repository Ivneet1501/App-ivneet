#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

/* ================= SENSOR DATA VALIDATION ================= */
int isValidSensorData(unsigned int sensor_id,
                      float temperature,
                      int humidity_fixed_point,
                      uint32_t timestamp)
{
    /* Sensor ID: 0–255 */
    if (sensor_id > 255)
        return 0;

    /* Temperature: -40°C to +125°C */
    if (temperature < -40.0f || temperature > 125.0f)
        return 0;

    /* Humidity: 0.000% – 100.000% (×1000) */
    if (humidity_fixed_point < 0 || humidity_fixed_point > 100000)
        return 0;

    /* Timestamp must be non-zero */
    if (timestamp == 0)
        return 0;

    return 1;
}

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

/* ================= SERIALIZATION ================= */
void serialize_sensor_data(uint8_t sensor_id,
                           float temperature,
                           uint16_t humidity_fixed_point,
                           uint32_t timestamp,
                           uint8_t *packet_buffer)
{
    packet_buffer[0] = sensor_id;

    memcpy(&packet_buffer[1], &temperature, sizeof(float));

    packet_buffer[5] = (uint8_t)(humidity_fixed_point & 0xFF);
    packet_buffer[6] = (uint8_t)((humidity_fixed_point >> 8) & 0xFF);

    packet_buffer[7]  = (uint8_t)(timestamp & 0xFF);
    packet_buffer[8]  = (uint8_t)((timestamp >> 8) & 0xFF);
    packet_buffer[9]  = (uint8_t)((timestamp >> 16) & 0xFF);
    packet_buffer[10] = (uint8_t)((timestamp >> 24) & 0xFF);
}

/* ================= PACKET CREATION ================= */
void create_full_sensor_packet(uint8_t sensor_id,
                               float temperature,
                               uint16_t humidity_fixed_point,
                               uint32_t timestamp,
                               uint8_t *packet)
{
    serialize_sensor_data(sensor_id,
                          temperature,
                          humidity_fixed_point,
                          timestamp,
                          packet);

    packet[11] = calculate_crc8(packet, 11);
}

/* ================= MAIN ================= */
int main(void)
{
    uint8_t packet[12];
    char choice;

    while (1)
    {
        unsigned int sensor_id_input;
        int humidity_input;
        uint8_t sensor_id;
        uint16_t humidity;
        float temperature;
        uint32_t timestamp;

        printf("\nEnter Sensor ID (0–255): ");
        scanf("%u", &sensor_id_input);

        printf("Enter Temperature (e.g. 25.5): ");
        scanf("%f", &temperature);

        printf("Enter Humidity (fixed-point, e.g. 45678 = 45.678%%): ");
        scanf("%d", &humidity_input);

        printf("Enter Timestamp (seconds since boot): ");
        scanf("%u", &timestamp);

        /* -------- VALIDATION -------- */
        if (!isValidSensorData(sensor_id_input,
                               temperature,
                               humidity_input,
                               timestamp))
        {
            printf("\nERROR: Invalid sensor data! Packet NOT created.\n");
            continue;
        }

        /* -------- SAFE CAST AFTER VALIDATION -------- */
        sensor_id = (uint8_t)sensor_id_input;
        humidity  = (uint16_t)humidity_input;

        /* --------Sender-side PACKET CREATION -------- */
        create_full_sensor_packet(sensor_id,
                                  temperature,
                                  humidity,
                                  timestamp,
                                  packet);

        printf("\nSensor Packet Bytes:\n");
        for (int i = 0; i < 12; i++)
            printf("Byte %2d : 0x%02X\n", i, packet[i]);

        /* -------- Receiver side-CRC CHECK -------- */
        uint8_t received_crc = packet[11];
        uint8_t calculated_crc = calculate_crc8(packet, 11);

        printf("\nCRC Verification:\n");
        printf("Received CRC   : 0x%02X\n", received_crc);
        printf("Calculated CRC : 0x%02X\n", calculated_crc);

        if (received_crc == calculated_crc)
            printf("Packet VALID (CRC matched)\n");
        else
            printf("Packet INVALID (CRC mismatch)\n");

        printf("\nDo you want to enter another packet? (y/n): ");
        scanf(" %c", &choice);

        if (choice == 'n' || choice == 'N')
        {
            printf("\nExiting program. Goodbye!\n");
            break;
        }
    }

    return 0;
}
