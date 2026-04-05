/**
 * @file	 Pathfinding.hpp
 * @author	 Yu Xuan
 * @brief	 For enemy pathfinding (was supposed to be more types)
 */

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
