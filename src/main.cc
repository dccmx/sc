#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sys/stat.h>

#include <google/protobuf/compiler/sc/sc_generator.h>
#include <google/protobuf/compiler/sc/sc_helpers.h>
#include <google/protobuf/compiler/command_line_interface.h>
#include <google/protobuf/stubs/common.h>

#include "config.h"

using namespace std;
using google::protobuf::internal::VersionString;

void Usage(const char *program_name) {
  cerr <<
"Usage: " << program_name << " [OPTION] [PROTOC OPTION] PROTO_FILES\n"
"Parse PROTO_FILES and generate output based on the options given:\n"
"  --python                    Generate python code\n"
"  --server                    Generate server side code \n"
"  --client                    Generate client side code \n"
"  --out=DIR                   Output directory of generated code\n"
"  --version                   Show version info and exit.\n"
"  -h, --help                  Show this text and exit.\n"
"for protoc options please refer to `protoc -h`" << endl;

  exit(0);
}

void Version() {
  cout<< "sc "
      << VERSION " (with libprotoc "
      << VersionString(GOOGLE_PROTOBUF_VERSION)
      << ")"
      <<endl;
  exit(0);
}


bool PrepareDir(string out_dir, string package_dir) {
  if (access(out_dir.c_str(), W_OK)) {
    if (mkdir(out_dir.c_str(), 0755)) return false;
  }
  if (access(package_dir.c_str(), W_OK)) {
    if (mkdir(package_dir.c_str(), 0755)) return false;
  }
  return true;
}

int cp(const char *from, const char *to) {
  int fd_to, fd_from;
  char buf[4096];
  ssize_t nread;
  int saved_errno;

  fd_from = open(from, O_RDONLY);
  if (fd_from < 0) return -1;

  fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
  if (fd_to < 0) goto out_error;

  while (nread = read(fd_from, buf, sizeof buf), nread > 0) {
    char *out_ptr = buf;
    ssize_t nwritten;

    do {
      nwritten = write(fd_to, out_ptr, nread);

      if (nwritten >= 0) {
        nread -= nwritten;
        out_ptr += nwritten;
      } else if (errno != EINTR) {
        goto out_error;
      }
    } while (nread > 0);
  }

  if (nread == 0) {
    if (close(fd_to) < 0) {
      fd_to = -1;
      goto out_error;
    }
    close(fd_from);

    /* Success! */
    return 0;
  }

out_error:
  saved_errno = errno;
  close(fd_from);
  if (fd_to >= 0) close(fd_to);
  errno = saved_errno;
  return -1;
}

void MoveSetup(string from, string to) {
  if (access(to.c_str(), F_OK)) {
    cp(from.c_str(), to.c_str());
  }
  unlink(from.c_str());
}

int main(int argc, char **argv) {
  vector<const char*> proto_argv;
  string side, lang, out_dir, type, package_dir, proto_file;
  google::protobuf::compiler::CommandLineInterface cli;

  proto_argv.push_back(argv[0]);
  proto_argv.push_back(NULL);  // reserve position for type

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--python") == 0 || strcmp(argv[i], "--cpp") == 0) {
      lang = argv[i];
    } else if (strcmp(argv[i], "--server") == 0 || strcmp(argv[i], "--client") == 0) {
      side = argv[i] + 2;
    } else if (strncmp(argv[i], "--out", 5) == 0) {
      out_dir = argv[i] + 6;
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      Usage(argv[0]);
    } else if (strcmp(argv[i], "--version") == 0) {
      Version();
    } else {
      proto_argv.push_back(argv[i]);
    }
  }

  if (argc < 5) Usage(argv[0]);

  proto_file = proto_argv.back();
  package_dir = out_dir + "/" + StripProto(proto_file);
  if (side == "server") {
    package_dir += "Server";
  }
  if (!PrepareDir(out_dir, package_dir)) {
    cerr << "can not create dir\n";
    exit(-1);
  }

  string proto_copy = out_dir + "/" + StripPath(proto_file);
  cp(proto_file.c_str(), proto_copy.c_str());

  type = lang + "_" + side + "_out=dir=" + package_dir + ":" + package_dir;
  proto_argv[1] = type.c_str();

  google::protobuf::compiler::sc::PythonServerGenerator python_service_generator;
  cli.RegisterGenerator("--python_server_out", &python_service_generator, "Generate server source files.");

  google::protobuf::compiler::sc::PythonClientGenerator python_client_generator;
  cli.RegisterGenerator("--python_client_out", &python_client_generator, "Generate client source files.");

  if (cli.Run(proto_argv.size(), proto_argv.data())) {
    return 1;
  }

  MoveSetup(package_dir + "/setup.py", out_dir + "/setup.py");

  return 0;
}
