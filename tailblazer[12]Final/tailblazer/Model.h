#ifndef MODEL
#define MODEL
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <gl\glut.h>

#define BITMAP_ID 0x4D42
#define MAX_VERTICES 8000
#define MAX_POLYGONS 8000
#define MAX_NORMALS  8000 

struct Vertex{
	float x, y, z;
};
struct Texcoor{
	float u, v;
};
struct Normal{
	float i, j, k;
};
struct Triangle{
	int v[3], t[3], n[3];
};	

class Model
{
	public:
		Model();
		~Model(void);

		int loadModel(char *mFilename);
		void draw();

		void loadTexture(char* tFilename);
		unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);

	private:
		Triangle triangles[MAX_POLYGONS];
		Texcoor texture[MAX_VERTICES];
		Vertex vertices[MAX_VERTICES];
		Normal normals[MAX_NORMALS];

		unsigned char* bitmapData;
		GLuint textureId;

		int triangleCount; 
		int verticesCount;
		int normalCount;
		int texCount;
		
};
#endif