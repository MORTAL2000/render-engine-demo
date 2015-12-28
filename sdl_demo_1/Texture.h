#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <unordered_map>
#include <string>

namespace Bagnall
{
	class Texture
	{
	public:
		static void Init();

		static GLuint LoadTexture(const char *name, const char *filePath, const char *bumpPath = nullptr);
		static GLuint LoadCubeMap(const char *name, const char *filePath, const char *bumpPath = nullptr);

		static GLuint GetTextureByName(std::string name);
		static GLuint GetTextureByFile(std::string path);
		static GLuint GetBumpMapByTexture(GLuint tex);

		static GLuint GetCubeMapByName(std::string name);
		static GLuint GetCubeMapByFile(std::string path);
		static GLuint GetBumpMapByCubeMap(GLuint cubeMap);

	private:
		static std::unordered_map<std::string, GLuint> textureMap;
		static std::unordered_map<std::string, GLuint> textureFileMap;
		static std::unordered_map<GLuint, GLuint> bumpMapMap;

		static std::unordered_map<std::string, GLuint> cubeMapMap;
		static std::unordered_map<std::string, GLuint> cubeMapFileMap;
		static std::unordered_map<GLuint, GLuint> cubeBumpMapMap;

		static GLuint loadTexture(const char *path);
		static GLuint loadCubeMap(const char *path);
		static GLuint loadSkyboxCubeMap(const char *path);
	};
}

#endif
