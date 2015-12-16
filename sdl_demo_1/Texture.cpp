#include "Texture.h"
#include <SDL\SDL_image.h>
#include <iostream>
#include <string>

namespace Bagnall
{
	// PUBLIC

	void Texture::Init()
	{
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

		GLuint tex = loadTexture("textures\\Shrek-and-Yoda.jpg");
		if (tex != 0)
			textureMap.emplace("shrek", tex);
		GLuint bump = loadTexture("bumpmaps\\Shrek-and-Yoda_NRM.jpg");
		if (bump != 0)
			BumpMapMap.emplace(tex, bump);

		tex = loadTexture("textures\\ben.jpg");
		if (tex != 0)
			textureMap.emplace("ben", tex);
		bump = loadTexture("bumpmaps\\ben_NRM.jpg");
		if (bump != 0)
			BumpMapMap.emplace(tex, bump);

		tex = loadTexture("textures\\stone_wall.jpg");
		if (tex != 0)
			textureMap.emplace("stone_wall", tex);
		bump = loadTexture("bumpmaps\\stone_wall_NRM.jpg");
		if (bump != 0)
			BumpMapMap.emplace(tex, bump);

		// release texture loading stuff
		IMG_Quit();
	}

	GLuint Texture::GetTextureByName(const char *name)
	{
		return textureMap[name];
	}

	GLuint Texture::GetBumpMapByTexture(GLuint tex)
	{
		if (BumpMapMap.find(tex) != BumpMapMap.end())
			return BumpMapMap[tex];
		else
			return 0;
	}

	// PRIVATE

	std::unordered_map<const char*, GLuint> Texture::textureMap;
	std::unordered_map<GLuint, GLuint> Texture::BumpMapMap;

	GLuint Texture::loadTexture(const char *path)
	{
		SDL_Surface *surface = IMG_Load(path);

		if (surface == NULL)
		{
			std::cerr << "unable to load file " << path << std::endl;
			return 0;
		}

		GLuint texture;

		glGenTextures(1, &texture);             // Generate a texture
		glBindTexture(GL_TEXTURE_2D, texture); // Bind that texture temporarily

		if (texture == 0)
		{
			std::cerr << "unable to get a free texture ID from opengl for file " << path << std::endl;
			return texture;
		}

		GLint mode = GL_RGB;
		if (surface->format->BytesPerPixel == 4)
			mode = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		// Generate mipmaps, by the way.
		glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind the texture
		glBindTexture(GL_TEXTURE_2D, NULL);

		// free the surface resource
		SDL_FreeSurface(surface);

		return texture;
	}
}
