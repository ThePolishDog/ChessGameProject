#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

const int BOARD_SIZE = 8;
const int SQUARE_SIZE = 60;
sf::Sprite f[32]; //figures
int board[8][8] = { -5,-4,-3,-2,-1,-3,-4,-5,
					-6,-6,-6,-6,-6,-6,-6,-6,
					 0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,
					 0,0,0,0,0,0,0,0,
					 6,6,6,6,6,6,6,6,
					 5,4,3,2,1,3,4,5 };

string toChessNote(sf::Vector2f p) {
	string s = "";
	s += char(p.x / SQUARE_SIZE + 97);
	s += char(7 - p.y / SQUARE_SIZE + 49);
	return s;
}

sf::Vector2f toCoord(char a, char b) {
	int x = int(a) - 97;
	int y = 7 - int(b) + 49;
	return sf::Vector2f(x * SQUARE_SIZE, y * SQUARE_SIZE);
}

void move(string str) {
	sf::Vector2f oldPos = toCoord(str[0], str[1]);
	sf::Vector2f newPos = toCoord(str[2], str[3]);

	for (int i = 0; i < 32; i++)
		if (f[i].getPosition() == newPos) f[i].setPosition(-100, -100);
	for (int i = 0; i < 32; i++)
		if (f[i].getPosition() == oldPos) f[i].setPosition(newPos);
}

string position = "";

void loadPosition() {
	int k = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			int n = board[i][j];
			if (!n) continue;
			int x = abs(n) - 1;
			int y = n > 0 ? 1 : 0;
			f[k].setTextureRect(sf::IntRect(SQUARE_SIZE * x, SQUARE_SIZE*y, SQUARE_SIZE, SQUARE_SIZE));
			f[k].setPosition(SQUARE_SIZE * j, SQUARE_SIZE * i);
			k++;
		}
	}
	for (int i = 0; i < position.length(); i += 5)
		move(position.substr(i, 4));
}

int main() {
	sf::RenderWindow window(sf::VideoMode(BOARD_SIZE * SQUARE_SIZE, BOARD_SIZE * SQUARE_SIZE), "Chess");
	sf::Texture figures, chessboard;
	if (!figures.loadFromFile("figures60r.png")) {
		cout << "Error, figures.png has not been found.\n";
	}
	if (!chessboard.loadFromFile("chessboard.png")) {
		cout << "Error, figures.png has not been found.\n";
	}
	sf::Sprite s(figures);
	sf::Sprite sBoard(chessboard);

	for (int i = 0; i < 32; i++) {
		f[i].setTexture(figures);
	}
	loadPosition();

	bool isMove = false;
	float dx = 0, dy = 0;
	sf::Vector2f oldPos, newPos;
	string str;
	int n = 0;

	while (window.isOpen()) {
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed)
				window.close();

			if(e.type==sf::Event::KeyPressed)
				if (e.key.code = sf::Keyboard::BackSpace)
				{
					position.erase(position.length() - 6, 5);
					loadPosition();
				}

			if(e.type==sf::Event::MouseButtonPressed)
				if(e.key.code==sf::Mouse::Left)
					for(int i=0; i<32; i++)
					if (f[i].getGlobalBounds().contains(pos.x, pos.y)) {
						isMove = true; n = i;
						dx = pos.x - f[i].getPosition().x;
						dy = pos.y - f[i].getPosition().y;
						oldPos = f[i].getPosition();
					}
			if (e.type == sf::Event::MouseButtonReleased)
				if (e.key.code == sf::Mouse::Left) {
					isMove = false;
					sf::Vector2f p = f[n].getPosition() + sf::Vector2f(SQUARE_SIZE / 2, SQUARE_SIZE / 2);
					sf::Vector2f newPos = sf::Vector2f(SQUARE_SIZE * int(p.x / SQUARE_SIZE), SQUARE_SIZE * int(p.y / SQUARE_SIZE));
					str = toChessNote(oldPos) + toChessNote(newPos);
					move(str); 
					position += str + "";
					cout << str << endl;
					f[n].setPosition(newPos);
				}
		}
		if (isMove) f[n].setPosition(pos.x - dx, pos.y - dy);
		window.clear();
		window.draw(sBoard);
		for (int i = 0; i < 32; i++) window.draw(f[i]);
		window.display();
	}
	return 0;
}