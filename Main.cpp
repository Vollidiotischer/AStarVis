

/*

left mouse: set Wall
right mouse: delete Wall
S: set start
F: set finish
SPACE: start

*/






#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "aStar.h"
#include "TileDoc.h"

void init();
void loop(); 
void draw(); 
void events(); 
void setState(int, int, char); 
void chooseColor(Tile&); 
void aStarVisualizer(); 

const int delay = 100; // ms
const int WIDTH = 1105; 
const int HEIGHT = 1105; 


bool running = true;
bool aStarRan = false; 

std::vector<std::string> string_map (20, std::string(20, ' '));
std::vector<std::vector<Tile>> map(20, std::vector<Tile>(20)); 

// SFML
sf::RenderWindow* window; 

int main() {
	init(); 

	loop(); 

	return 0; 
}

void init() {

	window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "A Star"); 
	window->setKeyRepeatEnabled(false); 


	// fill map
	for (int x = 0; x < 20; x++) {
		for (int y = 0; y < 20; y++) {
			map[x][y] = Tile(x * 50 + 5 * (x+1), y * 50 + 5 * (y+1)); 
		}
	}
}

void loop() {


	while (running) {
		events(); 
		draw();

	}

}

void draw() {
	window->clear(sf::Color::Black); 

	for (int x = 0; x < map.size(); x++) {
		for (int y = 0; y < map[x].size(); y++) {
			window->draw(map[x][y].shape);
		}
	}

	window->display(); 
}

void events() {

	static bool M1Pressed = false; 
	static bool M2Pressed = false; 

	sf::Event events; 

	while (window->pollEvent(events)) {
		if (events.type == sf::Event::Closed) {
			running = false; 
			window->close(); 
		}

		if (events.type == sf::Event::MouseButtonReleased) {
			if (events.mouseButton.button == sf::Mouse::Left) {
				M1Pressed = false; 
			}			
			
			if (events.mouseButton.button == sf::Mouse::Right) {
				M2Pressed = false; 
			}
		}

		if (events.type == sf::Event::KeyPressed || events.type == sf::Event::MouseButtonPressed) {

			auto mPos = sf::Mouse::getPosition(*window);
			mPos.x /= 55;
			mPos.y /= 55;

			if (mPos.x >= 0 && mPos.y >= 0 && mPos.x < 20 && mPos.y < 20) {

				if (events.key.code == sf::Keyboard::S) {
					setState(mPos.x, mPos.y, 's');

				}
				if (events.key.code == sf::Keyboard::F) {
					setState(mPos.x, mPos.y, 'f');


				}
				if (events.key.code == sf::Keyboard::C) {
					for (int x = 0; x < map.size(); x++) {
						for (int y = 0; y < map[x].size(); y++) {
							setState(x, y, ' ');

							chooseColor(map[x][y]);
						}
					}

				}

				if (events.mouseButton.button == sf::Mouse::Left) {
					M1Pressed = true;

				}


				if (events.mouseButton.button == sf::Mouse::Right) {
					M2Pressed = true;

				}

				chooseColor(map[mPos.x][mPos.y]);

				if (events.key.code == sf::Keyboard::Space) {
					if (!aStarRan) {
						aStarVisualizer();
						// translate to string map
						// run a*
						// visualize a*
						aStarRan = true;
					}
					else {
						// clean up AStarMess
						for (int x = 0; x < map.size(); x++) {
							for (int y = 0; y < map[x].size(); y++) {
								chooseColor(map[x][y]);
							}
						}
						aStarRan = false;
					}


				}
			}
		
		}
	}




	if (M1Pressed) {
		auto mPos = sf::Mouse::getPosition(*window);
		mPos.x /= 55;
		mPos.y /= 55;
		if (mPos.x >= 0 && mPos.y >= 0 && mPos.x < 20 && mPos.y < 20) {
			setState(mPos.x, mPos.y, 'x');

			chooseColor(map[mPos.x][mPos.y]);
		}
	}

	if (M2Pressed) {
		auto mPos = sf::Mouse::getPosition(*window);
		mPos.x /= 55;
		mPos.y /= 55;
		if (mPos.x >= 0 && mPos.y >= 0 && mPos.x < 20 && mPos.y < 20) {
			setState(mPos.x, mPos.y, ' ');
			chooseColor(map[mPos.x][mPos.y]);
		}
	}
}

void setState(int x, int y, char state) {

	if (x >= 0 && y >= 0 && x < 20 && y < 20) {
		map[x][y].state = state;
	}

}

void aStarVisualizer() {
	// translate map
	for (int x = 0; x < map.size(); x++) {
		for (int y = 0; y < map[x].size(); y++) {
			string_map[x][y] = map[x][y].state; 
		}
		std::cout << string_map[x] << std::endl; 
	}

	a_star_search astar(string_map); 
	astar.search_path(); 

	for (int x = 0; x < astar.explored.size(); x++) {
		int px = astar.explored[x].x;
		int py = astar.explored[x].y;
		if (map[px][py].state == ' ') {
			map[px][py].shape.setFillColor(sf::Color::Red);
		}
	}

	for (int x = 0; x < astar.path.size(); x++) {
		int px = astar.path[x].x;
		int py = astar.path[x].y;
		if (map[px][py].state == ' ') {
			map[px][py].shape.setFillColor(sf::Color::Cyan);
		}
	}

	for (int x = 0; x < astar.open.size(); x++) {
		int px = astar.open[x].x;
		int py = astar.open[x].y;
		if (map[px][py].state == ' ') {
			map[px][py].shape.setFillColor(sf::Color::Magenta);
		}
	}

}

void chooseColor(Tile& t) {
	switch (t.state) {
		case ' ': {
			t.shape.setFillColor(sf::Color::White);
			break; 
		}
		case 'x': {
			t.shape.setFillColor(sf::Color::Black);
			break; 
		}
		case 's': {
			t.shape.setFillColor(sf::Color::Green);
			break; 
		}
		case 'f': {
			t.shape.setFillColor(sf::Color::Blue);
			break; 
		}
	}
}

/*

window: 
	- width: 1105 px
	- height: 1105 px
	-> size of tiles plus the margin between them 
	-> plus start 5 and end 5

tiles: 
	- nrX = 20
	- nrY = 20
	- w: 50 px
	- h: 50 px

spacing: 
	- thickness: 5 px


controls:
	- s: place start block on mouse cursor
	- f: place finish block on mouse cursor
	- M1: place wall block on mouse cursor
	- M2: place air block on mouse cursor (remove stuff)

*/