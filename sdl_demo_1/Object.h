#ifndef OBJECT_H
#define OBJECT_H

#include "Bagnall.h"
#include "Material.h"
#include "UpdateGraph.h"
#include "ModelGraph.h"
#include <vector>

namespace Bagnall
{
	// flags for keeping track of changes to the object
	enum MatrixChanged
	{
		TRANSLATION_CHANGED = 1 << 0,
		ROTATIONX_CHANGED = 1 << 1,
		ROTATIONY_CHANGED = 1 << 2,
		ROTATIONZ_CHANGED = 1 << 3,
		SCALE_CHANGED = 1 << 4,
		PARENT_CHANGED = 1 << 5
	};

	enum IgnoreParentModel
	{
		IGNORE_PARENT_TRANSLATION = 1 << 0,
		IGNORE_PARENT_ROTATIONX = 1 << 1,
		IGNORE_PARENT_ROTATIONY = 1 << 2,
		IGNORE_PARENT_ROTATIONZ = 1 << 3,
		IGNORE_PARENT_SCALE = 1 << 4
	};

	class Object
	{
	public:
		/**********************************************************************
		Function:		Object constructor
		Purpose:		Create an Object object.
		**********************************************************************/
		Object(Object *parent);

		/**********************************************************************
		Function:		Object destructor
		Purpose:		Perform necessary cleanup of the Object when it
		is destroyed.
		**********************************************************************/
		virtual ~Object();

		/**********************************************************************
		Function:		Update
		Purpose:		Virtual update method to be overridden by a subclass.
		**********************************************************************/
		virtual void Update();

		int UpdateModel();

		void PassDownParentModel(Object *c) const;

		/**********************************************************************
		Function:		Draw
		Purpose:		Draw the Object object.
		Parameters:		bool select - draw id color if true
		**********************************************************************/
		virtual void Draw() const;

		/*virtual void DrawRaw() const;

		void StartDrawEmissive() const;

		void EndDrawEmissive() const;*/

		/**********************************************************************
		Function:		GetColor
		Purpose:		Getter for color member variable.
		**********************************************************************/
		vec4 GetColor() const;

		/**********************************************************************
		Function:		SetColor
		Purpose:		Setter for color member variable.
		Parameters:		vec4 color - new color value
		**********************************************************************/
		virtual void SetColor(const vec4& color);

		/**********************************************************************
		Function:		GetMaterial
		Purpose:		Getter for material member variable.
		**********************************************************************/
		Material GetMaterial() const;

		/**********************************************************************
		Function:		SetMaterial
		Purpose:		Setter for material member variable.
		Parameters:		const Material& m - new material
		**********************************************************************/
		virtual void SetMaterial(const Material& m, bool propagatetoChildren = false);

		/**********************************************************************
		Function:		UseMaterial
		Purpose:		Pass the given material values to the GPU but don't
		modify the object's material member variable
		Parameters:		const Material& m - material to use
		**********************************************************************/
		static void UseMaterial(const Material& m);

		/**********************************************************************
		Function:		GetPosition
		Purpose:		Getter for position member variable.
		**********************************************************************/
		vec4 GetPosition() const;

		/**********************************************************************
		Function:		SetPosition
		Purpose:		Setter for position member variable.
		Parameters:		vec4 position - new position value
		**********************************************************************/
		void SetPosition(const vec4& position);

		/**********************************************************************
		Function:		Translate
		Purpose:		Perform translation on the object.
		Parameters:		vec4 translation - translation vector
		**********************************************************************/
		void Translate(const vec4& translation);

		void SetCenterOfRotation(const vec4& c);

		/**********************************************************************
		Function:		GetRotationX
		Purpose:		Getter for rotationX member variable.
		**********************************************************************/
		float GetRotationX() const;

		/**********************************************************************
		Function:		SetRotationX
		Purpose:		Setter for rotationX member variable.
		Parameters:		float rotationX - new rotationX value
		**********************************************************************/
		void SetRotationX(float rotationX);

		/**********************************************************************
		Function:		RotateX
		Purpose:		Perform rotation on the object about the X axis.
		Parameters:		float x - rotation amount
		**********************************************************************/
		void RotateX(float x);

		/**********************************************************************
		Function:		GetRotationY
		Purpose:		Getter for rotationY member variable.
		**********************************************************************/
		float GetRotationY() const;

		/**********************************************************************
		Function:		SetRotationY
		Purpose:		Setter for rotationY member variable.
		Parameters:		float rotationY - new rotationY value
		**********************************************************************/
		void SetRotationY(float rotationY);

		/**********************************************************************
		Function:		RotateY
		Purpose:		Perform rotation on the object about the Y axis.
		Parameters:		float y - rotation amount
		**********************************************************************/
		void RotateY(float y);

		/**********************************************************************
		Function:		GetRotationZ
		Purpose:		Getter for rotationZ member variable.
		**********************************************************************/
		float GetRotationZ() const;

		/**********************************************************************
		Function:		SetRotationZ
		Purpose:		Setter for rotationZ member variable.
		Parameters:		float rotationZ - new rotationZ value
		**********************************************************************/
		void SetRotationZ(float rotationZ);

		/**********************************************************************
		Function:		RotateZ
		Purpose:		Perform rotation on the object about the Z axis.
		Parameters:		float z - rotation amount
		**********************************************************************/
		void RotateZ(float z);

		/**********************************************************************
		Function:		LookAt
		Purpose:		Set the X and Z rotation of the object so that it is
		"looking at" a given point
		Parameters:		const vec4& point - the point to look at
		Calls:			SetRotationX, SetRotationZ
		**********************************************************************/
		void LookAt(const vec4& point);

		/**********************************************************************
		Function:		GetScaleX
		Purpose:		Getter for scaleX member variable.
		**********************************************************************/
		float GetScaleX() const;

		/**********************************************************************
		Function:		SetScaleX
		Purpose:		Setter for scaleX member variable.
		Parameters:		float scaleX - new scaleX value
		**********************************************************************/
		virtual void SetScaleX(float scaleX);

		/**********************************************************************
		Function:		ScaleX
		Purpose:		Perform scaling on the object in the X axis.
		Parameters:		float scaleFactorX - scale amount
		**********************************************************************/
		void ScaleX(float scaleFactorX);

		/**********************************************************************
		Function:		GetScaleY
		Purpose:		Getter for scaleY member variable.
		**********************************************************************/
		float GetScaleY() const;

		/**********************************************************************
		Function:		SetScaleY
		Purpose:		Setter for scaleY member variable.
		Parameters:		float scaleY - new scaleY value
		**********************************************************************/
		virtual void SetScaleY(float scaleY);

		/**********************************************************************
		Function:		ScaleY
		Purpose:		Perform scaling on the object in the Y axis.
		Parameters:		float scaleFactorY - scale amount
		**********************************************************************/
		void ScaleY(float scaleFactorY);

		/**********************************************************************
		Function:		GetScaleZ
		Purpose:		Getter for scaleZ member variable.
		**********************************************************************/
		float GetScaleZ() const;

		/**********************************************************************
		Function:		SetScaleZ
		Purpose:		Setter for scaleZ member variable.
		Parameters:		float scaleZ - new scaleZ value
		**********************************************************************/
		virtual void SetScaleZ(float scaleZ);

		/**********************************************************************
		Function:		ScaleZ
		Purpose:		Perform scaling on the object in the Z axis.
		Parameters:		float scaleFactorZ - scale amount
		**********************************************************************/
		void ScaleZ(float scaleFactorZ);

		/**********************************************************************
		Function:		SetScale
		Purpose:		Setter for scaleX, scaleY, and scaleZ.
		Parameters:		float scale - new scale value
		**********************************************************************/
		virtual void SetScale(float scale);

		/**********************************************************************
		Function:		SetScale
		Purpose:		Setter for scaleX and scaleY
		Parameters:		float scaleX - new scale value for x
		float scaleY - new scale value for y
		**********************************************************************/
		virtual void SetScale(float scaleX, float scaleY);

		/**********************************************************************
		Function:		SetScale
		Purpose:		Setter for scaleX, scaleY, and scaleZ.
		Parameters:		float scaleX - new scale value for x
		float scaleY - new scale value for y
		float scaleZ - new scale value for z
		**********************************************************************/
		virtual void SetScale(float scaleX, float scaleY, float scaleZ);

		/**********************************************************************
		Function:		Scale
		Purpose:		Perform scaling on the object in the X, Y, and Z axes.
		Parameters:		float scaleFactor - scale amount
		**********************************************************************/
		void Scale(float scaleFactor);

		/**********************************************************************
		Function:		GetModel
		Purpose:		Getter for model member variable.
		**********************************************************************/
		mat4 GetModel() const;

		/**********************************************************************
		Function:		GetFinalModel
		Purpose:		Getter for finalModel member variable.
		**********************************************************************/
		mat4 GetFinalModel() const;

		mat4 GetRotationMatrix() const;

		/**********************************************************************
		Function:		SetParentModel
		Purpose:		Setter for parentModel member variable.
		Parameters:		const mat4& pt - new parent transform
		**********************************************************************/
		void SetParentModel(const mat4& pt);

		void SetParentModel(const mat4& pTranslation, const mat4& pRotationX, const mat4& pRotationY, const mat4& pRotationZ, const mat4& pScale);

		void AddIgnoreParentModelFlag(IgnoreParentModel ignoreFlag);

		void AddChild(Object *c);

		void RemoveChild(Object *c);

		UpdateNode* GetUpdateNode() const;

		ModelNode* GetModelNode() const;

	protected:
		vec4 color; // color of the object (for emission)

		vec4 position; // position of the object in world space
		vec4 oldPosition;
		mat4 translationMatrix; // current translation matrix
		mat4 oldTranslationMatrix; // previous translation matrix

		vec3 theta; // rotation about the three axes
		mat4 rotationMatrix;
		mat4 rotationXMatrix; // current rotationX matrix
		mat4 rotationYMatrix; // current rotationY matrix
		mat4 rotationZMatrix; // current rotationZ matrix

		vec4 centerOfRotation;
		mat4 centerOfRotationTranslationMatrix;
		bool useCenterOfRotation;

		vec3 scale; // scale in the three axes
		mat4 scaleMatrix; // current scale matrix

		unsigned int changedFlags; // flags for tracking changes to the object
		unsigned int ignoreParentModelFlags;
		mat4 model; // model-view matrix

		mat4 parentModel; // the parent transform matrix
		mat4 finalModel; // the product of parentModel and model

		Material material; // material of the object

		std::vector<Object*> children; // children of the object

		ModelNode *modelNode;
		UpdateNode *updateNode;

		int updateModel();
	};
}

#endif
