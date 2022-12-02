#pragma once

#include <vector>
#include "MathGeoLib/src/MathGeoLib.h"
#include "glmath.h"

typedef unsigned char GLubyte;

namespace RCube
{
	void CreateCube(std::vector<float3>& vertices, std::vector<unsigned int>& indices, std::vector<float2>& texCoords)
	{
		vertices =
		{
			{-0.5f, 0.5f, 0.5f},
			{-0.5f, -0.5f, 0.5f},
			{0.5f, -0.5f, 0.5f},
			{0.5f, 0.5f, 0.5f},

			{0.5f, -0.5f, -0.5f},
			{0.5f, 0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},
			{-0.5f, 0.5f, -0.5f},
		};

		indices =
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

		texCoords =
		{
			{0.0f, 1.0f},
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},

			{1.0f, 1.0f},
			{1.0f, 0.0f},
			{0.0f, 1.0f},
			{0.0f, 0.0f},
		};
	}
}

namespace RPyramide
{
	void CreatePyramide(std::vector<float3>& vertices, std::vector<unsigned int>& indices, std::vector<float2>& texCoords)
	{
		vertices =
		{
			{-0.5f, -0.5f, 0.5f},
			{0.5f, -0.5f, 0.5f},
			{-0.5f, -0.5f, -0.5f},
			{0.5f, -0.5f, -0.5f},
			{0.0f, 0.5f, 0.0f},
		};

		indices =
		{
			1,0,2,

			2,3,1,

			0,1,4,

			1,3,4,

			3,2,4,

			2,0,4
		};

		texCoords =
		{
			{1.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{0.5f, 1.0f},
		};
	}
}

namespace RSphere
{
	void CreateSphere(std::vector<float3>& vertices, std::vector<float3>& normals, std::vector<unsigned int>& indices, std::vector<float2>& texCoords)
	{
		float x, y, z, xy;                             
		float nx, ny, nz, lengthInv = 1.0f / 1.0f;   
		float s, t;                                     

		float sectorStep = 2 * M_PI / 20;
		float stackStep = M_PI / 20;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= 20; ++i)
		{
			stackAngle = M_PI / 2 - i * stackStep;        
			xy = 1.0f * cosf(stackAngle);             
			z = 1.0f * sinf(stackAngle);             

			for (int j = 0; j <= 20; ++j)
			{
				sectorAngle = j * sectorStep;          

				x = xy * cosf(sectorAngle);             
				y = xy * sinf(sectorAngle);             
				vertices.emplace_back(x, y, z);


				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				normals.emplace_back(nx, ny, nz);

				s = (float)j / 20;
				t = (float)i / 20;
				texCoords.emplace_back(s, t);
			}
		}

		int k1, k2;
		for (int i = 0; i < 20; ++i)
		{
			k1 = i * (20 + 1);     
			k2 = k1 + 20 + 1;      

			for (int j = 0; j < 20; ++j, ++k1, ++k2)
			{
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				if (i != (20 - 1))
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}
	}
}

namespace RCylinder
{
	void CreateCylinder(std::vector<float3>& vertices, std::vector<float3>& normals, std::vector<unsigned int>& indices, std::vector<float2>& texCoords)
	{
		const float PI = 3.1415926f;
		float sectorStep = 2 * PI / 50;
		float sectorAngle;  

		std::vector<float3> unitVertices;
		for (int i = 0; i <= 50; ++i)
		{
			sectorAngle = i * sectorStep;
			unitVertices.emplace_back(cos(sectorAngle), sin(sectorAngle), 0); 
		}

		vertices.reserve(204);
		texCoords.reserve(204);
		normals.reserve(204);
		indices.reserve(600);

		for (int i = 0; i < 2; ++i)
		{
			float h = -2.0f / 2.0f + i * 2.0f;           
			float t = 1.0f - i;                             

			for (int j = 0, k = 0; j <= 50; ++j, ++k)
			{
				float ux = unitVertices[k].x;
				float uy = unitVertices[k].y;
				float uz = unitVertices[k].z;
				vertices.emplace_back(ux * 1.0f, uy * 1.0f, h);

				normals.emplace_back(ux, uy, uz);

				texCoords.emplace_back((float)j / 50, t);
			}
		}

		int baseCenterIndex = (int)vertices.size();
		int topCenterIndex = baseCenterIndex + 50 + 1; 


		for (int i = 0; i < 2; ++i)
		{
			float h = -2.0f / 2.0f + i * 2.0f;           
			float nz = -1 + i * 2;                          

			vertices.emplace_back(0, 0, h);
			normals.emplace_back(0, 0, nz);
			texCoords.emplace_back(0.5f, 0.5f);

			for (int j = 0, k = 0; j < 50; ++j, ++k)
			{
				float ux = unitVertices[k].x;
				float uy = unitVertices[k].y;

				vertices.emplace_back(ux * 1.0f, uy * 1.0f, h);


				normals.emplace_back(0, 0, nz);


				texCoords.emplace_back(-ux * 0.5f + 0.5f, -uy * 0.5f + 0.5f);
			}
		}

		int k1 = 0;                
		int k2 = 50 + 1;      


		for (int i = 0; i < 50; ++i, ++k1, ++k2)
		{

			indices.emplace_back(k1);
			indices.emplace_back(k1 + 1);
			indices.emplace_back(k2);

			indices.emplace_back(k2);
			indices.emplace_back(k1 + 1);
			indices.emplace_back(k2 + 1);
		}

		for (int i = 0, k = baseCenterIndex + 1; i < 50; ++i, ++k)
		{
			if (i < 50 - 1)
			{
				indices.emplace_back(baseCenterIndex);
				indices.emplace_back(k + 1);
				indices.emplace_back(k);
			}
			else 
			{
				indices.emplace_back(baseCenterIndex);
				indices.emplace_back(baseCenterIndex + 1);
				indices.emplace_back(k);
			}
		}

		for (int i = 0, k = topCenterIndex + 1; i < 50; ++i, ++k)
		{
			if (i < 50 - 1)
			{
				indices.emplace_back(topCenterIndex);
				indices.emplace_back(k);
				indices.emplace_back(k + 1);
			}
			else 
			{
				indices.emplace_back(topCenterIndex);
				indices.emplace_back(k);
				indices.emplace_back(topCenterIndex + 1);
			}
		}
	}
}

namespace Checker
{
	struct CheckerImage
	{
		int id;
		int width;
		int height;
		GLubyte* checkerImage;
	};

	CheckerImage CreateChecker()
	{
		CheckerImage checker = CheckerImage();
		int size = 64 * 64 * 4;
		checker.checkerImage = new GLubyte[size];
		for (int i = 0; i < size; i += 4)
		{
			GLubyte c = ((i & 0x8) == 0) * 255;
			checker.checkerImage[i] = (GLubyte)c;
			checker.checkerImage[i + 1] = (GLubyte)c;
			checker.checkerImage[i + 2] = (GLubyte)c;
			checker.checkerImage[i + 3] = (GLubyte)255;
		}

		checker.width = 64;
		checker.height = 64;
		checker.id = 0;

		return checker;
	}
}