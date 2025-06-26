#include <iostream>
#include <string>
#include <cstring>
#include <ifaddrs.h>
#include <net/if_dl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void print_mac(const uint8_t *mac) {
    printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <interface> <target_ip>\n";
        return 1;
    }

    std::string iface_name = argv[1];
    std::string target_ip = argv[2];

    struct ifaddrs *ifap, *ifa;
    if (getifaddrs(&ifap) != 0) {
        perror("getifaddrs");
        return 1;
    }

    bool found = false;

    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) continue;
        if (iface_name != ifa->ifa_name) continue;

        if (ifa->ifa_addr->sa_family == AF_INET) {
            auto *sin = reinterpret_cast<struct sockaddr_in *>(ifa->ifa_addr);
            std::cout << "IP интерфейса: " << inet_ntoa(sin->sin_addr) << "\n";
            found = true;
        } else if (ifa->ifa_addr->sa_family == AF_LINK) {
            auto *sdl = reinterpret_cast<struct sockaddr_dl *>(ifa->ifa_addr);
            const uint8_t *mac = reinterpret_cast<const uint8_t *>(LLADDR(sdl));
            std::cout << "MAC интерфейса: ";
            print_mac(mac);
            found = true;
        }
    }

    freeifaddrs(ifap);

    if (!found) {
        std::cerr << "Интерфейс не найден или отсутствует MAC/IP\n";
        return 1;
    }

    return 0;
}