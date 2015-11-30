//
//  CPlayerState.cpp
//  AI-ABBYY
//
//  Created by Alexander Danilyak on 21/09/15.
//  Copyright © 2015 Alexander Danilyak. All rights reserved.
//
#pragma once

#include "CPlayerState.hpp"
#include <stdexcept>

PlayerState::PlayerState() {
    position = std::make_pair(0, 0);
    velocityVector = std::make_pair(0, 0);
}

PlayerState::PlayerState(std::pair< int, int > startPosition) {
    if (startPosition.first < 0 || startPosition.second < 0) {
        throw std::invalid_argument("start position coordinates must be >= 0");
    }
    position = startPosition;
    velocityVector = std::make_pair(0, 0);
}

PlayerState::PlayerState(int x, int y, int xVelocity, int yVelocity) {
    position = std::make_pair(x, y);
    velocityVector = std::make_pair(xVelocity, yVelocity);
}

PlayerState::~PlayerState() {
}

bool PlayerState::operator==(const IPlayerState &other) const {
    return position == other.getPosition() && velocityVector == other.getVelocityVector();
}

bool PlayerState::operator!=(const IPlayerState &other) const {
    return !(*this == other);
}

bool PlayerState::operator<(const IPlayerState &other) const
{
	return position < other.getPosition();
}

void PlayerState::changePosition( std::pair< int, int > inputPosition )
{
    if (inputPosition.first < 0 || inputPosition.second < 0) {
        throw std::invalid_argument("inputPosition coordinates must be >= 0");
    }
    position = inputPosition;
}

void PlayerState::changeVelocityVector(std::pair< int, int > inputVelocityVector) {
    velocityVector = inputVelocityVector;
}

void PlayerState::changePositionUsingVelocityVector() {
    position.first += velocityVector.first;
    position.second += velocityVector.second;
}

void PlayerState::dropVelocityVector() {
    velocityVector.first = 0;
    velocityVector.second = 0;
}

void PlayerState::Move( int dx, int dy )
{
	changePositionUsingVelocityVector();
	velocityVector.first += dx;
	velocityVector.second += dy;
	position.first += dx;
	position.second += dy;
}

int PlayerState::GetXVelocity() const
{
    return velocityVector.first;
}

int PlayerState::GetYVelocity() const {
    return velocityVector.second;
}

int PlayerState::GetX() const {
    return position.first;
}

int PlayerState::GetY() const {
    return position.second;
}

std::pair< int, int > PlayerState::getPosition() const {
    return position;
}

std::pair< int, int > PlayerState::getVelocityVector() const {
    return velocityVector;
}

PlayerState PlayerState::GetNextState( int dx, int dy ) const
{
	PlayerState temp( *this );
	temp.changePositionUsingVelocityVector();
	temp.position.first += dx;
	temp.position.second += dy;
	temp.velocityVector.first += dx;
	temp.velocityVector.second += dy;
	return temp;
}

void PlayerState::SetXVelocity( int xVelocity )
{
    velocityVector.first = xVelocity;
}

void PlayerState::SetYVelocity(int yVelocity) {
    velocityVector.second = yVelocity;
}

void PlayerState::SetX(int x) {
    position.first = x;
}

void PlayerState::SetY(int y) {
    position.second = y;
}

