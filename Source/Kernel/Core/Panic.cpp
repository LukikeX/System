#include "Panic.h"
#include "IO.h"

void panic(const String&) {
    IO::cli();
    *kvt << "test panic!";
}