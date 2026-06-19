#include "Model.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <variant>

namespace LumenCappuccino {
Model::Model() : shownTiles(), realTiles(), gameStartTime() {}

bool Model::setup() {
  std::array<Tile, 12> tilesOrder{COCOA_BEAN{},  COCOA_BEAN{}, COFFEE_BEAN{},
                                  COFFEE_BEAN{}, CAPPUCCINO{}, CAPPUCCINO{},
                                  GREEN_TEA{},   GREEN_TEA{},  ESPRESSO{},
                                  ESPRESSO{},    MACHIATTO{},  MACHIATTO{}};

  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(tilesOrder.begin(), tilesOrder.end(), gen);
  for (int row{}; row < 3; ++row) {
    for (int column{}; column < 4; ++column) {
      realTiles[row][column] = tilesOrder[row * 4 + column];
      shownTiles[row][column] = HIDDEN{};
    }
  }
  return true;
}

// static void printTile(const Tile& t) {
//   if (std::holds_alternative<COFFEE_BEAN>(t)) {
//     std::cout << "COFFEE_BEAN" << std::endl;
//   } else if (std::holds_alternative<GREEN_TEA>(t)) {
//     std::cout << "GREEN_TEA" << std::endl;
//   } else if (std::holds_alternative<COCOA_BEAN>(t)) {
//     std::cout << "COCOA_BEAN" << std::endl;
//   } else if (std::holds_alternative<MACHIATTO>(t)) {
//     std::cout << "MACHIATTO" << std::endl;
//   } else if (std::holds_alternative<ESPRESSO>(t)) {
//     std::cout << "ESPRESSO" << std::endl;
//   } else if (std::holds_alternative<CAPPUCCINO>(t)) {
//     std::cout << "CAPPUCCINO" << std::endl;
//   } else if (std::holds_alternative<HIDDEN>(t)) {
//     std::cout << "HIDDEN" << std::endl;
//   } else if (std::holds_alternative<DELETED>(t)) {
//     std::cout << "DELETED" << std::endl;
//   }
// }

bool Model::isGameStarted() const { return gameStartTime.has_value(); }
bool Model::startGame() {
  gameStartTime = Clock::now();
  return true;
}

std::optional<Model::TimePoint> Model::getStartTime() const {
  return gameStartTime;
}

bool Model::isGameOver() const { return false; }
bool Model::isGameWon() const { return false; }
bool Model::isGameLost() const { return false; }
std::optional<Tile> Model::getTile(int row, int column) const {
  if (row >= 0 && row <= 2 && column >= 0 && column <= 3) [[likely]]
    return shownTiles[row][column];
  // return realTiles[row][column];
  return {};
}
std::optional<Tile> Model::uncoverTile([[maybe_unused]] int row,
                                       [[maybe_unused]] int column) const {
  return {};
}
} // namespace LumenCappuccino
