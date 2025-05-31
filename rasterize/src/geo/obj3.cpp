//
// Created by Boris N on 3/5/25.
//

#include "./obj3.h"

obj3 obj3::load_model(std::string path) {
	std::vector<triangle> tris;
	tinyobj::ObjReaderConfig reader_config;
	tinyobj::ObjReader reader;
	reader_config.triangulate = true;

	if (!reader.ParseFromFile(path, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		throw std::runtime_error("Failed to load model");
	}

	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader: " << reader.Warning();
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();

	for (auto s : shapes) {
		// extract triangles into `tris`
		for (size_t f = 0; f < s.mesh.num_face_vertices.size(); f++) {
			int fv = s.mesh.num_face_vertices[f];
			triangle tri;
			for (size_t v = 0; v < fv; v++) {
				tinyobj::index_t idx = s.mesh.indices[f * 3 + v];
				Point3d vertex;
				vertex.x() = attrib.vertices[3 * idx.vertex_index + 0];
				vertex.y() = attrib.vertices[3 * idx.vertex_index + 1];
				vertex.z() = attrib.vertices[3 * idx.vertex_index + 2];
				// todo: apply orientation
				tri.vertices[v] = vertex;
				// tri.colors = { color::random(), color::random(), color::random() };
				tri.colors = { red(), red(), red() };
			}
			tris.push_back(tri);
		}
	}

	return { tris, static_cast<int> (tris.size()) };}