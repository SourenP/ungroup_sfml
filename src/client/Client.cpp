#include <stdio.h>
#include <iostream>
#include "Client.hpp"

Client::Client(int max_player_count, sf::Keyboard::Key keys[4]):mDirection(1.0, 1.0) {
    mInterpolating = false;
    for (int i=0; i < max_player_count; i++) {
        mGroupShapes.push_back(
            new GroupShape(
                1.f,
                sf::Vector2f(10.f, 10.f),
                sf::Color(rand() % 255, rand() % 255, rand() % 255)
            )
        );
    }

    // Set up input
    mKeys.up = keys[0];
    mKeys.down = keys[1];
    mKeys.right = keys[2];
    mKeys.left = keys[3];

    // Networking
    // TODO: assign id from client and use it to create player
    mNetworkingClient = new NetworkingClient();
    mNetworkingClient->Start();
}

Client::~Client() {
    //dtor
}

void Client::draw(sf::RenderTarget& target) {
    // Draw all groupCircles
    for (auto group_shape: mGroupShapes) {
        if (group_shape->isActive()) {
            group_shape->draw(target);
        }
    }
}

void Client::update() {
    std::vector<position> positions;
    std::unordered_map<sf::Uint32, direction> directions;

    // interpolate if network data is stale
    if (!mNetworkingClient->getIsFresh()) {
      if (!mInterpolating) {
        std::cout << "OMG MOM WE'RE INTERPOLATING" << std::endl;
        positions = mNetworkingClient->getPositions();
        mInterpolatedPositions = positions;
        mInterpolating = true;
      }
      // set this so following code can be blissfully
      // unaware it's working on the inteprolated values
      directions = mNetworkingClient->getDirections();
    } else {
      std::cout << "OMG DAD WE'RE NOT INTERPOLATING" << std::endl;
      positions = mNetworkingClient->getPositions();
      mNetworkingClient->setIsFresh(false);
      mInterpolating = false;
    }

    // update graphical state
    if (mInterpolating) {
      for(auto& position: mInterpolatedPositions) {
          if (mInterpolating) {
            position.x_pos += directions[position.id].x_dir;
            position.y_pos += directions[position.id].y_dir;
            std::cout << "interpol: " << position.id << " x,y: " << position.x_pos << position.y_pos << std::endl;
          }
          int active_group_id = position.id;
          if (active_group_id >= mGroupShapes.size()) {
              throw std::runtime_error("Update group with no corresponding GroupShape");
          }

          GroupShape* group_shape = mGroupShapes[active_group_id];

          group_shape->setPosition(sf::Vector2f(position.x_pos, position.y_pos));
          group_shape->setRadius(10.f);
          group_shape->setActive(true);
      }
    } else {
      for(auto position: positions) {
          if (mInterpolating) {
            position.x_pos += directions[position.id].x_dir;
            position.y_pos += directions[position.id].y_dir;
            std::cout << "interpol: " << position.id << " x,y: " << position.x_pos << position.y_pos << std::endl;
          }
          int active_group_id = position.id;
          if (active_group_id >= mGroupShapes.size()) {
              throw std::runtime_error("Update group with no corresponding GroupShape");
          }

          GroupShape* group_shape = mGroupShapes[active_group_id];

          group_shape->setPosition(sf::Vector2f(position.x_pos, position.y_pos));
          group_shape->setRadius(10.f);
          group_shape->setActive(true);
      }
    }
    // Network update direction
    direction d = {mDirection.x, mDirection.y};
    mNetworkingClient->setDirection(d);
}

sf::Vector2f Client::getDirection() const {
    return mDirection;
}

void Client::setId(sf::Uint32 id) {
    mId = id;
}


sf::Uint32 Client::getId() const {
    return mId;
}

void Client::handleEvents(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        mDirection = sf::Vector2f(0.f,0.f);
        if(sf::Keyboard::isKeyPressed(mKeys.up)) {
            mDirection += sf::Vector2f(0.f,-1.f);
        }
        if(sf::Keyboard::isKeyPressed(mKeys.down)) {
            mDirection += sf::Vector2f(0.f,1.f);
        }
        if(sf::Keyboard::isKeyPressed(mKeys.left)) {
            mDirection += sf::Vector2f(-1.f,0.f);
        }
        if(sf::Keyboard::isKeyPressed(mKeys.right)) {
            mDirection += sf::Vector2f(1.f,0.f);
        }
        mDirection = normalize(mDirection);
    }
}
