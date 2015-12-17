#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "Object.h"

namespace Bagnall
{
	struct RenderNode;

	class DrawableObject : public Object
	{
	public:
		DrawableObject(Object *par);

		DrawableObject(Object *par, GLuint tex, const Material& mat);

		DrawableObject(Object *par, const vec4& emissionCol);

		virtual void Draw() const;

		/*virtual void DrawRaw() const;

		void StartDrawEmissive() const;

		void EndDrawEmissive() const;*/

		bool GetEmissive() const;

		void SetEmissive(bool e);

		vec4 GetEmissionColor() const;

		virtual void SetEmissionColor(const vec4& color);

		bool GetBlend() const;

		void SetBlend(bool b);

		Material GetMaterial() const;

		virtual void SetMaterial(const Material& m);

		GLuint GetTexture() const;

		void SetTexture(GLuint tex);

		static void UseMaterial(const Material& m);

		void EnableRender();

		void DisableRender();

		void Cull();

		bool GetRenderEnabled() const;

		bool GetBumpMapEnabled() const;

	private:
		bool emissive;
		vec4 emissionColor;
		bool blend; // blend material with texture?
		bool bumpMapEnabled;
		bool renderEnabled;
		Material material;
		GLuint texture;
		RenderNode *renderNode;

		void init();

		void updateRenderNode();
	};
}

#endif
