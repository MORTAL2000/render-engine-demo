#include "DrawableObject.h"
#include "Bagnall.h"
#include "Shader.h"
#include "Material.h"
#include "RenderGraph.h"
#include "Game.h"

namespace Bagnall
{
	// PUBLIC

	DrawableObject::DrawableObject(Object *par) : Object(par)
	{
		SetTexture(0);
		emissive = false;
		SetMaterial(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
		UpdateRenderNode();
	}

	DrawableObject::DrawableObject(Object *par, GLuint tex, const Material& mat) : Object(par)
	{
		SetTexture(tex);
		SetMaterial(mat);
		blend = true;
		UpdateRenderNode();
	}

	DrawableObject::DrawableObject(Object *par, const vec4& emissionCol) : Object(par)
	{
		emissive = true;
		SetEmissionColor(emissionCol);
		UpdateRenderNode();
	}

	void DrawableObject::Draw() const
	{
		auto I = finalInverseModel * finalModel;
		Shader::SetModel(finalModel);
		Shader::SetInverseModel(finalInverseModel);
		/*Shader::SetMaterialAmbient(material.ambient);
		Shader::SetMaterialDiffuse(material.diffuse);
		Shader::SetMaterialSpecular(material.specular);
		Shader::SetMaterialShininess(material.shininess);*/
	};

	//void Object::DrawRaw() const
	//{
	//	glUniformMatrix4fv(Game::ModelLoc, 1, GL_TRUE, finalModel);
	//}

	//void Object::StartDrawEmissive() const
	//{
	//	glUniformMatrix4fv(Game::ModelLoc, 1, GL_TRUE, finalModel);
	//	glUniform1i(Game::EmissiveLoc, 1);
	//	glUniform4fv(Game::EmissionColorLoc, 1, color);
	//};

	//void Object::EndDrawEmissive() const
	//{
	//	glUniform1i(Game::EmissiveLoc, 0);
	//}

	bool DrawableObject::GetEmissive() const
	{
		return emissive;
	}

	void DrawableObject::SetEmissive(bool e)
	{
		emissive = e;
		UpdateRenderNode();
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
		UpdateRenderNode();
	}

	Material DrawableObject::GetMaterial() const
	{
		return material;
	}
	void DrawableObject::SetMaterial(const Material& m)
	{
		material = m;
		UpdateRenderNode();
	}

	GLuint DrawableObject::GetTexture() const
	{
		return texture;
	}

	void DrawableObject::SetTexture(GLuint tex)
	{
		texture = tex;
		UpdateRenderNode();
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

	void DrawableObject::UpdateRenderNode()
	{
		if (renderNode != NULL)
			renderNode->objects.erase(std::find(renderNode->objects.begin(), renderNode->objects.end(), this));
		renderNode = Game::GameRenderGraph->AddDrawableObject(this);
	}
}
