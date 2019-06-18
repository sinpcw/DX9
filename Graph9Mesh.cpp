#include"owlEngine9.hpp"

//***********************************************************************************************************************************************************************
//
//		Class Constructor / Destructor
//
//***********************************************************************************************************************************************************************
//		MESH9::MESH9()
//***********************************************************************************************************************************************************************
owl::engine::MESH9::MESH9(){
	uSize		= 0;
	pMesh		= NULL;
	pMaterial	= NULL;
}

//***********************************************************************************************************************************************************************
//		MESH9::~MESH9()
//***********************************************************************************************************************************************************************
owl::engine::MESH9::~MESH9(){
	uSize		= 0;
	SafeRelease(pMesh);
	SafeDeleteArray(pMaterial);
}

//***********************************************************************************************************************************************************************
//
//		GRAPH9 Function
//
//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::InitMesh(MESH9 &, const wchar_t *)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitMesh(owl::engine::MESH9 &_Mesh, const wchar_t *_FileName){
	ID3DXBuffer *pD3DXBuffer = NULL;
	if(_Mesh.pMesh != NULL){
		return (true);
	}
	if(FAILED(D3DXLoadMeshFromX(_FileName, D3DXMESH_MANAGED, pDevice, NULL, &pD3DXBuffer, NULL, reinterpret_cast<DWORD *>(&_Mesh.uSize), &_Mesh.pMesh))){
		QuitMesh(_Mesh);
		return (false);
	}
	_Mesh.pMaterial	= NULL;
	try{
		_Mesh.pMaterial	= new D3DMATERIAL9[_Mesh.uSize];
	}catch(std::bad_alloc){
		_Mesh.pMaterial	= NULL;
		QuitMesh(_Mesh);
		return (false);
	}
	D3DXMATERIAL *pD3DXMat	= static_cast<D3DXMATERIAL *>(pD3DXBuffer->GetBufferPointer());
	for(UINT u = 0; u < _Mesh.uSize; u++){
		_Mesh.pMaterial[u]			= pD3DXMat[u].MatD3D;
		_Mesh.pMaterial[u].Ambient	= _Mesh.pMaterial[u].Diffuse;
	}
	SafeRelease(pD3DXBuffer);
	#ifndef	_DEBUG
		ID3DXMesh	*pRefine	= NULL;
		if(FAILED(_Mesh.pMesh->Optimize(D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL, &pRefine))){
			QuitMesh(_Mesh);
			return (false);
		}
		SafeRelease(_Mesh.pMesh);
		_Mesh.pMesh	= pRefine;
	#endif
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::InitMesh(MESH9 &, const unsigned char * unsigned int)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitMesh(owl::engine::MESH9 &_Mesh, const unsigned char *_Stream, unsigned int _Count){
	ID3DXBuffer *pD3DXBuffer = NULL;
	if(_Mesh.pMesh != NULL){
		return (true);
	}
	if(FAILED(D3DXLoadMeshFromXInMemory(_Stream, _Count, D3DXMESH_MANAGED, pDevice, NULL, &pD3DXBuffer, NULL, reinterpret_cast<DWORD *>(&_Mesh.uSize), &_Mesh.pMesh))){
		QuitMesh(_Mesh);
		return (false);
	}
	_Mesh.pMaterial	= NULL;
	try{
		_Mesh.pMaterial	= new D3DMATERIAL9[_Mesh.uSize];
	}catch(std::bad_alloc){
		QuitMesh(_Mesh);
		return (false);
	}
	D3DXMATERIAL *pD3DXMat	= static_cast<D3DXMATERIAL *>(pD3DXBuffer->GetBufferPointer());
	for(UINT u = 0; u < _Mesh.uSize; u++){
		_Mesh.pMaterial[u]			= pD3DXMat[u].MatD3D;
		_Mesh.pMaterial[u].Ambient	= _Mesh.pMaterial[u].Diffuse;
	}
	SafeRelease(pD3DXBuffer);
	#ifndef	_DEBUG
		ID3DXMesh	*pRefine	= NULL;
		if(FAILED(_Mesh.pMesh->Optimize(D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL, &pRefine))){
			QuitMesh(_Mesh);
			return (false);
		}
		SafeRelease(_Mesh.pMesh);
		_Mesh.pMesh	= pRefine;
	#endif
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::InitMesh(MESH9 &, const MESH9CREATEPARAM &, const core::VECT4f &, const core::VECT4f &, const core::VECT4f &, float)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::InitMesh(owl::engine::MESH9 &_Mesh, const owl::engine::MESH9CREATEPARAM &_Param, const core::VECT4f &_Diffuse, const core::VECT4f &_Specular, const core::VECT4f &_Emissive, float _Power){
	if(_Mesh.pMesh != NULL){
		return (true);
	}
	switch(_Param.nInfo){
		case (owl::engine::MESH_BOX):
			if(FAILED(D3DXCreateBox(pDevice, _Param.fX, _Param.fY, _Param.fZ, &_Mesh.pMesh, NULL))){
				return (false);
			}
			break;
		case (owl::engine::MESH_TEXT):
			return (false);
		case (owl::engine::MESH_TORUS):
			if(FAILED(D3DXCreateTorus(pDevice, _Param.fX, _Param.fY, static_cast<unsigned int>(_Param.iU), static_cast<unsigned int>(_Param.iV), &_Mesh.pMesh, NULL))){
				return (false);
			}
			break;
		case (owl::engine::MESH_TEAPOT):
			if(FAILED(D3DXCreateTeapot(pDevice, &_Mesh.pMesh, NULL))){
				return (false);
			}
			break;
		case (owl::engine::MESH_SPHERE):
			if(FAILED(D3DXCreateSphere(pDevice, _Param.fX, static_cast<unsigned int>(_Param.iU), static_cast<unsigned int>(_Param.iV), &_Mesh.pMesh, NULL))){
				return (false);
			}
			break;
		case (owl::engine::MESH_CYLINDER):
			if(FAILED(D3DXCreateCylinder(pDevice, _Param.fX, _Param.fY, _Param.fZ, static_cast<unsigned int>(_Param.iU), static_cast<unsigned int>(_Param.iV), &_Mesh.pMesh, NULL))){
				return (false);
			}
			break;
		default:
			return (false);
	}
	try{
		_Mesh.pMaterial	= new D3DMATERIAL9[1];
	}catch(std::bad_alloc){
		QuitMesh(_Mesh);
		return (false);
	}
	_Mesh.uSize						= 1;
	_Mesh.pMaterial[0].Diffuse.r	= _Diffuse.x;
	_Mesh.pMaterial[0].Diffuse.g	= _Diffuse.y;
	_Mesh.pMaterial[0].Diffuse.b	= _Diffuse.z;
	_Mesh.pMaterial[0].Diffuse.a	= _Diffuse.w;
	_Mesh.pMaterial[0].Ambient		= _Mesh.pMaterial[0].Diffuse;
	_Mesh.pMaterial[0].Specular.r	= _Specular.x;
	_Mesh.pMaterial[0].Specular.g	= _Specular.y;
	_Mesh.pMaterial[0].Specular.b	= _Specular.z;
	_Mesh.pMaterial[0].Specular.a	= _Specular.w;
	_Mesh.pMaterial[0].Emissive.r	= _Emissive.x;
	_Mesh.pMaterial[0].Emissive.g	= _Emissive.y;
	_Mesh.pMaterial[0].Emissive.b	= _Emissive.z;
	_Mesh.pMaterial[0].Emissive.a	= _Emissive.w;
	_Mesh.pMaterial[0].Power		= _Power;
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::QuitMesh(MESH9 &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::QuitMesh(owl::engine::MESH9 &_Mesh){
	_Mesh.uSize		= 0;
	SafeRelease(_Mesh.pMesh);
	SafeDeleteArray(_Mesh.pMaterial);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawMesh(MESH9 &, owl::engine::MATRIX *, float)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::DrawMesh(owl::engine::MESH9 &_Mesh, owl::engine::MATRIX *_Mat, float _Alpha){
	D3DXMATRIX		MatI;
	D3DMATERIAL9	Material;
	if(_Mesh.pMesh == NULL){
		return (false);
	}
	D3DXMatrixIdentity(&MatI);
	pDevice->SetFVF(_Mesh.pMesh->GetFVF());
	pDevice->SetTexture(0, NULL);
	pDevice->SetTransform(D3DTS_WORLD, _Mat);
	pDevice->SetRenderState(D3DRS_LIGHTING        , TRUE);
	pDevice->SetRenderState(D3DRS_SPECULARENABLE  , TRUE);
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE        , D3DCULL_CCW);
	for(UINT u = 0; u < _Mesh.uSize; u++){
		Material			 = _Mesh.pMaterial[u];
		Material.Ambient.a	*= _Alpha;
		Material.Diffuse.a	*= _Alpha;
		Material.Emissive.a	*= _Alpha;
		Material.Specular.a	*= _Alpha;
		pDevice->SetMaterial(&Material);
		_Mesh.pMesh->DrawSubset(u);
	}
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	pDevice->SetTransform(D3DTS_WORLD, &MatI);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawMesh(MESH9 &, owl::engine::MATRIX *, float, const core::VECT4f &, const core::VECT4f &, const core::VECT4f &, const core::VECT4f &)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::DrawMesh(owl::engine::MESH9 &_Mesh, owl::engine::MATRIX *_Mat, float _Alpha, const core::VECT4f &_Diffuse, const core::VECT4f &_Specular, const core::VECT4f &_Emissive, float _Power){
	D3DXMATRIX		MatI;
	D3DMATERIAL9	Material;
	if(_Mesh.pMesh == NULL){
		return (false);
	}
	Material.Ambient.a	= _Diffuse.w;
	Material.Ambient.r	= _Diffuse.x;
	Material.Ambient.g	= _Diffuse.y;
	Material.Ambient.b	= _Diffuse.z;
	Material.Diffuse.a	= _Diffuse.w;
	Material.Diffuse.r	= _Diffuse.x;
	Material.Diffuse.g	= _Diffuse.y;
	Material.Diffuse.b	= _Diffuse.z;
	Material.Emissive.a	= _Emissive.w;
	Material.Emissive.r	= _Emissive.x;
	Material.Emissive.g	= _Emissive.y;
	Material.Emissive.b	= _Emissive.z;
	Material.Specular.a	= _Specular.w;
	Material.Specular.r	= _Specular.x;
	Material.Specular.g	= _Specular.y;
	Material.Specular.b	= _Specular.z;
	D3DXMatrixIdentity(&MatI);
	pDevice->SetTexture(0, NULL);
	pDevice->SetTransform(D3DTS_WORLD, _Mat);
	pDevice->SetRenderState(D3DRS_LIGHTING        , TRUE);
	pDevice->SetRenderState(D3DRS_SPECULARENABLE  , TRUE);
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE        , D3DCULL_CCW);
	pDevice->SetFVF(_Mesh.pMesh->GetFVF());
	pDevice->SetMaterial(&Material);
	for(UINT u = 0; u < _Mesh.uSize; u++){
		_Mesh.pMesh->DrawSubset(u);
	}
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	pDevice->SetTransform(D3DTS_WORLD, &MatI);
	return (true);
}

//***********************************************************************************************************************************************************************
//		owl::engine::GRAPH9::DrawMeshTexture(MESH9 &, owl::engine::MATRIX *, float)
//***********************************************************************************************************************************************************************
bool owl::engine::GRAPH9::DrawMeshTexture(owl::engine::MESH9 &_Mesh, owl::engine::MATRIX *_Mat, float _Alpha){
	D3DXMATRIX		MatI;
	D3DMATERIAL9	Material;
	if(_Mesh.pMesh == NULL){
		return (false);
	}
	D3DXMatrixIdentity(&MatI);
	pDevice->SetFVF(_Mesh.pMesh->GetFVF());
	pDevice->SetTransform(D3DTS_WORLD, _Mat);
	pDevice->SetRenderState(D3DRS_LIGHTING        , TRUE);
	pDevice->SetRenderState(D3DRS_SPECULARENABLE  , TRUE);
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE        , D3DCULL_CCW);
	for(UINT u = 0; u < _Mesh.uSize; u++){
		Material			 = _Mesh.pMaterial[u];
		Material.Ambient.a	*= _Alpha;
		Material.Diffuse.a	*= _Alpha;
		Material.Emissive.a	*= _Alpha;
		Material.Specular.a	*= _Alpha;
		pDevice->SetMaterial(&Material);
		_Mesh.pMesh->DrawSubset(u);
	}
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	pDevice->SetTransform(D3DTS_WORLD, &MatI);
	return (true);
}
