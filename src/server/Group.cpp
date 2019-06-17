#include <stdio.h>
#include <iostream>
#include <numeric>
#include "Group.hpp"
#include "../common/collision.hpp"

Group::Group(int id, sf::Vector2f position) {
    mId = id;
    mSize = 0;
    mCircle = new Circle(0.f, position);
}

Group::~Group() {
    //dtor
}

void Group::update() {
    refresh();

    if (isActive()) {
        // Update velocity
        sf::Vector2f new_velocity = std::accumulate(
            mMembers.begin(),
            mMembers.end(),
            sf::Vector2f(0.f, 0.f),
            [](sf::Vector2f curr_vel, std::shared_ptr<Player> player) {
                return curr_vel + player->getDirection();
            }
        );
        mCircle->setVelocity(new_velocity);

        // Update position
        mCircle->move();

    }
}

/**
    Sets group to active if any of its members are active.
*/
void Group::refresh() {
    bool any_active_members = std::any_of(
        mMembers.begin(), mMembers.end(),
        [](std::shared_ptr<Player> player){return player->isActive();}
    );
    if (any_active_members)
        setActive(true);
    else
        setActive(false);
    return;
}

void Group::addMember(std::shared_ptr<Player> player) {
    mMembers.push_back(player);
    mSize = mMembers.size();
    mCircle->setRadius(mSize * 10.f);
}

int Group::getId() const {
    return mId;
}

Circle* Group::getCircle() {
    return mCircle;
}

void Group::handleCollisions(std::vector<std::shared_ptr<Group>>& groups) {
    std::vector<Circle*> circles;
    std::transform(
        groups.begin(), groups.end(), std::back_inserter(circles),
        [](std::shared_ptr<Group> group){return group->getCircle();}
    );
    handle_circle_collision(circles);
}
