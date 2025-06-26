# arp_ping

ARP Ping — утилита для отправки ARP-запроса и получения MAC-адреса устройства по заданному IPv4 адресу.  
Работает только в локальной сети.

## Сборка

```bash
mkdir build
cd build
cmake ..
make

## Использование
sudo ./arp_ping <interface> <source_ip> <target_ip>

## Пример
sudo ./arp_ping eth0 192.168.1.10 192.168.1.100

# Требования
1. Linux
2. root-доступ (для raw-сокетов)