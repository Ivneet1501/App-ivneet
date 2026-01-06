#include "timesync.h"
#include <stdio.h>

int main() {
    int choice;
    DateTime dt;
    uint32_t counter;
    
    printf("=== Timesync Library Test ===\n");
    
    while (1) {
        printf("\n1. Timestamp → Counter\n");
        printf("2. Counter → Timestamp\n");
        printf("3. Exit\n");
        printf("Choose (1-3): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            continue;
        }
        
        if (choice == 1) {
            // TIMESTAMP → COUNTER
            printf("\nEnter date-time:\n");
            printf("Year (2000-2136): "); scanf("%hu", &dt.year);
            printf("Month (1-12): ");     scanf("%hhu", &dt.month);
            printf("Day: ");              scanf("%hhu", &dt.day);
            printf("Hour (0-23): ");      scanf("%hhu", &dt.hour);
            printf("Minute (0-59): ");    scanf("%hhu", &dt.minute);
            printf("Second (0-59): ");    scanf("%hhu", &dt.second);
            
            if (isValidDateTime(dt)) {
                counter = Apptime_gettimestamp(dt);
                printf("\nCounter = %u seconds\n", counter);
            } else {
                printf("Invalid date-time!\n");
            }
        }
        else if (choice == 2) {
            // COUNTER → TIMESTAMP
            printf("\nEnter RTC counter (seconds): ");
            scanf("%u", &counter);
            
            Apptimer_performtimesync(counter, &dt);
            printf("\nTimestamp: %04hu-%02hhu-%02hhu %02hhu:%02hhu:%02hhu\n",
                   dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);
        }
        else if (choice == 3) {
            printf("end\n");
            break;
        }
        else {
            printf(" Choose 1, 2, or 3 only!\n");
        }
    }
    
    return 0;
}
