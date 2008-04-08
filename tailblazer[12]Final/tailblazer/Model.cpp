#include "Model.h"

Model::Model(void)
{
	verticesCount = 0;
	triangleCount = 0;
    normalCount = 0;
	texCount = 0;
}

Model::~Model(void)
{
}
int Model::loadModel(char *mFilename)
{
	FILE *f;
	char buffer[256];
	
	if((f = fopen(mFilename, "r"))==NULL) return 0;

	while(!feof(f))
	{
		memset(buffer,0,255);

		fgets(buffer, 256, f);

		if( strncmp("vn ",buffer,3) == 0 )
		{
			sscanf((buffer+2),"%f%f%f",
							&normals[normalCount].i,
							&normals[normalCount].j,
							&normals[normalCount].k);
			++normalCount;
		}
		else if(strncmp("vt ",buffer,3) == 0 )
		{
			sscanf((buffer+2),"%f%f",
							&texture[texCount].u,
							&texture[texCount].v);
			++texCount;
		}
		else if(strncmp("v ",buffer,2) == 0 )
		{
			sscanf((buffer+1),"%f%f%f",
							&vertices[verticesCount].x,
							&vertices[verticesCount].y,
							&vertices[verticesCount].z);
			++verticesCount;
		}
		else if(strncmp("f ",buffer,2) == 0 )
		{
			sscanf((buffer+1),"%d/%d/%d%d/%d/%d%d/%d/%d",
				&triangles[triangleCount].v[0],&triangles[triangleCount].t[0],&triangles[triangleCount].n[0],
				&triangles[triangleCount].v[1],&triangles[triangleCount].t[1],&triangles[triangleCount].n[1],
				&triangles[triangleCount].v[2],&triangles[triangleCount].t[2],&triangles[triangleCount].n[2]);
			++triangleCount;			
		}
		
	}
	fclose(f);
	return 1;
}
void Model::draw()
{
	glBegin(GL_TRIANGLES);
	for(int i=0; i<triangleCount; i++)
	{
		glNormal3f( normals[triangles[i].n[0]-1].i,
					normals[triangles[i].n[0]-1].j,
					normals[triangles[i].n[0]-1].k);
		glTexCoord2f( texture[triangles[i].t[0]-1].u,
					  texture[triangles[i].t[0]-1].v);
		glVertex3f( vertices[triangles[i].v[0]-1].x,
					vertices[triangles[i].v[0]-1].y,
					vertices[triangles[i].v[0]-1].z);

		glNormal3f( normals[triangles[i].n[1]-1].i,
					normals[triangles[i].n[1]-1].j,
					normals[triangles[i].n[1]-1].k);
		glTexCoord2f( texture[triangles[i].t[1]-1].u,
					  texture[triangles[i].t[1]-1].v);
		glVertex3f( vertices[triangles[i].v[1]-1].x,
					vertices[triangles[i].v[1]-1].y,
					vertices[triangles[i].v[1]-1].z);

		glNormal3f( normals[triangles[i].n[2]-1].i,
					normals[triangles[i].n[2]-1 ].j,
					normals[triangles[i].n[2]-1 ].k);
		glTexCoord2f( texture[triangles[i].t[2]-1 ].u,
					  texture[triangles[i].t[2]-1 ].v);
		glVertex3f( vertices[triangles[i].v[2]-1].x,
					vertices[triangles[i].v[2]-1].y,
					vertices[triangles[i].v[2]-1].z);
		
	}
	glEnd();
}

void Model::loadTexture(char* f)
{
	BITMAPINFOHEADER	bitmapInfoHeader;
	unsigned char		*bitmapData;	

	bitmapData = LoadBitmapFile(f, &bitmapInfoHeader);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth, 
				bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);

}

unsigned char* Model::LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;							
	BITMAPFILEHEADER	bitmapFileHeader;		
	unsigned char		*bitmapImage;		
	int					imageIdx = 0;	
	unsigned char		tempRGB;				

	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);


	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	fclose(filePtr);
	return bitmapImage;
}
