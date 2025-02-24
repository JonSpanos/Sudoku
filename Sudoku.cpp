// Copyright [2024] Jonathan Spanos

#include "Sudoku.hpp"
#include <algorithm>
#include <iostream>
#include <memory>

namespace SB {

Sudoku::Sudoku() {
  // Get font
  if (!_font.loadFromFile("fonts/arial.ttf")) {
    exit(1);
  }

  // Allocate space for board matrix
  _board = new int*[9];
  for (size_t i = 0; i < 9; i++) {
    _board[i] = new int[9];
  }

  // Fill board with numbers
  for (size_t i = 0; i < 9; i++) {
    for (size_t j = 0; j < 9; j++) {
      _board[i][j] = ((j+i)%9)+1;
    }
  }
}

void Sudoku::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  
  // Draw individual cells
  for (size_t i = 0; i < 9; i++) {
    for (size_t j = 0; j < 9; j++) {
      // Cell
      sf::RectangleShape shape;
      shape.setOutlineColor({0,0,0});
      shape.setOutlineThickness(.5f);
      shape.setFillColor({230,230,230});
      shape.setSize({64, 64});
      shape.setPosition(64*i,64*j);
      target.draw(shape);

      // Cells value
      sf::Text value;
      value.setFont(_font);
      value.setPosition(64*i+20, 64*j+10); // Hard-coded tweaking of number to be *close* to center.
      value.setFillColor({0,0,0});
      value.setString(std::to_string(_board[i][j]));
      target.draw(value);

    }
  }

  // Draw cell blocks (3x3)
  for (size_t i = 0; i < 3; i++) {
    for (size_t j = 0; j < 3; j++) {
      sf::RectangleShape shape;
      shape.setOutlineColor({0,0,0});
      shape.setOutlineThickness(2.0f);
      shape.setFillColor(sf::Color::Transparent);
      shape.setSize({64*3, 64*3});
      shape.setPosition(64*3*i,64*3*j);
      target.draw(shape);
    }
  }
}

Sudoku::~Sudoku() {
  for (size_t i = 0; i < 9; i++) {
    delete[] _board[i];
  }
  delete[] _board;
}
bool checkWallBox(sf::Vector2u, SB::Direction, char**, int, int);

void Sokoban::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  sf::Image img;
  if (!img.loadFromFile("sokoban_spritesheet.png")) {
    std::cout << "Failed to load spritesheet.\n";
    return;
  }
  sf::Texture txt;
  txt.loadFromImage(img);
  sf::Sprite player(txt, sf::IntRect(64 * 8, 0, 64, 64));

  if (_playerDir == Up) {
    player.setTextureRect(sf::IntRect(64 * 8, 64, 64, 64));
  } else if (_playerDir == Down) {
    player.setTextureRect(sf::IntRect(64 * 8, 0, 64, 64));
  } else if (_playerDir == Right) {
    player.setTextureRect(sf::IntRect(64 * 8, 64 * 2, 64, 64));
  } else if (_playerDir == Left) {
    player.setTextureRect(sf::IntRect(64 * 8, 64 * 3, 64, 64));
  }

  sf::Sprite wall(txt, sf::IntRect(64 * 6, 64 * 7, 64, 64));
  sf::Sprite block(txt, sf::IntRect(64 * 6, 64 * 2, 64, 64));
  sf::Sprite floor(txt, sf::IntRect(64, 64 * 2, 64, 64));
  sf::Sprite storage(txt, sf::IntRect(64, 64, 64, 64));

  for (size_t i = 0; i <= _height; i++) {
    for (size_t a = 0; a < _width; a++) {
      // Floors are always drawn so no need to check for '.'
      floor.setPosition(sf::Vector2f(a * 64, i * 64));
      target.draw(floor);
      switch (_matrix[a][i]) {
        case '#':
          wall.setPosition(sf::Vector2f(a * 64, i * 64));
          target.draw(wall);
          break;
        case 'a':
          storage.setPosition(sf::Vector2f(a * 64, i * 64));
          target.draw(storage);
          break;
        case '1':
          storage.setPosition(sf::Vector2f(a * 64, i * 64));
          target.draw(storage);
        case 'A':
          block.setPosition(sf::Vector2f(a * 64, i * 64));
          target.draw(block);
          break;
        case '_':  // Player on Storage
          storage.setPosition(sf::Vector2f(a * 64, i * 64));
          target.draw(storage);
        case '@':
          player.setPosition(sf::Vector2f(a * 64, i * 64));
          target.draw(player);
          break;
      }
    }
  }
}

bool Sokoban::isWon() {
  bool noCaptures = true;
  bool noBoxes = true;
  bool noPlayeronZone = true;
  for (size_t i = 0; i < _height; i++) {
    if (noCaptures) {
      noCaptures = std::none_of(_matrix, _matrix + _width, [=](char* a) {
        return a[i] == 'a' ? true : false;
      });  // True if any Capture Points remain
    }
    if (noBoxes) {
      noBoxes = std::none_of(_matrix, _matrix + _width, [=](char* a) {
        return a[i] == 'A' ? true : false;
      });  // True if any uncaptured boxes remain
    }
    if (noPlayeronZone) {
      noPlayeronZone = std::none_of(_matrix, _matrix + _width, [=](char* a) {
        return a[i] == '_' ? true : false;
      });
    }
  }

  // If capture points *and* boxes still remain, return false
  // If at least one is gone, you won the game.
  return (noCaptures && noPlayeronZone) || noBoxes;
}

void Sokoban::movePlayer(Direction dr) {
  if (checkWallBox(_playerPos, dr, _matrix, _width, _height) == false) return;
  _playerDir = dr;
  switch (dr) {
    case Up:
      moveHelper(sf::Vector2u(0, -1));
      break;
    case Down:
      moveHelper(sf::Vector2u(0, 1));
      break;
    case Left:
      moveHelper(sf::Vector2u(-1, 0));
      break;
    case Right:
      moveHelper(sf::Vector2u(1, 0));
      break;
  }
}

// Helper Functions

bool checkWallBox(sf::Vector2u playerPos, SB::Direction dr, char** matrix,
                  int width, int height) {
  switch (dr) {
    case Up:
      if (playerPos.y == 0) return false;  // edge
      if (matrix[playerPos.x][playerPos.y - 1] == '#') return false;
      if (matrix[playerPos.x][playerPos.y - 1] == 'A' && playerPos.y - 1 == 0)
        return false;
      if (matrix[playerPos.x][playerPos.y - 1] == 'A' &&
          (matrix[playerPos.x][playerPos.y - 2] != '.' &&
           matrix[playerPos.x][playerPos.y - 2] != 'a'))
        return false;
      break;
    case Down:
      if (playerPos.y == (unsigned)height - 1) return false;  // edge
      if (matrix[playerPos.x][playerPos.y + 1] == '#') return false;
      if (matrix[playerPos.x][playerPos.y + 1] == 'A' &&
          playerPos.y + 1 == (unsigned)height - 1)
        return false;
      if (matrix[playerPos.x][playerPos.y + 1] == 'A' &&
          (matrix[playerPos.x][playerPos.y + 2] != '.' &&
           matrix[playerPos.x][playerPos.y + 2] != 'a'))
        return false;
      break;
    case Left:
      if (playerPos.x == 0) return false;  // edge
      if (matrix[playerPos.x - 1][playerPos.y] == '#') return false;
      if (matrix[playerPos.x - 1][playerPos.y] == 'A' && playerPos.x - 1 == 0)
        return false;
      if (matrix[playerPos.x - 1][playerPos.y] == 'A' &&
          (matrix[playerPos.x - 2][playerPos.y] != '.' &&
           matrix[playerPos.x - 2][playerPos.y] != 'a'))
        return false;
      break;
    case Right:
      if (playerPos.x == (unsigned)width - 1) return false;  // edge
      if (matrix[playerPos.x + 1][playerPos.y] == '#') return false;
      if (matrix[playerPos.x + 1][playerPos.y] == 'A' &&
          playerPos.x + 1 == (unsigned)width - 1)
        return false;
      if (matrix[playerPos.x + 1][playerPos.y] == 'A' &&
          (matrix[playerPos.x + 2][playerPos.y] != '.' &&
           matrix[playerPos.x + 2][playerPos.y] != 'a'))
        return false;

      break;
  }
  return true;
}

void Sokoban::moveHelper(sf::Vector2u vc) {
  if (_matrix[_playerPos.x][_playerPos.y] == '_')  // If player on capture zone
    _matrix[_playerPos.x][_playerPos.y] = 'a';
  else
    _matrix[_playerPos.x][_playerPos.y] = '.';

  _playerPos += vc;
  // Pushing rules
  if (_matrix[_playerPos.x][_playerPos.y] == 'A' ||
      _matrix[_playerPos.x][_playerPos.y] ==
          '1') {  // If moved onto a box or zone with box
    if (_matrix[_playerPos.x + vc.x][_playerPos.y + vc.y] ==
        'a')  // If the spot the move will move is a capture zone
      _matrix[_playerPos.x + vc.x][_playerPos.y + vc.y] = '1';
    else
      _matrix[_playerPos.x + vc.x][_playerPos.y + vc.y] = 'A';
  }

  if (_matrix[_playerPos.x][_playerPos.y] == 'a' ||
      _matrix[_playerPos.x][_playerPos.y] ==
          '1')  // If player will stand on a capture zone or captured zone
    _matrix[_playerPos.x][_playerPos.y] = '_';  // Player on Zone
  else
    _matrix[_playerPos.x][_playerPos.y] = '@';  // Player off Zone
}

// Operators/Destructor

std::istream& operator>>(std::istream& in, Sokoban& sb) {
  sb._playerDir = Down;
  std::string str;
  in >> sb._height >> sb._width;
  in.ignore(1, '\n');
  sb._matrix = new char*[sb._width];
  for (size_t i = 0; i < sb._width; i++) {
    sb._matrix[i] = new char[sb._height];
  }
  size_t coldepth = 0;
  size_t rowdepth = 0;
  while (std::getline(in, str)) {
    rowdepth = 0;  // reset row count
    for (char ch : str) {
      sb._matrix[rowdepth][coldepth] = ch;
      if (ch == '@')  // Get player position and store it
        sb._playerPos = sf::Vector2u(rowdepth, coldepth);
      rowdepth++;
    }
    coldepth++;
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, Sokoban& sb) {
  for (size_t i = 0; i <= sb._height; i++) {
    for (size_t a = 0; a < sb._width; a++) {
      out << sb._matrix[a][i];
    }
    out << std::endl;
  }
  return out;
}

Sokoban::~Sokoban() {
  for (size_t i = 0; i < _width; i++) {
    delete[] _matrix[i];
  }
  delete[] _matrix;
}
}  // namespace SB
