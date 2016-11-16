#ifndef VERTEX_H
#define VERTEX_H


typedef struct
{
    float x;
    float y;
    float z;
} Coord;


class Vertex
{
public:
    Vertex(float x, float y, float z, float u, float v);


    float u;
    float v;
};

#endif // VERTEX_H
