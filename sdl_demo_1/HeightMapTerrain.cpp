#include "HeightMapTerrain.h"
#include "VertexMesh.h"
#include "Shader.h"
#include <SDL\SDL_image.h>
#include <iostream>

namespace Bagnall
{
	// PUBLIC

	VertexMesh HeightMapTerrain::CreateTerrainMeshFromFile(const char *fileName, float scaleZ)
	{
		SDL_Surface *surface = IMG_Load(fileName);

		if (surface == NULL)
		{
			std::cerr << "unable to load file " << fileName << std::endl;
			return VertexMesh(nullptr, Material::None(), 0, 0, false);
		}

		unsigned char *pixels = (unsigned char*)surface->pixels;
		int bytesPerPixel = surface->format->BytesPerPixel;

		std::vector<vec4> vertices;
		for (int i = 0; i < surface->h; ++i)
			for (int j = 0; j < surface->w; ++j)
				vertices.push_back(vec4(i, j, pixels[(i * surface->w + j) * bytesPerPixel] * scaleZ, 1.0f));

		std::vector<vec4> normals;
		for (int i = 0; i < surface->h; ++i)
		{
			for (int j = 0; j < surface->w; ++j)
			{
				/*vec3 v1(-1.0f);
				if (i > 0)
					v1 = vec3(vertices[(i - 1) * surface->w + j]);

				vec3 v2(-1.0f);
				if (i > 0 && j < surface->w - 1)
					v2 = vec3(vertices[(i - 1) * surface->w + j + 1]);

				vec3 v3(-1.0f);
				if (j > 0)
					v3 = vec3(vertices[i * surface->w + j - 1]);

				vec3 v4 = vec3(vertices[i * surface->w + j]);

				vec3 v5(-1.0f); 
				if (j < surface->w - 1)
					v5 = vec3(vertices[i * surface->w + j + 1]);

				vec3 v6(-1.0f); 
				if (i < surface->h - 1 && j > 0)
					v6 = vec3(vertices[(i + 1) * surface->w + j - 1]);

				vec3 v7(-1.0f);
				if (i < surface->h - 1)
					v7 = vec3(vertices[(i + 1) * surface->w + j]);

				int n = 0;
				vec3 normalSum = vec3(0.0f, 0.0f, 0.0f);

				if (v1.x != -1.0f && v3.x != -1.0f && v4.x != -1.0f)
				{
					normalSum += normalize(cross(v1 - v4, v3 - v4));
					++n;
				}
				if (v1.x != -1.0f && v2.x != -1.0f && v4.x != -1.0f)
				{
					normalSum += normalize(cross(v4 - v1, v2 - v1));
					++n;
				}
				if (v2.x != -1.0f && v4.x != -1.0f && v5.x != -1.0f)
				{
					normalSum += normalize(cross(v2 - v5, v4 - v5));
					++n;
				}
				if (v3.x != -1.0f && v4.x != -1.0f && v6.x != -1.0f)
				{
					normalSum += normalize(cross(v6 - v3, v4 - v3));
					++n;
				}
				if (v4.x != -1.0f && v6.x != -1.0f && v7.x != -1.0f)
				{
					normalSum += normalize(cross(v4 - v7, v6 - v7));
					++n;
				}
				if (v4.x != -1.0f && v5.x != -1.0f && v7.x != -1.0f)
				{
					normalSum += normalize(cross(v7 - v4, v5 - v4));
					++n;
				}

				normals.push_back(vec4(normalSum / static_cast<float>(n), 0.0f));*/

				if (i == 0 || i == surface->h - 1 || j == 0 || j == surface->w - 1)
				{
					normals.push_back(vec4(0.0f, 0.0f, 1.0f, 0.0f));
					continue;
				}

				vec3 v1 = vec3(vertices[(i - 1) * surface->w + j]);
				vec3 v2 = vec3(vertices[(i - 1) * surface->w + j + 1]);
				vec3 v3 = vec3(vertices[i * surface->w + j - 1]);
				vec3 v4 = vec3(vertices[i * surface->w + j]);
				vec3 v5 = vec3(vertices[i * surface->w + j + 1]);
				vec3 v6 = vec3(vertices[(i + 1) * surface->w + j - 1]);
				vec3 v7 = vec3(vertices[(i + 1) * surface->w + j]);

				vec3 n1 = normalize(cross(v1 - v4, v3 - v4));
				vec3 n2 = normalize(cross(v4 - v1, v2 - v1));
				vec3 n3 = normalize(cross(v2 - v5, v4 - v5));
				vec3 n4 = normalize(cross(v6 - v3, v4 - v3));
				vec3 n5 = normalize(cross(v4 - v7, v6 - v7));
				vec3 n6 = normalize(cross(v7 - v4, v5 - v4));

				normals.push_back(normalize(vec4((n1 + n2 + n3 + n4 + n5 + n6) / 6.0f, 0.0f)));
			}
		}

		std::vector<vec4> tangents;
		std::vector<vec4> binormals;
		for (int i = 0; i < vertices.size(); ++i)
		{
			tangents.push_back(vec4(0.0f));
			binormals.push_back(vec4(0.0f));
		}

		std::vector<vec2> textureCoords;
		/*for (int i = 0; i < vertices.size(); ++i)
			textureCoords.push_back(vec2(0.0f));*/
		for (int i = 0; i < surface->h; ++i)
		{
			for (int j = 0; j < surface->w; ++j)
			{
				//textureCoords.push_back(vec2(i / static_cast<float>(surface->h), j / static_cast<float>(surface->w)));
				textureCoords.push_back(vec2(j / static_cast<float>(surface->w), i / static_cast<float>(surface->h)));
			}
		}

		int vertexOffset = Shader::Vertices.size();
		int indexOffset = Shader::VertexIndices.size();

		std::vector<uint> indices;
		for (int i = 0; i < surface->h - 1; ++i)
		{
			for (int j = 0; j < surface->w; ++j)
			{
				indices.push_back(vertexOffset + i * surface->w + j);
				indices.push_back(vertexOffset + (i + 1) * surface->w + j);
			}
			indices.push_back(UINT_MAX);
		}

		Shader::Vertices.insert(Shader::Vertices.end(), vertices.begin(), vertices.end());
		Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());
		Shader::Tangents.insert(Shader::Tangents.end(), tangents.begin(), tangents.end());
		Shader::Binormals.insert(Shader::Binormals.end(), binormals.begin(), binormals.end());
		Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), textureCoords.begin(), textureCoords.end());
		Shader::VertexIndices.insert(Shader::VertexIndices.end(), indices.begin(), indices.end());

		return VertexMesh(nullptr, Material::Plastic(vec4(0.5f, 0.5f, 0.5f, 1.0f)), indexOffset, indices.size(), true);

		// free the surface resource
		SDL_FreeSurface(surface);
	}

	// PRIVATE
}
