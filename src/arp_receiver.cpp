#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/if_ether.h>
#include <netpacket/packet.h>
#include <arpa/inet.h>

bool receive_arp_reply(const std::string& expected_ip_str) {
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sock < 0) {
        perror("socket");
        return false;
    }

    struct in_addr expected_ip;
    if (inet_aton(expected_ip_str.c_str(), &expected_ip) == 0) {
        std::cerr << "Неверный IP\n";
        close(sock);
        return false;
    }

    while (true) {
        uint8_t buffer[1500];
        ssize_t len = recv(sock, buffer, sizeof(buffer), 0);
        if (len < 0) {
            perror("recv");
            close(sock);
            return false;
        }

        struct ether_header* eth = (struct ether_header*)buffer;
        if (ntohs(eth->ether_type) != ETH_P_ARP) continue;

        struct ether_arp* arp = (struct ether_arp*)(buffer + sizeof(struct ether_header));
        if (ntohs(arp->ea_hdr.ar_op) != ARPOP_REPLY) continue;

        if (memcmp(arp->arp_spa, &expected_ip, 4) != 0) continue;

        printf("Ответ получен. MAC цели: %02X:%02X:%02X:%02X:%02X:%02X\n",
               arp->arp_sha[0], arp->arp_sha[1], arp->arp_sha[2],
               arp->arp_sha[3], arp->arp_sha[4], arp->arp_sha[5]);

        close(sock);
        return true;
    }
}