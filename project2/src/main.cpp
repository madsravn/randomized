#include <iostream>

// If we need a better precision data type later
typedef char character;
// Guard against weird compiler implementation
static_assert(sizeof(character) <= 2, "This data type exceeds the 16 bit bound");

int main(int argc, char** argv) {

    std::cout << "Hello world!" << std::endl;

	return 0;
}
