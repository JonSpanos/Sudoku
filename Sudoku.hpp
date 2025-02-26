// Copyright [2024] Jonathan Spanos
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

namespace SB {
enum Direction { Up, Down, Left, Right };

class Sudoku : public sf::Drawable {
public:
  Sudoku();
  ~Sudoku();
  
  bool solveSelf(int** board, int row, int col) const;


protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
  sf::Font _font;
  int** _board;
 
  bool isSafe(int** board, int row, int col, int num) const; // Helper to check if a value is able to be placed.
};

class Sokoban : public sf::Drawable {
 public:
  Sokoban() {}
  ~Sokoban();
  const size_t width() { return _width; }
  const size_t height() { return _height; }
  void movePlayer(Direction dr);
  const sf::Vector2u playerLoc() { return _playerPos; }
  bool isWon();
  friend std::istream& operator>>(std::istream& in, Sokoban& sb);
  friend std::ostream& operator<<(std::ostream& out, Sokoban& sb);

 protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

 private:
  sf::Vector2u _playerPos;
  Direction _playerDir;
  size_t _width;
  size_t _height;
  char** _matrix;

  // Helper Functions
  void moveHelper(sf::Vector2u vc);
};
}  // namespace SB
