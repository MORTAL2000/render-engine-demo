#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <unordered_map>

namespace Bagnall
{
	class Texture
	{
	public:
		static void Init();

		static GLuint GetTextureByName(const char *name);
		static GLuint GetBumpMapByTexture(GLuint tex);

		static GLuint GetCubeMapByName(const char *name);
		static GLuint GetBumpMapByCubeMap(GLuint cubeMap);

	private:
		static std::unordered_map<const char*, GLuint> textureMap;
		static std::unordered_map<GLuint, GLuint> bumpMapMap;

		static std::unordered_map<const char*, GLuint> cubeMapMap;
		static std::unordered_map<GLuint, GLuint> cubeBumpMapMap;

		static GLuint loadTexture(const char *path);

		static GLuint loadCubeMap(const char *path);
	};
}

#endif
