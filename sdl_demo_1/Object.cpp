#include "Object.h"
#include "Material.h"
#include "Shader.h"
#include "Game.h"
#include "Util.h"
#include <limits.h>

namespace Bagnall
{
	// PUBLIC

	Object::Object(const Object *parent)
	{
		position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		oldPosition = position;
		centerOfRotation = position;
		useCenterOfRotation = false;
		theta = vec3();
		scale = vec3(1.0, 1.0, 1.0);
		changedFlags = INT_MAX;
		ignoreParentModelFlags = 0;
		culled = false;

		if (parent == NULL)
		{
			updateNode = new UpdateNode(NULL, this);
			modelNode = new ModelNode(NULL, this);
			SetParentModel(mat4(1.0f));
			//SetParentInverseModel(mat4(1.0f));
		}
		else
		{
			updateNode = new UpdateNode(parent->updateNode, this);
			modelNode = new ModelNode(parent->modelNode, this);
			SetParentModel(parent->GetFinalModel());
			//SetParentInverseModel(parent->GetFinalInverseModel());
		}
	}

	Object::~Object()
	{
		// delete updateNode and its children nodes
		delete updateNode;

		// delete modelNode and its children nodes
		delete modelNode;

		// delete children objects (recursive)
		for (int i = 0; i < children.size(); ++i)
			delete children[i];
	}

	void Object::Update()
	{
		/*updateModel();
		for (std::vector<Object*>::iterator it = children.begin(); it != children.end(); ++it)
			(*it)->Update();*/
	}

	int Object::UpdateModel()
	{
		return updateModel();
	}

	void Object::PassDownParentModel(Object *c) const
	{
		if (!c->ignoreParentModelFlags)
		{
			c->SetParentModel(finalModel);
			//c->SetParentInverseModel(finalInverseModel);
		}
		else
		{
			c->SetParentModel(translationMatrix, rotationXMatrix, rotationYMatrix, rotationZMatrix, scaleMatrix);
			//c->SetParentInverseModel(translationInverseMatrix, rotationXInverseMatrix, rotationYInverseMatrix, rotationZInverseMatrix, scaleInverseMatrix);
		}
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
		//theta.x = wrapAngle(rotX);
		theta.x = rotX;
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
		//theta.y = wrapAngle(rotY);
		theta.y = rotY;
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

	/*mat4 Object::GetFinalInverseModel() const
	{
		return finalInverseModel;
	}*/

	mat4 Object::GetRotationMatrix() const
	{
		return rotationMatrix;
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

	/*void Object::SetParentInverseModel(const mat4& p)
	{
		parentInverseModel = p;
		changedFlags |= PARENT_CHANGED;
	}

	void Object::SetParentInverseModel(const mat4& pTranslation, const mat4& pRotationX, const mat4& pRotationY, const mat4& pRotationZ, const mat4& pScale)
	{
		parentInverseModel = mat4(1.0);

		if (!(ignoreParentModelFlags & IGNORE_PARENT_TRANSLATION))
			parentInverseModel = pTranslation * parentInverseModel;
		if (!(ignoreParentModelFlags & IGNORE_PARENT_ROTATIONZ))
			parentInverseModel = pRotationZ * parentInverseModel;
		if (!(ignoreParentModelFlags & IGNORE_PARENT_ROTATIONY))
			parentInverseModel = pRotationY * parentInverseModel;
		if (!(ignoreParentModelFlags & IGNORE_PARENT_ROTATIONX))
			parentInverseModel = pRotationX * parentInverseModel;
		if (!(ignoreParentModelFlags & IGNORE_PARENT_SCALE))
			parentInverseModel = pScale * parentInverseModel;

		changedFlags |= PARENT_CHANGED;
	}*/

	void Object::AddIgnoreParentModelFlag(IgnoreParentModel ignoreFlag)
	{
		ignoreParentModelFlags |= ignoreFlag;
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

	UpdateNode* Object::GetUpdateNode() const
	{
		return updateNode;
	}

	ModelNode* Object::GetModelNode() const
	{
		return modelNode;
	}

	void Object::Cull()
	{
		culled = true;
		for (auto it = children.begin(); it != children.end(); ++it)
			(*it)->Cull();
	}

	void Object::UnCull()
	{
		culled = false;
		for (auto it = children.begin(); it != children.end(); ++it)
			(*it)->UnCull();
	}

	bool Object::GetCulled() const
	{
		return culled;
	}

	// PROTECTED

	int Object::updateModel()
	{
		if (changedFlags)
		{
			// if our model view actually changed and not just the parent one
			if (changedFlags != PARENT_CHANGED)
			{
				if (changedFlags & TRANSLATION_CHANGED)
				{
					//oldTranslationMatrix = translationMatrix;
					if (!useCenterOfRotation)
						translationMatrix = translate(vec3(position));
					else
						translationMatrix = translate(vec3(position - centerOfRotation));

					//translationInverseMatrix = translate(-vec3(position));
				}
				if (changedFlags & ROTATIONX_CHANGED)
				{
					rotationXMatrix = Util::RotateX(theta.x);
					//rotationXInverseMatrix = Util::InverseRotateX(rotationXMatrix);
				}
				if (changedFlags & ROTATIONY_CHANGED)
				{
					rotationYMatrix = Util::RotateY(theta.y);
					//rotationYInverseMatrix = Util::InverseRotateY(rotationYMatrix);
				}
				if (changedFlags & ROTATIONZ_CHANGED)
				{
					rotationZMatrix = Util::RotateZ(theta.z);
					//rotationZInverseMatrix = Util::InverseRotateZ(rotationZMatrix);
				}
				if (changedFlags & SCALE_CHANGED)
				{
					scaleMatrix = glm::scale(scale);
					//scaleInverseMatrix = glm::scale(1.0f / scale);
				}

				// if only translation change just change the translation instead of computing everything again
				//if (changedFlags == TRANSLATION_CHANGED)
				//{
				//	//model = translationMatrix * Util::InverseTranslation(oldTranslationMatrix) * model;
				//	model = translate(vec3(position - oldPosition)) * model;
				//	inverseModel = translate(vec3(oldPosition - position)) * inverseModel;
				//}
				//else
				{
					rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;
					//rotationInverseMatrix = rotationXInverseMatrix * rotationYInverseMatrix * rotationZInverseMatrix;

					if (!useCenterOfRotation)
					{
						model = translationMatrix * rotationMatrix * scaleMatrix;
						//inverseModel = scaleInverseMatrix * rotationInverseMatrix * translationInverseMatrix;
					}
					else
					{
						model = translationMatrix * rotationMatrix * centerOfRotationTranslationMatrix * scaleMatrix;
						//inverseModel = scaleInverseMatrix * Util::InverseTranslation(centerOfRotationTranslationMatrix) * rotationInverseMatrix * translationInverseMatrix;
					}
				}
			}

			//auto I = model * inverseModel;

			finalModel = parentModel * model;
			//finalInverseModel = inverseModel * parentInverseModel;

			oldPosition = position;
			changedFlags = 0;

			return 1;
		}

		return 0;
	}
}
