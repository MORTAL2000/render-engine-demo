#include "Shadow.h"
#include "Bagnall.h"
#include "DrawableObject.h"
#include "Shader.h"
#include "Game.h"
#include "RenderGraph.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

namespace Bagnall
{
	// PUBLIC

	void Shadow::Init()
	{
		// SET UP FRAME BUFFER OBJECT
		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glEnable(GL_DEPTH_TEST);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		// SET UP DEPTH BUFFER
		glGenTextures(1, &depthBuffer);
		glBindTexture(GL_TEXTURE_2D, depthBuffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "Shadow::Init() - frame buffer error after depth cube map initialiation\n";
		}

		// SET UP DEPTH CUBE MAP
		glGenTextures(1, &depthCubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, depthCubeMap, 0);

		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "Shadow::Init() - frame buffer error after depth cube map initialiation\n";
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Shadow::RenderShadowOrthoMap(const vec3& sourcePos, const vec3& destPos)
	{
		Shader::SetProgram("depth");

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);

		glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);

		//mat4 projection = ortho(-zRange.y / 2.0f, zRange.y / 2.0f, -zRange.y / 2.0f, zRange.y / 2.0f, -zRange.y, zRange.y);
		mat4 projection = ortho(-zRange.y, zRange.y, -zRange.y, zRange.y, zRange.x, zRange.y);
		//mat4 projection = perspective(static_cast<float>(M_PI) / 2.0f, 1.0f, zRange.x, zRange.y);
		Shader::SetProjection(projection);

		vec3 destinationPos;
		if (sourcePos != destPos)
			destinationPos = destPos;
		else
			destinationPos = destPos + vec3(0.1f, 0.1f, 0.0f);

		mat4 cam = lookAt(sourcePos, destPos, vec3(0.0f, 0.0f, 1.0f));

		Shader::SetLightProjection(projection * cam);

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		Shader::SetCamera(cam);
		renderDepthRenderList();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		Shader::SetCamera(Game::Camera);
	}

	void Shadow::RenderShadowCubeMap(const vec3& sourcePos, DrawableObject* source)
	{
		Shader::SetProgram("depth");

		if (source != NULL)
			source->DisableRender();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);

		glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);

		mat4 projection = perspective(static_cast<float>(M_PI) / 2.0f, 1.0f, zRange.x, zRange.y);
		Shader::SetProjection(projection);

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, depthCubeMap, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)));
		renderDepthRenderList();

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, depthCubeMap, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)));
		renderDepthRenderList();

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, depthCubeMap, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
		renderDepthRenderList();

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, depthCubeMap, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
		renderDepthRenderList();

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, depthCubeMap, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)));
		renderDepthRenderList();

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, depthCubeMap, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		Shader::SetCamera(lookAt(sourcePos, sourcePos + vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f)));
		renderDepthRenderList();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		Shader::SetCamera(Game::Camera);

		if (source != NULL)
			source->EnableRender();
	}

	void Shadow::BindToGPU(int shadowMode)
	{
		if (shadowMode == SHADOW_MODE_UNI)
		{
			glActiveTexture(GL_TEXTURE0 + TEXTURE_SHADOW_2D);
			glBindTexture(GL_TEXTURE_2D, depthBuffer);
		}
		else if (shadowMode == SHADOW_MODE_OMNI)
		{
			glActiveTexture(GL_TEXTURE0 + TEXTURE_SHADOW_CUBE);
			glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
		}
	}

	void Shadow::AddToDepthRenderList(VertexMesh *o)
	{
		auto it = std::find(depthRenderList.begin(), depthRenderList.end(), o);
		if (it == depthRenderList.end())
			depthRenderList.push_back(o);
	}

	void Shadow::RemoveFromDepthRenderList(VertexMesh *o)
	{
		auto it = std::find(depthRenderList.begin(), depthRenderList.end(), o);
		if (it != depthRenderList.end())
			depthRenderList.erase(it);
	}

	void Shadow::SetZRange(const vec2& zr)
	{
		zRange = zr;
	}

	// PRIVATE

	GLuint Shadow::frameBuffer;
	GLuint Shadow::depthBuffer;
	GLuint Shadow::depthCubeMap;
	vec2 Shadow::zRange = vec2(1.0f, 2.0f);

	std::vector<VertexMesh*> Shadow::depthRenderList;

	void Shadow::renderDepthRenderList()
	{
		for (auto it = depthRenderList.begin(); it != depthRenderList.end(); ++it)
		{
			auto o = *it;
			if (o->GetRenderEnabled())
				o->Draw();
		}
	}
}
