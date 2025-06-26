#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <interface> <target_ip>\n";
        return 1;
    }

    std::string interface = argv[1];
    std::string target_ip = argv[2];

    std::cout << "Interface: " << interface << "\n";
    std::cout << "Target IP: " << target_ip << "\n";

    return 0;
}