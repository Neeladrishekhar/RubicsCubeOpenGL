#include "cube.hpp"
// #include "animater.hpp"
#include <vector>
#include <iostream>

class state
{
public:
	Cube cube_form;
	std::vector<moves_cube> m;

	state(Cube cube) {
		cube_form = cube;
	}

	state(Cube cube, std::vector<moves_cube> v) {
		// Cube temp_cube(cube);
		cube_form = cube;
		std::vector<moves_cube> temp_m( v );
		m = temp_m;
	}
};

int repeated = 0;

class solutionFinder
{
public:
	// state start_state(Cube);
	Cube start;

	std::vector<moves_cube> getTransitions( std::vector<Cube> visited, std::vector<state> fringe ) {
    
    std::cout << "number of moves in test: " << fringe[0].m.size() << std::endl;

		std::vector<state> new_fringe;
		for (int fr = 0; fr < fringe.size(); ++fr) {
			for (int i = 0; i < fringe[fr].cube_form.n; ++i) {
				for (int r = 0; r < 2; ++r) {	// possible types of rotation are two clockwise and anticlockwise
					for (int a = 0; a < 3; ++a) {	// possible axis of rotations are X, Y, Z hence 3
						Cube t_cube( fringe[fr].cube_form );
						t_cube.apply_transition(static_cast<Axis>(a), static_cast<Rotation>(r), i);
						bool exists = false;
						for (int c = 0; c < visited.size(); ++c) {
							if ( t_cube.isIdentical( visited[c] ) ) {exists = true; break;}
						}
						if ( !exists ) {
							moves_cube this_move(static_cast<Axis>(a), static_cast<Rotation>(r), i);
							state this_state( t_cube, fringe[fr].m );
							this_state.m.push_back(this_move);
							if ( t_cube.isSolved() ) {
								// do something for handling of getting a solution
								return this_state.m;
							} else {
								visited.push_back(t_cube);
						    // std::cout << "states now: " << visited.size() << std::endl;

								// futher required to maintain th efringe for next iteration
								new_fringe.push_back(this_state);
							}
						} else {
							repeated += 1;
					    // std::cout << "number of moves repeated: " << repeated << std::endl;
						}
					}
				}
			}
		}
		// If code reaches here then it means that this depth does not has a symmetric cube solution yet
		// We will need to call it again for deeper searches
		// This for maintaining the breadth first searh property
		if (new_fringe.size() == 0) {
			// This means we have exhausted all possible transitions and yet not found the solution
			// Hence the starting state must have been invalid
			std::vector<moves_cube> v_t;
			return v_t;// returning an empty vector in this case
		}
		return getTransitions( visited, new_fringe );
	}

	std::vector<moves_cube> solveIt() {
		std::vector<Cube> visited;
		std::vector<state> fringe;
		std::vector<moves_cube> bestMoves;

		state curr(start);
		repeated = 0;

		fringe.push_back(curr); visited.push_back(start);

		// call get transitions code here
		bestMoves = getTransitions( visited, fringe );

		return bestMoves;
	}

	solutionFinder(Cube some_cube) {
		// state s_now(some_cube);
		start = some_cube;
	}
};