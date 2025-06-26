#include <string>

bool send_arp_request(const std::string& iface_name, const std::string& src_ip_str, const std::string& target_ip_str);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <interface> <target_ip>\n";
        return 1;
    }

    std::string iface_name = argv[1];
    std::string target_ip = argv[2];


    if (!send_arp_request(iface_name, "0.0.0.0", target_ip)) {
        std::cerr << "Ошибка при отправке ARP-запроса\n";
        return 1;
    }

    return 0;
}