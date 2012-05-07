#include <unistd.h>
#include <iostream>

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

namespace {

// Should we generate generic services for this file?
inline bool HasGenericServices(const FileDescriptor *file) {
  return file->service_count() > 0 && file->options().py_generic_services();
}

}  // namespace

PythonServerGenerator::PythonServerGenerator() {}

PythonServerGenerator::~PythonServerGenerator() {}

bool PythonServerGenerator::Generate(const FileDescriptor* file,
                      const string& parameter,
                      OutputDirectory* context,
                      string* error) const {

  if (!HasGenericServices(file)) {
    cerr << "No service defined\n";
    exit(-1);
  }

  vector<pair<string, string> > options;
  ParseGeneratorParameter(parameter, &options);
  string output_dir_ = options[0].second;

  // touch __init__.py
  scoped_ptr<io::ZeroCopyOutputStream> init_output(context->Open("__init__.py"));

  ServerGenerator server_gen(file);
  scoped_ptr<io::ZeroCopyOutputStream> output(context->Open("server.py"));
  io::Printer printer(output.get(), '$');
  server_gen.Generate(&printer);

  string impl_filename = output_dir_ + PythonImplName(file->name()) + ".py";
  
  if (access(impl_filename.c_str(), F_OK)) {
    ImplementationGenerator impl_gen(file);
    scoped_ptr<io::ZeroCopyOutputStream> output(context->Open(PythonImplName(file->name()) + ".py"));
    io::Printer printer(output.get(), '$');
    impl_gen.Generate(&printer);
  }

  ServerSetupGenerator setup_gen(file);
  scoped_ptr<io::ZeroCopyOutputStream> setup_output(context->Open("setup.py"));
  io::Printer setup_printer(setup_output.get(), '$');
  setup_gen.Generate(&setup_printer);
  
  python_generator_.Generate(file, parameter, context, error);
}

}  // namespace sc
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
