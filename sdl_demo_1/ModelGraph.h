#ifndef MODELGRAPH_H
#define MODELGRAPH_H

#include <vector>

namespace Bagnall
{
	class Object;

	struct ModelNode
	{
		ModelNode *parent;
		std::vector<ModelNode*> children;
		Object *object;

		ModelNode(ModelNode *par, Object *o);

		~ModelNode();

		void Update();

		void AddChild(ModelNode *child);

		void RemoveChild(ModelNode *child);
	};
}

#endif
