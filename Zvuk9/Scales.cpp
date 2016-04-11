#include "Scales.h"

uint8_t getNotesPerScale(scale_t scale)    {

    switch(scale)   {

        case scaleNaturalMinor:
        case scaleMelodicMinor:
        case scaleHarmonicMinor:
        case scaleMajor:
        case scaleHarmonicMajor:
        return 7;

        case scaleMinorPentatonic:
        case scaleMajorPentatonic:
        return 5;

        default:
        return 0;

    }

}


note_t getScaleNote(scale_t scale, uint8_t note)  {

    //no error checking, be careful

    switch(scale)   {

        case scaleNaturalMinor:
        return scaleNaturalMinor_notes[note];
        break;

        case scaleMelodicMinor:
        return scaleMelodicMinor_notes[note];
        break;

        case scaleHarmonicMinor:
        return scaleHarmonicMinor_notes[note];
        break;

        case scaleMajor:
        return scaleMajor_notes[note];
        break;

        case scaleHarmonicMajor:
        return scaleHarmonicMajor_notes[note];
        break;

        case scaleMinorPentatonic:
        return scaleMinorPentatonic_notes[note];
        break;

        case scaleMajorPentatonic:
        return scaleMajorPentatonic_notes[note];
        break;

        default:
        return C;

    }

}
