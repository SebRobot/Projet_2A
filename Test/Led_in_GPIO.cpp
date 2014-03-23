/*******************************************************
 * Example code for user LEDs on the new beaglebone
 * black running the Linux Angstrom distribution
 ********************************************************
 * Instructions:
 * -Compile and run from the root directory
 * -For the older (white) beaglebone, change
 *  "beaglebone:green:usr0" to "beaglebone::usr0"
 *
 * Code adapted from:
 * - Derek Molloy, "Beaglebone: C/C++ Programming
 * Introduction for ARM Embedded Linux Development
 * using Eclipse CDT" video tutorial,
 * (link: www.youtube.com/watch?v=vFv_-ykLppo)
 * -  Mark A. Yoder, EBC Exercise 10 Flashing an LED
 * (link: "elinux.org/EBC_Exercise_10_Flashing_an_LED)
 *******************************************************/
 
/* Before executing code
echo 60 > export
echo out > direction
echo 1 > value //test
*/

#include <stdio.h>
#include <unistd.h>
 
using namespace std;
 
int main(int argc, char** argv) 
    {
    int i;
    FILE *LEDHandle = NULL;
    char *LEDBrightness1 = "/sys/class/gpio/gpio60/value";

    printf("\nStarting simple LED blink program\n");
 
    while(1)
        {
        if((LEDHandle = fopen(LEDBrightness1, "r+")) != NULL)
            {
            fwrite("1", sizeof(char), 1, LEDHandle);
            fclose(LEDHandle);
            }
        printf("LEDon\n");
        sleep(1);

        if((LEDHandle = fopen(LEDBrightness1, "r+")) != NULL)
            {
            fwrite("0", sizeof(char), 1, LEDHandle);
            fclose(LEDHandle);
            }
        printf("LEDoff\n");
        sleep(1);
        }
    return 0;
 
    }
