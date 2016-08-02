/*

Generate scales with defined gain.

*/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#define CURVE_STEPS 10
#define CURVE_STEP  1.0/(double)CURVE_STEPS
#define LOG_05      (double)log((double)0.5)

uint8_t scale[128];

double bias(double b, double x)    {

    return pow(x, log(b)/LOG_05);

}

double gain(double g, double x)    {

    if (x < 0.5)
        return bias(1-g, 2*x)/2;
    else return 1 - bias(1-g,2 - 2*x)/2;

}

void setupCurve(uint8_t curveGain, uint8_t min, uint8_t max)   {

    int i;

    if (curveGain == 50)    {

        //normally, gain 0.5 produces linear scale
        //however, we are converting decimal numbers to int so there is slight error
        //in this case, setup curve manually

        for (i=0; i<128; i++)
            scale[i] = i;

    }   else {

        double curveGain_double = curveGain/100.0; //scale gain value 0-1
        uint8_t numberOfValues = max-min;
        double step, stepValue = 0.0;

        //scale range
        step = 1.0/(double)numberOfValues;

        for (i=0; i<=numberOfValues; i++)   {

            //make sure that curve extremes are correct
            if (!i) scale[i] = 0;
            else if (i == numberOfValues) scale[i] = numberOfValues;
            else scale[i] = round(gain(curveGain_double, stepValue) * numberOfValues); //round up
            stepValue += step;

        }

    }

}

void setupAllCurves()   {

    int i, j;

    for (i=1; i<CURVE_STEPS; i++)   {

        printf("const uint8_t PROGMEM gain_%d[] PROGMEM = {\n\n", i);

        setupCurve(i*10, 0, 127); //set full midi range

        for (j=0; j<128; j++)   {

            printf("\t%d", scale[j]);
            //don't print comma on last value
            if (j != 127)
                printf(",\n");
            else printf("\n");

        }

        printf("\n};\n\n");

    }

    //place all scales in single progmem array
    printf("const uint8_t* const scaleArray[] PROGMEM = {\n\n");

    for (i=1; i<CURVE_STEPS; i++)   {

        printf("\tgain_%d", i);
        //again, don't print comma on last value
        if (i != CURVE_STEPS-1)
            printf(",\n");
        else printf("\n");

    }

    printf("\n};");

}

int main()  {

    setupAllCurves();
    return 0;

}
