#include <stdio.h>
#include <stdint.h>

#include "CRC.h"
#include "ntc_thermistor.h"
#include "timesync.h"

int main(void)
{
    int choice;

    while (1)
    {
        printf("\n===== MAIN MENU =====\n");
        printf("1. CRC Packet Demo\n");
        printf("2. NTC Thermistor Demo\n");
        printf("3. Time Sync (RTC) Demo\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1)
            return 0;

        switch (choice)
        {
        case 1:
            run_crc_demo();
            break;
        case 2:
            run_ntc_demo();
            break;
        case 3:
            run_timesync_demo();
            break;
        case 4:
            printf("\nExiting program. Goodbye!\n");
            return 0;
        default:
            printf("\n❌ Invalid choice! Please enter 1–4.\n");
            break;
        }
    }
    return 1;
}
// ./main_app.exe