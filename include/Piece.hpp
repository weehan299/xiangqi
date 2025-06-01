// include/Piece.h
#ifndef PIECE_H
#define PIECE_H

#include <SFML/Graphics.hpp>
enum class PieceType { General, Advisor, Elephant, Horse, Chariot, Cannon, Soldier };
enum class Side { Red, Black };

class Piece {
public:
    // these will be set by Board before any piece is drawn:
    static float offsetX, offsetY;  
    static float scaleX, scaleY;    

    // Constructor
    Piece(PieceType type, Side side, sf::Texture& texture);
    void draw(sf::RenderWindow& window);
    sf::Vector2i getBoardPosition() const;
    void setBoardPosition(int x, int y);
    PieceType getType() const;
    Side getSide() const;
    // New for dragging
    sf::Sprite& getSprite();
    void resetSpritePosition();
private:
    PieceType type;
    Side side;
    sf::Sprite sprite;
    sf::Vector2i boardPos;
};

#endif // PIECE_H