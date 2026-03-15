#include "SkeletalMesh.h"

SkeletalMesh::SkeletalMesh()
{
	animController = new CharacterAnimController();
}

SkeletalMesh::~SkeletalMesh()
{
	SAFE_DELETE(animController);
}

void SkeletalMesh::SetMaterial(SkeletalMaterial * material)
{
	this->material = material;
	material->SetTransform(transform);
}

void SkeletalMesh::SetResource(SkeletalMeshResource * mesh)
{
	skeleton = mesh->GetSkeleton();
	resType = mesh->GetResourceType();

	vector<BoneData> bones = skeleton->GetBones();
	size_t boneCount = bones.size();

	localPose.bones.resize(boneCount);
	globalPose.bones.resize(boneCount);
	finalBoneMatrices.bones.resize(boneCount);

	for (size_t i = 0; i < boneCount; i++)
	{
		localPose[i] = bones[i].localBindPose;
		globalPose[i] = bones[i].globalBindPose;
		D3DXMatrixIdentity(&finalBoneMatrices[i]);
	}
	
	CreateVertexBuffer();
	CreateIndexBuffer();
}

void SkeletalMesh::Update(Matrix V, Matrix P, float deltaTime)
{
	if (!skeleton || !animController)
		return;

	animController->Update(skeleton, localPose);
	BuildFinalBoneMatrices();

	transform->UpdateWorldMatrix();

	material->UpdateTransformBuffer(V, P);
	material->UpdateBoneBuffer(finalBoneMatrices.bones);
	material->UpdateCameraBuffer(V, P, CONTEXT->GetCamera()->GetPosition());
}


void SkeletalMesh::Render()
{
	if (!skeleton)
		return;

	material->Bind();

	UINT stride = sizeof(SkinnedVertex);
	UINT offset = 0;

	auto dc = D3D::GetDC();

	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	dc->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dc->DrawIndexed(skeleton->GetIndices().size(), 0, 0);
}

void SkeletalMesh::BuildFinalBoneMatrices()
{
	auto& bones = skeleton->GetBones();

	Matrix blender;
	D3DXMatrixRotationX(&blender, Math::ToRadian(-90.0f));

	for (size_t i = 0; i < bones.size(); i++)
	{
		int parent = bones[i].parentIndex;

		if (parent < 0)
		{
			switch (resType)
			{
			case MIXAMO:
				globalPose[i] = localPose[i];
				break;
			case BLENDER:
				globalPose[i] = localPose[i] * blender;
				break;
			case MAYA:
				break;
			case MAX:
				break;
			default:
				break;
			}
		}
		else
			globalPose[i] = localPose[i] * globalPose[parent];
		
		finalBoneMatrices[i] = bones[i].offset * globalPose[i];			
	}
}

void SkeletalMesh::CreateVertexBuffer()
{
	auto& vertices = skeleton->GetVertices();

	cout << vertices.size() << endl;

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(SkinnedVertex) * vertices.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertices.data();

	HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	Check(hr);
}

void SkeletalMesh::CreateIndexBuffer()
{
	auto& indices = skeleton->GetIndices();

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * indices.size();
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = indices.data();

	HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	Check(hr);
}

