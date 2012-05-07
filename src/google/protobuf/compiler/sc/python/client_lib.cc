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

ClientlibGenerator::ClientlibGenerator(const FileDescriptor* file) : file_(file) {}

ClientlibGenerator::~ClientlibGenerator() {}

void ClientlibGenerator::Generate(io::Printer* printer) {
  printer_ = printer;
  PrintImports();
  PrintClient();
  PrintChannel();
}

void ClientlibGenerator::PrintImports() const {
  const char tpl[] =
      "#!/usr/bin/python\n"
      "# coding: utf-8\n"
      "import stpclient\n"
      "import google.protobuf.service\n"
      "import $module$\n";
  printer_->Print(tpl, "module", PythonModuleName(file_->name()));
  printer_->Print("\n\n");
}

void ClientlibGenerator::PrintClient() const {
  const char class_tpl[] =
      "class $package$(object):\n"
      "    '''\n"
      "    example:\n"
      "        client = $package$('localhost', 3390)\n"
      "        req = $module$.XXXRequest()\n"
      "        prepare req\n"
      "        resp = client.XXXService.method(req)\n"
      "    '''\n"
      "    class _Client(object):\n"
      "        def __init__(self, stub):\n"
      "            self._stub = stub\n"
      "\n"
      "        def __getattr__(self, key):\n"
      "            def call(method, request):\n"
      "                class callbackClass(object):\n"
      "                    def __init__(self):\n"
      "                        self.response = None\n"
      "\n"
      "                    def __call__(self, response):\n"
      "                        self.response = response\n"
      "                controller = None  # Controller()\n"
      "                callback = callbackClass()\n"
      "                method(controller, request, callback)\n"
      "                return callback.response\n"
      "            return lambda request: call(getattr(self._stub, key), request)\n"
      "\n"
      "    def __init__(self, host, port):\n"
      "        self._stubs = {}\n"
      "        channel = STPChannel(host, port)\n";
  printer_->Print(class_tpl,
                  "package", PythonPackageName(file_->name()),
                  "module", PythonModuleName(file_->name()));

  for (int i = 0; i < file_->service_count(); ++i) {
    const char regist_service_tpl[] =
        "        s = $module$.$service$_Stub(channel)\n"
        "        self._stubs[s.GetDescriptor().name] = self._Client(s)\n";
    printer_->Print(regist_service_tpl,
                    "module", PythonModuleName(file_->name()),
                    "service", file_->service(i)->name());
  }

  const char getattr_tpl[] =
      "\n"
      "    def __getattr__(self, key):\n"
      "        return self._stubs[key]\n";
  printer_->Print(getattr_tpl);
  printer_->Print("\n\n");
}

void ClientlibGenerator::PrintChannel() const {
  const char tpl[] =
      "class STPChannel(google.protobuf.service.RpcChannel):\n"
      "    def __init__(self, host, port):\n"
      "        google.protobuf.service.RpcChannel.__init__(self)\n"
      "        self._client = stpclient.Client(host, port)\n"
      "\n"
      "    def CallMethod(self, methodDescriptor, rpcController, request, responseClass, done=None):\n"
      "        full_method = methodDescriptor.containing_service.name + '.' + methodDescriptor.name\n"
      "        data = self._client.call([full_method, request.SerializeToString()])[0]\n"
      "        response = responseClass()\n"
      "        response.ParseFromString(data)\n"
      "        done(response)\n";
  printer_->Print(tpl);
}

}  // namespace sc
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

