//
// Created by Boris N on 3/5/25.
//

#include "./obj3.h"

tinyobj::ObjReaderConfig reader_config;
tinyobj::ObjReader reader;

obj3 obj3::load_model(std::string path) {
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



    return obj3(std::vector<triangle>(), 0);
}