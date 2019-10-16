#ifndef game_settings_hpp
#define game_settings_hpp

#include <SFML/Graphics.hpp>
#include <chrono>

/* Game Logic */
const int MAX_PLAYER_COUNT = 10;
const int MAX_MINE_COUNT = 5;
const bool USE_SHADER = false;
const sf::Vector2f WINDOW_RESOLUTION(800, 600);
const float MINE_SIZE = 20.f;
const float GROUP_START_OFFSET_X = 20.f;
const float GROUP_START_OFFSET_Y = 20.f;
const float MINE_START_OFFSET_X = 50.f;
const float MINE_START_OFFSET_Y = 100.f;
const float GROUP_MEMBER_SIZE = 15.f;
const float GROUP_SPEED = 100.f;

/* Threads */

const std::chrono::milliseconds SERVER_INPUT_WINDOW_SLEEP(50);

const std::chrono::milliseconds CLIENT_RELIABLE_SEND_SLEEP(16);
const std::chrono::milliseconds SERVER_RELIABLE_REVC_SEND_SLEEP(
    (CLIENT_RELIABLE_SEND_SLEEP/MAX_PLAYER_COUNT)/2);
const std::chrono::milliseconds CLIENT_RELIBALE_RECV_SLEEP(SERVER_RELIABLE_REVC_SEND_SLEEP/2);

const std::chrono::milliseconds CLIENT_UNRELIABLE_SEND_SLEEP(16);
const std::chrono::milliseconds SERVER_UNRELIABLE_RECV_SLEEP(
    (CLIENT_UNRELIABLE_SEND_SLEEP/MAX_PLAYER_COUNT)/2);

const std::chrono::milliseconds SERVER_BROADCAST_GAME_STATE_SLEEP(16);
const std::chrono::milliseconds CLIENT_UNRELIABLE_RECV_SLEEP(SERVER_BROADCAST_GAME_STATE_SLEEP/2);

/* Networking */
static const char* CLIENT_IP = "127.0.0.1";
static const unsigned short SERVER_TCP_PORT = 4844;
static const char* SERVER_IP = "127.0.0.1";

#endif /* game_settings_hpp */