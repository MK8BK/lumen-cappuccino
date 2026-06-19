#ifndef MODEL__HPP
#define MODEL__HPP
#include <array>
#include <chrono>
#include <optional>
#include <variant>

namespace LumenCappuccino {

class COFFEE_BEAN {};
class COCOA_BEAN {};
class CAPPUCCINO {};
class ESPRESSO {};
class MACHIATTO {};
class GREEN_TEA {};
class HIDDEN {};
class DELETED {};

using Tile = std::variant<COFFEE_BEAN, COCOA_BEAN, CAPPUCCINO, ESPRESSO,
                          MACHIATTO, GREEN_TEA, HIDDEN, DELETED>;
class Model {
private:
  std::array<std::array<Tile, 4>, 3> tiles;
  using Clock = std::chrono::system_clock;
  using TimePoint = Clock::time_point;
  std::optional<TimePoint> gameStartTime;

public:
  Model();
  bool setup();
  bool startGame();
  bool isGameStarted() const;
  bool isGameOver() const;
  bool isGameWon() const;
  bool isGameLost() const;
  std::optional<Tile> getTile(int row, int column) const;
  std::optional<Tile> uncoverTile(int row, int column) const;
};

} // namespace LumenCappuccino

#endif
