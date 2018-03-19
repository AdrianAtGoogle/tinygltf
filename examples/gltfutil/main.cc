#include <iostream>
#include <string>

#include "gltfuilconfig.h"
#include "texture_dumper.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <tiny_gltf.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace gltfutil {
int usage() {
  using std::cout;
  cout << "gltfutil: tool for manipulating gltf files\n"
       << " usage information:\n\n"
       << "\t gltfutil (-i|-d|-h) [path to .gltf/glb] (-o [path to output "
          "directory])\n\n"
       << "\t\t -i: start in interactive mode\n"
       << "\t\t -d: dump enclosed content (image assets)\n"
       << "\t\t -h: print this help\n";
  return 0;
}

int arg_error() {
  (void)usage();
  return -1;
}

int parse_args(int argc, char** argv) {
  gltfutil::configuration config;

  for (size_t i = 1; i < size_t(argc); ++i) {
    char* arg = argv[i];
    if (arg[0] == '-') switch (arg[1]) {
        case 'h':
          config.mode = ui_mode::cli;
          config.action = cli_action::help;
        case 'd':
          config.mode = ui_mode::cli;
          config.action = cli_action::dump;
          break;
        case 'i':
          config.mode = ui_mode::interactive;
          break;
        case 'o':
          // TODO impl
          break;
        default:
          return arg_error();
      }
    else {
      // set the input path
      config.input_path = argv[i];
    }
  }

  if (config.is_valid()) {
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string error;
    bool state;
    switch (detectType(config.input_path)) {
      case FileType::Ascii:
        state = loader.LoadASCIIFromFile(&model, &error, config.input_path);
        break;
      case FileType::Binary:
        state = loader.LoadBinaryFromFile(&model, &error, config.input_path);
        break;
      case FileType::Unknown:
      default:
        return arg_error();
        break;
    }

    std::cerr << "state is " << state << '\n';

    if (config.mode == ui_mode::interactive) {
      // interactive usage now;
      // TODO impl
    } else {
      switch (config.action) {
        case cli_action::help:
          return usage();
        case cli_action::dump: {
          texture_dumper dumper(model);
          if (config.output_dir.empty())
            dumper.dump_to_folder();
          else
            dumper.dump_to_folder(config.output_dir);
        } break;
        default:
          return arg_error();
      }
    }
  } else {
    return arg_error();
  }
  return 0;
}
}  // namespace gltfutil

int main(int argc, char* argv[]) {
  if (argc == 1) return gltfutil::usage();
  if (argc > 1) return gltfutil::parse_args(argc, argv);
}
