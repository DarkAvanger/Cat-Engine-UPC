#pragma once

#include "Component.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "MathGeoLib/src/MathGeoLib.h"

#include <vector>

typedef unsigned int GLuint;
typedef unsigned char GLubyte;

class TransformComponent;
class MaterialComponent;
class Mesh;
class Resource;

class MeshComponent : public Component
{
public:
	MeshComponent(GameObject* own, TransformComponent* trans);
	~MeshComponent();

	void Draw() override;
	void OnEditor() override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void SetMesh(std::shared_ptr<Resource> m);
	inline void SetTransform(TransformComponent* trans) { transform = trans; }
	inline void SetMaterial(MaterialComponent* mat) { material = mat; }
	inline AABB GetLocalAABB() { return localBoundingBox; }
	const Mesh* GetMesh() const { return mesh.get(); }

private:

	TransformComponent* transform;
	MaterialComponent* material;

	float normalLength;
	float3 colorNormal;
	
	bool showMeshMenu;
	bool faceNormals;
	bool verticesNormals;
	std::shared_ptr<Mesh> mesh;

	AABB localBoundingBox;

	VertexBuffer* vboAabb;
	IndexBuffer* eboAabb;

};