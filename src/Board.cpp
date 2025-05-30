
// ---------- src/Board.cpp ----------
#include "Board.h"
#include <iostream>
#include <algorithm>
#include <array>
#include <string>


Board::Board() 
    : currentTurn(Side::Red)
{

    loadTextures();

    // 1) assign the texture
    boardSprite.setTexture(boardTexture);

    // 2) figure out how big the image really is
    sf::Vector2u tex = boardTexture.getSize();

    // 3) compute scale factors to stretch/shrink it to 576×640
    float scaleX = (COLUMNS * CELL_SIZE)  / float(tex.x);
    float scaleY = (ROWS * CELL_SIZE) / float(tex.y);
    Piece::scaleX  = scaleX;
    Piece::scaleY  = scaleY;
    // also compute the world‐offset (cropped margin * scale):
    Piece::offsetX = MARGIN * scaleX;
    Piece::offsetY = MARGIN * scaleY;

    // Debug output
    //std::cout << "image size: " << tex.x << ", " << tex.y << std::endl;
    //std::cout << "Scale factors: " << scaleX << ", " << scaleY << std::endl;

    // 4) apply uniform (or non-uniform) scale
    boardSprite.setScale(scaleX, scaleY);

    // 5) make sure it lives at the top-left corner
    boardSprite.setPosition(0.f, 0.f);

    initPieces();
}

Side Board::getCurrentTurn() const {
    return currentTurn;
}

void Board::loadTextures() {
    if (!boardTexture.loadFromFile("assets/board_cropped.png")) {
        throw std::runtime_error("Failed to load assets/board_cropped.png");
    }
    static const std::array<std::string,7> names = {"general","advisor","elephant","horse","chariot","cannon","soldier"};
    for (int t = 0; t < 7; ++t) {
        // Red side = index 0
        std::string redPath = "assets/red_" + names[t] + ".png";
        if (!pieceTextures[t][0].loadFromFile(redPath))
            throw std::runtime_error("Failed to load " + redPath);
        // Black side = index 1
        std::string blackPath = "assets/black_" + names[t] + ".png";
        if (!pieceTextures[t][1].loadFromFile(blackPath))
            throw std::runtime_error("Failed to load " + blackPath);
    }
}

void Board::initPieces() {
    pieces.clear();
    pieces.reserve(32);
    struct Setup { PieceType type; int x; int y; Side side; };
    const std::vector<Setup> setups = {
        // Black back rank
        //{PieceType::Chariot,0,0,Side::Black}, {PieceType::Horse,1,0,Side::Black}, {PieceType::Elephant,2,0,Side::Black},
        {PieceType::Advisor,3,0,Side::Black}, {PieceType::General,4,0,Side::Black}, {PieceType::Advisor,5,0,Side::Black},
        //{PieceType::Elephant,6,0,Side::Black}, {PieceType::Horse,7,0,Side::Black}, {PieceType::Chariot,8,0,Side::Black},
        // Black cannons
        //{PieceType::Cannon,1,2,Side::Black}, {PieceType::Cannon,7,2,Side::Black},
        // Black soldiers
        //{PieceType::Soldier,0,3,Side::Black}, {PieceType::Soldier,2,3,Side::Black}, {PieceType::Soldier,4,3,Side::Black},
        //{PieceType::Soldier,6,3,Side::Black}, {PieceType::Soldier,8,3,Side::Black},
        // Red back rank
        {PieceType::Chariot,0,9,Side::Red}, {PieceType::Horse,1,9,Side::Red}, {PieceType::Elephant,2,9,Side::Red},
        {PieceType::Advisor,3,9,Side::Red}, {PieceType::General,4,9,Side::Red}, {PieceType::Advisor,5,9,Side::Red},
        {PieceType::Elephant,6,9,Side::Red}, {PieceType::Horse,7,9,Side::Red}, {PieceType::Chariot,8,9,Side::Red},
        // Red cannons
        {PieceType::Cannon,1,7,Side::Red}, {PieceType::Cannon,7,7,Side::Red},
        // Red soldiers
        //{PieceType::Soldier,0,6,Side::Red}, {PieceType::Soldier,2,6,Side::Red}, {PieceType::Soldier,4,6,Side::Red},
        //{PieceType::Soldier,6,6,Side::Red}, {PieceType::Soldier,8,6,Side::Red}
    };
    for (const auto& s : setups) {
        int sideIndex = (s.side == Side::Red ? 0 : 1);
        sf::Texture& tex = pieceTextures[int(s.type)][sideIndex];
        pieces.emplace_back(s.type, s.side, tex);
        pieces.back().setBoardPosition(s.x, s.y);
    }
}

void Board::draw(sf::RenderWindow& window) {
    window.draw(boardSprite);
    for (auto& p : pieces) p.draw(window);
}

Piece* Board::pieceAt(int x, int y) {
    for (auto& p : pieces) {
        if (p.getBoardPosition() == sf::Vector2i(x, y))
            return &p;
    }
    return nullptr;
}

// const overload – essentially identical, but returns a const pointer
const Piece* Board::pieceAt(int x, int y) const {
    for (const auto& p : pieces)
        if (p.getBoardPosition() == sf::Vector2i(x, y))
            return &p;
    return nullptr;
}

bool Board::isLegalMove(const Piece* p, int dx, int dy) const {
    // For now, just check if the destination is within bounds
    if (dx < 0 || dx >= COLUMNS || dy < 0 || dy >= ROWS) return false;
    sf::Vector2i s = p->getBoardPosition();

    int ddx = dx - s.x;
    int ddy = dy - s.y;

    if (s.x == dx && s.y == dy) return false; // No move
    PieceType type = p->getType();
    Side side = p->getSide();
    const Piece* dest    = pieceAt(dx, dy);

    // Basic checks for legality
    switch (type)  // Use switch-case for PieceType
    {
    case PieceType::General: {
        // One step orthogonal
        if (std::abs(ddx) + std::abs(ddy) != 1) return false;
        // Within palace
        int minX = 3, maxX = 5;
        int minY = (side == Side::Black ? 0 : 7);
        int maxY = (side == Side::Black ? 2 : 9);
        return (dx >= minX && dx <= maxX && dy >= minY && dy <= maxY);
    }
    case PieceType::Advisor: {
        // One step diagonal
        if (std::abs(ddx) != 1 || std::abs(ddy) != 1) return false;
        // Within palace
        int minX = 3, maxX = 5;
        int minY = (side == Side::Black ? 0 : 7);
        int maxY = (side == Side::Black ? 2 : 9);
        return (dx >= minX && dx <= maxX && dy >= minY && dy <= maxY);
    }
    case PieceType::Elephant: {
        // Two steps diagonal
        if (std::abs(ddx) != 2 || std::abs(ddy) != 2) return false;
        // Cannot cross river
        if (side == Side::Black && dy > 4) return false;
        if (side == Side::Red   && dy < 5) return false;
        // No blocking at midpoint
        int mx = (s.x + dx) / 2;
        int my = (s.y + dy) / 2;
        if (pieceAt(mx, my)) return false;
        return true;
    }
    case PieceType::Horse: {
        // L-shape: (2,1) or (1,2)
        if (!( (std::abs(ddx) == 2 && std::abs(ddy) == 1) ||
               (std::abs(ddx) == 1 && std::abs(ddy) == 2) )) return false;
        // Check leg-block
        if (std::abs(ddx) == 2) {
            int bx = s.x + ddx/2;
            if (pieceAt(bx, s.y)) return false;
        } else {
            int by = s.y + ddy/2;
            if (pieceAt(s.x, by)) return false;
        }
        return true;
    }
    case PieceType::Chariot: {
        // Straight line
        if (ddx != 0 && ddy != 0) return false;
        // No blocking between
        int steps = std::max(std::abs(ddx), std::abs(ddy));
        int dxs = (ddx == 0 ? 0 : (ddx > 0 ? 1 : -1));
        int dys = (ddy == 0 ? 0 : (ddy > 0 ? 1 : -1));
        for (int i = 1; i < steps; ++i) {
            int ix = s.x + dxs * i;
            int iy = s.y + dys * i;
            if (pieceAt(ix, iy)) return false;
        }
        return true;
    }
    case PieceType::Cannon: {
        // Straight line
        if (ddx != 0 && ddy != 0) return false;
        int steps = std::max(std::abs(ddx), std::abs(ddy));
        int dxs = (ddx == 0 ? 0 : (ddx > 0 ? 1 : -1));
        int dys = (ddy == 0 ? 0 : (ddy > 0 ? 1 : -1));
        // Count intervening pieces
        int count = 0;
        for (int i = 1; i < steps; ++i) {
            int ix = s.x + dxs * i;
            int iy = s.y + dys * i;
            if (pieceAt(ix, iy)) ++count;
        }
        if (dest) {
            // Capture: exactly one in between
            return (count == 1);
        } else {
            // Non-capture: none in between
            return (count == 0);
        }
    }
    case PieceType::Soldier: {
        // Forward only (one step)
        if (side == Side::Black) {
            if (ddy != 1 || std::abs(ddx) > 0) {
                // After crossing river, can move sideways
                if (s.y >= 5 && ddy == 0 && std::abs(ddx) == 1) return true;
                return false;
            }
            return (ddx == 0 && ddy == 1);
        } else {
            if (ddy != -1 || std::abs(ddx) > 0) {
                // After crossing river
                if (s.y <= 4 && ddy == 0 && std::abs(ddx) == 1) return true;
                return false;
            }
            return (ddx == 0 && ddy == -1);
        }
    }
    
    default:
       return true;
    }
    return true;
}
//---------------------------------------
// Actual move logic (no in-check guard)
//---------------------------------------
bool Board::movePieceInternal(int sx, int sy, int dx, int dy) {
    // 1) find source piece
    auto srcIt = std::find_if(pieces.begin(), pieces.end(), [&](const Piece& p) {
        return p.getBoardPosition() == sf::Vector2i(sx, sy);
    });
    if (srcIt == pieces.end()) return false;

    // 2) find any destination piece
    auto dstIt = std::find_if(pieces.begin(), pieces.end(), [&](const Piece& p) {
        return p.getBoardPosition() == sf::Vector2i(dx, dy);
    });

    // 3) record the move
    MoveRecord rec{ sx, sy, dx, dy, std::nullopt };
    if (dstIt != pieces.end()) {
        rec.capturedPiece = *dstIt;
        pieces.erase(dstIt);
        if (dstIt < srcIt) --srcIt;
    }
    moveHistory.push_back(rec);

    // 4) move the piece
    srcIt->setBoardPosition(dx, dy);

    // 5) switch turn
    currentTurn = (currentTurn == Side::Red ? Side::Black : Side::Red);

    return true;
}


std::vector<std::pair<sf::Vector2i, sf::Vector2i>>
Board::getLegalMoves(Side side) const {
    std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves;
    for (const Piece& p : pieces) {
        if (p.getSide() != side) continue;
        sf::Vector2i src = p.getBoardPosition();

        for (int x = 0; x < COLUMNS; ++x) {
            for (int y = 0; y < ROWS; ++y) {
                const Piece* dest = pieceAt(x, y);       // calls the const overload
                if (dest && dest->getSide() == side)     // same-side → skip
                    continue;
                if (isLegalMove(&p, x, y))
                    moves.emplace_back(src, sf::Vector2i(x, y));
            }
        }
    }
    // If we're not in check, all those moves are fine:
    if (!isInCheck(side)) return moves;

    // only give moves that escape checks
    std::vector<std::pair<sf::Vector2i, sf::Vector2i>> escapeMoves;
    Board* self = const_cast<Board*>(this);
    for (const auto& m : moves) {
        // apply
        self->movePieceInternal(m.first.x, m.first.y, m.second.x, m.second.y);
        // if king is safe now, keep it

        if (!self->isInCheck(side)){
            std::cout << "Found escape move: " << m.first.x << "," << m.first.y << " → " << m.second.x << "," << m.second.y << std::endl;
            escapeMoves.push_back(m);
        }
        // undo
        self->undoMove();
    }
    return escapeMoves;
}



// ----------------------------------------------------------------------------
// 1) isInCheck implementation
// ----------------------------------------------------------------------------
bool Board::isInCheck(Side side) const {
    // 1) Find that side’s general
    sf::Vector2i kingPos(-1, -1);
    for (const auto& p : pieces) {
        if (p.getType() == PieceType::General && p.getSide() == side) {
            kingPos = p.getBoardPosition();
            break;
        }
    }
    if (kingPos.x < 0) {
        // no general found?  shouldn’t happen in a valid game, but safe-guard
        return false;
    }

    // 2) See if any enemy piece has a legal (shape-only) attack on kingPos
    for (const auto& attacker : pieces) {
        if (attacker.getSide() == side) continue;
        // isLegalMove only checks move-shape and blocking rules, not “leaving king in check”
        if (isLegalMove(&attacker, kingPos.x, kingPos.y))
            return true;
    }

    return false;
}

/*

bool Board::movePiece(int sx, int sy, int dx, int dy) {
    Piece* p = pieceAt(sx, sy);
    if (!p) return false;
    if (!isLegalMove(p, dx, dy)) {
        std::cout << "Illegal move for piece at: " << sx << ", " << sy << " to " << dx << ", " << dy << std::endl;
        return false;
    }
    Piece* dest = pieceAt(dx, dy);
    // Can't capture your own
    if (dest && dest->getSide() == p->getSide()) return false;
    // Capture
    if (dest) {
        pieces.erase(
            //remove_if doesn’t actually shrink the vector; 
            //instead it reorders elements so that all those for which the predicate is false end up at the front, 
            // and returns an iterator new_end pointing to “one past the last valid element.” 
            //Everything from new_end to the original end() is now “garbage” and should be erased.
            std::remove_if(pieces.begin(), pieces.end(), [&](const Piece& q) {
                return q.getBoardPosition() == sf::Vector2i(dx, dy);
            }),
            pieces.end()
        );
        std::cout << "Captured piece at: " << dx << ", " << dy << std::endl;
    }
    p->setBoardPosition(dx, dy);
    return true;
}
*/


bool Board::movePiece(int sx, int sy, int dx, int dy) {
    // Locate the source piece iterator
    auto srcIt = std::find_if(pieces.begin(), pieces.end(), [&](const Piece& q) {
        return q.getBoardPosition() == sf::Vector2i(sx, sy);
    });

    //Enforce alternate-turn rule
    if (srcIt->getSide() != currentTurn) return false;

    if (srcIt == pieces.end()) return false;

    // Check legality first
    if (!isLegalMove(&*srcIt, dx, dy)) return false;

    // Locate any destination piece
    auto dstIt = std::find_if(pieces.begin(), pieces.end(), [&](const Piece& q) {
        return q.getBoardPosition() == sf::Vector2i(dx, dy);
    });
    // Prevent capturing your own
    if (dstIt != pieces.end() && dstIt->getSide() == srcIt->getSide()) return false;
    
    // --- RECORD the move (aggregate‐init leaves capturedPiece empty) ---
    MoveRecord rec{ sx, sy, dx, dy, std::nullopt };
    if (dstIt != pieces.end()) {
        // copy the captured Piece into the optional
        rec.capturedPiece = *dstIt;
    }
    moveHistory.push_back(std::move(rec));

    // Remove destination piece if present
    if (dstIt != pieces.end()) {
        pieces.erase(dstIt);
        // If the erased piece was before our source, shift srcIt back by one
        if (dstIt < srcIt) {
            --srcIt;
        }
    }

    // Move the source piece
    srcIt->setBoardPosition(dx, dy);

    // Switch turns
    currentTurn = (currentTurn == Side::Red ? Side::Black : Side::Red);
    return true;
}

bool Board::canUndo() const {
    return !moveHistory.empty();
}

void Board::undoMove() {
    if (moveHistory.empty()) return;

    // 1) pop the last record
    MoveRecord rec = std::move(moveHistory.back());
    moveHistory.pop_back();

    // 2) flip turn back
    currentTurn = (currentTurn == Side::Red ? Side::Black : Side::Red);

    // 3) move the piece back from rec.dx,rec.dy → rec.sx,rec.sy
    Piece* moved = pieceAt(rec.dx, rec.dy);
    if (moved) {
        moved->setBoardPosition(rec.sx, rec.sy);
    } else {
        // shouldn't happen if logic is correct
        std::cerr << "Undo: moved piece not found at expected dst\n";
    }

    // 4) if there was a capture, restore it
    if (rec.capturedPiece) {
        // place it back at the capture square
        Piece& cp = *rec.capturedPiece;
        cp.setBoardPosition(rec.dx, rec.dy);
        pieces.push_back(std::move(cp));
    }
}

bool Board::isCheckmate() const {
    std::cout << "Checking for checkmate..." <<  "Current turn: " << (currentTurn == Side::Red ? "Red" : "Black") << std::endl;
    // 1) Whose turn is it?
    Side sideToMove = currentTurn;

    // 2) If that side isn't even in check, it can't be checkmate
    if (!isInCheck(sideToMove)) 
        return false;

    // 3) Generate all legal “escape” moves.  If there are none, it’s checkmate.
    auto escapes = getLegalMoves(sideToMove);
    return escapes.empty();
}