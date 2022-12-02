#pragma once

#include <string>
#include <vector>
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

#include "MathGeoLib/src/MathGeoLib.h"

typedef unsigned int uint;

typedef json_array_t JSON_Array;
class JsonParsing;
class VertexBuffer;
class IndexBuffer;

class GameObject
{
public:
	GameObject();
	~GameObject();

	bool Update(float dt);
	void Draw();
	void DrawEditor();
	void DebugColliders();

	Component* CreateComponent(ComponentType type);
	void AddComponent(Component* component);
	void AddChild(GameObject* object);

	inline void SetParent(GameObject* object) { parent = object; }
	inline void SetName(const char* n) { name = n; }

	char* GetNameBuffer();
	inline uint const GetUUID() const { return uuid; }
	inline const char* GetName() const { return name.c_str(); }
	inline GameObject* GetParent() const { return parent; }
	inline const bool& GetActive() const { return active; }
	inline std::vector<GameObject*>& GetChilds() { return children; }
	void SetAABB(std::vector<float3>& vertices);
	void SetTotalAABB();
	inline AABB GetAABB() { return globalAabb; }
	void SetAABB(AABB newAABB);

	void MoveChildrenUp(GameObject *child);
	void MoveChildrenDown(GameObject *child);

	void OnLoad(JsonParsing& node);
	void OnSave(JsonParsing& node, JSON_Array* array);

	template<typename T>
	T* GetComponent();

	template<typename T>
	std::vector<T*> GetAllComponent();
private:
	std::string name;
	bool active;
	std::vector<Component*> components;

	GameObject* parent;
	std::vector<GameObject*> children;

	bool newComponent;
	bool colliders;

	AABB globalAabb;
	OBB globalObb;

	VertexBuffer* vertex;
	IndexBuffer* index;

	uint uuid;
};

template<typename T>
inline T* GameObject::GetComponent()
{
	T* component = nullptr;
	
	for (std::vector<Component*>::iterator i = components.begin(); i < components.end(); ++i)
	{
		component = dynamic_cast<T*>(*i);
		if (component != nullptr)
			return component;
	}

	return component;
}

template<typename T>
inline std::vector<T*> GameObject::GetAllComponent()
{
	std::vector<T*> component;

	for (std::vector<Component*>::iterator i = components.begin(); i < components.end(); ++i)
	{
		T* c= nullptr;
		c = dynamic_cast<T*>(*i);
		if (c != nullptr)
			component.push_back(c);
	}

	return component;
}