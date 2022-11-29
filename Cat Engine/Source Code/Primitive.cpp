#include "Primitive.h" 

#include "glew/include/GL/glew.h"
#include "glmath.h"
#include "Globals.h"

#include "Profiling.h"

PCube::PCube(float3 t, float3 r, float3 s) : Primitive()
{
	transform = t;
	rotate = r;
	scale = s;
	
	// Front
	indexVertex.push_back(-0.5f);
	indexVertex.push_back(0.5f);
	indexVertex.push_back(0.0f);

	indexVertex.push_back(-0.5f);
	indexVertex.push_back(-0.5f);
	indexVertex.push_back(0.0f);

	indexVertex.push_back(0.5f);
	indexVertex.push_back(-0.5f);
	indexVertex.push_back(0.f);

	indexVertex.push_back(0.5f);
	indexVertex.push_back(0.5f);
	indexVertex.push_back(0.0f);

	// Back 
	indexVertex.push_back(0.5f);
	indexVertex.push_back(-0.5f);
	indexVertex.push_back(-1.0f);

	indexVertex.push_back(0.5f);
	indexVertex.push_back(0.5f);
	indexVertex.push_back(-1.0f);

	indexVertex.push_back(-0.5f);
	indexVertex.push_back(-0.5f);
	indexVertex.push_back(-1.0f);

	indexVertex.push_back(-0.5f);
	indexVertex.push_back(0.5f);
	indexVertex.push_back(-1.0f);

	texCoords.push_back(float2(0.0f, 1.0f)); // 0 
	texCoords.push_back(float2(0.0f, 0.0f)); // 1
	texCoords.push_back(float2(1.0f, 0.0f)); // 2
	texCoords.push_back(float2(1.0f, 1.0f)); // 3

	texCoords.push_back(float2(0.0f, 1.0f)); // 5
	texCoords.push_back(float2(0.0f, 0.0f)); // 4
	texCoords.push_back(float2(1.0f, 0.0f)); // 6
	texCoords.push_back(float2(1.0f, 1.0f)); // 7

	GLuint indices[36] =
	{
		0,1,2,
		2,3,0,

		3,2,4,
		4,5,3,

		5,4,6,
		6,7,5,

		7,6,1,
		1,0,7,

		3,5,7,
		7,0,3, 

		1,6,4,
		4,2,1

	};

	CreateCheckerImage();

	glGenBuffers(1, &tbo);
	glGenTextures(1, &texId);

	vertex = new VertexBuffer(indexVertex.data(), indexVertex.size() * sizeof(GLfloat));
	index = new IndexBuffer(indices, 36);
	
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float2), texCoords.data(), GL_STATIC_DRAW);

	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);


	glBindTexture(GL_TEXTURE_2D, 0);
	index->Unbind();
	vertex->Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

PCube::~PCube()
{
	RELEASE(vertex);
	RELEASE(index);
}

void PCube::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	vertex->Bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindTexture(GL_TEXTURE_2D, texId);

	index->Bind();

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	index->Unbind();
	vertex->Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void PCube::CreateCheckerImage()
{
	for (int i = 0; i < 128; ++i)
	{
		for (int j = 0; j < 128; ++j)
		{
			GLubyte c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}
}

PPlane::PPlane(float3 t, float3 r, float3 s) : Primitive()
{
	transform = t;
	rotate = r;
	scale = s;
}

PPlane::~PPlane()
{
}

void PPlane::Draw()
{

}

PCylinder::PCylinder(int sectCount, float h, float rad) : sectorCount(sectCount), height(h), radius(rad)
{
	BuildVerticesSmooth();
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	vertex = new VertexBuffer(vertices.data(), vertices.size() * sizeof(GLfloat));
	index = new IndexBuffer(indices.data(), indices.size());

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
	glEnableVertexAttribArray(0);

}

PCylinder::~PCylinder()
{

}

std::vector<float> PCylinder::GetUnitCircleVertices()
{
    const float PI = 3.1415926f;
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // rad

    std::vector<float> unitCircleVertices;
    for (int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(sin(sectorAngle)); // y
        unitCircleVertices.push_back(0);                // z
    }

    return unitCircleVertices;
}

void PCylinder::Draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, index->GetSize(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void PCylinder::BuildVerticesSmooth()
{
    std::vector<GLfloat>().swap(vertices);
    std::vector<GLfloat>().swap(normals);
    std::vector<GLfloat>().swap(texCoords);

    std::vector<float> unitVertices = GetUnitCircleVertices();

    for (int i = 0; i < 2; ++i)
    {
        float h = -height / 2.0f + i * height;           
        float t = 1.0f - i;                              

        for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
        {
            float ux = unitVertices[k];
            float uy = unitVertices[k + 1];
            float uz = unitVertices[k + 2];
            // Position
            vertices.push_back(ux * radius);             
            vertices.push_back(uy * radius);             
            vertices.push_back(h);                       
            // Normal
            normals.push_back(ux);                       
            normals.push_back(uy);                      
            normals.push_back(uz);                      
            // Texture 
            texCoords.push_back((float)j / sectorCount); 
            texCoords.push_back(t);                      
        }
    }

    int baseCenterIndex = (int)vertices.size() / 3;
    int topCenterIndex = baseCenterIndex + sectorCount + 1; 

    for (int i = 0; i < 2; ++i)
    {
        float h = -height / 2.0f + i * height;           
        float nz = -1.0f + i * 2.0f;                           

        // center point
        vertices.push_back(0);     vertices.push_back(0);     vertices.push_back(h);
        normals.push_back(0);      normals.push_back(0);      normals.push_back(nz);
        texCoords.push_back(0.5f); texCoords.push_back(0.5f);

        for (int j = 0, k = 0; j < sectorCount; ++j, k += 3)
        {
            float ux = unitVertices[k];
            float uy = unitVertices[k + 1];
            // Position
            vertices.push_back(ux * radius);             
            vertices.push_back(uy * radius);         
            vertices.push_back(h);                     
            // Normal
            normals.push_back(0);                     
            normals.push_back(0);                      
            normals.push_back(nz);                      
            // Texture
            texCoords.push_back(-ux * 0.5f + 0.5f);     
            texCoords.push_back(-uy * 0.5f + 0.5f);     
        }
    }

    
    int k1 = 0;                         
    int k2 = sectorCount + 1;           

    for (int i = 0; i < sectorCount; ++i, ++k1, ++k2)
    {
        indices.push_back(k1);
        indices.push_back(k1 + 1);
        indices.push_back(k2);

        indices.push_back(k2);
        indices.push_back(k1 + 1);
        indices.push_back(k2 + 1);
    }

    for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
    {
        if (i < sectorCount - 1)
        {
            indices.push_back(baseCenterIndex);
            indices.push_back(k + 1);
            indices.push_back(k);
        }
        else 
        {
            indices.push_back(baseCenterIndex);
            indices.push_back(baseCenterIndex + 1);
            indices.push_back(k);
        }
    }

    for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
    {
        if (i < sectorCount - 1)
        {
            indices.push_back(topCenterIndex);
            indices.push_back(k);
            indices.push_back(k + 1);
        }
        else 
        {
            indices.push_back(topCenterIndex);
            indices.push_back(k);
            indices.push_back(topCenterIndex + 1);
        }
    }
}

PPyramid::PPyramid(float3 t, float3 r, float3 s) : Primitive()
{
	transform = t;
	rotate = r;
	scale = s;

	float vertices[15] =
	{
		transform.x, transform.y, transform.z,  
		transform.x + scale.x, transform.y, transform.z,  
		transform.x, transform.y, -(transform.z + scale.z), 
		transform.x + scale.x, transform.y, -(transform.z + scale.z),  
		transform.x + (scale.x / 2), transform.y + scale.y, -(transform.z + (scale.z / 2))  
	};

	unsigned int indices[18] =
	{
		1,0,2,

		2,3,1,

		0,1,4,

		1,3,4,

		3,2,4,

		2,0,4
	};


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	vertex = new VertexBuffer(vertices, 15 * sizeof(GLfloat));
	index = new IndexBuffer(indices, 18);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
	glEnableVertexAttribArray(0);
}

PPyramid::~PPyramid()
{

}

void PPyramid::Draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, index->GetSize(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

PSphere::PSphere(float radius, unsigned int rings, unsigned int sectors) : Primitive() 
{
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	unsigned int r, s;

	vertices.resize(rings * sectors * 3);
	normals.resize(rings * sectors * 3);
	texCoords.resize(rings * sectors * 3);

	std::vector<GLfloat>::iterator v = vertices.begin();
	std::vector<GLfloat>::iterator n = normals.begin();
	std::vector<GLfloat>::iterator t = texCoords.begin();
	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			float const y = sin(-M_PI_2 + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			*t++ = s * S;
			*t++ = r * R;

			*v++ = x * radius;
			*v++ = y * radius;
			*v++ = z * radius;

			*n++ = x;
			*n++ = y;
			*n++ = z;
		}
	}

	indices.resize(rings * sectors * 4);
	std::vector<GLushort>::iterator i = indices.begin();
	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			*i++ = r * sectors + s;
			*i++ = r * sectors + (s + 1);
			*i++ = (r + 1) * sectors + (s + 1);
			*i++ = (r + 1) * sectors + s;
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	vertex = new VertexBuffer(vertices.data(), vertices.size() * sizeof(GLfloat));
	index = new IndexBuffer(indices.data(), indices.size());

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
	glEnableVertexAttribArray(0);
}

PSphere::~PSphere()
{

}

void PSphere::Draw()
{

	glBindVertexArray(vao);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(0);
}

PGrid::PGrid(float w, float h) : width(w), height(h), Primitive()
{
	CreateGrid();
	transform = { 0.0f, 0.0f, 0.0f };
	vertex = new VertexBuffer(vertices.data(), sizeof(float3) * vertices.size());
}

PGrid::~PGrid()
{
	RELEASE(vertex);
}

void PGrid::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	vertex->Bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_LINES, 0, vertex->GetSize());

	DrawAxis();

	vertex->Unbind();
	glDisableClientState(GL_VERTEX_ARRAY);

}

void PGrid::CreateGrid()
{
	
	for (int j = 0; j < width; ++j)
	{
		float3 line = {};
		line = { (-width / 2) + (j), 0, (-width / 2)};
		vertices.push_back(line);

		line = { (-width / 2) + (j), 0, (width / 2) };
		vertices.push_back(line);
	}

	for (int i = 0; i < height; ++i)
	{
		float3 line = {};
		line = { (-width / 2), 0, (-width / 2) + (i) };
		vertices.push_back(line);

		line = { (width / 2), 0, (-width / 2) + (i) };
		vertices.push_back(line);
	}	
}

void Primitive::DrawAxis()
{
	glLineWidth(3.0f);

	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(transform.x, transform.y, transform.z);
	glVertex3f(transform.x + 1.0f, transform.y, transform.z);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(transform.x, transform.y, transform.z);
	glVertex3f(transform.x, transform.y + 1.0f, transform.z);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(transform.x, transform.y, transform.z);
	glVertex3f(transform.x, transform.y, transform.z + 1.0f);

	glColor3f(1.0f, 1.0f, 1.0f);


	glEnd();

	glLineWidth(1.0f);
}