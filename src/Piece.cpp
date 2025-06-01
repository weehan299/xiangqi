
// src/Piece.cpp
#include "Piece.hpp"
#include "Board.hpp"

// define & initialize static members
float Piece::offsetX = 0.f;
float Piece::offsetY = 0.f;
float Piece::scaleX  = 1.f;
float Piece::scaleY  = 1.f;

Piece::Piece(PieceType type, Side side, sf::Texture& texture)
    : type(type), side(side)
{
    sprite.setTexture(texture);
    // 1) scale so the sprite width == CELL_SIZE
    auto texSz = texture.getSize();              // e.g. {120,120}
    float s = CELL_SIZE / float(texSz.x);
    sprite.setScale(s, s);

    // 2) center the origin
    sprite.setOrigin(texSz.x/2.f, texSz.y/2.f);

    // default at (0,0)
    boardPos = {0,0};
    resetSpritePosition();
    
}


void Piece::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::Vector2i Piece::getBoardPosition() const {
    return boardPos;
}

void Piece::setBoardPosition(int x, int y) {
    boardPos = { x, y };
    resetSpritePosition();
}

PieceType Piece::getType() const {
    return type;
}

Side Piece::getSide() const {
    return side;
}

sf::Sprite& Piece::getSprite() {
    return sprite;
}

void Piece::resetSpritePosition() {
    // apply the same offset & scaleX/Y that Board computed:
    sprite.setPosition(
      offsetX + boardPos.x * CELL_SIZE,
      offsetY + boardPos.y * CELL_SIZE
    );
}
