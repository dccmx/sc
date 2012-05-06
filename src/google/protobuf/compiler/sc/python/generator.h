#ifndef GOOGLE_PROTOBUF_COMPILER_SC_PYTHON_GENERATOR_H__
#define GOOGLE_PROTOBUF_COMPILER_SC_PYTHON_GENERATOR_H__ 

#include <google/protobuf/stubs/common.h>

namespace google {
namespace protobuf {

class ServiceDescriptor;        // descriptor.h
class FileDescriptor;        // descriptor.h
namespace io { class Printer; }            // printer.h

namespace compiler {
namespace sc {

// sever side
class ServerGenerator {
 public:
  ServerGenerator(const FileDescriptor* file);
  ~ServerGenerator();

  void Generate(io::Printer* printer);

 private:
  void PrintImports() const;
  void PrintArgparse() const;
  void PrintRequestHandler() const;
  void PrintApplication() const;
  void PrintMain() const;
  void GenerateImplementationTemplate(io::Printer *printer) const;

 private:
  mutable const FileDescriptor* file_;
  mutable io::Printer* printer_;
};

class ImplementationGenerator {
 public:
  ImplementationGenerator(const FileDescriptor* file);
  ~ImplementationGenerator();

  void Generate(io::Printer* printer);

 private:
  void PrintImports() const;
  void PrintServices() const;

 private:
  mutable const FileDescriptor* file_;
  mutable io::Printer* printer_;
};

class ServerSetupGenerator {
 public:
  ServerSetupGenerator(const FileDescriptor* file);
  ~ServerSetupGenerator();

  void Generate(io::Printer* printer);

 private:
  mutable const FileDescriptor* file_;
  mutable io::Printer* printer_;
};

class ClientSetupGenerator {
 public:
  ClientSetupGenerator(const FileDescriptor* file);
  ~ClientSetupGenerator();

  void Generate(io::Printer* printer);

 private:
  mutable const FileDescriptor* file_;
  mutable io::Printer* printer_;
};

// client side
class ClientlibGenerator {
 public:
  ClientlibGenerator(const FileDescriptor* file);
  ~ClientlibGenerator();

  void Generate(io::Printer* printer);

 private:
  void PrintImports() const;
  void PrintClient() const;
  void PrintChannel() const;

 private:
  mutable const FileDescriptor* file_;
  mutable io::Printer* printer_;
};

class ClientInitGenerator {
 public:
  ClientInitGenerator(const FileDescriptor* file);
  ~ClientInitGenerator();

  void Generate(io::Printer* printer);

 private:
  void PrintImports() const;

 private:
  mutable const FileDescriptor* file_;
  mutable io::Printer* printer_;
};

}  // namespace sc
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
#endif  // GOOGLE_PROTOBUF_COMPILER_SC_PYTHON_GENERATOR_H__ 
