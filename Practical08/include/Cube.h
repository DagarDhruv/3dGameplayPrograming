#ifndef CUBE_H
#define CUBE_H

#include <GL/glew.h>

//Cube Vertices
/*
		  (-1.0f, 1.0f, -1.0f)          (1.0f, 1.0f, -1.0f)
		          [7]                          [6]
		          #-----------------------------#
		         /|                            /|
		        / |                           / |
	  (-1.0f, 1.0f, 1.0f)           (1.0f, 1.0f, 1.0f)
		  [3] /                         [2] /
		     #-----------------------------#    |
		     |    |                        |    |
		     |    |                        |    |
		     |   [4]                       |   [5]
		  (-1.0f, -1.0f, -1.0f)         (1.0f, -1.0f, -1.0f)
		     |    #-----------------------------#
		     |   /                         |   /
		     |  /                          |  /
		     | /                           | /
		     |/                            |/
		     #-----------------------------#
		    [0]                           [1]
	(-1.0f, -1.0f, 1.0f)         (1.0f, -1.0f, 1.0f)
*/

// Constants for the cube geometry
// Each surface must have a defined specification to enable 
// the application of a texel onto it.
// Updated counts based on your OBJ
const static unsigned int VERTICES = 26;
const static unsigned int INDICES = 36 * 3; // 36 faces converted to triangles (3 indices each)
const static unsigned int UVS = VERTICES * 2;

// Vertex positions (x,y,z)
GLfloat vertices[VERTICES * 3] = {
        // Original cube vertices (v 1-8 in OBJ)
        -1.000000f, -1.000000f,  1.000000f,  // 0
        -1.000000f,  1.000000f,  1.000000f,  // 1
        -1.000000f, -1.000000f, -1.000000f,  // 2
        -1.000000f,  1.000000f, -1.000000f,  // 3
        1.000000f, -1.000000f,  1.000000f,  // 4
        1.000000f,  1.000000f,  1.000000f,  // 5
        1.000000f, -1.000000f, -1.000000f,  // 6
        1.000000f,  1.000000f, -1.000000f,  // 7

        // Additional vertices (v 9-26 in OBJ)
        -1.011429f,  1.000000f,  1.002253f,  // 8
        -0.989275f,  1.000000f, -1.000000f,  // 9
        1.000000f,  1.000000f, -0.990743f,  // 10
        0.990362f,  1.000000f,  1.000000f,  // 11
        0.000000f,  2.074010f,  0.000000f,  // 12

        // Duplicate vertices with slight modifications (v 14-26)
        -1.000000f, -0.999897f,  1.000000f,  // 13
        -1.000000f,  1.000103f,  1.000000f,  // 14
        -1.000000f, -0.999897f, -1.000000f,  // 15
        -1.000000f,  1.000103f, -1.000000f,  // 16
        1.000000f, -0.999897f,  1.000000f,  // 17
        1.000000f,  1.000103f,  1.000000f,  // 18
        1.000000f, -0.999897f, -1.000000f,  // 19
        1.000000f,  1.000103f, -1.000000f,  // 20
        -1.011428f,  1.000103f,  1.002253f, // 21
        -0.989274f,  1.000103f, -1.000000f,  // 22
        1.000000f,  1.000103f, -0.990743f,  // 23
        0.990362f,  1.000103f,  1.000000f,  // 24
        0.000000f,  2.074113f, -0.000000f   // 25
};

// Colors (RGBA) - One per vertex
GLfloat colors[VERTICES * 4] = {
        1.0f, 0.0f, 0.0f, 1.0f,  // Red
        0.0f, 1.0f, 0.0f, 1.0f,  // Green
        0.0f, 0.0f, 1.0f, 1.0f,  // Blue
        1.0f, 1.0f, 0.0f, 1.0f,  // Yellow
        1.0f, 0.0f, 1.0f, 1.0f,  // Magenta
        0.0f, 1.0f, 1.0f, 1.0f,  // Cyan
        0.5f, 0.5f, 0.5f, 1.0f,  // Gray
        1.0f, 1.0f, 1.0f, 1.0f,  // White
        // Extended colors for additional vertices
        0.8f, 0.2f, 0.2f, 1.0f,
        0.2f, 0.8f, 0.2f, 1.0f,
        0.2f, 0.2f, 0.8f, 1.0f,
        0.8f, 0.8f, 0.2f, 1.0f,
        0.8f, 0.2f, 0.8f, 1.0f,
        // ... repeat or create new colors for all vertices
};

// UV coordinates (s,t) - From vt entries in OBJ
GLfloat uvs[UVS] = {
        0.838754f, 0.668613f,   // 0
        0.838754f, 0.711896f,   // 1
        0.882037f, 0.711896f,   // 2
        0.882037f, 0.690254f,   // 3
        0.882037f, 0.668613f,   // 4
        0.838754f, 0.755178f,   // 5
        0.882037f, 0.755178f,   // 6
        0.882037f, 0.733537f,   // 7
        0.838754f, 0.798461f,   // 8
        0.882037f, 0.798461f,   // 9
        0.882037f, 0.776819f,   // 10
        0.838754f, 0.841743f,   // 11
        0.882037f, 0.841743f,   // 12
        0.882037f, 0.820102f,   // 13
        0.795472f, 0.755178f,   // 14
        0.795472f, 0.798461f,   // 15
        0.903678f, 0.776819f,   // 16
        0.903678f, 0.798461f,   // 17
        0.925319f, 0.798461f,   // 18
        0.925319f, 0.776819f,   // 19
        0.903678f, 0.755178f,   // 20
        0.925319f, 0.755178f,   // 21
        0.000670f, 0.334814f,   // 22
        0.000670f, 0.002196f,   // 23
        0.000295f, 0.000295f,   // 24
        0.333288f, 0.002196f    // 25
};

// Triangulated indices (converted from OBJ quads to triangles)
GLuint indices[INDICES] = {
        // Face 1 (original quad: 1/1/1 3/2/1 4/3/1 9/4/1 2/5/1)
        0, 2, 3,    // Triangle 1
        0, 3, 8,    // Triangle 2
        0, 8, 1,    // Triangle 3

        // Face 2 (3/2/2 7/6/2 8/7/2 10/8/2 4/3/2)
        2, 6, 7,
        2, 7, 9,
        2, 9, 3,

        // Face 3 (7/6/3 5/9/3 6/10/3 11/11/3 8/7/3)
        6, 4, 5,
        6, 5, 10,
        6, 10, 7,

        // Face 4 (5/9/4 1/12/4 2/13/4 12/14/4 6/10/4)
        4, 0, 1,
        4, 1, 11,
        4, 11, 5,

        // Face 5 (3/15/5 1/16/5 5/9/5 7/6/5)
        2, 0, 4,
        2, 4, 6,

        // Face 6 (13/17/6 12/18/6 2/19/6 9/20/6)
        12, 11, 1,
        12, 1, 8,

        // ... Continue with all other faces converted to triangles
        // Note: Each quad becomes 2 triangles, pentagons become 3 triangles

        // Example for the remaining faces:
        // Face 7
        10, 5, 11,
        10, 11, 12,

        // Face 8
        7, 10, 12,
        7, 12, 9,

        // Face 9
        9, 12, 8,
        9, 8, 3,

        // ... Add all remaining faces following the same pattern
};

#endif

