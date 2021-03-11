//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Mon Feb 22 2021
//
//
#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include <map>
#include <vector>
#include "core/geometry/Vec3.h"

// Forward Declarations
class DrBitmap;
class DrImage;
class DrPointF;
class DrVec2;
class DrVec3;
class Vertex;

// Type Definitions
typedef std::map<DrVec3, std::vector<Vertex>> NeighborMap;

// Defines
#define PAR_RGB  3
#define PAR_RGBA 4

// Constants
const int   c_vertex_length =   11;
const float c_extrude_depth =   0.1f;
const float c_cube_depth =      0.5f;

// Local Enums
enum Triangulation {
    TRIANGULATION_EAR_CLIPPING,
    TRIANGULATION_TRIANGULATE_OPT,
    TRIANGULATION_MONOTONE,
    TRIANGULATION_DELAUNAY,
};

enum Triangle_Point {
    TRIANGLE_POINT1,
    TRIANGLE_POINT2,
    TRIANGLE_POINT3,
};


//####################################################################################
//##    Vertex
//############################
struct Vertex {
    float px, py, pz;       // position
    float nx, ny, nz;       // normal
    float tx, ty;           // texture_coords
    float bx, by, bz;       // barycentric

    static      Vertex createVertex(DrVec3 pos, DrVec3 norm, DrVec3 uv, DrVec3 bary);
};

union Triangle {
	Vertex v[3];
	char data[sizeof(Vertex) * 3];
};


//####################################################################################
//##    DrMesh
//##        Stores a list of triangles for rendering
//############################
class DrMesh
{    
public:
    std::vector<unsigned int>   indices;
    std::vector<Vertex>         vertices;

    bool                        wireframe = false;
    float                       image_size = 0.0f;

public:    
    // Constructor
    DrMesh();

    // Properties
    int             indexCount() const      { return indices.size(); }
    int             vertexCount() const     { return vertices.size(); }

    // Creation Functions
    void    initializeExtrudedImage(DrImage* image, int quality);
    void    initializeTextureCone();
    void    initializeTextureCube();
    void    initializeTextureQuad();

    // Optimize Mesh
    void    optimizeMesh();
    void    smoothMesh();

    // Helper Functions
    static  std::vector<DrPointF>   insertPoints(const std::vector<DrPointF>& outline_points);
    static  std::vector<DrPointF>   smoothPoints(const std::vector<DrPointF>& outline_points, int neighbors, double neighbor_distance, double weight);


    // Extrusion Functions
    void    extrudeFacePolygon(const std::vector<DrPointF>& outline_points, int width, int height, int steps, bool reverse = false);
    void    triangulateFace(const std::vector<DrPointF>& outline_points, const std::vector<std::vector<DrPointF>>& hole_list,
                            const DrBitmap& image, bool wireframe, Triangulation type, double alpha_tolerance);

    // Assignment
    static  void set(Vertex &from_vertex, Vertex &to_vertex);    

    // Building Functions
    void    add(const DrVec3& vertex, const DrVec3& normal, const DrVec2& text_coord, Triangle_Point point_number);
    void    extrude(float x1, float y1, float tx1, float ty1,
                    float x2, float y2, float tx2, float ty2, int steps = 1);
    void    cube(float x1, float y1, float tx1, float ty1,
                 float x2, float y2, float tx2, float ty2,
                 float x3, float y3, float tx3, float ty3,
                 float x4, float y4, float tx4, float ty4, float depth);
    void    quad(float x1, float y1, float tx1, float ty1,
                 float x2, float y2, float tx2, float ty2,
                 float x3, float y3, float tx3, float ty3,
                 float x4, float y4, float tx4, float ty4);
    void    triangle(float x1, float y1, float tx1, float ty1,
                     float x2, float y2, float tx2, float ty2,
                     float x3, float y3, float tx3, float ty3);
};


#endif // ENGINE_MESH_H











