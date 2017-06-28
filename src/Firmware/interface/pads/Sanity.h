#pragma once

#define PAD_CHECK(pad)              (((pad < 0) || (pad >= NUMBER_OF_PADS)) ? 0 : 1)
#define NOTE_ASSIGN_CHECK(note)     (((note < 0) || (note >= NOTES_PER_PAD)) ? 0 : 1)
#define SCALE_CHECK(scale)          ((scale < 0) || (scale >= (PREDEFINED_SCALES+NUMBER_OF_USER_SCALES) ? 0 : 1))