#include "DrawableObject.h"
#include "Bagnall.h"
#include "Shader.h"
#include "Material.h"
#include "RenderGraph.h"
#include "Game.h"
#include "Shadow.h"
#include "VertexMesh.h"

namespace Bagnall
{
	// PUBLIC

	DrawableObject::DrawableObject(Object *par) : Object(par)
	{
		SetTexture(0);
		emissive = false;
		SetMaterial(Material::None());
		init();
	}

	DrawableObject::DrawableObject(Object *par, GLuint tex, const Material& mat) : Object(par)
	{
		SetTexture(tex);
		SetMaterial(mat);
		blend = true;
		init();
	}

	DrawableObject::DrawableObject(Object *par, const vec4& emissionCol) : Object(par)
	{
		emissive = true;
		SetEmissionColor(emissionCol);
		init();
	}

	void DrawableObject::Draw() const
	{
		Shader::SetModel(finalModel);
		if (cubeMap != 0)
			Shader::SetReflectiveCubeMap(reflectiveCubeMap);
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it).Draw();
	};
	
	bool DrawableObject::GetEmissive() const
	{
		return emissive;
	}

	void DrawableObject::SetEmissive(bool e)
	{
		emissive = e;
		updateRenderNode();
	}

	vec4 DrawableObject::GetEmissionColor() const
	{
		return emissionColor;
	}

	bool DrawableObject::GetBlend() const
	{
		return blend;
	}

	void DrawableObject::SetBlend(bool b)
	{
		blend = b;
	}

	void DrawableObject::SetEmissionColor(const vec4& c)
	{
		emissionColor = c;
		updateRenderNode();
	}

	Material DrawableObject::GetMaterial() const
	{
		return material;
	}
	void DrawableObject::SetMaterial(const Material& m)
	{
		material = m;
		updateRenderNode();
	}

	GLuint DrawableObject::GetTexture() const
	{
		return texture;
	}

	void DrawableObject::SetTexture(GLuint tex)
	{
		texture = tex;
		updateRenderNode();
	}

	GLuint DrawableObject::GetCubeMap() const
	{
		return cubeMap;
	}

	void DrawableObject::SetCubeMap(GLuint cm)
	{
		cubeMap = cm;
		updateRenderNode();
	}

	void DrawableObject::UseMaterial(const Material& m)
	{
		/*glUniform4fv(Game::MaterialAmbientLoc, 1, m.ambient);
		glUniform4fv(Game::MaterialDiffuseLoc, 1, m.diffuse);
		glUniform4fv(Game::MaterialSpecularLoc, 1, m.specular);
		glUniform1f(Game::MaterialShininessLoc, m.shininess);*/
		Shader::SetMaterialAmbient(m.ambient);
		Shader::SetMaterialDiffuse(m.diffuse);
		Shader::SetMaterialSpecular(m.specular);
		Shader::SetMaterialShininess(m.shininess);
	}

	void DrawableObject::EnableRender()
	{
		renderEnabled = true;
		updateRenderNode();

		for (auto it = children.begin(); it != children.end(); ++it)
			static_cast<DrawableObject*>(*it)->EnableRender();
	}

	void DrawableObject::DisableRender()
	{
		renderEnabled = false;

		for (auto it = children.begin(); it != children.end(); ++it)
			static_cast<DrawableObject*>(*it)->DisableRender();
	}

	void DrawableObject::Cull()
	{
		if (renderNode != NULL)
			renderNode->objects.erase(std::find(renderNode->objects.begin(), renderNode->objects.end(), this));
		renderNode = NULL;

		Shadow::RemoveFromDepthRenderList(this);

		Object::Cull();
	}

	void DrawableObject::UnCull()
	{
		updateRenderNode();

		Shadow::AddToDepthRenderList(this);

		Object::UnCull();
	}

	bool DrawableObject::GetRenderEnabled() const
	{
		return renderEnabled;
	}

	bool DrawableObject::GetBumpMapEnabled() const
	{
		return bumpMapEnabled;
	}

	bool DrawableObject::GetReflectiveCubeMap() const
	{
		return reflectiveCubeMap;
	}

	void DrawableObject::SetReflectiveCubeMap(bool b)
	{
		reflectiveCubeMap = b;
	}

	// PRIVATE

	void DrawableObject::init()
	{
		bumpMapEnabled = true;
		renderEnabled = true;
		cubeMap = 0;
		reflectiveCubeMap = false;
		updateRenderNode();
		Shadow::AddToDepthRenderList(this);
	}

	void DrawableObject::updateRenderNode()
	{
		if (renderNode != NULL)
			renderNode->objects.erase(std::find(renderNode->objects.begin(), renderNode->objects.end(), this));
		renderNode = Game::MainRenderGraph->AddDrawableObject(this);
	}
}
