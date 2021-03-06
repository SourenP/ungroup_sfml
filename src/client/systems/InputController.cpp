#include "InputController.hpp"

#include <iostream>

InputController::InputController(InputDef::InputKeys keys) :
    m_inputKeys(keys), m_reliableInput{false, false, false}, m_unreliableInput{false, false, false,
                                                                               false, false} {
}

std::pair<InputDef::ReliableInput, InputDef::UnreliableInput>
InputController::collectInputs(sf::RenderWindow& window) {
    // Reset updates
    m_reliableInput.setAll(false);
    m_unreliableInput.setAll(false);

    // Process events
    sf::Event event;
    while (window.pollEvent(event)) {
        // Close window: exit
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            m_reliableInput = {
                .toggle_ungroup = sf::Keyboard::isKeyPressed(m_inputKeys.ungroup),
                .toggle_joinable = sf::Keyboard::isKeyPressed(m_inputKeys.joinable),
                .toggle_intent = sf::Keyboard::isKeyPressed(m_inputKeys.intent),
            };
            m_unreliableInput = {
                .toggle_up = sf::Keyboard::isKeyPressed(m_inputKeys.up),
                .toggle_down = sf::Keyboard::isKeyPressed(m_inputKeys.down),
                .toggle_right = sf::Keyboard::isKeyPressed(m_inputKeys.right),
                .toggle_left = sf::Keyboard::isKeyPressed(m_inputKeys.left),
                .toggle_stop = sf::Keyboard::isKeyPressed(m_inputKeys.stop),
            };
        }
    }

    return std::pair<InputDef::ReliableInput, InputDef::UnreliableInput>(m_reliableInput,
                                                                         m_unreliableInput);
}

InputDef::PlayerInputs InputController::getPlayerInputs(uint32_t player_id) {
    return getPlayerInputs(player_id, m_reliableInput, m_unreliableInput);
}

InputDef::PlayerInputs InputController::getPlayerInputs(uint32_t player_id,
                                                        InputDef::ReliableInput ri,
                                                        InputDef::UnreliableInput ui) {
    InputDef::PlayerInputs pi;

    if (!m_reliableInput.allFalse()) {
        InputDef::PlayerReliableInput player_reliable_input = {
            .player_id = player_id,
            .reliable_input = ri,
        };
        pi.player_reliable_inputs.push_back(player_reliable_input);
    }
    if (!m_unreliableInput.allFalse()) {
        InputDef::PlayerUnreliableInput player_unreliable_input = {
            .player_id = player_id,
            .unreliable_input = ui,
        };
        pi.player_unreliable_inputs.push_back(player_unreliable_input);
    }

    return pi;
}