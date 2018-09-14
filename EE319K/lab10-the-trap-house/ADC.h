// put prototypes for public functions, explain what it does
// put your names here, date
#include <stdint.h>

typedef struct CoordPOT_t {
uint32_t x;
uint32_t y;
} CoordPot;

void ADC_Init(void);
CoordPot ADC_In(void);
