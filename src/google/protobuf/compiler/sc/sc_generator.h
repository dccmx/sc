#ifndef GOOGLE_PROTOBUF_COMPILER_SC_GENERATOR_H
#define GOOGLE_PROTOBUF_COMPILER_SC_GENERATOR_H 

#include <string>
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/compiler/python/python_generator.h>

namespace google {
namespace protobuf {

class FileDescriptor;

namespace io { class Printer; }

namespace compiler {

class OutputDirectory;

namespace python { class Generator; }

namespace sc {

using std::string;

class PythonServiceGenerator : public CodeGenerator {
 public:
  PythonServiceGenerator();
  virtual ~PythonServiceGenerator();

  virtual bool Generate(const FileDescriptor* file,
                        const string& parameter,
                        OutputDirectory* generator_context,
                        string* error) const;

 private:
  python::Generator python_generator_;
};

class PythonClientGenerator : public CodeGenerator {
 public:
  PythonClientGenerator();
  virtual ~PythonClientGenerator();

  virtual bool Generate(const FileDescriptor* file,
                        const string& parameter,
                        OutputDirectory* generator_context,
                        string* error) const;

 private:
  mutable const FileDescriptor* file_;
  mutable io::Printer* printer_;

  python::Generator python_generator_;
};

}  // namespace sc
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

#endif  // GOOGLE_PROTOBUF_COMPILER_SC_GENERATOR_H 
