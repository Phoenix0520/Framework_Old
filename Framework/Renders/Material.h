#pragma once

#include "Framework.h"

class Texture;
class Shader;
class Transform;
class GBuffer;

class Material
{
public:
	Material(Texture* albedo, Shader* shader);
	Material(Texture* albedo, Texture* normal, Texture* orm, Shader* shader);
	~Material();

public:
	void Bind();
	void BindGBuffer(GBuffer* gBuffer);

	void UpdateTransformBuffer(Matrix view, Matrix proj);
	void UpdateEffectBuffer(UINT path, Color color = Color(0, 0, 0, 0), float val1 = 0.0f, float val2 = 0.0f, float val3 = 0.0f);
	void UpdateEffectBuffer(EffectBuffer buffer);
	void UpdateLightingBuffer(UINT lightIndex, Vector3 lightPos, Vector3 lightDir, float lightRadius, float lightRange = 15.0f, float lightIntensity = 1.0f, Vector3 lightColor = Vector3(0, 0, 0), int lightType = 0, float innerCone = 20.0f, float outerCone = 30.0f);
	void UpdateLightingBuffer(UINT lightIndex, LightDesc lightDesc);
	void UpdateLightingBuffer(LightingBuffer buffer);
	void UpdateCameraBuffer(Matrix view, Matrix proj, Vector3 camPos);
	void UpdateCameraBuffer(CameraBuffer buffer);

	void UpdateUVRect();
	void UpdateUVCircle(int radialSlice = 16);

	void UpdateNormalMap();
	void UpdateOrm();

public:
	void SetDepthDisable();
	void SetDepthEnable();

	void SetWireFrame(bool val) { wireFrame = val; }

public:
	FORCEINLINE void SetTransform(Transform* transform)  { this->transform = transform; }

	FORCEINLINE void SetColor(Color color) { this->color = color; }

	FORCEINLINE Texture* GetAlbedoTexture() { return albedoTex; }
	FORCEINLINE Texture* GetNormalTexture() { return normalTex; }
	FORCEINLINE Texture* GetORMTexture()	{ return ormTex; }

	FORCEINLINE Shader* GetShader()			{ return shader; }

	FORCEINLINE void SetAlbedoTexture(Texture* ptr) { albedoTex = ptr; UpdateUVRect(); }
	FORCEINLINE void SetNormalTexture(Texture* ptr) { normalTex = ptr; UpdateNormalMap(); }
	FORCEINLINE void SetORMTexture(Texture* ptr)	{ ormTex = ptr; UpdateOrm(); }


	FORCEINLINE void SetShader(Shader* ptr) { shader = ptr; }

	FORCEINLINE Vector2*		GetUVRect()		{ return uv; };
	FORCEINLINE vector<Vector2> GetUVCircle()	{ return circleUV; };

private:
	void CreateTransformBuffer();		
	void CreateEffectBuffer();
	void CreateLightingBuffer();
	void CreateCameraBuffer();
	void CreateSampleState();
	void CreateBlendState();
	void CreateRasterState();
	void CreateDepthState();

private:
	Texture*	albedoTex = nullptr;
	Texture*	normalTex = nullptr;
	Texture*	ormTex = nullptr;
	Shader*		shader = nullptr;
	Shader*		gBufferShader = nullptr;
	Transform*	transform = nullptr;

	Color color;
	UVData uvData;

	Vector2 uv[4];
	vector<Vector2> circleUV;
	Rect uvRect;

	ID3D11SamplerState*			samplerState			= nullptr;
	ID3D11BlendState*			blendState				= nullptr;
	ID3D11RasterizerState*		rasterState				= nullptr;
	ID3D11RasterizerState*		wireFrameRasterState	= nullptr;
	ID3D11DepthStencilState*	depthState				= nullptr;
	ID3D11DepthStencilState*	depthDisableState		= nullptr;

	// Buffers
	ID3D11Buffer* transformBuffer	= nullptr;
	ID3D11Buffer* effectBuffer		= nullptr;
	ID3D11Buffer* lightingBuffer	= nullptr;
	ID3D11Buffer* cameraBuffer		= nullptr;
	ID3D11Buffer* lightingBuffers[8] = { nullptr, nullptr , nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

	bool wireFrame = false;

};