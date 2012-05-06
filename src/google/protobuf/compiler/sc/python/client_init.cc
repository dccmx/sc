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

ClientInitGenerator::ClientInitGenerator(const FileDescriptor* file) : file_(file) {}

ClientInitGenerator::~ClientInitGenerator() {}

void ClientInitGenerator::Generate(io::Printer* printer) {
  printer->Print("from client import $package$\n", "package", PythonPackageName(file_->name()));
  printer->Print("from $module$ import *\n", "module", PythonModuleName(file_->name()));
  printer->Print("__all__ = ['$package$'", "package", PythonPackageName(file_->name()));
  for (int i = 0; i < file_->message_type_count(); ++i) {
    printer->Print(",'$message$'", "message", file_->message_type(i)->name());
  }
  printer->Print("]");
}

}  // namespace sc
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
