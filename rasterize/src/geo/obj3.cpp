//
// Created by Boris N on 3/5/25.
//

#include "./obj3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../util/stb_image.h"

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

	int tex_width = 0, tex_height = 0, tex_channels = 0;
	unsigned char* tex_data = nullptr;

	std::string texture_filename;

	for (const auto& mat : materials) {
		if (!mat.diffuse_texname.empty()) {
			texture_filename = mat.diffuse_texname;
			break;
		}
	}

	if (!texture_filename.empty()) {
		std::string base_dir = path.substr(0, path.find_last_of("/\\") + 1);
		std::string tex_path = base_dir + texture_filename;
		tex_data = stbi_load(tex_path.c_str(), &tex_width, &tex_height, &tex_channels, 3); // force RGB
		if (!tex_data) {
			std::cerr << "Failed to load texture: " << tex_path << "\n";
		} else {
			std::clog << "Loaded texture: " << tex_path << "\n";
		}
	} else {
		std::cerr << "Failed to find a texture: " << path << "\n";
	}

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
				tri.vertices[v] = vertex;

				tri.colors = { color::random(), color::random(), color::random() };
				if (tex_data && idx.texcoord_index >= 0) {
					tri.uvs[v].x() = attrib.texcoords[2 * idx.texcoord_index + 0];
					tri.uvs[v].y() = attrib.texcoords[2 * idx.texcoord_index + 1];
				}
				// 	float u = tri.uvs[v].x();
				// 	float v_coord = tri.uvs[v].y();
				//
				// 	// flip v-coordinate (stb_image loads images with origin at top-left)
				// 	int x = std::min(int(u * tex_width), tex_width - 1);
				// 	int y = std::min(int((1.0f - v_coord) * tex_height), tex_height - 1);
				//
				// 	int pixel_index = (y * tex_width + x) * 3;
				// 	unsigned char r = tex_data[pixel_index + 0];
				// 	unsigned char g = tex_data[pixel_index + 1];
				// 	unsigned char b = tex_data[pixel_index + 2];
				//
				// 	std::clog << "Texture color at (" << x << ", " << y << "): "
				// 	     << "R: " << static_cast<int>(r) << ", "
				// 	     << "G: " << static_cast<int>(g) << ", "
				// 	     << "B: " << static_cast<int>(b) << "\n";
				//
				// 	tri.colors[v] = color(r / 255.0, g / 255.0, b / 255.0);
				// }
			}
			std::clog << tex_data << "\n";std::clog << "Texture pointer: '" << static_cast<void*>(tex_data) << "'. width: " << tex_width << ". height: " << tex_width << "\n";
			std::clog << (tex_data == nullptr ? "null" : "not null") << "\n";

			if (tex_data != nullptr) {
				tri.tex = tex_data;
				tri.tex_width = tex_width;
			}
			std::clog << (tri.tex == nullptr ? "null" : "not null") << "\n";
			tris.push_back(tri);
		}
	}

	return { tris, static_cast<int> (tris.size()) };
}