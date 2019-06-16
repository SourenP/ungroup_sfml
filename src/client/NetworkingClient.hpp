#ifndef NetworkingClient_hpp
#define NetworkingClient_hpp

#include <stdio.h>
#include <SFML/Network.hpp>
#include "../common/game_def.hpp"


class NetworkingClient {

    const char* SERVER_IP = "127.0.0.1";

    // these will be strictly for ungroup/group/register/deregister and other Big abstraction changes
    // not sure it's really necessary, but implementing retry logic for something like ungrouping specifically
    // would be annoying. We're a lot more OK with duplication/out of order sends/dropped datagrams for stuff like moving.
    void api_client_recv(sf::TcpSocket* api_client) {}
    void api_client_send(sf::TcpSocket* api_client) {}


    sf::TcpSocket* create_api_client()
    {
        sf::TcpSocket* api_client = new sf::TcpSocket;
        api_client->connect(SERVER_IP, 4844);
        return api_client;
    }

    sf::UdpSocket* create_realtime_client()
    {
        sf::UdpSocket* realtime_client = new sf::UdpSocket;
        realtime_client->bind(4846);
        return realtime_client;
    }

    public:
        NetworkingClient();
        ~NetworkingClient();

        std::vector<group_circle_update> getClientGroupUpdates();
        void setDirection(sf::Vector2f direction);

    private:
        // Methods
        void ReadRegistrationResponse();
        void RegisterNetworkingClient();
        void RealtimeClientSend();
        void RealtimeClientRecv();
        void SyncServerState();

        // Variables
        sf::Uint32 mClientId;
        sf::Uint32 mCurrentTick;
        bool mIsRegistered = false;

        sf::TcpSocket* mApiClient;
        sf::UdpSocket* mRealtimeClient;

        std::atomic<bool> mAcceptingNetworkGameObjectsRead;
        std::vector<group_circle_update> mClientGroupUpdates;
        std::atomic<bool> mAcceptingDirectionRead;
        sf::Vector2f mDirection;
};

#endif /* NetworkingClient_hpp */
