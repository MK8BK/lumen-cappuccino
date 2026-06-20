#include "Model.hpp"
#include <algorithm>
#include <random>
#include <variant>

namespace LumenCappuccino {
Model::Model() : shownTiles(), realTiles(), gameStartTime(), gameWon(false) {}

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

// std::optional<Model::TimePoint> Model::getStartTime() const {
//   return gameStartTime;
// }

float Model::getTimerPercent() const {
#ifdef LUMEN_STRICT_CHECKS
  if (!gameStartTime) {
#  ifdef LUMEN_LOG
    spdlog::error("Game not started yet.");
#  endif
    return -1.0f;
  }
#endif
  int ms =
      static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
                              Model::Clock::now() - gameStartTime.value())
                              .count());
  return std::min(static_cast<float>(ms * 100.0f / MAX_TIME_MILLISECONDS),
                  100.0f);
}

bool Model::isGameOver() const { return isGameWon() || isGameLost(); }

bool Model::isGameWon() const { return gameWon; }
bool Model::isGameLost() const {
  return !gameWon && (getTimerPercent() >= 100.0f);
}
std::optional<Tile> Model::getTile(int row, int column) const {
  if (row >= 0 && row <= 2 && column >= 0 && column <= 3) [[likely]]
    return shownTiles[row][column];
  // return realTiles[row][column];
  return {};
}

void Model::uncoverTile(int row, int column) {
  if (getTimerPercent() >= 100.0f) {
    return;
  }
  static int tilesVisible = 0;
#ifdef LUMEN_STRICT_CHECKS
  if (!isGameStarted() || isGameOver())
    return;
#endif
  if (row < 0 || row > 2 || column < 0 || column > 3) [[unlikely]] {
    return;
  }
  Tile shownTile{shownTiles.at(row).at(column)};
  if (std::holds_alternative<DELETED>(shownTile)) [[unlikely]]
    return;
  else if (!std::holds_alternative<HIDDEN>(shownTile)) {
    return;
  }
  if (tilesVisible == 2) { // assuming they are different
    // hide all tiles and show the new one
    for (int row_p{}; row_p < 3; ++row_p) {
      for (int column_p{}; column_p < 4; ++column_p) {
        if (!std::holds_alternative<DELETED>(shownTiles[row_p][column_p]))
            [[likely]] {
          shownTiles[row_p][column_p] = HIDDEN{};
        }
      }
    }
    shownTiles[row][column] = realTiles[row][column];
    tilesVisible = 1;
  } else if (tilesVisible == 1) {
    Tile otherVisible{HIDDEN{}};
    int otherRow{-1}, otherColumn{-1};
    for (int row_p{}; row_p < 3; ++row_p) {
      for (int column_p{}; column_p < 4; ++column_p) {
        if (!std::holds_alternative<HIDDEN>(shownTiles[row_p][column_p]) &&
            !std::holds_alternative<DELETED>(shownTiles[row_p][column_p]))
            [[unlikely]] {
          otherVisible = shownTiles[row_p][column_p];
          otherRow = row_p, otherColumn = column_p;
          break;
        }
      }
      if (!std::holds_alternative<HIDDEN>(otherVisible)) [[likely]] {
        break;
      }
    }
#ifdef LUMEN_STRICT_CHECKS
    if (otherRow == -1 || otherColumn == -1) {
#  ifdef LUMEN_LOG
      spdlog::error("Inconsistent model state.");
#  endif
      return;
    }
#endif
    if (otherVisible.index() == realTiles[row][column].index()) {
      shownTiles[row][column] = DELETED{};
      shownTiles[otherRow][otherColumn] = DELETED{};
      tilesVisible = 0;
      for (int row_p{}; row_p < 3; ++row_p) {
        for (int column_p{}; column_p < 4; ++column_p) {
          if (!std::holds_alternative<DELETED>(shownTiles[row_p][column_p])) {
            return;
          }
        }
      }
      gameWon = true;
    } else {
      shownTiles[row][column] = realTiles[row][column];
      tilesVisible++;
    }

  } else { // 0
    // show the tile
    shownTiles[row][column] = realTiles[row][column];
    tilesVisible++;
  }
}

} // namespace LumenCappuccino
