#pragma once
#include <stdio.h>
#include <fstream>

class Vertex
{
public:

	//constructor

	Vertex(int x = 0, int y = 0, int z = 0);

	//each dimension of vertex

	int X;
	int Y;
	int Z;

	//setvertex

	void setVertex(int x, int y, int z);
};

class Face
{
public:
	
	//constructor

	Face();

	//intended constructor

	Face(Vertex* v1, Vertex* v2, Vertex* v3);

	//setFace

	void setFace(Vertex* v1, Vertex* v2, Vertex* v3);

	//3 vertexes make a face

	Vertex* v1;
	Vertex* v2;
	Vertex* v3;
};

class Mesh
{
private:

	short vertCount;
	short faceCount; //all faces are Tris

	Vertex* vertexArray; //array of vertexes
	Face* faceArray; //array of faces, each element of face will point to a vertex in the vert list

public:

	//constructor

	Mesh(short VC = NULL, short FC = NULL, Vertex* VA = NULL, Face* FA = NULL);

};

class MeshParser
{
private:

	FILE* tempFile;

public:

	//constructor



};