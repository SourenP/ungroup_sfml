#include "Player.hpp"

#include <iostream>

#include "../util/network_util.hpp"

Player::Player(uint32_t id) : GameObject(id), m_direction(0.0, 0.0), m_winCondition(id) {
}

Player::~Player() {
}

/* Network Util */

PlayerUpdate Player::getUpdate() const {
    PlayerUpdate pu = {
        .player_id = static_cast<sf::Uint32>(getId()),
        .is_active = isActive(),
        .direction = getDirection(),
        .joinable = getJoinable(),
        .ungroup = getUngroup(),
        .resource_counts_to_win = getResourceCountsToWin(),
        .intent = static_cast<sf::Uint32>(getIntent()),
    };
    return pu;
}

void Player::applyUpdate(PlayerUpdate pu) {
    setActive(pu.is_active);
    setDirection(pu.direction);
    setJoinable(pu.joinable);
    setUngroup(pu.ungroup);
    setResourceCountToWin(pu.resource_counts_to_win);
    setIntent(ResourceType(pu.intent));
}

sf::Packet& operator<<(sf::Packet& packet, const PlayerUpdate& player_update) {
    return packet << player_update.player_id << player_update.is_active << player_update.direction
                  << player_update.joinable << player_update.ungroup
                  << player_update.resource_counts_to_win << player_update.intent;
}

sf::Packet& operator>>(sf::Packet& packet, PlayerUpdate& player_update) {
    return packet >> player_update.player_id >> player_update.is_active >>
           player_update.direction >> player_update.joinable >> player_update.ungroup >>
           player_update.resource_counts_to_win >> player_update.intent;
}
