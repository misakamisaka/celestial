namespace cpp celestial

struct TaskInfo {
    1: optional i64 task_id,
    2: string group,
    3: i64 creation_time,
    4: string owner,
    5: i64 eta,
    6: string value
}

enum OwnershipMode {
    IncludeOwned,
    ExcludeOwned
}

enum PickUpPolicy {
    RANDOM,
    OLDEST
}

struct QueryRequest {
    1: string group,
    2: i32 limit,
    3: OwnershipMode own_mode
}
struct QueryResponse {
    1: bool ret_status,
    2: list<TaskInfo> tasks
}

struct GetTaskCountRequest {
    1: string group,
    2: i32 limit,
    3: OwnershipMode own_mode
}

struct GetTaskCountResponse {
    1: bool ret_status,
    2: i32 count
}

struct QueryAndOwnRequest {
    1: string group,
    2: i32 limit,
    3: PickUpPolicy policy,
    4: i32 lease_period_seconds,
    5: string worker_name
}

struct QueryAndOwnResponse {
    1: bool ret_status,
    2: list<TaskInfo> tasks
}

struct UpdateRequest {
    1: list<TaskInfo> deletions,
    2: list<TaskInfo> inserts,
    3: string worker_name
}
struct UpdateResponse {
    1: bool ret_status,
    2: list<TaskInfo> tasks
}

service ClientService {

    QueryResponse query(1: QueryRequest request),

    GetTaskCountResponse getTaskCount(1: GetTaskCountRequest request),

    QueryAndOwnResponse queryAndOwn(1: QueryAndOwnRequest request),
    
    UpdateResponse update(1: UpdateRequest request)
}
