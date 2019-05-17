// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Group.hpp"
#include "Player.hpp"
#include "Client.hpp"
#include "GameController.hpp"

const int PLAYER_COUNT = 10;

int main(int, char const**)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile("../resources/images/icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Load a music to play
    sf::Music music;
    if (!music.openFromFile("../resources/audio/nice_music.ogg")) {
        return EXIT_FAILURE;
    }

    // Play the music
    music.play();

    // Window
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    window.setTitle("Ungroup");

    // Create game controller
    GameController game_controller;
    Client client_1(PLAYER_COUNT);

    // Create players
    sf::Keyboard::Key keys[] = {sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Right, sf::Keyboard::Left};
    game_controller.createPlayer(keys);

    sf::Keyboard::Key keys2[] = {sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::D, sf::Keyboard::A};
    game_controller.createPlayer(keys2);

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape
            ) {
                window.close();
            }

            // Handle game controller events
            game_controller.handleEvents(event);
        }

        // Update
        game_controller.update();
        client_1.update(game_controller.getGroups());

        // Display
        window.clear(sf::Color::White);
        client_1.draw(window);
        window.display();
    }

    return EXIT_SUCCESS;
}
