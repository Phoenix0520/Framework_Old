#pragma once

#include "Framework.h"

#define MAX_LIGHT 8

class Shader;
class Transform;

class SkeletalMaterial
{
public:
	SkeletalMaterial(Texture* texture, Shader* shader);
	SkeletalMaterial(Texture* albedo, Texture* normal, Texture* orm, Shader* shader);
	~SkeletalMaterial();

public:
	void Bind();

	void UpdateTransformBuffer(Matrix view, Matrix proj);
	void UpdateTransformBuffer(TransformBuffer buffer);
	void UpdateBoneBuffer(const vector<Matrix>& boneMatrices);

	void UpdateShadowBuffer(Matrix lightViewProj);
	void UpdateShadowBuffer(ShadowBuffer buffer);
	void UpdateLightingBuffer(UINT lightIndex, Vector3 lightPos, Vector3 lightDir, float lightRadius, float lightRange = 15.0f, float lightIntensity = 1.0f, Vector3 lightColor = Vector3(0, 0, 0), int lightType = 0, float innerCone = 20.0f, float outerCone = 30.0f);
	void UpdateLightingBuffer(UINT lightIndex, LightDesc lightDesc);
	void UpdateLightingBuffer(LightingBuffer buffer);
	void SetLightCount(UINT count) { lightBuf.lightCount = count; }
	void UpdateCameraBuffer(Matrix view, Matrix proj, Vector3 camPos);
	void UpdateCameraBuffer(CameraBuffer buffer);

public:
	void SetDepthDisable();
	void SetDepthEnable();

	void SetWireFrame(bool val) { wireFrame = val; }

public:
	FORCEINLINE void SetTransform(Transform* transform) { this->transform = transform; }
	FORCEINLINE Transform* GetTransform() { return transform; }

	FORCEINLINE Shader* GetShader() { return shader; }
	FORCEINLINE void SetShader(Shader* ptr) { shader = ptr; }

private:
	void CreateTransformBuffer();
	void CreateBoneBuffer();
	void CreateLightingBuffer();
	void CreateCameraBuffer();
	void CreateShadowBuffer();

	void CreateSampleState();
	void CreateBlendState();
	void CreateRasterState();
	void CreateDepthState();

private:
	Transform*	transform = nullptr;
	Texture*	albedoTex = nullptr;
	Texture*	normalTex = nullptr;
	Texture*	subTex = nullptr;
	
	Shader*		shader = nullptr;

	ID3D11SamplerState*			samplerState = nullptr;
	ID3D11SamplerState*			shadowSampler = nullptr;
	ID3D11BlendState*			blendState = nullptr;
	ID3D11RasterizerState*		rasterState = nullptr;
	ID3D11RasterizerState*		wireFrameRasterState = nullptr;
	ID3D11DepthStencilState*	depthState = nullptr;
	ID3D11DepthStencilState*	depthDisableState = nullptr;

	// Buffers
	ID3D11Buffer* transformBuffer = nullptr;
	ID3D11Buffer* boneBuffer = nullptr;
	ID3D11Buffer* lightingBuffer = nullptr;
	ID3D11Buffer* cameraBuffer = nullptr;
	ID3D11Buffer* shadowBuffer = nullptr;

	LightingBuffer lightBuf;

	bool wireFrame = false;

};