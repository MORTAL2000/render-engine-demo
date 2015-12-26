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

		GLuint tex;
		GLuint bump;

		// SHREK
		tex = loadTexture("textures\\Shrek-and-Yoda.jpg");
		if (tex != 0)
		{
			textureMap.emplace("shrek", tex);
			bump = loadTexture("bumpmaps\\Shrek-and-Yoda_NRM.jpg");
			if (bump != 0)
				bumpMapMap.emplace(tex, bump);
		}

		// BEN
		tex = loadTexture("textures\\ben.jpg");
		if (tex != 0)
		{
			textureMap.emplace("ben", tex);
			bump = loadTexture("bumpmaps\\ben_NRM.jpg");
			if (bump != 0)
				bumpMapMap.emplace(tex, bump);
		}

		// STONE WALL
		tex = loadTexture("textures\\stone_wall.jpg");
		if (tex != 0)
		{
			textureMap.emplace("stone_wall", tex);
			bump = loadTexture("bumpmaps\\stone_wall_NRM.jpg");
			if (bump != 0)
				bumpMapMap.emplace(tex, bump);
		}

		// COSTANZA
		tex = loadTexture("textures\\costanza.jpg");
		if (tex != 0)
		{
			textureMap.emplace("costanza", tex);
			bump = loadTexture("bumpmaps\\costanza_NRM.jpg");
			if (bump != 0)
				bumpMapMap.emplace(tex, bump);
		}

		// DOWM FURRY
		tex = loadTexture("textures\\dowm furry.jpg");
		if (tex != 0)
		{
			textureMap.emplace("dowm_furry", tex);
			bump = loadTexture("bumpmaps\\dowm furry_NRM.jpg");
			if (bump != 0)
				bumpMapMap.emplace(tex, bump);
		}

		// ISAAC FINAL FORM
		tex = loadTexture("textures\\finalformsquare.jpg");
		if (tex != 0)
		{
			textureMap.emplace("isaac_final_form", tex);
			bump = loadTexture("bumpmaps\\finalformsquare_NRM.jpg");
			if (bump != 0)
				bumpMapMap.emplace(tex, bump);
		}

		// BILL
		tex = loadTexture("textures\\bill.png");
		if (tex != 0)
		{
			textureMap.emplace("bill", tex);
			bump = loadTexture("bumpmaps\\bill_NRM.png");
			if (bump != 0)
				bumpMapMap.emplace(tex, bump);
		}

		// CUBEMAP TEST 1
		GLuint cubeMap = loadCubeMap("textures\\cubemap_test_1.jpg");
		if (cubeMap != 0)
		{
			cubeMapMap.emplace("cubemap_test_1", cubeMap);
			bump = loadCubeMap("bumpmaps\\cubemap_test_1_NRM.jpg");
			if (bump != 0)
				cubeBumpMapMap.emplace(cubeMap, bump);
		}

		// MILLENNIUM FALCON TOP
		tex = loadTexture("textures\\Millennium_Falcon_Top_D.tga");
		if (tex != 0)
		{
			textureMap.emplace("millennium_falcon_top", tex);
			textureFileMap.emplace("Millennium_Falcon_Top_D.tga", tex);
			bump = loadTexture("bumpmaps\\Millennium_Falcon_Top_N.tga");
			if (bump != 0)
				bumpMapMap.emplace(tex, bump);
		}

		// MILLENNIUM FALCON BOTTOM
		tex = loadTexture("textures\\Millennium_Falcon_Bottom_D.tga");
		if (tex != 0)
		{
			textureMap.emplace("millennium_falcon_bottom", tex);
			textureFileMap.emplace("Millennium_Falcon_Bottom_D.tga", tex);
			bump = loadTexture("bumpmaps\\Millennium_Falcon_Bottom_N.tga");
			if (bump != 0)
				bumpMapMap.emplace(tex, bump);
		}

		// R2D2
		tex = loadTexture("textures\\R2-D2_D.tga");
		if (tex != 0)
		{
			textureMap.emplace("r2d2", tex);
			textureFileMap.emplace("R2-D2_D.tga", tex);
			bump = loadTexture("bumpmaps\\R2-D2_N.tga");
			if (bump != 0)
				bumpMapMap.emplace(tex, bump);
		}

		// C3PO
		tex = loadTexture("textures\\C-3PO_Dirty_D.tga");
		if (tex != 0)
		{
			textureMap.emplace("c3po", tex);
			textureFileMap.emplace("C-3PO_Dirty_D.tga", tex);
			bump = loadTexture("bumpmaps\\C-3PO_Dirty_N.tga");
			if (bump != 0)
				bumpMapMap.emplace(tex, bump);
		}

		// sand1
		tex = loadTexture("textures\\sand1.jpg");
		if (tex != 0)
		{
			textureMap.emplace("sand1", tex);
			textureFileMap.emplace("sand1.jpg", tex);
			bump = loadTexture("bumpmaps\\sand1_NRM.jpg");
			if (bump != 0)
				bumpMapMap.emplace(tex, bump);
		}

		// grass1
		tex = loadTexture("textures\\grass1.jpg");
		if (tex != 0)
		{
			textureMap.emplace("grass1", tex);
			textureFileMap.emplace("grass1.jpg", tex);
			bump = loadTexture("bumpmaps\\grass1_NRM.jpg");
			if (bump != 0)
				bumpMapMap.emplace(tex, bump);
		}

		// stone1
		tex = loadTexture("textures\\stone1.jpg");
		if (tex != 0)
		{
			textureMap.emplace("stone1", tex);
			textureFileMap.emplace("stone1.jpg", tex);
			bump = loadTexture("bumpmaps\\stone1_NRM.jpg");
			if (bump != 0)
				bumpMapMap.emplace(tex, bump);
		}

		// snow1
		tex = loadTexture("textures\\snow1.jpg");
		if (tex != 0)
		{
			textureMap.emplace("snow1", tex);
			textureFileMap.emplace("snow1.jpg", tex);
			bump = loadTexture("bumpmaps\\snow1_NRM.jpg");
			if (bump != 0)
				bumpMapMap.emplace(tex, bump);
		}

		// release texture loading stuff
		IMG_Quit();
	}

	GLuint Texture::GetTextureByName(std::string name)
	{
		return textureMap[name];
	}

	GLuint Texture::GetTextureByFile(std::string path)
	{
		return textureFileMap[path];
	}

	GLuint Texture::GetBumpMapByTexture(GLuint tex)
	{
		if (bumpMapMap.find(tex) != bumpMapMap.end())
			return bumpMapMap[tex];
		else
			return 0;
	}

	GLuint Texture::GetCubeMapByName(std::string name)
	{
		return cubeMapMap[name];
	}

	GLuint Texture::GetCubeMapByFile(std::string path)
	{
		return cubeMapFileMap[path];
	}

	GLuint Texture::GetBumpMapByCubeMap(GLuint cubeMap)
	{
		if (cubeBumpMapMap.find(cubeMap) != cubeBumpMapMap.end())
			return cubeBumpMapMap[cubeMap];
		else
			return 0;
	}

	// PRIVATE

	std::unordered_map<std::string, GLuint> Texture::textureMap;
	std::unordered_map<std::string, GLuint> Texture::textureFileMap;
	std::unordered_map<GLuint, GLuint> Texture::bumpMapMap;

	std::unordered_map<std::string, GLuint> Texture::cubeMapMap;
	std::unordered_map<std::string, GLuint> Texture::cubeMapFileMap;
	std::unordered_map<GLuint, GLuint> Texture::cubeBumpMapMap;

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

		GLint internalFormat = GL_RGB;
		if (surface->format->BytesPerPixel == 4)
			internalFormat = GL_RGBA;

		GLint format;
		if (strcmp(strrchr(path, '.'), ".tga"))
		{
			format = GL_RGB;
			if (surface->format->BytesPerPixel == 4)
				format = GL_RGBA;
		}
		else
		{
			format = GL_BGR;
			if (surface->format->BytesPerPixel == 4)
				format = GL_BGRA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		// Generate mipmaps, by the way.
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind the texture
		glBindTexture(GL_TEXTURE_2D, NULL);

		// free the surface resource
		SDL_FreeSurface(surface);

		return texture;
	}

	GLuint Texture::loadCubeMap(const char *path)
	{
		SDL_Surface *surface = IMG_Load(path);

		if (surface == NULL)
		{
			std::cerr << "unable to load file " << path << std::endl;
			return 0;
		}

		GLuint cubeMap;
		glGenTextures(1, &cubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		SDL_Rect rect;
		rect.w = surface->w / 4;
		rect.h = surface->h / 3;
		/*Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif
		SDL_Surface *subSurface = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, rmask, gmask, bmask, amask);*/
		SDL_Surface *subSurface = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);

		rect.x = 0;
		rect.y = rect.h;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

		rect.x = rect.w;
		rect.y = rect.h;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

		rect.x = rect.w * 2;
		rect.y = rect.h;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);
		
		rect.x = rect.w * 3;
		rect.y = rect.h;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

		rect.x = rect.w;
		rect.y = 0;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);
		
		rect.x = rect.w;
		rect.y = rect.h * 2;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

		// Unbind the texture
		glBindTexture(GL_TEXTURE_CUBE_MAP, NULL);

		// free the surface resources
		SDL_FreeSurface(surface);
		SDL_FreeSurface(subSurface);

		return cubeMap;
	}
}
