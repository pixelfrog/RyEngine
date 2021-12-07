#include "pch.h"
#include <memory>
#include "MeshGeometry.h"
#include "VertexBuffer.h"
#include "UploadBuffer.h"
#include "IndexBuffer.h"
#include "CommandManager.h"
#include "Core/Graphics/Mesh.h"
#include "Core/Vertex.h"
#include "Core/SubSystemManager.h"

namespace RE
{
	GeometeryManager::GeometeryManager():
		_mMeshes(),
		_mDirty(false)
	{}

	GeometeryManager::~GeometeryManager()
	{
		/*std::vector<MeshGeometry*>::iterator i = _mMeshes.begin();
		for (; i != _mMeshes.end(); i++)
		{
			delete *i;
		}*/
		_mMeshes.clear();
	}

	void GeometeryManager::Submit(Mesh& mesh)
	{
		//MeshGeometry* m = new MeshGeometry(mesh);
		//_mMeshes.push_back(m);
	}

	void GeometeryManager::AddMesh(MeshGeometry& mesh)
	{
		_mMeshes.emplace_back(std::move(mesh));
		_mDirty = true;
	}

	void GeometeryManager::UploadAll(ID3D12GraphicsCommandList* commandList)
	{
		ID3D12Device* device = DirectXCore::GetDevice();
		for (auto& geo : _mMeshes)
		{
			// TODO Get rid of this uploaded flag once everything is converted
			if (geo.VertexCount() <= 0 || geo.IndexCount() <= 0)
				continue;

			//TODO this stuff should be put into it's own method to remove code replication
			VertexBuffer& vertBuf = geo._mVertexBuffer;
			UploadBuffer& vertUploadBuf = geo._mVertexUploadResource;

			//D3D12_SUBRESOURCE_DATA data = geo->GetResourceData();
			vertUploadBuf.Create(geo.VertexCount(), geo.VertexElementSize());
			vertUploadBuf.UploadResource(commandList, &vertBuf, geo.GetVertexResourceData());
			/*commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(vertBuf.GetResource(), vertBuf.GetCurrentState(), D3D12_RESOURCE_STATE_COPY_DEST));
			vertBuf.SetCurrentState(D3D12_RESOURCE_STATE_COPY_DEST);
			UpdateSubresources<1>(commandList, vertBuf.GetResource(), upBuf.GetResource(), 0, 0, 1, &data);
			commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(vertBuf.GetResource(), vertBuf.GetCurrentState(), D3D12_RESOURCE_STATE_GENERIC_READ));
			vertBuf.SetCurrentState(D3D12_RESOURCE_STATE_GENERIC_READ);*/


			IndexBuffer& indBuf = geo._mIndexBuffer;
			UploadBuffer& indexUploadBuf = geo._mIndexUploadResource;
			indexUploadBuf.Create(geo.IndexCount(), geo.IndexElementSize());

			indexUploadBuf.UploadResource(commandList, &indBuf, geo.GetIndexResourceData());
			/*D3D12_SUBRESOURCE_DATA indexData = geo->GetIndexResourceData();
			commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(indBuf.GetResource(), indBuf.GetCurrentState(), D3D12_RESOURCE_STATE_COPY_DEST));
			indBuf.SetCurrentState(D3D12_RESOURCE_STATE_COPY_DEST);
			UpdateSubresources<1>(commandList, indBuf.GetResource(), indUpBuf.GetResource(), 0, 0, 1, &indexData);
			commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(indBuf.GetResource(), indBuf.GetCurrentState(), D3D12_RESOURCE_STATE_GENERIC_READ));
			indBuf.SetCurrentState(D3D12_RESOURCE_STATE_GENERIC_READ);*/
		}
		_mDirty = false;
	}

	UINT GeometeryManager::MeshCount()
	{
		return _mMeshes.size();
	}

	UINT GeometeryManager::VertexCount()
	{
		UINT total = 0;
		for(auto& mesh : _mMeshes)
		{
			total += mesh.VertexCount();
		}

		return total;
	}

	UINT GeometeryManager::IndexCount()
	{
		UINT total = 0;
		for(auto& mesh : _mMeshes)
		{
			total += mesh.IndexCount();
		}

		return total;
	}

	MeshGeometry& GeometeryManager::GetMesh(int index)
	{
		ASSERT(index < MeshCount());

		return _mMeshes[index];
	}

	//----------------------------------Mesh Geometry------------------------------//
	MeshGeometry::MeshGeometry() :
		_mVertexBuffer(),
		_mIndexBuffer(),
		_mVertexUploadResource(),
		_mIndexUploadResource(),
		_mSubMeshData()
	{}

	MeshGeometry::MeshGeometry(VertexBuffer& vb, IndexBuffer& ib, std::vector<SubMeshData> subMeshData) :
		_mVertexBuffer(std::move(vb)),
		_mIndexBuffer(std::move(ib)),
		_mSubMeshData(subMeshData)
	{}

	D3D12_VERTEX_BUFFER_VIEW MeshGeometry::VertexBufferView()
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = _mVertexBuffer.GetGpuAddress();
		vbv.StrideInBytes = sizeof(Vertex);
		vbv.SizeInBytes = _mVertexBuffer.GetCount() * sizeof(Vertex);
		return vbv;
	}

	D3D12_INDEX_BUFFER_VIEW MeshGeometry::IndexBufferView()
	{
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = _mIndexBuffer.GetGpuAddress();
		ibv.Format = DXGI_FORMAT_R16_UINT;
		ibv.SizeInBytes = _mIndexBuffer.GetCount() * sizeof(RE_INDEX);
		return ibv;
	}

	D3D12_SUBRESOURCE_DATA MeshGeometry::GetVertexResourceData()
	{
		const Vertex* data = _mVertexBuffer.GetVerticies().data();
		D3D12_SUBRESOURCE_DATA resource;

		resource.pData = data;
		resource.RowPitch = VertexCount() * VertexElementSize();
		resource.SlicePitch = resource.RowPitch;

		return resource;
	}

	D3D12_SUBRESOURCE_DATA MeshGeometry::GetIndexResourceData()
	{
		const RE_INDEX* data = _mIndexBuffer.GetIndicies().data();
		D3D12_SUBRESOURCE_DATA resource;

		resource.pData = data;
		resource.RowPitch = IndexCount() * IndexElementSize();
		resource.SlicePitch = resource.RowPitch;

		return resource;
	}
}