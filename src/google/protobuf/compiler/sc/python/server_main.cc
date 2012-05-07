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

ServerGenerator::ServerGenerator(const FileDescriptor* file) : file_(file) {}

ServerGenerator::~ServerGenerator() {}

void ServerGenerator::Generate(io::Printer* printer) {
  printer_ = printer;
  PrintImports();
  PrintArgparse();
  PrintRequestHandler();
  PrintApplication();
  PrintMain();
}

void ServerGenerator::PrintImports() const {
  const char tpl[] =
      "#!/usr/bin/python\n"
      "# coding: utf-8\n"
      "import argparse\n"
      "import $impl$\n"
      "import sniorfy.rpc\n"
      "import sniorfy.ioloop\n";
  printer_->Print(tpl, "impl", PythonImplName(file_->name()));
  printer_->Print("\n\n");
}

void ServerGenerator::PrintArgparse() const {
  const char tpl[] =
      "argparser = argparse.ArgumentParser(description='$name$ server', conflict_handler='resolve')\n"
      "argparser.add_argument('-p', metavar='port', dest='port', type=int, help='port number of $name$ service')\n"
      "args = argparser.parse_args()\n";
  printer_->Print(tpl, "name", StripProto(file_->name()));
  printer_->Print("\n\n");
}

void ServerGenerator::PrintRequestHandler() const {
  string impl = PythonImplName(file_->name());
  const char class_tpl[] =
      "class RequestHandler(sniorfy.rpc.RequestHandler):\n"
      "    def __init__(self, *args, **kwargs):\n"
      "        sniorfy.rpc.RequestHandler.__init__(self, *args, **kwargs)\n"
      "        self._services = {}\n";
  const char deal_tpl[] =
      "\n"
      "    def deal(self):\n"
      "        full_method = self.request.argv[0].split('.')\n"
      "        service = self._services[full_method[0]]\n"
      "        method = service.GetDescriptor().FindMethodByName(full_method[1])\n"
      "        request = service.GetRequestClass(method)()\n"
      "        request.ParseFromString(self.request.argv[1])\n"
      "        controller = None\n"
      "\n"
      "        class callbackClass(object):\n"
      "            def __init__(self):\n"
      "                self.response = None\n"
      "\n"
      "            def __call__(self, response):\n"
      "                self.response = response\n"
      "        callback = callbackClass()\n"
      "        service.CallMethod(method, controller, request, callback)\n"
      "        self.addarg(callback.response.SerializeToString())\n";
  const char service_tpl[] =
      "        s = $impl$.$service$()\n"
      "        self._services[s.GetDescriptor().name] = s\n";

  printer_->Print(class_tpl);
  for (int i = 0; i < file_->service_count(); ++i) {
    printer_->Print(service_tpl, "impl", impl, "service", file_->service(i)->name());
  }
  printer_->Print(deal_tpl);
  printer_->Print("\n\n");
}

void ServerGenerator::PrintApplication() const {
  const char tpl[] =
      "class Application(sniorfy.rpc.Application):\n"
      "    def __init__(self, request_callback):\n"
      "        sniorfy.rpc.Application.__init__(self, RequestHandler)\n";
  printer_->Print(tpl);
  printer_->Print("\n\n");
}

void ServerGenerator::PrintMain() const {
  const char tpl[] =
      "def main():\n"
      "    app = Application(RequestHandler)\n"
      "    app.listen(args.port)\n"
      "    sniorfy.ioloop.IOLoop.instance().start()\n"
      "\n"
      "\n"
      "if __name__ == '__main__':\n"
      "    main()\n";
  printer_->Print(tpl);
}

}  // namespace sc
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
