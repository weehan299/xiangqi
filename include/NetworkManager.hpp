
// ---------- include/NetworkManager.h ----------
#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#include <SFML/Network.hpp>

class NetworkManager {
public:
    NetworkManager();
    bool hostGame(unsigned short port);
    bool joinGame(const sf::IpAddress& ip, unsigned short port);
    void sendMove(int sx, int sy, int dx, int dy);
    bool receiveMove(int& sx, int& sy, int& dx, int& dy);
private:
    sf::TcpSocket socket;
    sf::TcpListener listener;
};

#endif // NETWORKMANAGER_H
