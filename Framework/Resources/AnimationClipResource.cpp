#include "AnimationClipResource.h"

bool AnimationClipResource::LoadBinary(const string& path)
{
	ifstream in(path, ios::binary);
	if (!in.is_open())
		return false;

	animClip->GetBoneAnim().clear();

	float duration = 0.0f;

	in.read((char*)&duration, sizeof(float));

	animClip->SetDuration(duration);

	UINT boneAnimCount = 0;
	in.read((char*)&boneAnimCount, sizeof(UINT));

	for (UINT i = 0; i < boneAnimCount; i++)
	{
		UINT nameLen = 0;
		in.read((char*)&nameLen, sizeof(UINT));

		string name;
		name.resize(nameLen);
		in.read(&name[0], nameLen);

		UINT keyCount = 0;
		in.read((char*)&keyCount, sizeof(UINT));

		BoneAnim anim;
		anim.boneName = name;
		anim.keyFrames.resize(keyCount);

		for (UINT k = 0; k < keyCount; k++)
		{
			in.read((char*)&anim.keyFrames[k].time, sizeof(float));
			in.read((char*)&anim.keyFrames[k].translation, sizeof(D3DXVECTOR3));
			in.read((char*)&anim.keyFrames[k].scale, sizeof(D3DXVECTOR3));
			in.read((char*)&anim.keyFrames[k].rotation, sizeof(D3DXQUATERNION));
		}

		animClip->GetBoneAnim().push_back(anim);
	}

	in.close();
	return true;
}
