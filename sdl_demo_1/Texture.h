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

	private:
		static std::unordered_map<const char*, GLuint> textureMap;
		static std::unordered_map<GLuint, GLuint> BumpMapMap;

		static GLuint loadTexture(const char *path);
	};
}

#endif
