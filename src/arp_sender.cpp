#include <iostream>
#include <cstring>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netpacket/packet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>

bool send_arp_request(const std::string& iface_name, const std::string& src_ip_str, const std::string& target_ip_str) {
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sock < 0) {
        perror("socket");
        return false;
    }

    struct ifreq ifr{};
    strncpy(ifr.ifr_name, iface_name.c_str(), IFNAMSIZ - 1);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        perror("SIOCGIFINDEX");
        close(sock);
        return false;
    }
    int ifindex = ifr.ifr_ifindex;

    if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
        perror("SIOCGIFHWADDR");
        close(sock);
        return false;
    }
    uint8_t src_mac[6];
    memcpy(src_mac, ifr.ifr_hwaddr.sa_data, 6);

    struct sockaddr_ll socket_address{};
    socket_address.sll_ifindex = ifindex;
    socket_address.sll_halen = ETH_ALEN;
    memset(socket_address.sll_addr, 0xff, 6);

    uint8_t buffer[42] = {0};

    struct ether_header* eth = (struct ether_header*) buffer;
    memset(eth->ether_dhost, 0xff, 6);
    memcpy(eth->ether_shost, src_mac, 6);
    eth->ether_type = htons(ETH_P_ARP);

    struct ether_arp* arp = (struct ether_arp*) (buffer + sizeof(struct ether_header));
    arp->ea_hdr.ar_hrd = htons(ARPHRD_ETHER);
    arp->ea_hdr.ar_pro = htons(ETH_P_IP);
    arp->ea_hdr.ar_hln = 6;
    arp->ea_hdr.ar_pln = 4;
    arp->ea_hdr.ar_op  = htons(ARPOP_REQUEST);

    memcpy(arp->arp_sha, src_mac, 6);
    inet_pton(AF_INET, src_ip_str.c_str(), arp->arp_spa);
    memset(arp->arp_tha, 0x00, 6);
    inet_pton(AF_INET, target_ip_str.c_str(), arp->arp_tpa);

    if (sendto(sock, buffer, 42, 0, (struct sockaddr*)&socket_address, sizeof(socket_address)) < 0) {
        perror("sendto");
        close(sock);
        return false;
    }

    std::cout << "ARP-запрос отправлен на " << target_ip_str << "\n";
    close(sock);
    return true;
}