#include "ModelGraph.h"
#include "Object.h"
#include <thread>
#include <ppl.h>

namespace Bagnall
{
	// PUBLIC

	ModelNode::ModelNode(ModelNode *par, Object *o)
	{
		parent = par;
		object = o;

		if (parent != NULL)
			parent->AddChild(this);
	}

	ModelNode::~ModelNode()
	{
		for (auto it = children.begin(); it != children.end(); ++it)
			delete *it;

		if (parent != NULL)
			parent->RemoveChild(this);
	}

	void ModelNode::Update()
	{
		if (object->UpdateModel())
		{
			concurrency::parallel_for_each(begin(children), end(children), [&](ModelNode *child)
			{
				object->PassDownParentModel(child->object);
				child->Update();
			}, concurrency::static_partitioner());

			/*for (auto it = children.begin(); it != children.end(); ++it)
			{
				auto child = *it;
				object->PassDownParentModel(child->object);
				child->Update();
			}*/
		}
		else
		{
			concurrency::parallel_for_each(begin(children), end(children), [&](ModelNode *child) {
				child->Update();
			}, concurrency::static_partitioner());
			/*for (auto it = children.begin(); it != children.end(); ++it)
				(*it)->Update();*/
		}
	}

	void ModelNode::AddChild(ModelNode *child)
	{
		children.push_back(child);
	}

	void ModelNode::RemoveChild(ModelNode *child)
	{
		auto it = std::find(children.begin(), children.end(), child);
		if (it != children.end())
			children.erase(it);
	}
}
