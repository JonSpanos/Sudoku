// Copyright [2024] Jonathan Spanos

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <iostream>

#include "Sudoku.hpp"

int main(int argc, char* argv[]) {
  // Create Game Window
  sf::RenderWindow window(sf::VideoMode(64 * 9, 64 * 9),
                          "Sudoku");

  // Create Game
  SB::Sudoku sudoku;
  // Center window on computer screen
  auto desktop = sf::VideoMode::getDesktopMode();
  window.setPosition(sf::Vector2i(desktop.width/2 - window.getSize().x/2, desktop.height/2 - window.getSize().y/2));

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }
    window.clear();
    window.draw(sudoku);
    window.display();
  }
}