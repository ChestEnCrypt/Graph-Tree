#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include <utility>
#include <iostream>
#include <unordered_map>

using namespace std;
using namespace sf;

struct Point {
private:
	Vertex& vertex;

public:

	Point(Vertex& _v) : vertex(_v) {}

	Vertex& getVertex() {
		return vertex;
	}

};

int main()
{
	RenderWindow window(VideoMode(800, 600), "Graph Tree");
	window.setFramerateLimit(15);

}
