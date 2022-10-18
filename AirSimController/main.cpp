#include "AirSimControl.h"


int main(int argc, const char **argv) {
    switch (argc) {
        case 1: AirSimController("stdin", 0).run(); break;
        case 2: AirSimController(argv[1], 8080).run(); break;
        case 3: AirSimController(argv[1], atoi(argv[2])).run(); break;
        default: std::cerr << "Invalid argument count\n"
                    << "Argument count 0 : stdin\n"
                    << "Argument count 1 : ip, (default port = 8080)\n"
                    << "Arguemnt count 2 : ip, port\n"; break;
    }
}
