#include <google/protobuf/compiler/sc/python/generator.h>

#include <google/protobuf/compiler/sc/sc_generator.h>
#include <google/protobuf/compiler/sc/sc_helpers.h>
#include <google/protobuf/compiler/sc/python/generator.h>
#include <google/protobuf/descriptor.pb.h>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/zero_copy_stream.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace sc {

ClientSetupGenerator::ClientSetupGenerator(const FileDescriptor* file) : file_(file) {}

ClientSetupGenerator::~ClientSetupGenerator() {}

void ClientSetupGenerator::Generate(io::Printer* printer) {
  const char import_tpl[] =
      "#!/usr/bin/env python\n"
      "# coding: utf-8\n"
      "from setuptools import setup\n";
  printer->Print(import_tpl);
  printer->Print("\n\n");

  const char setup_tpl[] =
      "setup(name='$package$',\n"
      "      version='VERSION',\n"
      "      description='YOUR DESCRIPTION',\n"
      "      author='YOUR NAME',\n"
      "      author_email='YOUR EMAIL',\n"
      "      url='HOME PAGE OF THIS PROJECT',\n"
      "      download_url='DOWNLOAD URL OF THE PACKAGE',\n"
      "      packages=['$package$'],\n"
      "      install_requires=['stpclient'],\n"
      "      classifiers=[\n"
      "        'Development Status :: STATUS',\n"
      "        'Intended Audience :: AUDIENCE',\n"
      "        'License :: LICENSE',\n"
      "        'Operating System :: OS Independent',\n"
      "        'Programming Language :: Python',\n"
      "        'Topic :: TOPIC',\n"
      "        ])\n";
  printer->Print(setup_tpl, "package", PythonImplName(file_->name()));
}

}  // namespace sc
}  // namespace compiler
}  // namespace protobuf
}  // namespace google


