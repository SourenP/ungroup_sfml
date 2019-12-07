#include "RenderingController.hpp"

RenderingController::RenderingController(sf::RenderWindow& window, GameObjectController& goc,
                                         ResourceStore& rs) :
    m_window(window),
    m_gameObjectController(goc), m_resourceStore(rs), m_animationController(m_resourceStore),
    m_guiController(m_window.getSize(), m_resourceStore), m_uiData({}) {
    sf::Vector2f window_size = sf::Vector2f(m_window.getSize());

    // Create buffer to draw game onto. Buffer can be scaled to give pixelated effect
    if (!m_buffer.create(window_size.x, window_size.y)) {
        throw std::runtime_error("Failed to create buffer.");
    }
    m_buffer.setSmooth(false);

    m_bufferSprite = sf::Sprite(m_buffer.getTexture());
    m_bufferScalingFactor = {GAME_SCALE * GAME_SIZE.x / WINDOW_RESOLUTION.x,
                             GAME_SCALE * GAME_SIZE.y / WINDOW_RESOLUTION.y};
    m_bufferSprite.setScale(m_bufferScalingFactor);

    // Create views to draw GUI and player view
    m_windowView = sf::View(window_size / 2.f, window_size);
    m_playerView = sf::View({}, sf::Vector2f(m_window.getSize()));

    // Winner text parameters
    m_winnerText.setFont(*rs.getFont(RenderingDef::FontKey::monogram));
    m_winnerText.setString("WINNER: NO DATA");
    m_winnerText.setCharacterSize(100.f);
    m_winnerText.setFillColor(sf::Color::White);
}

void RenderingController::preUpdate() {
    // noop
}

void RenderingController::update(sf::Int32 delta_ms) {
    m_animationController.update(delta_ms);
}

void RenderingController::postUpdate(const sf::Vector2f& player_position, const UIData& ui_data) {
    m_playerPosition = player_position;
    m_uiData = ui_data;

    sf::Vector2f player_view_position = {m_playerPosition.x * m_bufferScalingFactor.x,
                                         m_playerPosition.y * m_bufferScalingFactor.y};
    m_playerView.setCenter(player_view_position);
    m_guiController.update(m_uiData);
}

void RenderingController::draw() {
    switch (m_uiData.game_status) {
        case GameStatus::not_started: {
            drawNotStarted();
            break;
        }
        case GameStatus::playing: {
            drawPlaying();
            break;
        }
        case GameStatus::game_over: {
            drawGameOver();
            break;
        }
    }
}

void RenderingController::drawNotStarted() {
    m_window.clear(sf::Color::Blue);
    m_window.display();
}

void RenderingController::drawPlaying() {
    m_window.clear(sf::Color::Green);
    m_buffer.clear(RenderingDef::BACKGROUND_COLOR);

    // Draw game from player view
    m_window.setView(m_playerView);

    m_gameObjectController.draw(m_buffer);
    m_animationController.draw(m_buffer);
    m_window.draw(m_bufferSprite);

    // Draw GUI from window view
    m_window.setView(m_windowView);
    m_guiController.draw(m_window);

    m_buffer.display();
    m_window.display();
}

void RenderingController::drawGameOver() {
    m_window.clear(sf::Color::Red);
    m_winnerText.setString("WINNER: " + std::to_string(m_uiData.winner_player_id));
    sf::FloatRect text_bounds = m_winnerText.getLocalBounds();
    m_winnerText.setPosition(
        (sf::Vector2f(m_window.getSize()) - sf::Vector2f(text_bounds.width, text_bounds.height)) /
        2.f);
    m_window.draw(m_winnerText);
    m_window.display();
}