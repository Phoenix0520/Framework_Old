#include "Minecraft.h"

void Minecraft::Initialize()
{
	CONTEXT->GetCamera()->SetPosition(0, 2, -2.0f);
	CONTEXT->GetCamera()->SetRotation(Math::ToRadian(25.f), Math::ToRadian(0.f), Math::ToRadian(0.0f));

	size_t index = 0 ;

	for (float x = -50.0f; x <= 50.0f; x += 10.0f)
	{
		for (float y = -50.0f; y <= 50.0f; y += 10.0f)
		{
			blocks.push_back(new Block());
			blocks[index]->GetTransform()->SetScale(10.0f, 10.0f, 10.0f);
			blocks[index]->GetTransform()->SetPosition(x, -5.0f, y);

			index++;
		}
	}

	dLight = new DirectionalLight();
	dLight->Initialize();
	
	dLight->GetTransform()->SetPosition(-5.0f, 0.0f, -10.0f);


	pLight = new PointLight(1.0f);
	pLight->Initialize();

	pLight->GetTransform()->SetPosition(5.0f, 0.0f, -10.0f);

	sLight = new SpotLight(1.0f, 15.0f, cosf(Math::ToRadian(20.0f)), cosf(Math::ToRadian(30.0f)));
	sLight->Initialize();

	sLight->GetTransform()->SetPosition(0.0f, 0.0f, -10.0f);
	
	chara = new Chara();
	chara->Initialize();
	chara->GetTransform()->SetRotation(0.0f, 180.0f, 0.0f);
}

void Minecraft::ReadMap(wstring filePath)
{
	initMap.clear();
	
	wifstream fin = wifstream(filePath);

	fin.close();
}

void Minecraft::WriteMap(wstring filePath)
{
	wofstream fout(filePath);

	fout.close();
}


void Minecraft::Destroy()
{
	
}

void Minecraft::Reset()
{
}

void Minecraft::Update()
{
	static Color color = Color();
	static int index = 0;
	static int path = 0;
	static float val[3] = { 0, 0, 0 };


	static float lightPos[3] = { 0, 0, 0 };
	static float lightDir[3] = { 0, 0, 0 };
	static float lightRadius = 0.0f;
	static float lightRange = 0.0f;
	static float lightColor[3] = { 0, 0, 0 };
	static float lightIntensity = 0.0f;
	static int lightType = 0;
	static float radius = 1.0f;
	static float range = 15.0f;
	static float innerCone = 10.0f;
	static float outerCone = 20.0f;

	static int	 m_lightIndex = 0;
	static float m_lightPos[3] = { 0, 0, 0 };
	static float m_lightDir[3] = { 0, 0, 0 };
	static float m_lightRadius = 0.0f;
	static float m_lightRange = 0.0f;
	static float m_lightColor[3] = { 0, 0, 0 };
	static float m_lightIntensity = 0.0f;
	static int m_lightType = 0;
	static float m_radius = 1.0f;
	static float m_range = 15.0f;
	static float m_innerCone = 10.0f;
	static float m_outerCone = 20.0f;

	
	static float rotVal[3] = { 0, 0, 0 };

	if (ImGui::BeginTabBar("##TabBar1")) {
		if (ImGui::BeginTabItem("ModelLighting")){
			if (ImGui::DragInt("ModelLightBufferIndex", &m_lightIndex, 1, 0, MAX_LIGHT))
			{
				chara->GetMaterial()->UpdateLightingBuffer(m_lightIndex, m_lightPos, m_lightDir, m_lightRadius, m_lightRange, m_lightIntensity, m_lightColor, m_lightType, m_innerCone, m_outerCone);
			}
			if (ImGui::DragFloat3("Pos", m_lightPos, 0.1f, -100.0f, 100.0f, "%.1f")) {
				chara->GetMaterial()->UpdateLightingBuffer(m_lightIndex, m_lightPos, m_lightDir, m_lightRadius, m_lightRange, m_lightIntensity, m_lightColor, m_lightType, m_innerCone, m_outerCone);
			}
			if (ImGui::DragFloat3("Dir", m_lightDir, 0.1f, -1.0f, 1.0f, "%.1f")) {
				chara->GetMaterial()->UpdateLightingBuffer(m_lightIndex, m_lightPos, m_lightDir, m_lightRadius, m_lightRange, m_lightIntensity, m_lightColor, m_lightType, m_innerCone, m_outerCone);
			}
			if (ImGui::DragFloat("Rad", &m_lightRadius, 0.1f, 1.0f, 20.0f, "%.1f")) {
				chara->GetMaterial()->UpdateLightingBuffer(m_lightIndex, m_lightPos, m_lightDir, m_lightRadius, m_lightRange, m_lightIntensity, m_lightColor, m_lightType, m_innerCone, m_outerCone);
			}
			if (ImGui::DragFloat("Ran", &m_lightRange, 0.1f, 1.0f, 100.0f, "%.1f")) {
				chara->GetMaterial()->UpdateLightingBuffer(m_lightIndex, m_lightPos, m_lightDir, m_lightRadius, m_lightRange, m_lightIntensity, m_lightColor, m_lightType, m_innerCone, m_outerCone);
			}

			if (ImGui::DragFloat("Int", &m_lightIntensity, 0.1f, 0.0f, 100.0f, "%.1f")) {
				chara->GetMaterial()->UpdateLightingBuffer(m_lightIndex, m_lightPos, m_lightDir, m_lightRadius, m_lightRange, m_lightIntensity, m_lightColor, m_lightType, m_innerCone, m_outerCone);
			}

			if (ImGui::ColorEdit3("Col", m_lightColor)) {
				chara->GetMaterial()->UpdateLightingBuffer(m_lightIndex, m_lightPos, m_lightDir, m_lightRadius, m_lightRange, m_lightIntensity, m_lightColor, m_lightType, m_innerCone, m_outerCone);
			}

			if (ImGui::DragFloat("ICone", &innerCone, 1.0f, 0.0f, 80.0f, "%.f")) {
				chara->GetMaterial()->UpdateLightingBuffer(m_lightIndex, m_lightPos, m_lightDir, m_lightRadius, m_lightRange, m_lightIntensity, m_lightColor, m_lightType, m_innerCone, m_outerCone);
			}
			if (ImGui::DragFloat("OCone", &outerCone, 1.0f, 1.0f, 90.0f, "%.f")) {
				chara->GetMaterial()->UpdateLightingBuffer(m_lightIndex, m_lightPos, m_lightDir, m_lightRadius, m_lightRange, m_lightIntensity, m_lightColor, m_lightType, m_innerCone, m_outerCone);
			}

			if (ImGui::DragInt("Typ", &m_lightType, 0.1f, 0, 12)) {
				chara->GetMaterial()->UpdateLightingBuffer(m_lightIndex, m_lightPos, m_lightDir, m_lightRadius, m_lightRange, m_lightIntensity, m_lightColor, m_lightType, m_innerCone, m_outerCone);
			}


			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Shader")) {

			if (ImGui::InputInt("SelectedBlock", &index, 0)) {
				if (index > (int)blocks.size() - 1)
					index = 0;
			}
			ImGui::Separator();
			ImGui::Text("EffectBuffer");
			if (ImGui::DragInt("Path", &path, 0.1f, 0, 12)) {
				blocks[index]->GetMesh()->GetMaterial()->UpdateEffectBuffer(path, color, val[0], val[1], val[2]);
			}
			if (ImGui::DragFloat3("Value", val, 0.1f, -100.0f, 100.0f, "%.1f")) {
				blocks[index]->GetMesh()->GetMaterial()->UpdateEffectBuffer(path, color, val[0], val[1], val[2]);
			}
			if (ImGui::ColorEdit4("EffectColor", color)) {
				blocks[index]->GetMesh()->GetMaterial()->UpdateEffectBuffer(path, color, val[0], val[1], val[2]);
			}

			ImGui::Separator();
			ImGui::Text("LightingBuffer");

			if (ImGui::DragFloat3("Position", lightPos, 0.1f, -100.0f, 100.0f, "%.1f")) {
				for (auto block : blocks)
					block->GetMesh()->GetMaterial()->UpdateLightingBuffer(0, lightPos, lightDir, lightRadius, lightRange, lightIntensity, lightColor, lightType, innerCone, outerCone);
			}
			if (ImGui::DragFloat3("Direction", lightDir, 0.1f, -1.0f, 1.0f, "%.1f")) {
				for (auto block : blocks)
					block->GetMesh()->GetMaterial()->UpdateLightingBuffer(0, lightPos, lightDir, lightRadius, lightRange, lightIntensity, lightColor, lightType, innerCone, outerCone);
			}
			if (ImGui::DragFloat("Radius", &lightRadius, 0.1f, 1.0f, 20.0f, "%.1f")) {
				for (auto block : blocks)
					block->GetMesh()->GetMaterial()->UpdateLightingBuffer(0, lightPos, lightDir, lightRadius, lightRange, lightIntensity, lightColor, lightType, innerCone, outerCone);
			}
			if (ImGui::DragFloat("Range", &lightRange, 0.1f, 1.0f, 100.0f, "%.1f")) {
				for (auto block : blocks)
					block->GetMesh()->GetMaterial()->UpdateLightingBuffer(0, lightPos, lightDir, lightRadius, lightRange, lightIntensity, lightColor, lightType, innerCone, outerCone);
			}

			if (ImGui::DragFloat("Intensity", &lightIntensity, 0.1f, 0.0f, 100.0f, "%.1f")) {
				for (auto block : blocks)
					block->GetMesh()->GetMaterial()->UpdateLightingBuffer(0, lightPos, lightDir, lightRadius, lightRange, lightIntensity, lightColor, lightType, innerCone, outerCone);
			}

			if (ImGui::ColorEdit3("Color", lightColor)) {
				for (auto block : blocks)
					block->GetMesh()->GetMaterial()->UpdateLightingBuffer(0, lightPos, lightDir, lightRadius, lightRange, lightIntensity, lightColor, lightType, innerCone, outerCone);
			}

			if (ImGui::DragFloat("InnerCone", &innerCone, 1.0f, 0.0f, 80.0f, "%.f")) {
				for (auto block : blocks)
					block->GetMesh()->GetMaterial()->UpdateLightingBuffer(0, lightPos, lightDir, lightRadius, lightRange, lightIntensity, lightColor, lightType, innerCone, outerCone);
			}
			if (ImGui::DragFloat("OuterCone", &outerCone, 1.0f, 1.0f, 90.0f, "%.f")) {
				for (auto block : blocks)
					block->GetMesh()->GetMaterial()->UpdateLightingBuffer(0, lightPos, lightDir, lightRadius, lightRange, lightIntensity, lightColor, lightType, innerCone, outerCone);
			}

			if (ImGui::DragInt("Type", &lightType, 0.1f, 0, 12)) {
				for (auto block : blocks)
					block->GetMesh()->GetMaterial()->UpdateLightingBuffer(0, lightPos, lightDir, lightRadius, lightRange, lightIntensity, lightColor, lightType, innerCone, outerCone);
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Texture")) {

			if (ImGui::Button("ChangeAlbedo")) {
				wstring fileName;

				wchar_t oldPath[MAX_PATH];
				GetCurrentDirectory(MAX_PATH, oldPath);

				fileName = Path::OpenFileDialog(oldPath, oldPath, Path::imageFilter);

				SetCurrentDirectory(oldPath);

				Texture* texture = TEXTUREMANAGER->LoadTextureByAbsoultePath(fileName);

				if (texture != nullptr)
					blocks[index]->GetMesh()->GetMaterial()->SetAlbedoTexture(texture);
			}

			if (ImGui::Button("ChangeNormalMap")) {
				wstring fileName;

				wchar_t oldPath[MAX_PATH];
				GetCurrentDirectory(MAX_PATH, oldPath);

				fileName = Path::OpenFileDialog(oldPath, oldPath, Path::imageFilter);

				SetCurrentDirectory(oldPath);

				Texture* texture = TEXTUREMANAGER->LoadTextureByAbsoultePath(fileName);

				if (texture != nullptr)
					blocks[index]->GetMesh()->GetMaterial()->SetNormalTexture(texture);
			}

			if (ImGui::Button("ChangeORM")) {
				wstring fileName;

				wchar_t oldPath[MAX_PATH];
				GetCurrentDirectory(MAX_PATH, oldPath);

				fileName = Path::OpenFileDialog(oldPath, oldPath, Path::imageFilter);

				SetCurrentDirectory(oldPath);

				Texture* texture = TEXTUREMANAGER->LoadTextureByAbsoultePath(fileName);

				if (texture != nullptr)
					blocks[index]->GetMesh()->GetMaterial()->SetORMTexture(texture);
			}

			if (ImGui::DragFloat3("SetDirectionalLightDir", rotVal, 1.0f, 0.0f, 360.0f, "%.1f")) {
				
				Quaternion quat = dLight->GetTransform()->GetQuaternion();

				Vector3 lrot = Math::ToRadian(rotVal);
				D3DXQuaternionRotationYawPitchRoll(&quat, lrot.y, lrot.x, lrot.z);
			
				dLight->GetTransform()->SetQuaternion(quat);
			}

			if (ImGui::DragFloat("SetPointLightRadius", &radius, 1.0f, 0.0f, 100, "%.1f")) {

				pLight->SetLightRadius(radius);
				pLight->UpdateDetails();
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Camera")) {

			static float camSpeed = 35.0f;
			static float camRotSpeed = 50.0f;
			static Vector3 camPos = CONTEXT->GetCamera()->GetPosition();
			static Vector3 camRot = CONTEXT->GetCamera()->GetRotation();

			if (ImGui::DragFloat("CamSpeed", &camSpeed, 0.1f, 0.1f, 100.0f, "%.1f")){
				CONTEXT->GetCamera()->SetMoveSpeed(camSpeed);
			}

			if (ImGui::DragFloat("CamRotSpeed", &camRotSpeed, 0.1f, 50.0f, 200.0f, "%.1f")) {
				CONTEXT->GetCamera()->SetRotationSpeed(camRotSpeed);
			}

			if (ImGui::DragFloat3("CamPosition", camPos, 1.0f, -1000.0f, 1000.0f, "%.1f")) {
				CONTEXT->GetCamera()->SetPosition(camPos);
			}

			if (ImGui::DragFloat3("CamRotation", camRot, 1.0f, 0.0f, 365.0f, "%.1f")) {
				CONTEXT->GetCamera()->SetRotation(Math::ToRadian(camRot));
					
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Collider")) {

			static bool visible = true;

			if (ImGui::Checkbox("Visible", &visible)) {
				for (auto a : blocks)
					a->GetCollider()->SetVisibleState(visible);
			}

			ImGui::EndTabItem();
		}
		
		ImGui::EndTabBar();
	}
	
	Matrix V = CONTEXT->GetViewMatrix();
	Matrix P = CONTEXT->GetProjectionMatrix();

	float minDist = FLT_MAX;
	Block* newFocused = nullptr;

	for (auto block : blocks)
	{
		float hitDist;

		if (block->GetCollider()->IntersectMouse(hitDist))
		{
			if (hitDist < minDist)
			{
				minDist = hitDist;
				newFocused = block;
			}
		}

		block->Update(V, P);
	}

	if (focusedBlock != newFocused)
	{
		// 이전 Hover 끄기
		if (focusedBlock != nullptr)
			focusedBlock->GetCollider()->SetHoverState(false);

		focusedBlock = newFocused;

		// 새 Hover 켜기
		if (focusedBlock != nullptr)
			focusedBlock->GetCollider()->SetHoverState(true);
	}

	if (MOUSEDOWN(MBUTTON_L))
	{
		for (UINT i = 0; i < blocks.size(); i++) {
			if (focusedBlock == blocks[i])
				index = i;
		}
	}

	int result = -1;

	if (KEYPRESS('W'))
	{
		result = 0;
	}
	if (KEYPRESS('S'))
	{
		result = 180;
	}
	if (KEYPRESS('A'))
	{
		if (result == 0)
			result = -45;
		else if (result == 180)
			result = 225;
		else
			result = -90;
	}
	if (KEYPRESS('D'))
	{
		if (result == 0)
			result = 45;
		else if (result == 180)
			result = 135;
		else
			result = 90;
	}

	chara->SetMoveState((MoveState)result);

	//if (KEYPRESS(VK_SPACE))
	//{
	//	chara->Dead(true);
	//	chara->Jump();
	//}

	if (MOUSEDOWN(MBUTTON_L))
	{
		chara->Atk();
	}

	//static Vector3 cPos = CONTEXT->GetCamera()->GetPosition();
	//cPos.y += 15.0f;
	//cPos.z -= 15.0f;
	//
	//CONTEXT->GetCamera()->SetPosition(cPos);

	//dLight->Update(V, P);
	//pLight->Update(V, P);
	//sLight->Update(V, P);

	chara->Update(V, P);

	auto t = chara->GetTransform();

	Vector3 cPos = t->GetPosition() + Math::Mul(Vector3(-30.0f, 0.0f, -30.0f), chara->GetTransform()->GetForwardVector());
	cPos.x = t->GetPosition().x + 0.0f;
	cPos.z = t->GetPosition().z - 30.0f;
	cPos.y = 30.0f;
	CONTEXT->GetCamera()->SetPosition(cPos);
	//MOUSE->SetMousePointLock(true);

	//Vector3 cRot = Math::Mul(t->GetRotation(), Vector3(1.0f, 1.0f, 1.0f));
	//cRot.x = 15.0f;
	//
	//CONTEXT->GetCamera()->SetRotationDegree(cRot);

	Vector3 delta = MOUSE->GetDelta();
}

void Minecraft::Render()
{
	//dLight->Render();
	//pLight->Render();
	//sLight->Render();

	Matrix V = CONTEXT->GetViewMatrix();
	Matrix P = CONTEXT->GetProjectionMatrix();

	chara->Render();

	for (auto block : blocks)
	{	
		block->Render();
	}
}

void Minecraft::RenderGBuffer(GBuffer * gBuffer)
{
	for (auto block : blocks)
	{
		block->RenderGBuffer(gBuffer);
	}
}
