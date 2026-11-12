#include <stdio.h>
#include <pigpio.h>

#define LED 17

int main(void) {
    double minTemp = 68.0;
    double maxTemp = 76.0;
    double temp, sum = 0.0;
    int count = 0;
    int choice;

    FILE *file;

    if (gpioInitialise() < 0) {
        printf("GPIO initialization failed.\n");
        return 1;
    }

    gpioSetMode(LED, PI_OUTPUT);
    gpioWrite(LED, 0);

    file = fopen("temperatures.txt", "a");

    if (file == NULL) {
        printf("Could not open file.\n");
        gpioTerminate();
        return 1;
    }

    do {
        printf("\n--- Classroom Comfort Monitor ---\n");
        printf("1. Enter temperature\n");
        printf("2. View average temperature\n");
        printf("3. Change acceptable range\n");
        printf("4. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            break;
        }

        switch (choice) {
            case 1:
                printf("Enter temperature (F): ");

                if (scanf("%lf", &temp) != 1) {
                    printf("Invalid temperature.\n");
                    choice = 4;
                    break;
                }

                sum += temp;
                count++;

                if (temp >= minTemp &&
                    temp <= maxTemp) {

                    printf("Status: NORMAL\n");
                    printf("LED: OFF\n");

                    gpioWrite(LED, 0);

                    fprintf(file,
                        "%.2f F - NORMAL\n", temp);
                } else {
                    printf("Status: WARNING\n");
                    printf("LED: ON\n");

                    gpioWrite(LED, 1);

                    fprintf(file,
                        "%.2f F - WARNING\n", temp);
                }

                fflush(file);
                break;

            case 2:
                if (count > 0) {
                    printf("Average: %.2f F\n",
                           sum / count);
                } else {
                    printf("No readings recorded.\n");
                }
                break;

            case 3:
                printf("Enter minimum temperature: ");
                if (scanf("%lf", &minTemp) != 1) {
                    choice = 4;
                    break;
                }

                printf("Enter maximum temperature: ");
                if (scanf("%lf", &maxTemp) != 1) {
                    choice = 4;
                    break;
                }

                if (minTemp > maxTemp) {
                    printf("Invalid range.\n");
                    minTemp = 68.0;
                    maxTemp = 76.0;
                } else {
                    printf("Range updated.\n");
                }
                break;

            case 4:
                printf("Exiting program...\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 4);

    gpioWrite(LED, 0);
    fclose(file);
    gpioTerminate();

    return 0;
}

