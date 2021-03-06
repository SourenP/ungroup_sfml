#include "DirectionArrows.hpp"

#include <iostream>
#include <math.h>

#include "../../common/physics/VectorUtil.hpp"
#include "../../common/util/game_settings.hpp"
#include "RenderingDef.hpp"

const float MIN_SPEED = 1.f;            // Minimum speed at which velocity arrow will still show
const float TWO_MINUS_SQRT_3 = 0.2679f; // 2 - sqrt(3);
const float ARROW_SIZE = 6.f;           // Size of the triangle's base
const float DISTANCE_FROM_EDGE = 3.0f;  // Additional distance from cricle's edge

DirectionArrows::DirectionArrows() : m_velocityTriangle(0, 3) {
    m_targetTriangles.reserve(GAME_SETTINGS.MAX_PLAYER_COUNT);
    for (size_t i = 0; i < GAME_SETTINGS.MAX_PLAYER_COUNT; i++) {
        m_targetTriangles.push_back(sf::CircleShape(0, 3));
    }
}

void DirectionArrows::draw(
    sf::RenderTarget& render_target, float radius, sf::Vector2f position, sf::Vector2f velocity,
    const std::vector<std::pair<sf::Vector2f, sf::Color>>& direction_color_pairs) {

    drawTargetArrows(render_target, radius, position, direction_color_pairs);
    if (RenderingDef::SHOW_VELOCITY_ARROW) {
        drawVelocityArrow(render_target, radius, position, velocity,
                          RenderingDef::DIRECTION_ARROW_COLOR);
    }
}

void DirectionArrows::drawVelocityArrow(sf::RenderTarget& render_target, float radius,
                                        sf::Vector2f position, sf::Vector2f velocity,
                                        sf::Color color) {
    if (abs(velocity.x) < MIN_SPEED && abs(velocity.y) < MIN_SPEED) {
        return;
    }

    sf::Vector2f direction = VectorUtil::normalize(velocity);

    m_velocityTriangle.setFillColor(color);
    m_velocityTriangle.setRadius(ARROW_SIZE / 2.f);

    positionTriangle(m_velocityTriangle, direction, position, ARROW_SIZE, radius);

    render_target.draw(m_velocityTriangle);
}

void DirectionArrows::drawTargetArrows(
    sf::RenderTarget& render_target, float radius, sf::Vector2f position,
    const std::vector<std::pair<sf::Vector2f, sf::Color>>& direction_color_pairs) {
    // Keep track of directions we've already drawn to avoid drawing same direction more than once
    // std::vector<sf::Vector2f> seen_directions;
    for (size_t i = 0; i < direction_color_pairs.size(); i++) {
        const auto& direction = VectorUtil::normalize(direction_color_pairs[i].first);
        auto color = direction_color_pairs[i].second;

        if (direction == sf::Vector2f(0, 0)) {
            continue;
        }

        // if (std::find(seen_directions.begin(), seen_directions.end(), direction) !=
        //         seen_directions.end()) {
        //     continue;
        // }
        // seen_directions.push_back(direction);

        sf::CircleShape& target_triangle = m_targetTriangles[i];

        target_triangle.setFillColor(color);
        target_triangle.setRadius(ARROW_SIZE / 2.f);
        positionTriangle(target_triangle, direction, position, ARROW_SIZE, radius);

        render_target.draw(target_triangle);
    }
}

void DirectionArrows::positionTriangle(sf::CircleShape& triangle, sf::Vector2f direction,
                                       sf::Vector2f position, float size, float radius) {
    triangle.setOrigin(sf::Vector2f(size / 2.f, size / 2.f));
    triangle.setRotation(VectorUtil::angle(direction) - 270.f);

    // Move from corner to middle of circle
    triangle.setPosition(position.x + radius, position.y + radius);

    // Move to edge of circle
    triangle.move(direction * (radius + (TWO_MINUS_SQRT_3 * size) + DISTANCE_FROM_EDGE));
}
