#include"owlEngine9.hpp"

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::SetBlendDefault()
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::SetBlendDefault(){
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::SetBlendAdd()
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::SetBlendAdd(){
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::SetBlendSub()
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::SetBlendSub(){
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::SetBlendMod()
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::SetBlendMod(){
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::SetBlendInv()
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::SetBlendInv(){
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::SetSamplerStateNone()
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::SetSamplerStateNone(){
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::SetSamplerStatePoint()
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::SetSamplerStatePoint(){
	if(D3D_OK != pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT)){
		pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	}
	if(D3D_OK != pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT)){
		pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	}
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::SetSamplerStateLinear()
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::SetSamplerStateLinear(){
	if(D3D_OK != pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR)){
		pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	}
	if(D3D_OK != pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR)){
		pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	}
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::SetSamplerStateAsiotropic()
//***********************************************************************************************************************************************************************
void owl::engine::GRAPH9::SetSamplerStateAsiotropic(){
	if(D3D_OK != pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC)){
		if(D3D_OK != pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR)){
			pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
		}
	}
	if(D3D_OK != pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC)){
		if(D3D_OK != pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR)){
			pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
		}
	}
}