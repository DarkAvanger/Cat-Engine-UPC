#pragma once //To Do

#include <vector>
#include "MathGeoLib/src/MathGeoLib.h"

typedef unsigned char GLubyte;

namespace RCube
{
	void CreateCube(std::vector<float3>& vertices, std::vector<unsigned int>& indices, std::vector<float2>& texCoords)
	{

	}
}

namespace RPyramide
{
	void CreatePyramide(std::vector<float3>& vertices, std::vector<unsigned int>& indices, std::vector<float2>& texCoords)
	{
		
	}
}

namespace RSphere
{
	void CreateSphere(std::vector<float3>& vertices, std::vector<float3>& normals, std::vector<unsigned int>& indices, std::vector<float2>& texCoords)
	{

	}
}

namespace RCylinder
{
	void CreateCylinder(std::vector<float3>& vertices, std::vector<float3>& normals, std::vector<unsigned int>& indices, std::vector<float2>& texCoords)
	{
		
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