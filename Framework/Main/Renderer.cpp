#include "Renderer.h"

void Renderer::Initialize()
{
	UINT width = (UINT)D3D::GetWinWidth();
	UINT height = (UINT)D3D::GetWinHeight();

	gBuffer = make_unique<GBuffer>();
	gBuffer->Initialize(width, height);

	lightingPass = make_unique<LightingPass>();
	lightingPass->Initialize(width, height);
}

void Renderer::Destroy()
{
	lightingPass.reset();
	gBuffer.reset();
}

void Renderer::PreRender()
{
	//UpdateViewport();
}

void Renderer::Render()
{
	D3DSYSTEM->SetRenderTarget();
	D3DSYSTEM->Clear();

	//ID3D11RasterizerState* noCullState;
	//D3D::GetDC()->RSSetState(noCullState);

	//ClearViewport();
	
	//RenderGeometryPass(SCENEMANAGER->GetCurScene());
	
	//UpdateViewport();
	
	//RenderLightingPass();

	SCENEMANAGER->GetCurScene()->Render();
}

void Renderer::PostRender()
{
	//D3DSYSTEM->SetRenderTarget();
	
	//ClearViewport();
}

void Renderer::RenderGeometryPass(Scene* scene)
{
	if (!gBuffer.get())
	{
		assert(1);
		return;
	}
	
	gBuffer->UnbindSRVS();

	gBuffer->Bind();

	gBuffer->Clear();

	gBuffer->BindShader();

	scene->RenderGBuffer(gBuffer.get());
}

void Renderer::RenderLightingPass()
{
	lightingPass->Render(gBuffer.get());
}

void Renderer::UpdateViewport()
{
	D3D11_VIEWPORT vp{};
	vp.Width = (float)D3D::GetWinWidth();
	vp.Height = (float)D3D::GetWinHeight();
	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	D3D::GetDC()->RSSetViewports(1, &vp);
}

void Renderer::ClearViewport()
{
	auto dc = D3D::GetDC();

	ID3D11ShaderResourceView* nullSRV[8]{};
	dc->PSSetShaderResources(0, 8, nullSRV);

	dc->OMSetDepthStencilState(nullptr, 0);

	dc->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	dc->RSSetState(nullptr);
}

void Renderer::ResizeScreen(UINT width, UINT height)
{
	gBuffer->ResizeScreen(width, height);
	lightingPass->ResizeScreen(width, height);
}