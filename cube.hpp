#include <cmath>
#include <GL/glut.h>
#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include "animater.hpp"

using namespace Eigen;

class Face
{
public:
	int n;

	std::vector<c_color> v;

	void rotate_clock() {
		std::vector<c_color> v_temp (v);
		for (int i = 0; i < n/2; ++i)
		{
			for (int j = 0; j < n-(2*i); ++j) { v_temp[(j*n)+i+(n*i)] = v[(n*i)+n-1-i-j]; }
			for (int j = 0; j < n-(2*i); ++j) { v_temp[(n*n)-n+j-(i*n)+i] = v[(j*n)+i+(n*i)]; }
			for (int j = 0; j < n-(2*i); ++j) { v_temp[(j*n)+n-1+(i*n)-i] = v[(n*n)-j-1-(i*n)-i]; }
			for (int j = 0; j < n-(2*i); ++j) { v_temp[i+(n*i)+j] = v[(j*n)+n-1+(i*n)-i]; }
		}
		v = v_temp;
	}

	void rotate_anticlock() {
		std::vector<c_color> v_temp (v);
		for (int i = 0; i < n/2; ++i)
		{
			for (int j = 0; j < n-(2*i); ++j) { v_temp[(n*i)+n-1-i-j] = v[(j*n)+i+(n*i)]; }
			for (int j = 0; j < n-(2*i); ++j) { v_temp[(j*n)+i+(n*i)] = v[(n*n)-n+j-(i*n)+i]; }
			for (int j = 0; j < n-(2*i); ++j) { v_temp[(n*n)-j-1-(i*n)-i] = v[(j*n)+n-1+(i*n)-i]; }
			for (int j = 0; j < n-(2*i); ++j) { v_temp[(j*n)+n-1+(i*n)-i] = v[i+(n*i)+j]; }
		}
		v = v_temp;
	}

	bool isIdentical(Face a_face) {
		bool ans = true;
		for (int face_pos = 0; face_pos < (n*n); ++face_pos) {
			if (v[face_pos]!=a_face.v[face_pos]) {ans = false; break;}
		}
		// Very poor quality as the rotated faces are not yet considered
		// but that cannot be done as we need this to measure overall cube identicality
		return ans;
	}

	bool isSameColored() {
		bool ans = true; c_color c = v[0];
		for (int face_pos = 1; face_pos < (n*n); ++face_pos) {
			if (v[face_pos]!=c) {ans = false; break;}
		}
		return ans;
	}

	// c_color getColor() { return v[0]; }
	
	Face() {
		n = 3;
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				v.push_back(red);
			}
		}
	}

	Face(int _n, c_color c) {
		n = _n;
		for (int i = 0; i < _n; ++i)
		{
			for (int j = 0; j < _n; ++j)
			{
				v.push_back(c);
			}
		}
	}

	Face(int _n, std::vector<c_color> vc) {
		n = _n;
		for (int i = 0; i < _n*_n; ++i)
		{
			v.push_back(vc[i]);
		}
	}

	// Face(Face &f) {
	// 	n = f.n;
	// 	for (int i = 0; i < (n*n); ++i) {
	// 		v.push_back( f.v[i] );
	// 	}
	// }
};

class Cube
{
public:
	float side;	// length of the side of the cube

	const static int num_faces = 6; // this defines the cube

	int n;	// by default its a 3 * 3 rubics cube
	
	std::vector<Face> faces;	// has the 6 faces of the cube

	bool isIdentical(Cube a_cube) {
		bool ans = true;
		// std::vector<Cube> cube_6;
		// for (int j = 0; j < 4; ++j) {
		// 	cube_6.push_back(a_cube);
		// 	for (int i = 0; i < n; ++i) { a_cube.apply_transition(Y, clockwise, i); }
		// }
		// for (int i = 0; i < n; ++i) { a_cube.apply_transition(Z, clockwise, i); }
		// cube_6.push_back(a_cube);
		// for (int i = 0; i < n; ++i) { a_cube.apply_transition(Z, clockwise, i); }
		// for (int i = 0; i < n; ++i) { a_cube.apply_transition(Z, clockwise, i); }
		// cube_6.push_back(a_cube);

		for (int face = 0; face < num_faces; ++face) {
			if (!faces[face].isIdentical(a_cube.faces[face])) {ans = false; break;}
		}
		// This is way too poor quality as the cube looked by different angles will still seem to be different
		// Need to code more here
		// think it as looking accross diagonal and 3 types for each diagonal = 3 * 8 = 24 combinations
		// or keep each face along +X and all 4 rotations with it = 6 * 4 = 24 combinations

		// I just now found that there are approx 43 quintillion states from a cube
		// Clearly a simple breadth first search is not going to work good
		// its only enough for 3 transitions from solution form

		return ans;
	}

	bool isSolved() {
		bool ans = true;
		for (int face = 0; face < num_faces; ++face) {
			if (!faces[face].isSameColored()) {ans = false; break;}
			// can also check for different color for every face but not feeling necessary
		}
		return ans;
	}

	void Transition(Axis a, Rotation r, int index) {
		// if (!inTransition && !inSolveMode) {
		if (!inTransition) {
			rot_axis = a; rot_type = r; rot_index = index; inTransition = true;
			apply_transition(a, r, index);
			glutTimerFunc(1000/framerate, Animator, 0);
			glutPostRedisplay();
		}
	}

	void manyTransitions() {
		// apply all transitions one after the other
		// No idea how to do
	}

	void apply_transition(Axis a, Rotation r, int i) {
		switch(a) {
			case X: if (r == clockwise) { rotate_clock_x(i); } else { rotate_anticlock_x(i); } break;
			case Y: if (r == clockwise) { rotate_clock_y(i); } else { rotate_anticlock_y(i); } break;
			case Z: if (r == clockwise) { rotate_clock_z(i); } else { rotate_anticlock_z(i); } break;
			default: break;
		}
	}

	bool affected(int face, int face_pos) {
		bool ans = false; int index = rot_index;
		switch(rot_axis) {
			case X: 
				if ( (face == 0 && index == 0) || (face == 3 && index == n-1) ) { ans = true; break; }// face will rotate completely
				for (int i = 0; i < n; ++i) { if( (face==5 && face_pos==(n*n)-n-(i*n)+index) || (face==4 && face_pos==i+(index*n)) || (face==2 && face_pos==n-1+(i*n)-index) || (face==1 && face_pos==i+(index*n)) ){ans=true;break;} }
				break;
			case Y: 
				if ( (face == 1 && index == 0) || (face == 4 && index == n-1) ) { ans = true; break; }// face will rotate completely
				for (int i = 0; i < n; ++i) { if( (face==3 && face_pos==(n*n)-n-(i*n)+index) || (face==2 && face_pos==i+(index*n)) || (face==0 && face_pos==n-1+(i*n)-index) || (face==5 && face_pos==i+(index*n)) ){ans=true;break;} }
				break;
			case Z: 
				if ( (face == 2 && index == 0) || (face == 5 && index == n-1) ) { ans = true; break; }// face will rotate completely
				for (int i = 0; i < n; ++i) { if( (face==4 && face_pos==(n*n)-n-(i*n)+index) || (face==0 && face_pos==i+(index*n)) || (face==1 && face_pos==n-1+(i*n)-index) || (face==3 && face_pos==i+(index*n)) ){ans=true;break;} }
				break;
			default: break;
		}
		return ans;
	}

	std::vector<Vector3f> get_new_directions(int face, Vector3f s, Vector3f j_cap, Vector3f i_cap) {
		std::vector<Vector3f> dir;
		Vector3f sn, jn, in; Affine3f ax; float neg = -1.0;
		if (rot_type == anticlockwise) { neg = 1.0; }
		float theta = (M_PI * neg * current_frame * 90.0 )/(180.0 * transition_frames); theta += -1*neg*M_PI/2;
		switch(rot_axis) { case X: ax = AngleAxisf(theta, Vector3f::UnitX()); break;	case Y: ax = AngleAxisf(theta, Vector3f::UnitY()); break; case Z: ax = AngleAxisf(theta, Vector3f::UnitZ()); break;	default: break;	}
		sn = ax * s; jn = ax * j_cap; in = ax * i_cap;		
		dir.push_back(sn); dir.push_back(jn); dir.push_back(in);
		// std::cout << "theta: " << theta << ", current_frame: " << current_frame << ", transition_frames: " << transition_frames << std::endl;
		// std::cout << "face: " << face << ", s: " << s.transpose() << std::endl;
		// std::cout << "face: " << face << ", s_after: " << sn.transpose() << std::endl;
		return dir;
	}

	void rotate_clock_x(int index) {
		if (index < n) {
			if (index == 0) {
				faces[0].rotate_anticlock();
			} else if (index == n-1) {
				faces[3].rotate_clock();
			}
			std::vector<c_color> s4(n), s2(n), s1(n), s5(n);
			for (int i = 0; i < n; ++i) { s4[i] = faces[5].v[(n*n)-n-(i*n)+index]; s2[i] = faces[4].v[i+(index*n)]; s1[i] = faces[2].v[n-1+(i*n)-index]; s5[i] = faces[1].v[i+(index*n)]; }
			for (int i = 0; i < n; ++i) { faces[5].v[(n*n)-n-(i*n)+index] = s5[i]; faces[4].v[i+(index*n)] = s4[i]; faces[2].v[n-1+(i*n)-index] = s2[i]; faces[1].v[i+(index*n)] = s1[i]; }
		}
		// animate and redraw
	}

	void rotate_clock_y(int index) {
		if (index < n) {
			if (index == 0) {
				faces[1].rotate_anticlock();
			} else if (index == n-1) {
				faces[4].rotate_clock();
			}
			std::vector<c_color> s0(n), s2(n), s3(n), s5(n);
			for (int i = 0; i < n; ++i) { s0[i] = faces[5].v[i+(n*index)]; s2[i] = faces[0].v[n-1+(i*n)-index]; s3[i] = faces[2].v[i+(n*index)]; s5[i] = faces[3].v[(n*n)-n-(i*n)+index]; }
			for (int i = 0; i < n; ++i) { faces[5].v[i+(n*index)] = s5[i]; faces[0].v[n-1+(i*n)-index] = s0[i]; faces[2].v[i+(n*index)] = s2[i]; faces[3].v[(n*n)-n-(i*n)+index] = s3[i]; }
		}
		// animate and redraw
	}

	void rotate_clock_z(int index) {
		if (index < n) {
			if (index == 0) {
				faces[2].rotate_anticlock();
			} else if (index == n-1) {
				faces[5].rotate_clock();
			}
			std::vector<c_color> s4(n), s3(n), s1(n), s0(n);
			for (int i = 0; i < n; ++i) { s4[i] = faces[0].v[i+(n*index)]; s3[i] = faces[4].v[(n*n)-n-(i*n)+index]; s1[i] = faces[3].v[i+(n*index)]; s0[i] = faces[1].v[n-1+(i*n)-index]; }
			for (int i = 0; i < n; ++i) { faces[0].v[i+(n*index)] = s0[i]; faces[4].v[(n*n)-n-(i*n)+index] = s4[i]; faces[3].v[i+(n*index)] = s3[i]; faces[1].v[n-1+(i*n)-index] = s1[i]; }
		}
		// animate and redraw
	}

	void rotate_anticlock_x(int index) {
		if (index < n) {
			if (index == 0) {
				faces[0].rotate_clock();
			} else if (index == n-1) {
				faces[3].rotate_anticlock();
			}
			std::vector<c_color> s4(n), s2(n), s1(n), s5(n);
			for (int i = 0; i < n; ++i) { s1[i] = faces[5].v[(n*n)-n-(i*n)+index]; s5[i] = faces[4].v[i+(index*n)]; s4[i] = faces[2].v[n-1+(i*n)-index]; s2[i] = faces[1].v[i+(index*n)]; }
			for (int i = 0; i < n; ++i) { faces[5].v[(n*n)-n-(i*n)+index] = s5[i]; faces[4].v[i+(index*n)] = s4[i]; faces[2].v[n-1+(i*n)-index] = s2[i]; faces[1].v[i+(index*n)] = s1[i]; }
		}
		// animate and redraw
	}

	void rotate_anticlock_y(int index) {
		if (index < n) {
			if (index == 0) {
				faces[1].rotate_clock();
			} else if (index == n-1) {
				faces[4].rotate_anticlock();
			}
			std::vector<c_color> s0(n), s2(n), s3(n), s5(n);
			for (int i = 0; i < n; ++i) { s3[i] = faces[5].v[i+(n*index)]; s5[i] = faces[0].v[n-1+(i*n)-index]; s0[i] = faces[2].v[i+(n*index)]; s2[i] = faces[3].v[(n*n)-n-(i*n)+index]; }
			for (int i = 0; i < n; ++i) { faces[5].v[i+(n*index)] = s5[i]; faces[0].v[n-1+(i*n)-index] = s0[i]; faces[2].v[i+(n*index)] = s2[i]; faces[3].v[(n*n)-n-(i*n)+index] = s3[i]; }
		}
		// animate and redraw
	}

	void rotate_anticlock_z(int index) {
		if (index < n) {
			if (index == 0) {
				faces[2].rotate_clock();
			} else if (index == n-1) {
				faces[5].rotate_anticlock();
			}
			std::vector<c_color> s4(n), s3(n), s1(n), s0(n);
			for (int i = 0; i < n; ++i) { s1[i] = faces[0].v[i+(n*index)]; s0[i] = faces[4].v[(n*n)-n-(i*n)+index]; s4[i] = faces[3].v[i+(n*index)]; s3[i] = faces[1].v[n-1+(i*n)-index]; }
			for (int i = 0; i < n; ++i) { faces[0].v[i+(n*index)] = s0[i]; faces[4].v[(n*n)-n-(i*n)+index] = s4[i]; faces[3].v[i+(n*index)] = s3[i]; faces[1].v[n-1+(i*n)-index] = s1[i]; }
		}
		// animate and redraw
	}

	std::vector<float> get_color(c_color c) { std::vector<float> color(3,0);
		switch(c) {
			case cyn: color[1]=1.0; color[2]=1.0; break;
			case mag: color[0]=1.0; color[2]=1.0; break;
			case yel: color[0]=1.0; color[1]=1.0; break;
			case red: color[0]=1.0; break;
			case gre: color[1]=1.0; break;
			case blu: color[2]=1.0; break;
			default: break;
		}
		return color;
	}

	void render() {
		// std::cout << "Render " << current_frame << std::endl;
		// float side = my_cube->side;
		int cubed = n;
		float strip = side * 0.02;
		// float len = (side - (2.0 * cubed * strip) ) / cubed;
		float len_act = 1.0 * side / cubed;

		Vector3f s = Vector3f((-0.5 * side),0,0);
		Vector3f j_cap = -1 * Vector3f::UnitY();   // such that s dot j = 0 and j is unit
		Vector3f i_cap = Vector3f::UnitZ();   // such that s cross j is along i and i is unit
		std::vector<float> color;

		for (int face = 0; face < num_faces; ++face) {
			switch(face) {
			  case 1: s = Vector3f(0,(-0.5 * side),0); j_cap = -1 * Vector3f::UnitZ(); i_cap = Vector3f::UnitX(); break;
			  case 2: s = Vector3f(0,0,(-0.5 * side)); j_cap = -1 * Vector3f::UnitX(); i_cap = Vector3f::UnitY(); break;
			  case 3: s = Vector3f((0.5 * side),0,0); j_cap = Vector3f::UnitY(); i_cap = Vector3f::UnitZ(); break;
			  case 4: s = Vector3f(0,(0.5 * side),0); j_cap = Vector3f::UnitZ(); i_cap = Vector3f::UnitX(); break;
			  case 5: s = Vector3f(0,0,(0.5 * side)); j_cap = Vector3f::UnitX(); i_cap = Vector3f::UnitY(); break;
			  default: break;
			}
			Vector3f p,p1,p2,p3; int face_pos = 0;
			for (int i = 0; i < cubed; ++i) {
				for (int j = 0; j < cubed; ++j) {
					Vector3f s_ori, j_cap_ori, i_cap_ori; bool effect = false;
					if (inTransition && affected(face, face_pos)) {
						s_ori = s; j_cap_ori = j_cap; i_cap_ori = i_cap; effect = true;
						// code to handle in case within a transition animation
						std::vector<Vector3f> new_dir = get_new_directions(face, s, j_cap, i_cap);
						s = new_dir[0]; j_cap = new_dir[1]; i_cap = new_dir[2];
						// std::cout << "face: " << face << ", s after : " << new_dir[0].transpose() << std::endl;

					}
					p = s + (j * len_act * j_cap) + (i * len_act * i_cap) - (0.5 * side * j_cap) - (0.5 * side * i_cap);
					p1 = p + (len_act * j_cap);
					p2 = p1 + (len_act * i_cap);
					p3 = p + (len_act * i_cap);
					// getting the strips around the partition
					glColor3f(0.2,0.2,0.2);
					glBegin(GL_POLYGON);
					glVertex3f(p[0],p[1],p[2]);glVertex3f(p1[0],p1[1],p1[2]);glVertex3f(p2[0],p2[1],p2[2]);glVertex3f(p3[0],p3[1],p3[2]);
					glEnd();

					Vector3f delta_s = s / s.norm();
					p = p + (strip * j_cap) + (strip * i_cap) + delta_s;
					p1 = p1 - (strip * j_cap) + (strip * i_cap) + delta_s;
					p2 = p2 - (strip * j_cap) - (strip * i_cap) + delta_s;
					p3 = p3 + (strip * j_cap) - (strip * i_cap) + delta_s;
					// the main color of this partition on the side
					color = get_color( faces[face].v[face_pos] ); face_pos++;
					glColor3f(color[0],color[1],color[2]);
					glBegin(GL_POLYGON);
					glVertex3f(p[0],p[1],p[2]);glVertex3f(p1[0],p1[1],p1[2]);glVertex3f(p2[0],p2[1],p2[2]);glVertex3f(p3[0],p3[1],p3[2]);
					glEnd();

					p = p - delta_s - delta_s; p1 = p1 - delta_s - delta_s; p2 = p2 - delta_s - delta_s; p3 = p3 - delta_s - delta_s; 
					glBegin(GL_POLYGON);
					glVertex3f(p[0],p[1],p[2]);glVertex3f(p1[0],p1[1],p1[2]);glVertex3f(p2[0],p2[1],p2[2]);glVertex3f(p3[0],p3[1],p3[2]);
					glEnd();

					if (effect) { s = s_ori; j_cap = j_cap_ori; i_cap = i_cap_ori; }
				}
			}
		}
	}

	Cube() {
		side = 200;
		n = 3;
		for (int i = 0; i < num_faces; ++i)
		{
			Face temp_face(n, static_cast<c_color>(i) );
			faces.push_back(temp_face);
		}
	}

	Cube(int _n, float _side) {
		side = _side;
		n = _n;
		for (int i = 0; i < num_faces; ++i)
		{
			Face temp_face(_n, static_cast<c_color>(i) );
			faces.push_back(temp_face);
		}
	}

	Cube(int _n, float _side, std::vector<Face> v_faces) {
		side = _side;
		n = _n;
		for (int i = 0; i < num_faces; ++i)
		{
			Face temp_face( v_faces[i].n, v_faces[i].v );
			faces.push_back(temp_face);
		}
	}

	// Cube(Cube cube) {
	// 	side = cube.side;
	// 	n = cube.n;
	// 	for (int i = 0; i < num_faces; ++i)
	// 	{
	// 		Face temp_face( cube.faces[i] );
	// 		faces.push_back(temp_face);
	// 	}
	// }
};