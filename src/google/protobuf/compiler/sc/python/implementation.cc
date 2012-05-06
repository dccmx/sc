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

ImplementationGenerator::ImplementationGenerator(const FileDescriptor* file) : file_(file) {}

ImplementationGenerator::~ImplementationGenerator() {}

void ImplementationGenerator::Generate(io::Printer* printer) {
  printer_ = printer;
  PrintImports();
  PrintServices();
}

void ImplementationGenerator::PrintImports() const {
  const char tpl[] =
      "#!/usr/bin/python\n"
      "# coding: utf-8\n"
      "import $module$\n";
  printer_->Print(tpl, "module", PythonModuleName(file_->name()));
  printer_->Print("\n\n");
}

void ImplementationGenerator::PrintServices() const {
  for (int i = 0; i < file_->service_count(); ++i) {
    const ServiceDescriptor *service = file_->service(i);
    printer_->Print("class $service$(echo_pb2.$service$):\n", "service", service->name());
    for (int j = 0; j < service->method_count(); j++) {
      const char method_tpl[] =
          "    def $method$(self, rpc_controller, request, done):\n"
          "        # TODO implement method here to finish the server\n"
          "        # example:\n"
          "        #   response = $module$.$response$()\n"
          "        #   do some thing to fill the response\n"
          "        #   done(response)\n"
          "        pass";
      printer_->Print(method_tpl,
                      "method", service->method(j)->name(),
                      "module", PythonModuleName(file_->name()),
                      "response", service->method(j)->output_type()->name());
      if (j != service->method_count() - 1) printer_->Print("\n");
    }
    if (i != file_->service_count() - 1) printer_->Print("\n\n");
  }
}

}  // namespace sc
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

