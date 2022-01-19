#pragma once

struct Tile {
	int x, y; 

	char state = ' '; 

	sf::RectangleShape shape;

	Tile(int x, int y) {
		this->x = x; 
		this->y = y; 

		shape.setSize({ 50, 50 }); 
		shape.setPosition({ (float)x, (float)y }); 
		shape.setFillColor(sf::Color::White); 

	}

	Tile() {}
};