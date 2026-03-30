#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

namespace Path {

	struct Vect {
		float x;
		float y;
	};

	// Pick out a random point to move to
	Vect randPoint(void);
}

#endif // PATHFINDING_HPP
