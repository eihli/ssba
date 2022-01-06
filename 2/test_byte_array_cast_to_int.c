#include <stdio.h>
#include <stdint.h>

int main() {
    // Mimic reading that TCP header file 1 byte at a time into a buffer.
    // The first 2 bytes are the source port.
    // In this case, port 257.
    unsigned char buffer[2] = { 0x01, 0x01 }; // 0000001 00000001 = 257

    // Now convert those two bytes into a single int, since ports are 16 bits.
    int source_port = (buffer[0] << 8) + buffer[1];
    printf("This is 257: %d\n",       source_port);
    // -> This is 257: 257

    // That works. But I thought...
    // Shifting and adding to convert char to 16, 32, or 64 bytes is going to get annoying.
    // Especially when we need to read 4 bytes for
    // Sequence and Acknowledgement #s.

    int source_port_1 = *((int *)buffer);
    printf("This should be 257: %d\n", source_port_1);
    // -> This should be 257: -1694498559
    // But that fails!

    // Ah hah! I was reading the Wikipedia C Data Types page wrong.
    // Ints are MINIMUM 16 bits. But it's actually compiler-dependent.
    // Same with shorts.
    // The only way to guarantee a 16-bit type is to include stdint.h and use int16_t.
    int16_t source_port_2 = ((int16_t *)buffer)[0];
    printf("This is 257: %d\n",       source_port_2);
    // -> This is 257: 257

    // The thing that tricked me good is that the `int` type
    // worked for the first case where I was bit-shifting and adding.
    //
    // But that's because I was initializing the variable to `buffer[0] << 8`.
    // But when I casted the buffer to `(int *)`, I was actually reading
    // WAY past the 2 bytes that I initialized to 0x01, 0x01 in the buffer.
    // And everything past the first two bytes in that array is just
    // random uninitialized noise.
    //
    // And I was reading past those first two initialized values and
    // nothing was telling me that I couldn't.
    //
    // I wasn't used to a language that lets me read past the end of an array.
}
