#ifndef SECTIONS_H_
#define SECTIONS_H_

typedef enum {

    programLastActiveProgramSection,
    programLastActiveScaleSection,
    programGlobalSettingsSection,
    programLocalSettingsSection,
    programScalePredefinedSection,
    PROGRAM_SECTIONS

} programSection;

typedef enum {

    limitPressureLower,
    limitPressureUpper,
    limitXlower,
    limitXupper,
    limitYlower,
    limitYupper,
    LIMIT_SECTIONS

} limitSection;

typedef enum {

    padNotesSection,
    USER_SCALE_SECTIONS

} userScaleSection;

#endif