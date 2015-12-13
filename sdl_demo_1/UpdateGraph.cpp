#include "UpdateGraph.h"
#include "Object.h"

namespace Bagnall
{
	// PUBLIC

	UpdateNode::UpdateNode(UpdateNode *par, Object *o)
	{
		parent = par;
		object = o;
		parallel = false;

		if (parent != NULL)
			parent->AddChild(this);
	}

	UpdateNode::~UpdateNode()
	{
		for (auto it = children.begin(); it != children.end(); ++it)
			delete *it;

		if (parent != NULL)
			parent->RemoveChild(this);
	}

	void UpdateNode::Update()
	{
		object->Update();

		if (parallel)
		{
			// update children in parallel
		}
		else
		{
			// update children sequentially
			for (auto it = children.begin(); it != children.end(); ++it)
				(*it)->Update();
		}
	}

	void UpdateNode::AddChild(UpdateNode *child)
	{
		children.push_back(child);
	}

	void UpdateNode::RemoveChild(UpdateNode *child)
	{
		auto it = std::find(children.begin(), children.end(), child);
		if (it != children.end())
			children.erase(it);
	}
}