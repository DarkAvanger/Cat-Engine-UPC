#pragma once

#include "Component.h"
#include "MathGeoLib/src/Geometry/AABB.h"
#include <string>

class GameObject;
class TransformComponent;
class IndexBuffer;
class VertexBuffer;

class AudioReverbZoneComponent : public Component
{
public:
	AudioReverbZoneComponent(GameObject* own, TransformComponent* trans);

	~AudioReverbZoneComponent();

	void ResizeReverbZone();

	inline AABB GetReverbZoneAABB() const { return reverbBoxZone; }
	inline std::string GetReverbBusName() const { return busReverb; }

	void CompileBuffers();
	void Draw() override;

	bool Update(float dt) override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	void OnEditor() override;

private:
	TransformComponent* transform;

	AABB reverbBoxZone;
	std::string busReverb;

	IndexBuffer* ebo;
	VertexBuffer* vbo;
	float3 dimensions;
};