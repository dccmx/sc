sc - server compiler
===================

sc is a tool that compiles protocol buffer definitions to runnable server code and client library.

example (a simple echo service):

    option py_generic_services = true;
    
    message EchoRequest {
      required string text = 1;
    };
    
    message EchoResponse {
      required string text = 1;
    };
    
    service EchoService {
      rpc Echo(EchoRequest) returns (EchoResponse);
    };

Run `sc --server --python --out=echo-server echo.proto` to generate server side code

Run `sc --client --python --out=echo-client echo.proto` to generate client side code

Then sc will generate a empty server implementation file named echo.py in echo-server/echoServer. it might looks like this:
    #!/usr/bin/python
    # coding: utf-8
    import echo_pb2
    
    
    class EchoService(echo_pb2.EchoService):
        def Echo(self, rpc_controller, request, done):
            # TODO implement method here to finish the server
            # example:
            #   response = echo_pb2.EchoResponse()
            #   do some thing to fill the response
            #   done(response)
            pass

Just finish the TODOs to complete the server. 

Then run `python setup.py install` to deploy the server. and deliver the echo-client to your clients.

That all. Life is not so hard.
