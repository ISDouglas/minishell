#include <unistd.h>
#include <stdio.h>

int main(void)
{
    // Check standard input (fd 0)
    if (isatty(0))
        printf("✅ Standard input is from the keyboard (terminal)\n");
    else
        printf("❌ Standard input is redirected\n");

    // Check standard output (fd 1)
    if (isatty(1))
        printf("✅ Standard output is connected to the screen (terminal)\n");
    else
        printf("❌ Standard output is redirected\n");

    return (0);
}
