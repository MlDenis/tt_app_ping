#include <iostream>
#include <string>

bool send_arp_request(const std::string& iface_name, const std::string& src_ip_str, const std::string& target_ip_str);
bool receive_arp_reply(const std::string& expected_ip_str);

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <interface> <source_ip> <target_ip>\n";
        return 1;
    }

    std::string iface_name = argv[1];
    std::string src_ip     = argv[2];
    std::string target_ip  = argv[3];

    if (!send_arp_request(iface_name, src_ip, target_ip)) {
        std::cerr << "Ошибка при отправке ARP-запроса\n";
        return 1;
    }

    if (!receive_arp_reply(target_ip)) {
        std::cerr << "Ошибка при получении ARP-ответа\n";
        return 1;
    }

    return 0;
}