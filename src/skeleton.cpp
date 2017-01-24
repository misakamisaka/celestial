#include "skeleton.h"
#include <fstream>
#include <iterator>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <glog/logging.h>
#include "query_handler.h"
#include "update_handler.h"
#include "task_manager_handler.h"
#include "base_journal_manager.h"
#include "base_checkpoint_manager.h"
#include "file_journal_manager.h"
#include "file_checkpoint_manager.h"
#include "tm_exception.h"

using std::string;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using ::apache::thrift::server::TThreadPoolServer;
using apache::thrift::concurrency::ThreadManager;
using apache::thrift::concurrency::PosixThreadFactory;

namespace task_manager {

const static std::string CONF_FILENAME = "conf/task_manager.conf";

void Skeleton::init() {

    std::ifstream conf_ifs(CONF_FILENAME);
    std::string conf_str((std::istreambuf_iterator<char>(conf_ifs)), std::istreambuf_iterator<char>());
    Json::Value conf;
    Json::Reader reader;
    if (!reader.parse(conf_str, conf)) {
        throw TMException("conf parse failed");
    }
    
    LOG(INFO) << "tm config parse ok";

    Json::Value journal_conf = conf["journal"];
    string journal_type = journal_conf["type"].asString();

    std::unique_ptr<BaseJournalManager> journal_manager_ptr;
    if (journal_type == "file") {
        journal_manager_ptr.reset(new FileJournalManager);
    }
    if (!journal_manager_ptr) {
        throw TMException("unsupported journal type:" + journal_type);
    }
    journal_manager_ptr->init(journal_conf);
    
    LOG(INFO) << "journal manager[type:" << journal_type << "] init ok";

    Json::Value checkpoint_conf = conf["checkpoint"]; 
    string checkpoint_type = checkpoint_conf["type"].asString();

    if (checkpoint_type == "file") {
        checkpoint_manager_.reset(new FileCheckpointManager);
    }

    if (!journal_manager_ptr) {
        throw TMException("unsupported checkpoint type:" + checkpoint_type);
    }
    checkpoint_manager_->init(checkpoint_conf);
    checkpoint_manager_->set_journal_manager(std::move(journal_manager_ptr));
    checkpoint_manager_->load();

    LOG(INFO) << "checkpoint manager[type:" << checkpoint_type << "] init ok";

    std::shared_ptr<TaskCollection> task_collection = checkpoint_manager_->buildTaskCollection();
    
    std::shared_ptr<QueryHandler> query_handler = std::make_shared<QueryHandler>();
    std::shared_ptr<UpdateHandler> update_handler = std::make_shared<UpdateHandler>();;
    query_handler->init(task_collection);
    update_handler = std::make_shared<UpdateHandler>();
    update_handler->init(task_collection, checkpoint_manager_);


    uint64_t check_interval = conf["check_interval"].asUInt64();
    eta_checker_ = std::make_shared<ETAChecker>();
    eta_checker_->init(check_interval, task_collection, update_handler);

    port_ = conf["port"].asUInt();

    boost::shared_ptr<TaskManagerHandler> handler(new TaskManagerHandler(query_handler, update_handler));
    boost::shared_ptr<TProcessor> processor(new TaskManagerProcessor(handler));
    boost::shared_ptr<TServerTransport> server_transport(new TServerSocket(port_));
    boost::shared_ptr<TTransportFactory> transport_factory(new TBufferedTransportFactory());
    boost::shared_ptr<TProtocolFactory> protocol_factory(new TBinaryProtocolFactory());

    boost::shared_ptr<ThreadManager> thread_manager = ThreadManager::newSimpleThreadManager(20);
    boost::shared_ptr<PosixThreadFactory> thread_factory(new PosixThreadFactory());
    thread_manager->threadFactory(thread_factory);
    thread_manager->start();

    server_ = std::make_shared<TThreadPoolServer>(processor, server_transport, transport_factory, protocol_factory, thread_manager);
}

void Skeleton::run() {
    checkpoint_manager_->start();
    eta_checker_->start();
    server_->serve();
}
}
