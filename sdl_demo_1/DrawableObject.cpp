#include "DrawableObject.h"
#include "Bagnall.h"
#include "Shader.h"
#include "Material.h"
#include "RenderGraph.h"
#include "Game.h"
#include "Shadow.h"
#include "VertexMesh.h"
#include "Schematic.h"
#include "Texture.h"

namespace Bagnall
{
	// PUBLIC

	DrawableObject::DrawableObject(Object *par) : Object(par)
	{
		//SetTexture(0);
		//emissive = false;
		//SetMaterial(Material::None());
		init();
	}

	//DrawableObject::DrawableObject(Object *par, GLuint tex, const Material& mat) : Object(par)
	//{
	//	//SetTexture(tex);
	//	//SetMaterial(mat);
	//	//blend = true;
	//	init();
	//}

	//DrawableObject::DrawableObject(Object *par, const vec4& emissionCol) : Object(par)
	//{
	//	//emissive = true;
	//	//SetEmissionColor(emissionCol);
	//	init();
	//}

	DrawableObject::DrawableObject(Object *par, SchematicNode *schematic) : DrawableObject(par)
	{
		for (auto it = schematic->vertexMeshes.begin(); it != schematic->vertexMeshes.end(); ++it)
		{
			vertexMeshes.push_back(*it);
			vertexMeshes[vertexMeshes.size() - 1].SetOwner(this);
		}

		for (auto it = schematic->children.begin(); it != schematic->children.end(); ++it)
			AddChild(new DrawableObject(this, *it));

		updateRenderNodes();
	}

	void DrawableObject::Draw() const
	{
		Shader::SetModel(finalModel);
		/*if (cubeMap != 0)
			Shader::SetReflectiveCubeMap(reflectiveCubeMap);*/
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it).Draw();
	};

	void DrawableObject::SendTransformToGPU()
	{
		Shader::SetModel(finalModel);
	}

	void DrawableObject::AddVertexMesh(const VertexMesh& vertexMesh)
	{
		vertexMeshes.push_back(vertexMesh);
	}
	
	//bool DrawableObject::GetEmissive() const
	//{
	//	return emissive;
	//}

	void DrawableObject::SetEmissive(bool e)
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it).SetEmissive(e);
		updateRenderNodes();
	}

	//vec4 DrawableObject::GetEmissionColor() const
	//{
	//	return emissionColor;
	//}

	//bool DrawableObject::GetBlend() const
	//{
	//	return blend;
	//}

	//void DrawableObject::SetBlend(bool b)
	//{
	//	blend = b;
	//}

	void DrawableObject::SetEmissionColor(const vec4& c)
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it).SetEmissionColor(c);
		updateRenderNodes();
	}

	//Material DrawableObject::GetMaterial() const
	//{
	//	return material;
	//}
	void DrawableObject::SetMaterial(const Material& m)
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it).SetMaterial(m);
		updateRenderNodes();
	}

	//GLuint DrawableObject::GetTexture() const
	//{
	//	return texture;
	//}

	void DrawableObject::SetTexture(GLuint tex)
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it).SetTexture(tex);
		updateRenderNodes();
	}

	//GLuint DrawableObject::GetCubeMap() const
	//{
	//	return cubeMap;
	//}

	//void DrawableObject::SetCubeMap(GLuint cm)
	//{
	//	cubeMap = cm;
	//	updateRenderNode();
	//}

	//void DrawableObject::UseMaterial(const Material& m)
	//{
	//	/*glUniform4fv(Game::MaterialAmbientLoc, 1, m.ambient);
	//	glUniform4fv(Game::MaterialDiffuseLoc, 1, m.diffuse);
	//	glUniform4fv(Game::MaterialSpecularLoc, 1, m.specular);
	//	glUniform1f(Game::MaterialShininessLoc, m.shininess);*/
	//	Shader::SetMaterialAmbient(m.ambient);
	//	Shader::SetMaterialDiffuse(m.diffuse);
	//	Shader::SetMaterialSpecular(m.specular);
	//	Shader::SetMaterialShininess(m.shininess);
	//}

	void DrawableObject::EnableRender()
	{
		renderEnabled = true;

		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it).EnableRender();

		for (auto it = children.begin(); it != children.end(); ++it)
			static_cast<DrawableObject*>(*it)->EnableRender();

		updateRenderNodes();
	}

	void DrawableObject::DisableRender()
	{
		renderEnabled = false;

		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it).DisableRender();

		for (auto it = children.begin(); it != children.end(); ++it)
			static_cast<DrawableObject*>(*it)->DisableRender();
	}

	void DrawableObject::Cull()
	{
		/*if (renderNode != NULL)
			renderNode->objects.erase(std::find(renderNode->objects.begin(), renderNode->objects.end(), this));
		renderNode = NULL;

		Shadow::RemoveFromDepthRenderList(this);*/

		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it).Cull();

		Object::Cull();
	}

	void DrawableObject::UnCull()
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it).UnCull();

		Object::UnCull();

		updateRenderNodes();
	}

	bool DrawableObject::GetRenderEnabled() const
	{
		return renderEnabled;
	}

	/*bool DrawableObject::GetBumpMapEnabled() const
	{
		return bumpMapEnabled;
	}*/

	void DrawableObject::SetBumpMapEnabled(bool b)
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it).SetBumpMapEnabled(b);
	}
	
	/*bool DrawableObject::GetReflectiveCubeMap() const
	{
		return reflectiveCubeMap;
	}

	void DrawableObject::SetReflectiveCubeMap(bool b)
	{
		reflectiveCubeMap = b;
	}*/

	// PRIVATE

	void DrawableObject::init()
	{
		//bumpMapEnabled = true;
		renderEnabled = true;
		//cubeMap = 0;
		//reflectiveCubeMap = false;
		updateRenderNodes();
		//Shadow::AddToDepthRenderList(this);
	}

	void DrawableObject::updateRenderNodes()
	{
		/*if (renderNode != NULL)
			renderNode->objects.erase(std::find(renderNode->objects.begin(), renderNode->objects.end(), this));
		renderNode = Game::MainRenderGraph->AddDrawableObject(this);*/
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it).UpdateRenderNode();
	}
}
