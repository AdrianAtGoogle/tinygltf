#include <iostream>

#include "stb_image_write.h"
#include "texture_dumper.h"

using namespace gltfutil;
using namespace tinygltf;
using std::cout;

texture_dumper::texture_dumper(const Model& input)
    : model(input), configured_format(texture_output_format::png) {
  cout << "Texture dumper\n";
}

void texture_dumper::dump_to_folder(const std::string& path) {
  cout << "dumping to folder " << path << '\n';
  cout << "model file has " << model.textures.size() << " textures.\n";
  size_t index;
  for (const auto& texture : model.textures) {
    index++;
    const auto& image = model.images[texture.source];
    cout << "image name is: \"" << image.name << "\"\n";
    cout << "image size is: " << image.width << 'x' << image.height << '\n';
    cout << "pixel channel count :" << image.component << '\n';
    std::string name = image.name.empty() ? std::to_string(index) : image.name;
    switch (configured_format) {
      case texture_output_format::png:
        name = path + "/" + name + ".png";
        std::cout << "Image will be written to " << name << '\n';
        stbi_write_png(name.c_str(), image.width, image.height, image.component,
                       image.image.data(), 0);
        break;
      case texture_output_format::bmp:
        std::cout << "Image will be written to " << name << '\n';
        name = path + "/" + name + ".bmp";
        stbi_write_bmp(name.c_str(), image.width, image.height, image.component,
                       image.image.data());
        break;
      case texture_output_format::tga:
        std::cout << "Image will be written to " << name << '\n';
        name = path + "/" + name + ".tga";
        stbi_write_tga(name.c_str(), image.width, image.height, image.component,
                       image.image.data());
        break;
    }
  }
}
