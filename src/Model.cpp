#include "Model.hpp"

namespace LumenCappuccino {
Model::Model() : tiles() {
   for(auto& row : tiles)
    for(Tile& tile : row)
        tile = HIDDEN{};
}
bool Model::setup() { return true; }
bool Model::startGame() { return true; }
bool Model::isGameOver() const { return false; }
bool Model::isGameWon() const { return false; }
bool Model::isGameLost() const { return false; }
std::optional<Tile> Model::getTile(int row, int column) const {
  if (row >= 0 && row <= 2 && column >= 0 && column <= 3) [[likely]]
    return tiles[row][column];
  return {};
}
std::optional<Tile> Model::uncoverTile([[maybe_unused]] int row,
                                       [[maybe_unused]] int column) const {
  return {};
}
} // namespace LumenCappuccino
