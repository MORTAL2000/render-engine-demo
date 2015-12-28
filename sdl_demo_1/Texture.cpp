#include "Texture.h"
#include "SDLUtil.h"
#include <SDL\SDL_image.h>
#include <SDL2_gfx/SDL2_rotozoom.h>
#include <iostream>
#include <string>

namespace Bagnall
{
	// PUBLIC

	void Texture::Init()
	{
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

		// enable seamless cube maps
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		GLuint tex;
		GLuint bump;

		// SHREK
		LoadTexture("shrek", "textures\\Shrek-and-Yoda.jpg", "bumpmaps\\Shrek-and-Yoda_NRM.jpg");

		// BEN
		LoadTexture("ben", "textures\\ben.jpg", "bumpmaps\\ben_NRM.jpg");

		// STONE WALL
		LoadTexture("stone_wall", "textures\\stone_wall.jpg", "bumpmaps\\stone_wall_NRM.jpg");

		// COSTANZA
		LoadTexture("costanza", "textures\\costanza.jpg", "bumpmaps\\costanza_NRM.jpg");

		// DOWM FURRY
		LoadTexture("dowm_furry", "textures\\dowm furry.jpg", "bumpmaps\\dowm furry_NRM.jpg");

		// ISAAC FINAL FORM
		LoadTexture("isaac_final_form", "textures\\finalformsquare.jpg", "bumpmaps\\finalformsquare_NRM.jpg");

		// BILL
		LoadTexture("bill", "textures\\bill.png", "bumpmaps\\bill_NRM.jpg");

		// CUBEMAP TEST 1
		LoadCubeMap("cubemap_test_1", "textures\\cubemap_test_1.jpg", "bumpmaps\\cubemap_test_1_NRM.jpg");

		// skybox 1
		LoadTexture("skybox_2", "textures\\skybox2.jpg");
		LoadCubeMap("skybox_2", "textures\\skybox2.jpg");

		// sand1
		LoadTexture("sand1", "textures\\sand1.jpg", "bumpmaps\\sand1_NRM.jpg");

		// grass1
		LoadTexture("grass1", "textures\\grass1.jpg", "bumpmaps\\grass1_NRM.jpg");

		// stone1
		LoadTexture("stone1", "textures\\stone1.jpg", "bumpmaps\\stone1_NRM.jpg");

		// snow1
		LoadTexture("snow1", "textures\\snow1.jpg", "bumpmaps\\snow1_NRM.jpg");

		// release texture loading stuff
		IMG_Quit();
	}

	GLuint Texture::LoadTexture(const char *name, const char *filePath, const char *bumpPath)
	{
		GLuint tex = loadTexture(filePath);
		if (tex != 0)
		{
			textureMap.emplace(name, tex);
			GLuint bump = loadTexture(bumpPath);
			if (bump != 0)
				bumpMapMap.emplace(tex, bump);
		}
		return tex;
	}

	GLuint Texture::LoadCubeMap(const char *name, const char *filePath, const char *bumpPath)
	{
		GLuint cubeMap = loadCubeMap(filePath);
		if (cubeMap != 0)
		{
			cubeMapMap.emplace(name, cubeMap);
			GLuint bump = loadCubeMap(bumpPath);
			if (bump != 0)
				cubeBumpMapMap.emplace(cubeMap, bump);
		}
		return cubeMap;
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
		if (path == nullptr)
			return 0;

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
		if (path == nullptr)
			return 0;

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

		// LEFT
		rect.x = 0;
		rect.y = rect.h;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		//subSurfaceRotated = rotozoomSurface(subSurface, 90.0, 1.0, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

		// FRONT
		rect.x = rect.w;
		rect.y = rect.h;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

		// RIGHT
		rect.x = rect.w * 2;
		rect.y = rect.h;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		//subSurfaceRotated = rotozoomSurface(subSurface, 90.0, 1.0, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

		// BACK
		rect.x = rect.w * 3;
		rect.y = rect.h;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

		// TOP
		rect.x = rect.w;
		rect.y = 0;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

		// BOTTOM
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

	GLuint Texture::loadSkyboxCubeMap(const char *path)
	{
		if (path == nullptr)
			return 0;

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
		SDL_Surface *subSurface = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);
		SDL_Surface *subSurfaceMirrored = nullptr;

		// LEFT
		rect.x = 0;
		rect.y = rect.h;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		subSurfaceMirrored = SDLUtil::mirrorSurfaceX(subSurface);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurfaceMirrored->pixels);

		// FRONT
		rect.x = rect.w;
		rect.y = rect.h;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

		// RIGHT
		rect.x = rect.w * 2;
		rect.y = rect.h;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		//subSurfaceRotated = rotozoomSurface(subSurface, 90.0, 1.0, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

		// BACK
		rect.x = rect.w * 3;
		rect.y = rect.h;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

		// TOP
		rect.x = rect.w;
		rect.y = 0;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

		// BOTTOM
		rect.x = rect.w;
		rect.y = rect.h * 2;
		SDL_BlitSurface(surface, &rect, subSurface, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, rect.w, rect.h, 0, GL_BGRA, GL_UNSIGNED_BYTE, subSurface->pixels);

		// Unbind the texture
		glBindTexture(GL_TEXTURE_CUBE_MAP, NULL);

		// free the surface resources
		SDL_FreeSurface(surface);
		SDL_FreeSurface(subSurface);
		SDL_FreeSurface(subSurfaceMirrored);

		return cubeMap;
	}
}
