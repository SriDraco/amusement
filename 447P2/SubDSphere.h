/*
 * Sphere.h: Code for sphere generation
 *
 * (c) 2001 Stephen Chenney, University of Wisconsin at Madison
 */

#ifndef _SUBDSPHERE_H_
#define _SUBDSPHERE_H_

#include <stdio.h>
#include <math.h>

typedef struct _Vertex {
    float   	    x[3];
    } Vertex, *VertexPtr;

typedef struct _Edge {
    unsigned int    vs;
    unsigned int    ve;
    unsigned int    v_new;
    unsigned int    s_child;
    unsigned int    e_child;
    } Edge, *EdgePtr;

typedef struct _Triangle {
    unsigned int    edges[3];
    bool            forward[3];
    } Triangle, *TrianglePtr;


class SubDSphere {
private:
	unsigned int    num_vertices;
	Vertex  	    *vertices;
	unsigned int    num_edges;
	Edge    	    *edges;
	unsigned int    num_faces;
	Triangle	    *faces;

public:
	~SubDSphere(void);

	bool	Initialize(void);

	void    Subdivide(unsigned int);

	void    Draw(const bool smooth = true);
};

#endif
