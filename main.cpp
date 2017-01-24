#include <glog/logging.h>
#include <gflags/gflags.h>
#include "signal.h"
#include "tm_exception.h"
#include "skeleton.h"

using std::string;
using task_manager::Skeleton;

int main(int argc, char** argv){
    google::SetVersionString(VERSION);
    google::ParseCommandLineFlags(&argc, &argv, false);
    google::InitGoogleLogging(argv[0]);
    FLAGS_max_log_size = 128;
    FLAGS_stop_logging_if_full_disk = true;
    FLAGS_logbufsecs = 0; 
    FLAGS_logtostderr = false;
    FLAGS_alsologtostderr = false;
    FLAGS_log_dir = "";
    google::SetLogDestination(google::GLOG_INFO, "log/tm_info_log.");
    google::SetLogDestination(google::GLOG_WARNING, "log/tm_warning_log.");
    google::SetLogDestination(google::GLOG_ERROR, "log/tm_error_log.");
    signal(SIGPIPE, SIG_IGN);

    Skeleton skeleton;
    try {
        skeleton.init();
        skeleton.run();
    } catch (task_manager::TMException &e) {
        LOG(ERROR) << "tm error:" << e.what();
        exit(1);
    } catch (std::exception &e) {
        LOG(ERROR) << "std error:" << e.what();
        exit(1);
    }

    sleep(1);
    return 0;
}

