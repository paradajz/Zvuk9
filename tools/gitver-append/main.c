#include <stdio.h>
#include <inttypes.h>

#define INSERT_OFFSET   0x1DFF6
#define POINTS          4

int main(void) {

    FILE *f_bin       = fopen("FLASH.bin", "r+");
    FILE *f_version   = fopen("version", "r");

    if (f_bin && f_version)   {

        int point = 0, points = 0;

        do {

            fscanf(f_version, "%d,", &point);
            //each point requires two bytes of flash memory
            //split point into lower and upper byte
            int offset = INSERT_OFFSET+(points*2);
            fseek(f_bin, offset, SEEK_SET);
            fputc(point & 0xFF, f_bin);

            fseek(f_bin, offset+1, SEEK_SET);
            fputc((point >> 8) & 0xFF, f_bin);

            points++;

        }   while (points < POINTS);

    }

    fclose(f_version);
    fclose(f_bin);

    return 0;

}
