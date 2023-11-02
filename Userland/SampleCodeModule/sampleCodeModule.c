/* sampleCodeModule.c */
#include <menu.h>
#include <pepsiman.h>

char *v = (char *)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;

int main()
{
	// All the following code may be removed
	v = 'X';
	*(v + 1) = 0x74;

	call_drawRectangle(0xFFFF00, 200, 200, 50, 50);
	// Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;
	return 0xDEADBEEF;

	// drawMenu();

	return 0;
}