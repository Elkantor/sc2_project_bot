#pragma once
#include "QuadTree.h"

	QuadTree::QuadTree(Square* limit) {
		this->points.reserve(this->quadtree_node_capacity);
		this->limit = limit;
	}

	bool QuadTree::Insert(Point3D p) {
		if (!(this->limit->ContainsPoint(p))) {
			return false;
		}

		if (this->points.size() < this->quadtree_node_capacity) {
			this->points.push_back(p);
			return true;
		}

		if (this->north_west == NULL) {
			this->Subdivide();
		}
		if (this->north_west->Insert(p)) {
			return true;
		}
		if (this->north_east->Insert(p)) {
			return true;
		}
		if (this->south_west->Insert(p)) {
			return true;
		}
		if (this->south_east->Insert(p)) {
			return true;
		}
		return false;
	}

	void QuadTree::Subdivide() {
		Square* NW = new Square();
		Square* NE = new Square();
		Square* SW = new Square();
		Square* SE = new Square();
		
		NW->center = new Point3D((this->limit->center->x - (this->limit->sizeX / 2)), (this->limit->center->y - (this->limit->sizeY / 2)), (this->limit->center->z));
		NW->sizeX = ((this->limit->sizeX) / 2);
		NW->sizeY = ((this->limit->sizeY) / 2);
		this->north_west = new QuadTree(NW);

		NE->center = new Point3D((this->limit->center->x + (this->limit->sizeX / 2)), (this->limit->center->y - (this->limit->sizeY / 2)), (this->limit->center->z));
		NE->sizeX = ((this->limit->sizeX) / 2);
		NE->sizeY = ((this->limit->sizeY) / 2);
		this->north_east = new QuadTree(NE);

		SW->center = new Point3D((this->limit->center->x - (this->limit->sizeX / 2)), (this->limit->center->y + (this->limit->sizeY / 2)), (this->limit->center->z));
		SW->sizeX = ((this->limit->sizeX) / 2);
		SW->sizeY = ((this->limit->sizeY) / 2);
		this->south_west = new QuadTree(SW);

		SE->center = new Point3D((this->limit->center->x + (this->limit->sizeX / 2)), (this->limit->center->y + (this->limit->sizeY / 2)), (this->limit->center->z));
		SE->sizeX = ((this->limit->sizeX) / 2);
		SE->sizeY = ((this->limit->sizeY) / 2);
		this->south_east = new QuadTree(SE);
	}

	std::vector<Point3D> QuadTree::QueryRange(Square range) {
		std::vector<Point3D> pointsInRange;

		if (!this->limit->intersectSquare(range)) {
			return pointsInRange;
		}
		for (int p = 0; p < this->points.size();p++) {
			if (range.ContainsPoint(this->points[p])) {
				pointsInRange.push_back(this->points[p]);
			}
		}
		if(this->north_west == NULL){
			return pointsInRange;
		}

			pointsInRange = appendArray(pointsInRange, this->north_west->QueryRange(range));
			pointsInRange = appendArray(pointsInRange, this->north_east->QueryRange(range));
			pointsInRange = appendArray(pointsInRange, this->south_west->QueryRange(range));
			pointsInRange = appendArray(pointsInRange, this->south_east->QueryRange(range));

			return pointsInRange;
	}

	std::vector<Point3D> QuadTree::appendArray(std::vector<Point3D> first, std::vector<Point3D> second) {
		first.insert(first.end(), second.begin(), second.end());
		return first;
	}