namespace cpp celestial

enum EntryType {
    UNKNOWN,
    DATA,
    NOOP
}

struct Entry {
    1: i64 term,
    2: EntryType entry_type,
    3: binary data
}

struct AppendEntriesRequest {
    1: i64 serverid,
    2: i64 term,
    3: i64 prev_log_index,
    4: i64 prev_log_term,
    5: list<Entry> entries,
    6: i64 commit_index
}

struct AppendEntriesResponse {
    1: i64 term,
    2: bool success,
    3: optional i64 last_log_index
}

struct InstallSnapshotRequest {
    1: i64 serverid,
    2: i64 term,
    3: i64 last_included_index,
    4: i64 last_included_term,
    5: i64 offset,
    6: binary data,
    7: bool done
}

struct InstallSnapshotResponse {
    1: i64 term,
    2: i64 bytes_stored 
}

struct RequestVoteRequest {
    1: i64 serverid,
    2: i64 term,
    3: i64 last_log_index,
    4: i64 last_log_term
}

struct RequestVoteReponse {
    1: i64 term,
    2: bool granted
}

service RaftService {

    AppendEntriesResponse appendEntries(1: AppendEntriesRequest request),

    InstallSnapshotResponse installSnapshot(1: InstallSnapshotRequest request),

    RequestVoteReponse requestVote(1: RequestVoteRequest request)
}
