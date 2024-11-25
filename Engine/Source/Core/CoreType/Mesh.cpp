#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

Core::CoreType::Mesh::Mesh(Vertex _vertices_array[], const unsigned int _vertices_count, unsigned int _indexes_array[], const unsigned int _indexes_count)
{
	CopyVerticesArray(_vertices_array, _vertices_count);

	if (_indexes_array && _indexes_count)
	{
		CopyIndexesArray(_indexes_array, _indexes_count);
		indexed = true;
	}
}

Core::CoreType::Mesh::Mesh(const PrimitivesMesh::PrimitivesMeshType _mesh_type)
{
	if (_mesh_type == PrimitivesMesh::PrimitivesMeshType::Cube)
	{
		CopyVerticesArray(PrimitivesMesh::CubeVertices, _countof(PrimitivesMesh::CubeVertices));
		CopyIndexesArray(PrimitivesMesh::CubeIndexes, _countof(PrimitivesMesh::CubeIndexes));
		indexed = true;
		name = "Cube";
		mesh_path = "Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Cube";
	}
	else if (_mesh_type == PrimitivesMesh::PrimitivesMeshType::Plane)
	{
		CopyVerticesArray(PrimitivesMesh::PlaneVertices, _countof(PrimitivesMesh::PlaneVertices));
		CopyIndexesArray(PrimitivesMesh::PlaneIndexes, _countof(PrimitivesMesh::PlaneIndexes));
		indexed = true;
		name = "Plane";
		mesh_path = "Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Plane";
	}
	else if (_mesh_type == PrimitivesMesh::PrimitivesMeshType::Pyramide)
	{
		CopyVerticesArray(PrimitivesMesh::PyramideVertices, _countof(PrimitivesMesh::PyramideVertices));
		CopyIndexesArray(PrimitivesMesh::PyramideIndexes, _countof(PrimitivesMesh::PyramideIndexes));
		indexed = true;
		name = "Pyramide";
		mesh_path = "Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Pyramide";
	}
	else if (_mesh_type == PrimitivesMesh::PrimitivesMeshType::Empty)
	{
		CopyVerticesArray(PrimitivesMesh::EmptyVertices, _countof(PrimitivesMesh::EmptyVertices));
		CopyIndexesArray(PrimitivesMesh::EmptyIndexes, _countof(PrimitivesMesh::EmptyIndexes));
		indexed = true;
		name = "Empty";
		mesh_path = "Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Empty";
	}
	
}

Core::CoreType::Mesh::Mesh(std::string _mesh_path) 
{

	if (_mesh_path == "Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Cube")
	{
		CopyVerticesArray(PrimitivesMesh::CubeVertices, _countof(PrimitivesMesh::CubeVertices));
		CopyIndexesArray(PrimitivesMesh::CubeIndexes, _countof(PrimitivesMesh::CubeIndexes));
		indexed = true;
		name = "Cube";
		mesh_path = "Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Cube";
	}
	else if (_mesh_path == "Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Plane")
	{
		CopyVerticesArray(PrimitivesMesh::PlaneVertices, _countof(PrimitivesMesh::PlaneVertices));
		CopyIndexesArray(PrimitivesMesh::PlaneIndexes, _countof(PrimitivesMesh::PlaneIndexes));
		indexed = true;
		name = "Plane";
		mesh_path = "Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Plane";
	}
	else if (_mesh_path == "Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Pyramide")
	{
		CopyVerticesArray(PrimitivesMesh::PyramideVertices, _countof(PrimitivesMesh::PyramideVertices));
		CopyIndexesArray(PrimitivesMesh::PyramideIndexes, _countof(PrimitivesMesh::PyramideIndexes));
		indexed = true;
		name = "Pyramide";
		mesh_path = "Core::CoreType::PrimitivesMesh::PrimitivesMeshType::Pyramide";
	}
	else 
	{

		std::ifstream fichier(_mesh_path);
		char separateur = ' ';
		char separateur2 = '/';

		std::vector<std::vector<float>> texture_coord;

		if (fichier.is_open()) 
		{
			std::string line;
			while (std::getline(fichier, line)) 
			{
				std::istringstream iss(line);
				std::vector<std::string> morceaux = {};
				std::string morceau;

				while (std::getline(iss, morceau, separateur)) 
				{ 
					morceaux.push_back(morceau);
				}
				if (morceaux.size() != 0) {

					if (morceaux[0] == "v")
					{
						Core::CoreType::Vertex vert = Core::CoreType::Vertex();
						vert.SetLocation(std::stof(morceaux[1]), std::stof(morceaux[2]), std::stof(morceaux[3]));
						vertices.push_back(vert);
					}
					else if (morceaux[0] == "vt")
					{
						std::vector<float> t_coord = { std::stof(morceaux[1]), std::stof(morceaux[2]) };
						texture_coord.push_back(t_coord);
					}
					else if (morceaux[0] == "f")
					{
						std::vector<int> inde;
						for (int i = 1; i < morceaux.size(); i++)
						{
							std::istringstream iss2(morceaux[i]);
							std::vector<std::string> vector_part = {};
							std::string part;

							while (std::getline(iss2, part, separateur2))
							{
								vector_part.push_back(part);
							}
							vertices[std::stoi(vector_part[0]) - 1].SetTextureCoord(texture_coord[std::stoi(vector_part[1]) - 1][0], texture_coord[std::stoi(vector_part[1]) - 1][1]);
							inde.push_back(std::stoi(vector_part[0]) - 1);
						}
						if (morceaux.size() - 1 == 4) {
							indexes.push_back(inde[0]);
							indexes.push_back(inde[1]);
							indexes.push_back(inde[2]);
							indexes.push_back(inde[2]);
							indexes.push_back(inde[3]);
							indexes.push_back(inde[0]);
						}
						else {
							indexes.push_back(inde[0]);
							indexes.push_back(inde[1]);
							indexes.push_back(inde[2]);
						}
					}
				}
			}
			indexed = true;
			std::filesystem::path pathObj(_mesh_path);
			name = pathObj.filename().string();
			mesh_path = "\"" + _mesh_path + "\"";
		}
	}
}

void Core::CoreType::Mesh::SetColor(DirectX::XMVECTORF32 _color)
{
	for (auto it = vertices.begin(); it != vertices.end(); ++it)
	{
		it->SetColor(_color);
	}
}

void Core::CoreType::Mesh::SetColor(float _r, float _g, float _b, float _a)
{
	for (auto it = vertices.begin(); it != vertices.end(); ++it)
	{
		it->SetColor(_r, _g, _b, _a);
	}
}


bool Core::CoreType::Mesh::IsIndexed() const
{
	return indexed;
}

Core::CoreType::Vertex* Core::CoreType::Mesh::GetVertices() const
{
	return const_cast<Core::CoreType::Vertex*>(vertices.data());
}

unsigned int* Core::CoreType::Mesh::GetIndexes() const
{
	return  const_cast<unsigned int*>(indexes.data());
}

int Core::CoreType::Mesh::GetVertexCount() const
{
	return static_cast<int>(vertices.size());
}

int Core::CoreType::Mesh::GetIndexCount() const
{
	return static_cast<int>(indexes.size());
}

std::string Core::CoreType::Mesh::GetName() const
{
	return name;
}

std::string Core::CoreType::Mesh::GetMeshPath() const 
{
	return mesh_path;
}

void Core::CoreType::Mesh::CopyVerticesArray(const Vertex _vertices_array[], const unsigned int _vertices_count)
{
	vertices.reserve(_vertices_count);

	for (unsigned int i = 0; i < _vertices_count; ++i)
		vertices.push_back(_vertices_array[i]);
}

void Core::CoreType::Mesh::CopyIndexesArray(unsigned int _indexes_array[], const unsigned int _indexes_count)
{
	indexes.reserve(_indexes_count);

	for (unsigned int i = 0; i < _indexes_count; ++i)
		indexes.push_back(_indexes_array[i]);
}