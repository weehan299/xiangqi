
// ---------- include/Board.h ----------
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Piece.h"
#include <optional>

static constexpr float CELL_SIZE = 64.f;
static constexpr unsigned COLUMNS = 9;
static constexpr unsigned ROWS    = 10;
static constexpr unsigned MARGIN    = 55;

class Board {
public:
    Board();
    void loadTextures();
    void initPieces();
    void draw(sf::RenderWindow& window);
    Piece* pieceAt(int x, int y);
    const Piece* pieceAt(int x, int y) const;
    bool movePiece(int sx, int sy, int dx, int dy);
    Side getCurrentTurn() const;
    std::vector<std::pair<sf::Vector2i, sf::Vector2i>> getLegalMoves(Side side) const;
    /// Can we undo?
    bool canUndo() const;
    /// Undo the last move (if any)
    void undoMove();

private:
    Side currentTurn; // Whose turn it is (Red starts)
    bool isLegalMove(const Piece* p, int dx, int dy) const;
    sf::Texture boardTexture;
    sf::Sprite boardSprite;
    sf::Texture pieceTextures[7][2]; // type & side
    std::vector<Piece> pieces;
    // record one move; if nothing was captured, capturedPiece==std::nullopt
    struct MoveRecord {
        int sx, sy;
        int dx, dy;
        std::optional<Piece>  capturedPiece;  // copy of the captured piece
    };
    std::vector<MoveRecord> moveHistory;
};

#endif // BOARD_H
