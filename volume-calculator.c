#include <stdio.h>
#include <string.h>
#include <assert.h>

#define X 0
#define Y 1
#define Z 2

#define EPSILON 0.0001

typedef double vector3d[3];

/**
 * Calculate the volume of the tetrahedron given as input. The
 * calculation is carried out in the way described in Code 4.16 of
 * O'Rourke's Computational Geometry in C (2nd ed.), that is, by
 * translating the tetrahedron so that the vertex d is placed at the
 * origin.
 *
 * tetrahedron is an array of vector3d, of size 4.
 */
double tetrahedron_volume(vector3d *tetrahedron) {
    
    double ax, ay, az, bx, by, bz, cx, cy, cz;
    double vol;
    
    ax = tetrahedron[0][X] - tetrahedron[3][X];
    ay = tetrahedron[0][Y] - tetrahedron[3][Y];
    az = tetrahedron[0][Z] - tetrahedron[3][Z];
    bx = tetrahedron[1][X] - tetrahedron[3][X];
    by = tetrahedron[1][Y] - tetrahedron[3][Y];
    bz = tetrahedron[1][Z] - tetrahedron[3][Z];
    cx = tetrahedron[2][X] - tetrahedron[3][X];
    cy = tetrahedron[2][Y] - tetrahedron[3][Y];
    cz = tetrahedron[2][Z] - tetrahedron[3][Z];

    vol = (ax * (by * cz - bz * cy)
           + ay * (bz * cx - bx * cz)
           + az * (bx * cy - by * cx)) / 6.0;

    return vol;        
}

/**
 * An implementation of polyhedral volume calculation following the
 * divergence theorem and O' Rourke's Computational Geometry in C (2nd ed.).
 * The function arguments are as in exercise 4.7.7:
 *
 * num_vertices is the number of vertices of the polyhedron
 * vectors is an array of 3D vectors of size num_vertices
 * num_trfaces is the number of triangle faces of the polyhedron.
 * vectors_indices is an array of vector indices, such that vector_indices[i],
 * vector_indices[i+1] and vector_indices[i+2] are the vectors composing
 * the triangular face i.
 *
 * It is assumed that the vertices are given in counter clockwise order.
 */
double calculate_volume(int num_vertices, vector3d *vectors, int num_trfaces,
                        int *vector_indices) {

    vector3d tetrahedron[4];
    double volume = 0.0;
    int i, j;
    
    for (i = 0; i < num_trfaces; i++) {
        memset(tetrahedron, 0, sizeof(tetrahedron[0]) * 4);
        for (j = 0; j < 3; j++) {
            int index = vector_indices[(i * 3) + j];
            memcpy(tetrahedron[j], vectors[index], sizeof(tetrahedron[0]));
        }
        memcpy(tetrahedron[3], vectors[0], sizeof(tetrahedron[0]));
        volume += tetrahedron_volume(tetrahedron);
    }
    return volume;
}

/**
 * Calculate the volume of a quadrilaterally-faced hexahedron. The
 * vertices of the hexahedron function are as in the following figure,
 * describing the case for cubes.
 *          
 *       4  .__________. 7
 *         /|      6  /|
 *    5  ./_|_______./ |
 *       |  |       |  |
 *       |  |0      |  |
 *       |  |_______|__|3
 *       | /        | /
 *    1  |/_________|/ 2
 *
 * vertices is an array of 3d vectors in the order shown above.
 */
double calculate_volume_qfhexahedron(vector3d *vertices) {
    
    int num_vertices = 8;
    int num_trfaces = 12;

    int vector_indices[] = {
        0, 2, 1, // base
        0, 3, 2, // base
        4, 5, 6, // top
        4, 6, 7, // top
        0, 5, 4, // left
        0, 1, 5, // left
        1, 6, 5, // front
        1, 2, 6, // front
        3, 6, 2, // right
        3, 7, 6, // right
        0, 7, 3, // back
        0, 4, 7 // back        
    };

    double volume =
        calculate_volume(num_vertices, vertices, num_trfaces, vector_indices);

    return volume;
}

/**
 * Calculate the volume of a prism. The vertices of the prism are as
 * in the following figure. It is not necessary for the top and the bottom to
 * be parallel to each other.
 * 
 *        3 .
 *         /|\
 *     4 ./_|_\. 5
 *       |  |  |  
 *       |  |0 |
 *       |  |  |
 *       | / \ |
 *    1  |/___\| 2
 *
 * vertices is an array of 3d vectors in the order shown above
 */
double calculate_volume_prism(vector3d *vertices) {
    
    int num_vertices = 6;

    int num_trfaces = 8;
    int vector_indices[] = {
        0, 1, 2, // base
        3, 4, 5, // top
        0, 4, 3, // left
        0, 1, 4, // left
        1, 5, 4, // front
        1, 2, 5, // front
        0, 3, 5, // right
        0, 5, 2, // right
    };

    double volume =
        calculate_volume(num_vertices, vertices, num_trfaces, vector_indices);
    return volume;
    
}

void test_pyramid() {
    
    int num_vertices = 4;
    vector3d vertices[] = {
        {0, 0, 0},
        {1, 0, 0},
        {0, 1, 0},
        {1, 1, 1}
    };
    int num_trfaces = 4;
    int vector_indices[] = {
        1, 0, 2,
        1, 3, 0,
        2, 3, 0,
        1, 2, 3
    };

    double volume =
        calculate_volume(num_vertices, vertices, num_trfaces, vector_indices);
    assert(volume == 1/6.0);
    printf("%lf\n", volume);

}

/**
 * Test the volume of a cube. The vertices of the cube used for testing
 * in the function are as in the following figure.
 *          
 *       4  .__________. 7
 *         /|      6  /|
 *    5  ./_|_______./ |
 *       |  |       |  |
 *       |  |0      |  |
 *       |  |_______|__|3
 *       | /        | /
 *    1  |/_________|/ 2
 */
void test_cube() {
    
    vector3d vertices[] = {
        {0, 0, 0}, // 0
        {2, 0, 0}, // 1
        {2, 2, 0}, // 2
        {0, 2, 0}, // 3
        {0, 0, 2}, // 4
        {2, 0, 2}, // 5
        {2, 2, 2}, // 6
        {0, 2, 2} // 7
    };

    double volume = calculate_volume_qfhexahedron(vertices);
    assert(volume - 8.0 < EPSILON);
    printf("%lf\n", volume);
}

/**
 * Test the volume of a parallelepiped. The vertices of the
 * parallelepiped used for testing in the function are as in
 * test_cube().
 */
void test_parallelepiped() {

    vector3d vertices[] = {
        {0, 0, 0}, // 0
        {4, 0, 0}, // 1
        {4, 2, 0}, // 2
        {0, 2, 0}, // 3
        {0, 0, 2}, // 4
        {4, 0, 2}, // 5
        {4, 2, 2}, // 6
        {0, 2, 2}  // 7
    };

    double volume = calculate_volume_qfhexahedron(vertices);
    assert(volume - 16.0 < EPSILON);
    printf("%lf\n", volume);
}

/**
 * Test the volume of a slanted parallelepiped. The vertices of the
 * parallelepiped used for testing in the function are as in
 * test_cube().
 */
void test_slanted_parallelepiped() {

    vector3d vertices[] = {
        {0, 0, 0}, // 0
        {4, 0, 0}, // 1
        {4, 2, 0}, // 2
        {0, 2, 0}, // 3
        {0, 1, 2}, // 4
        {4, 1, 2}, // 5
        {4, 3, 2}, // 6
        {0, 3, 2}  // 7
    };

    double volume = calculate_volume_qfhexahedron(vertices);
    assert(volume - 16.0 < EPSILON);
    printf("%lf\n", volume);
}


/**
 * Test the volume of a prism. The vertices of the prism used for testing
 * in the function are as in the following figure.
 * 
 *        3 .
 *         /|\
 *     4 ./_|_\. 5
 *       |  |  |  
 *       |  |0 |
 *       |  |  |
 *       | / \ |
 *    1  |/___\| 2
 */
void test_prism() {

    vector3d vertices[] = {
        {0, 0, 0}, // 0
        {4, 0, 0}, // 1
        {0, 4, 0}, // 2
        {0, 0, 6}, // 3
        {4, 0, 6}, // 4
        {0, 4, 6}, // 5
    };

    double volume =
        calculate_volume_prism(vertices);
    assert(volume -48.0 < EPSILON);
    printf("%lf\n", volume);
}

/**
 * Test the volume of a 3d trapezium. The trapezium is defined as a
 * parallepiped with a wedge on top.
 */
void test_3d_trapezium() {

    vector3d vertices[] = {
        {0, 0, 0}, // 0
        {4, 0, 0}, // 1
        {4, 2, 0}, // 2
        {0, 2, 0}, // 3
        {0, 0, 2}, // 4
        {4, 0, 2}, // 5
        {4, 2, 4}, // 6
        {0, 2, 4}  // 7
    };

    double volume =
        calculate_volume_qfhexahedron(vertices);
    assert(volume - 24.0 < EPSILON);
    printf("%lf\n", volume);
}

int main() {
    test_pyramid();
    test_cube();
    test_parallelepiped();
    test_slanted_parallelepiped();
    test_prism();
    test_3d_trapezium();
    return 0;
}
