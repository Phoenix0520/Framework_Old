#pragma once

#include "Framework.h"

#include "Resources/AnimationClipResource.h"
#include "Resources/SkeletalMeshResource.h"
#include "Renders/SkeletalMaterial.h"
#include "Animations/AnimationController.h"
#include "Instances/CharacterAnimController.h"

#include "Skeleton.h"

class Skeleton;
class SkeletalMeshResource;
class AnimationClip;
class AnimationClipResource;
class AnimationController;

class SkeletalMesh
{
public:
	SkeletalMesh();
	~SkeletalMesh();

public:
	void SetTransform(Transform* transform)		{ this->transform = transform; }
	Transform* GetTransform()					{ return transform; }

	SkeletalMaterial* GetMaterial()				{ return material; }

	void Update(Matrix V, Matrix P, float deltaTime = DELTA);
	void Render();

public:
	void SetMaterial(SkeletalMaterial* material);
	void SetResource(SkeletalMeshResource* mesh);

	FORCEINLINE void AddAnimation(AnimationClipResource* clip, string name)	{ animController->Add(clip->GetAnimationClip(), name); }
	FORCEINLINE AnimationController* GetAnimController() { return animController; }


	const AnimPose& GetFinalBoneMatrices() const { return finalBoneMatrices; }

private:
	void CreateVertexBuffer();
	void CreateIndexBuffer();

	void BuildFinalBoneMatrices();

private:
	Transform* transform;

	SkeletalMaterial* material = nullptr;

	Skeleton*				skeleton		= nullptr;
	AnimationController*	animController	= nullptr;

	ID3D11Buffer* vertexBuffer	= nullptr;
	ID3D11Buffer* indexBuffer	= nullptr;

	AnimPose localPose;			// 애니메이션 결과
	AnimPose globalPose;		// 부모 반영
	AnimPose finalBoneMatrices;	// GPU 전달용

	ResourceType resType;
	
};