#include "Object.h"
#include "Material.h"
#include "Shader.h"
#include "Game.h"
#include "Util.h"
#include <limits.h>

namespace Bagnall
{
	// PUBLIC

	Object::Object()
	{
		position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		oldPosition = position;
		centerOfRotation = position;
		useCenterOfRotation = false;
		//color = vec4(0.0, 0.0, 0.0, 1.0);
		theta = vec3();
		scale = vec3(1.0, 1.0, 1.0);
		SetMaterial(Material::Plastic(vec4(1.0, 1.0, 1.0, 1.0)));
		SetParentModel(mat4(1.0));
		changedFlags = INT_MAX;
		ignoreParentModelFlags = 0;
	}

	Object::~Object()
	{
		/*for (std::vector<Object*>::iterator it = children.begin(); it != children.end(); ++it)
		delete *it;*/
		for (int i = 0; i < children.size(); ++i)
			delete children[i];
	}

	void Object::Update()
	{
		updateModel();
		for (std::vector<Object*>::iterator it = children.begin(); it != children.end(); ++it)
			(*it)->Update();
	}

	void Object::Draw() const
	{
		//glUniformMatrix4fv(Game::ModelLoc, 1, GL_TRUE, finalModel);
		////glUniform4fv(Game::ColorLoc, 1, color);
		//glUniform4fv(Game::MaterialAmbientLoc, 1, material.ambient);
		//glUniform4fv(Game::MaterialDiffuseLoc, 1, material.diffuse);
		//glUniform4fv(Game::MaterialSpecularLoc, 1, material.specular);
		//glUniform1f(Game::MaterialShininessLoc, material.shininess);
		Shader::SetModel(finalModel);
		Shader::SetMaterialAmbient(material.ambient);
		Shader::SetMaterialDiffuse(material.diffuse);
		Shader::SetMaterialSpecular(material.specular);
		Shader::SetMaterialShininess(material.shininess);
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

	vec4 Object::GetColor() const
	{
		return color;
	}
	void Object::SetColor(const vec4& c)
	{
		color = c;
	}

	Material Object::GetMaterial() const
	{
		return material;
	}
	void Object::SetMaterial(const Material& m, bool propagateToChildren)
	{
		material = m;
		/*ambientProduct = Util::Product(material.ambient, LightSource::Default.ambient);
		diffuseProduct = Util::Product(material.diffuse, LightSource::Default.diffuse);
		specularProduct = Util::Product(material.specular, LightSource::Default.specular);*/

		if (propagateToChildren)
			for (auto it = children.begin(); it != children.end(); ++it)
				(*it)->SetMaterial(m, true);
	}
	void Object::UseMaterial(const Material& m)
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

	vec4 Object::GetPosition() const
	{
		return position;
	}
	void Object::SetPosition(const vec4& p)
	{
		//vec4 translation = p - position;
		//Translate(translation);
		position = p;
		changedFlags |= TRANSLATION_CHANGED;
	}
	void Object::Translate(const vec4& translation)
	{
		position += translation;
		changedFlags |= TRANSLATION_CHANGED;

		//if (position.x != position.x || position.y != position.y)
		//	std::cout << "wtf inside Object::Translate\n";
		//if (position.w != 1.0)
		//	std::cout << "wtf\n";
	}

	void Object::SetCenterOfRotation(const vec4& c)
	{
		centerOfRotation = c;
		centerOfRotationTranslationMatrix = translate(vec3(centerOfRotation));
		useCenterOfRotation = true;
		changedFlags = INT_MAX;
	}

	float Object::GetRotationX() const
	{
		return theta.x;
	}
	void Object::SetRotationX(float rotX)
	{
		theta.x = wrapAngle(rotX);
		changedFlags |= ROTATIONX_CHANGED;
	}
	void Object::RotateX(float x)
	{
		theta.x = wrapAngle(theta.x + x);
		changedFlags |= ROTATIONX_CHANGED;
	}

	float Object::GetRotationY() const
	{
		return theta.y;
	}
	void Object::SetRotationY(float rotY)
	{
		theta.y = wrapAngle(rotY);
		changedFlags |= ROTATIONY_CHANGED;
	}
	void Object::RotateY(float y)
	{
		theta.y = wrapAngle(theta.y + y);
		changedFlags |= ROTATIONY_CHANGED;
	}

	float Object::GetRotationZ() const
	{
		return theta.z;
	}
	void Object::SetRotationZ(float rotZ)
	{
		//theta.z = Util::WrapAngle(rotZ);
		theta.z = rotZ;
		changedFlags |= ROTATIONZ_CHANGED;
	}
	void Object::RotateZ(float z)
	{
		theta.z = wrapAngle(theta.z + z);
		changedFlags |= ROTATIONZ_CHANGED;
	}

	void Object::LookAt(const vec4& point)
	{
		vec4 displacement = point - (parentModel * position);
		SetRotationZ(atan2(displacement.y, displacement.x));
		SetRotationY(-atan2(displacement.z, length(vec2(displacement))));
	}

	float Object::GetScaleX() const
	{
		return scale.x;
	}
	void Object::SetScaleX(float x)
	{
		scale.x = x;
		changedFlags |= SCALE_CHANGED;
	}
	void Object::ScaleX(float scaleFactorX)
	{
		SetScaleX(scale.x * scaleFactorX);
	}

	float Object::GetScaleY() const
	{
		return scale.y;
	}
	void Object::SetScaleY(float y)
	{
		scale.y = y;
		changedFlags |= SCALE_CHANGED;
	}
	void Object::ScaleY(float scaleFactorY)
	{
		SetScaleY(scale.y * scaleFactorY);
	}

	float Object::GetScaleZ() const
	{
		return scale.z;
	}
	void Object::SetScaleZ(float z)
	{
		scale.z = z;
		changedFlags |= SCALE_CHANGED;
	}
	void Object::ScaleZ(float scaleFactorZ)
	{
		SetScaleZ(scale.z * scaleFactorZ);
	}

	void Object::SetScale(float s)
	{
		scale.x = scale.y = scale.z = s;
		changedFlags |= SCALE_CHANGED;
	}
	void Object::SetScale(float x, float y)
	{
		scale.x = x;
		scale.y = y;
		changedFlags |= SCALE_CHANGED;
	}
	void Object::SetScale(float x, float y, float z)
	{
		scale.x = x;
		scale.y = y;
		scale.z = z;
		changedFlags |= SCALE_CHANGED;
	}
	void Object::Scale(float scaleFactor)
	{
		scale.x *= scaleFactor;
		scale.y *= scaleFactor;
		scale.z *= scaleFactor;
		changedFlags |= SCALE_CHANGED;
	}

	mat4 Object::GetModel() const
	{
		return model;
	}

	mat4 Object::GetFinalModel() const
	{
		return finalModel;
	}

	mat4 Object::GetRotationMatrix() const
	{
		return rotationMatrix;
	}

	void Object::AddChild(Object *c)
	{
		c->SetParentModel(finalModel);
		children.push_back(c);
	}

	void Object::RemoveChild(Object *c)
	{
		//c->SetParentModel(Util::Identity());
		std::vector<Object*>::iterator it = std::find(children.begin(), children.end(), c);
		if (it != children.end())
			children.erase(std::find(children.begin(), children.end(), c));
	}

	void Object::SetParentModel(const mat4& pt)
	{
		parentModel = pt;
		changedFlags |= PARENT_CHANGED;
	}

	void Object::SetParentModel(const mat4& pTranslation, const mat4& pRotationX, const mat4& pRotationY, const mat4& pRotationZ, const mat4& pScale)
	{
		parentModel = mat4(1.0);

		if (!(ignoreParentModelFlags & IGNORE_PARENT_SCALE))
			parentModel = pScale * parentModel;
		if (!(ignoreParentModelFlags & IGNORE_PARENT_ROTATIONX))
			parentModel = pRotationX * parentModel;
		if (!(ignoreParentModelFlags & IGNORE_PARENT_ROTATIONY))
			parentModel = pRotationY * parentModel;
		if (!(ignoreParentModelFlags & IGNORE_PARENT_ROTATIONZ))
			parentModel = pRotationZ * parentModel;
		if (!(ignoreParentModelFlags & IGNORE_PARENT_TRANSLATION))
			parentModel = pTranslation * parentModel;

		changedFlags |= PARENT_CHANGED;
	}

	void Object::AddIgnoreParentModelFlag(IgnoreParentModel ignoreFlag)
	{
		ignoreParentModelFlags |= ignoreFlag;
	}

	// PROTECTED

	void Object::updateModel()
	{
		if (changedFlags)
		{
			// if our model view actually changed and not just the parent one
			if (changedFlags != PARENT_CHANGED)
			{
				if (changedFlags & TRANSLATION_CHANGED)
				{
					//oldTranslationMatrix = translationMatrix;
					if (useCenterOfRotation)
						translationMatrix = translate(vec3(position - centerOfRotation));
					else
						translationMatrix = translate(vec3(position));
				}
				if (changedFlags & ROTATIONX_CHANGED)
					rotationXMatrix = Util::RotateX(theta.x);
				if (changedFlags & ROTATIONY_CHANGED)
					rotationYMatrix = Util::RotateY(theta.y);
				if (changedFlags & ROTATIONZ_CHANGED)
					rotationZMatrix = Util::RotateZ(theta.z);
				if (changedFlags & SCALE_CHANGED)
					scaleMatrix = glm::scale(scale);

				// if only translation change just change the translation instead of computing everything again
				if (changedFlags == TRANSLATION_CHANGED)
					//model = translationMatrix * Util::InverseTranslation(oldTranslationMatrix) * model;
					model = translate(vec3(position - oldPosition)) * model;
				else
				{
					rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;
					if (useCenterOfRotation)
						model = translationMatrix * rotationMatrix * centerOfRotationTranslationMatrix * scaleMatrix;
					else
						model = translationMatrix * rotationMatrix * scaleMatrix;
				}
			}

			finalModel = parentModel * model;

			updateChildrenParentModels();

			oldPosition = position;
			changedFlags = 0;
		}
	}

	void Object::updateChildrenParentModels()
	{
		for (std::vector<Object*>::iterator it = children.begin(); it != children.end(); ++it)
		{
			if ((*it)->ignoreParentModelFlags)
				(*it)->SetParentModel(translationMatrix, rotationXMatrix, rotationYMatrix, rotationZMatrix, scaleMatrix);
			else
				(*it)->SetParentModel(finalModel);
		}
	}
}
