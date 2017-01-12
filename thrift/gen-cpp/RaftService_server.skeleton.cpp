// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "RaftService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::celestial;

class RaftServiceHandler : virtual public RaftServiceIf {
 public:
  RaftServiceHandler() {
    // Your initialization goes here
  }

  void appendEntries(AppendEntriesResponse& _return, const AppendEntriesRequest& request) {
    // Your implementation goes here
    printf("appendEntries\n");
  }

  void installSnapshot(InstallSnapshotResponse& _return, const InstallSnapshotRequest& request) {
    // Your implementation goes here
    printf("installSnapshot\n");
  }

  void requestVote(RequestVoteReponse& _return, const RequestVoteRequest& request) {
    // Your implementation goes here
    printf("requestVote\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<RaftServiceHandler> handler(new RaftServiceHandler());
  shared_ptr<TProcessor> processor(new RaftServiceProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

