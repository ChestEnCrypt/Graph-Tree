#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include <utility>
#include <iostream>
#include <unordered_map>

using namespace std;
using namespace sf;

unordered_map<int, Vertex> points;// = { {0, Vertex(Vector2f(50.f, 50.f))}, {1, Vertex(Vector2f(75.f, 100.f))} , {2, Vertex(Vector2f(25.f, 100.f))} };
int p_size = 0;
unordered_map<int, Vector3i> edges;// = { {0, Vector2i(0, 1)}, {1, Vector2i(0, 2)}, {2, Vector2i(2, 1)} };
int e_size = 0;
int normal_edge_length = 100;

int find_edge(Vector2i _edge) {
	for (auto it = edges.begin(); it != edges.end(); ++it) {
		int key = it->first;
		Vector3i edge = it->second;

		if ((edge.x == _edge.x && edge.y == _edge.y) ||
			(edge.x == _edge.y && edge.y == _edge.x)) {
			return key;
		}
	}
	return -1;
}

int update_points() {
	for (auto it = edges.begin(); it != edges.end(); ++it) {
		int key = it->first;
		Vector3i edge = it->second;

		Vector2f point1 = points[edge.x].position;
		Vector2f point2 = points[edge.y].position;

		Vector2f dfrn = point1 - point2;

		float length = sqrt(dfrn.x * dfrn.x + dfrn.y * dfrn.y);

		if (length > normal_edge_length) {
			Vector2f dir = dfrn;
			dir *= (normal_edge_length - length) / 5000.f;

			point1 += dir;
			point2 -= dir;

			points[edge.x].position = point1;
			points[edge.y].position = point2;
		}
		else if (length < normal_edge_length) {
			Vector2f dir = dfrn;
			dir *= (normal_edge_length - length) / 5000.f;

			point1 += dir;
			point2 -= dir;

			points[edge.x].position = point1;
			points[edge.y].position = point2;
		}
	}
	return 0;
}


void delete_edge(int edge) {
	edges.erase(edge);
	cout << "edges.size = " << edges.size() << endl;
}

void delete_point(int point) {
	for (auto it = edges.begin(); it != edges.end(); ) {
		int key = it->first;
		const Vector3i& edge = it->second;
		if (edge.x == point || edge.y == point) {
			it = edges.erase(it);
		}
		else {
			++it;
		}
	}
}

int main()
{
	RenderWindow window(VideoMode(1280, 720), "Graph Tree");
	//window.setFramerateLimit(60);

	Vector2i mps;
	bool mbl = false, mblc = true;
	bool mbr = false, mbrc = true;
	bool mbm = false, mbmc = true;
	bool dlt = false;
	bool spc = false;
	bool ctr = false;

	int mbm_select = -1;

	bool edge_creating = false;
	int vertex_nearby = -1;
	int vertex_selected = -1;

	while (window.isOpen()) {
		Event event = Event();
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			mps = Mouse::getPosition(window);

			mbl = Mouse::isButtonPressed(Mouse::Button::Left);
			mbr = Mouse::isButtonPressed(Mouse::Button::Right);
			mbm = Mouse::isButtonPressed(Mouse::Button::Middle);
			dlt = Keyboard::isKeyPressed(Keyboard::Delete);
			spc = Keyboard::isKeyPressed(Keyboard::Space);
			ctr = Keyboard::isKeyPressed(Keyboard::LControl);

		}

		update_points();
		if (ctr && dlt) {
			points.clear();
			edges.clear();
		}

		window.clear(Color(128, 128, 128, 255));

		vertex_nearby = -1;
		for (auto it = points.begin(); it != points.end(); ) {
			int key = it->first;
			Vertex& point = it->second;
			Vector2f pps = point.position;

			CircleShape circle;
			circle.setRadius(7);
			circle.setOrigin(4.5, 4.5);
			circle.setPosition(point.position);

			if (mps.x - 25 < pps.x && pps.x < mps.x + 25 &&
				mps.y - 25 < pps.y && pps.y < mps.y + 25) {

				if (dlt) {
					delete_point(key);
					it = points.erase(it);
					continue;
				}

				if (mbm) {
					mbm_select = key;
				}

				vertex_nearby = key;
				circle.setFillColor(Color::Red);
			}
			if (key == vertex_selected) {
				circle.setFillColor(Color::Green);
			}
			window.draw(circle);

			circle.setRadius(5);
			circle.setOrigin(2.5, 2.5);
			circle.setFillColor(Color::Black);
			circle.setPosition(point.position);

			window.draw(circle);

			++it;
		}

		for (auto it = edges.begin(); it != edges.end(); ) {
			int key = it->first;
			const Vector3i& edge = it->second;
			
			Vector2f a = points[edge.x].position;
			Vector2f b = points[edge.y].position;
			Vector2f m = Vector2f(mps);

			Vector2f ab = b - a;
			Vector2f am = m - a;

			float ab_len_sq = ab.x * ab.x + ab.y * ab.y;
			float t = (ab.x * am.x + ab.y * am.y) / ab_len_sq;

			t = max(0.f, min(1.f, t));

			Vector2f projection = a + t * ab;

			Vector2f dist = m - projection;

			float distance = sqrt(dist.x * dist.x + dist.y * dist.y);

			if (distance < 7.f) {
				if (dlt) {
					it = edges.erase(it);
					continue;
				}
				for (int i = -1; i < 2; i++) {
					for (int j = -1; j < 2; j++) {
						Vertex line[2];
						line[0] = points[edge.x].position + Vector2f(2.5 + i, 2.5 + j);
						if (edge.y == -1) {
							line[1] = Vertex(Vector2f(mps) + Vector2f(2.5 + i, 2.5 + j));
						}
						else {
							line[1] = points[edge.y].position + Vector2f(2.5 + i, 2.5 + j);
						}
						line[0].color = Color::Red;
						line[1].color = Color::Red;
						window.draw(line, 2, Lines);
					}
				}
			}
			
			Vertex line[2];
			line[0] = points[edge.x].position + Vector2f(2.5, 2.5);
			if (edge.y == -1) {
				line[1] = Vertex(Vector2f(mps) + Vector2f(2.5, 2.5));
			}
			else {
				line[1] = points[edge.y].position + Vector2f(2.5, 2.5);
			}

			window.draw(line, 2, Lines);
			++it;
		}
		if (vertex_selected != -1) {
			Vertex line[] = { points[vertex_selected].position + Vector2f(2.5, 2.5), Vertex(Vector2f(mps) + Vector2f(2.5, 2.5)) };
			window.draw(line, 2, Lines);
		}


		if (mbm_select != -1 && mbm) {
			points[mbm_select].position = Vector2f(mps);
		}
		else {
			mbm_select = -1;
		}

		if (mbl) {
			if (mblc && vertex_nearby == -1) {
				points[p_size++] = Vertex(Vector2f(mps));
				mblc = false;
			}
		}
		else {
			mblc = true;
		}
		if (mbr) {
			if (mbrc && vertex_nearby != -1) {
				if (!edge_creating) {
					vertex_selected = vertex_nearby;
					edge_creating = true;
				}
				else {
					int _find_edge = find_edge(Vector2i(vertex_selected, vertex_nearby));
					if (_find_edge != -1) {
						cout << ++edges[_find_edge].z << endl;
					}
					else {
					Vector3i& edge = edges[e_size++];
						edge = Vector3i(vertex_selected, vertex_nearby, 1);
					}
					vertex_nearby = -1;
					vertex_selected = -1;
					edge_creating = false;
				}
				mbrc = false;
			}
		}
		else {
			mbrc = true;
		}

		window.display();
	}
}
