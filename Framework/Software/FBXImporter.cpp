#include "FBXImporter.h"

vector<SkinnedVertex>	FBXImporter::vertices = vector<SkinnedVertex>();
vector<UINT>			FBXImporter::indices = vector<UINT>();
Skeleton*				FBXImporter::skeleton = new Skeleton();
vector<AnimationClip*>	FBXImporter::animClip = vector<AnimationClip*>();

bool FBXImporter::LoadFBXFile(const string & path, UINT flag)
{
	vertices.clear();
	indices.clear();
	animClip.clear();
	skeleton->Clear();

	Assimp::Importer importer;

	std::string fullPath = "C:/Users/user/Desktop/wjdrndud/Framework/_Assets/_FBX/";
	fullPath += path;

	const aiScene* scene = importer.ReadFile(fullPath, flag);

	if (!scene || !scene->mRootNode)
		return false;

	ProcessNode(scene->mRootNode, scene, -1);

	LoadAnimation(scene);

	string meshOut = "C:/Users/user/Desktop/wjdrndud/Framework/_Assets/_FBX/_Mesh/";
	string fileName = path; 
	
	size_t pos = fileName.rfind('.');
	if (pos != string::npos)
	{
		fileName = fileName.substr(0, pos);
	}

	meshOut += fileName;

	int out = 1;

	out *= SaveMeshBinary(meshOut + ".mesh", vertices, indices, skeleton);

	for (size_t i = 0; i < animClip.size(); i++)
	{
		string animOut = "C:/Users/user/Desktop/wjdrndud/Framework/_Assets/_FBX/_Anim/";
		animOut += fileName + "_" + to_string(i) + ".anim";

		out *= SaveAnimationBinary(animOut, animClip[i]->GetDuration(), animClip[i]);
	}

	return out;
}

bool FBXImporter::SaveMeshBinary(const string& path, const vector<SkinnedVertex>& vertices, const vector<UINT>& indices, Skeleton* skeleton)
{
	ofstream out(path, ios::binary);
	if (!out.is_open())
		return false;

	UINT vertexCount = (UINT)vertices.size();
	out.write((char*)&vertexCount, sizeof(UINT));
	out.write((char*)vertices.data(),
		sizeof(SkinnedVertex) * vertexCount);

	UINT indexCount = (UINT)indices.size();
	out.write((char*)&indexCount, sizeof(UINT));
	out.write((char*)indices.data(),
		sizeof(UINT) * indexCount);

	const auto& bones = skeleton->GetBones();
	UINT boneCount = (UINT)bones.size();
	out.write((char*)&boneCount, sizeof(UINT));

	for (auto& bone : bones)
	{
		UINT nameLen = (UINT)bone.name.size();
		out.write((char*)&nameLen, sizeof(UINT));
		out.write(bone.name.c_str(), nameLen);

		out.write((char*)&bone.parentIndex, sizeof(int));
		out.write((char*)&bone.localBindPose, sizeof(D3DXMATRIX));
		out.write((char*)&bone.globalBindPose, sizeof(D3DXMATRIX));
		out.write((char*)&bone.offset, sizeof(D3DXMATRIX));
	}

	out.close();
	return true;
}

bool FBXImporter::SaveAnimationBinary(const string& path, float duration, AnimationClip* clip)
{
	ofstream out(path, ios::binary);
	if (!out.is_open())
		return false;

	out.write((char*)&duration, sizeof(float));

	UINT boneAnimCount = (UINT)clip->GetBoneAnim().size();
	out.write((char*)&boneAnimCount, sizeof(UINT));

	for (const auto& anim : clip->GetBoneAnim())
	{
		UINT nameLen = (UINT)anim.boneName.size();
		out.write((char*)&nameLen, sizeof(UINT));
		out.write(anim.boneName.c_str(), nameLen);

		UINT keyCount = (UINT)anim.keyFrames.size();
		out.write((char*)&keyCount, sizeof(UINT));

		for (const auto& key : anim.keyFrames)
		{
			out.write((char*)&key.time, sizeof(float));
			out.write((char*)&key.translation, sizeof(Vector3));
			out.write((char*)&key.scale, sizeof(Vector3));
			out.write((char*)&key.rotation, sizeof(Quaternion));
		}
	}

	out.close();
	return true;
}

void FBXImporter::LoadAnimation(const aiScene * scene)
{
	for (UINT i = 0; i < scene->mNumAnimations; i++)
	{
		aiAnimation* anim = scene->mAnimations[i];

		AnimationClip* clip = new AnimationClip();

		float duration = (float)(anim->mDuration / anim->mTicksPerSecond);

		clip->SetDuration(duration);

		for (UINT c = 0; c < anim->mNumChannels; c++)
		{
			aiNodeAnim* channel = anim->mChannels[c];

			string boneName = channel->mNodeName.C_Str();
			int boneIndex = skeleton->GetBoneIndex(boneName);

			if (boneIndex < 0)
				continue;

			BoneAnim boneAnim;

			boneAnim.boneIndex = boneIndex;
			boneAnim.boneName = boneName;

			UINT keyCount = channel->mNumPositionKeys;

			for (UINT k = 0; k < keyCount; k++)
			{
				KeyFrame key;

				key.time = (float)(channel->mPositionKeys[k].mTime / anim->mTicksPerSecond);

				key.translation =
				{
					channel->mPositionKeys[k].mValue.x,
					channel->mPositionKeys[k].mValue.y,
					channel->mPositionKeys[k].mValue.z
				};

				if (k < channel->mNumRotationKeys)
				{
					key.rotation =
					{
						channel->mRotationKeys[k].mValue.x,
						channel->mRotationKeys[k].mValue.y,
						channel->mRotationKeys[k].mValue.z,
						channel->mRotationKeys[k].mValue.w
					};
				}

				if (k < channel->mNumScalingKeys)
				{
					key.scale =
					{
						channel->mScalingKeys[k].mValue.x,
						channel->mScalingKeys[k].mValue.y,
						channel->mScalingKeys[k].mValue.z
					};
				}

				boneAnim.keyFrames.push_back(key);
			}

			clip->AddBoneAnim(boneAnim);
		}

		animClip.push_back(clip);
	}
}

void FBXImporter::ProcessNode(aiNode* node, const aiScene* scene, int parentIndex)
{
	int currentBoneIndex = skeleton->GetBoneIndex(node->mName.C_Str());

	// 이 노드가 본이면 parent 설정
	if (currentBoneIndex >= 0)
	{
		skeleton->SetBoneParentIndex(currentBoneIndex, parentIndex);
		parentIndex = currentBoneIndex;
	}

	// 메시 처리
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene, parentIndex);
	}

	// 자식 노드 재귀
	for (UINT i = 0; i < node->mNumChildren; i++)
		ProcessNode(node->mChildren[i], scene, parentIndex);
}

void FBXImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene, int parentIndex)
{
	UINT baseVertex = (UINT)vertices.size();

	// -------------------------
	// 정점 생성
	// -------------------------
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		SkinnedVertex vertex = {};

		vertex.position = Vector3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		if (mesh->HasNormals())
		{
			vertex.normal = Vector3(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			);
		}

		if (mesh->HasTextureCoords(0))
		{
			vertex.uv = Vector2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
		}

		if (mesh->HasTangentsAndBitangents())
		{
			vertex.tangent = Vector3(
				mesh->mTangents[i].x,
				mesh->mTangents[i].y,
				mesh->mTangents[i].z
			);
		}

		for (int j = 0; j < 4; j++)
		{
			vertex.boneIndices[j] = 0;
			vertex.boneWeights[j] = 0.0f;
		}

		vertices.push_back(vertex);
	}

	// -------------------------
	// 인덱스 생성
	// -------------------------
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace& face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(baseVertex + face.mIndices[j]);
	}

	// -------------------------
	// 본 처리
	// -------------------------
	ProcessBones(mesh, scene, baseVertex);
}

void FBXImporter::ProcessBones(aiMesh* mesh, const aiScene* scene, int baseVertex)
{
	for (UINT i = 0; i < mesh->mNumBones; i++)
	{
		aiBone* aiBone = mesh->mBones[i];
		string boneName = aiBone->mName.C_Str();

		int boneIndex = skeleton->GetBoneIndex(boneName);

		// -------------------------
		// 본이 없으면 생성
		// -------------------------
		if (boneIndex == -1)
		{
			BoneData bone;
			bone.name = boneName;

			D3DXMatrixIdentity(&bone.localBindPose);
			D3DXMatrixIdentity(&bone.globalBindPose);
			D3DXMatrixIdentity(&bone.offset);

			// -------------------------
			// Node 찾기
			// -------------------------
			aiNode* node = FindNode(scene->mRootNode, boneName);

			if (node)
			{
				// -------------------------
				// localBindPose
				// -------------------------
				aiMatrix4x4 m = node->mTransformation;

				D3DXMATRIX local(
					m.a1, m.b1, m.c1, m.d1,
					m.a2, m.b2, m.c2, m.d2,
					m.a3, m.b3, m.c3, m.d3,
					m.a4, m.b4, m.c4, m.d4
				);

				bone.localBindPose = local;

				// -------------------------
				// 부모 찾기
				// -------------------------
				if (node->mParent)
				{
					string parentName = node->mParent->mName.C_Str();
					int parentIndex = skeleton->GetBoneIndex(parentName);

					bone.parentIndex = parentIndex;

					if (parentIndex >= 0)
					{
						bone.globalBindPose = skeleton->GetBones()[parentIndex].globalBindPose * bone.localBindPose;
					}
					else
					{
						bone.globalBindPose = bone.localBindPose;
					}
				}
				else
				{
					bone.parentIndex = -1;
					bone.globalBindPose = bone.localBindPose;
				}
			}

			// -------------------------
			// offset 은 Assimp 값 그대로
			// -------------------------
			aiMatrix4x4 om = aiBone->mOffsetMatrix;

			D3DXMATRIX offset(
				om.a1, om.b1, om.c1, om.d1,
				om.a2, om.b2, om.c2, om.d2,
				om.a3, om.b3, om.c3, om.d3,
				om.a4, om.b4, om.c4, om.d4
			);

			bone.offset = offset;

			boneIndex = skeleton->AddBone(bone);
		}

		for (UINT w = 0; w < aiBone->mNumWeights; w++)
		{
			aiVertexWeight weight = aiBone->mWeights[w];

			UINT vertexID = baseVertex + weight.mVertexId;
			float value = weight.mWeight;

			for (int j = 0; j < 4; j++)
			{
				if (vertices[vertexID].boneWeights[j] == 0.0f)
				{
					vertices[vertexID].boneIndices[j] = boneIndex;
					vertices[vertexID].boneWeights[j] = value;
					break;
				}
			}
		}
	}
}

aiNode * FBXImporter::FindNode(aiNode * node, const string & name) {
	if (node->mName.C_Str() == name)
		return node;

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		aiNode* found = FindNode(node->mChildren[i], name);
		if (found)
			return found;
	}

	return nullptr;
}
