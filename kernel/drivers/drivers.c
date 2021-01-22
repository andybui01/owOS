#include <drivers/drivers.h>
#include <drivers/kbd.h>

void drivers_bootstrap(void)
{
    // Initialize all our drivers
    keyboard_bootstrap();
}
