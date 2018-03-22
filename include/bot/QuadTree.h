#ifndef BOT_QUADTREE_H_
#define BOT_QUADTREE_H_

#include <sc2api/sc2_api.h>
#include <vector>
using namespace sc2;

struct Square {
	Point3D* center;
	int sizeX;
	int sizeY;

	bool ContainsPoint(Point3D p) {
		return ((p.x <= center->x+sizeX/2 && p.x>= center->x-sizeX/2)&&(p.y >= center->y+sizeY/2 && p.y <= center->y-sizeY/2));
	}
	bool intersectSquare(Square other) {
		return ((((center->x + sizeX/2) > (other.center->x - other.sizeX/2))||((center->x - sizeX/2) < (other.center->x + other.sizeX/2)))&&(((center->y + sizeY/2) > (other.center->y - other.sizeY/2))||((center->y - sizeY/2)<(other.center->y + other.sizeY/2))));
	}
};

class QuadTree {
protected:
	const int quadtree_node_capacity = 4;
	Square* limit;
	std::vector<Point3D> points;

	QuadTree* north_west;
	QuadTree* north_east;
	QuadTree* south_west;
	QuadTree* south_east;

public:
	QuadTree(Square* limit);
	bool Insert(Point3D p);
	void Subdivide();
	std::vector<Point3D> QueryRange(Square range);
	std::vector<Point3D> appendArray(std::vector<Point3D> first, std::vector<Point3D> second);
};

#endif // BOT_QUADTREE_H_