package raft

//
// this is an outline of the API that raft must expose to
// the service (or tester). see comments below for
// each of these functions for more details.
//
// rf = Make(...)
//   create a new Raft server.
// rf.Start(command interface{}) (index, term, isleader)
//   start agreement on a new log entry
// rf.GetState() (term, isLeader)
//   ask a Raft for its current term, and whether it thinks it is leader
// ApplyMsg
//   each time a new entry is committed to the log, each Raft peer
//   should send an ApplyMsg to the service (or tester)
//   in the same server.
//

import (
	"fmt"
	"labrpc"
	"math/rand"
	"sync"
	"time"
)

// import "bytes"
// import "encoding/gob"

//
// as each Raft peer becomes aware that successive log entries are
// committed, the peer should send an ApplyMsg to the service (or
// tester) on the same server, via the applyCh passed to Make().
//
type ApplyMsg struct {
	Index       int
	Command     interface{}
	UseSnapshot bool   // ignore for lab2; only used in lab3
	Snapshot    []byte // ignore for lab2; only used in lab3
}

//
// A Go object implementing a single Raft peer.
//
type Raft struct {
	mu          sync.Mutex          // Lock to protect shared access to this peer's state
	peers       []*labrpc.ClientEnd // RPC end points of all peers
	persister   *Persister          // Object to hold this peer's persisted state
	me          int                 // this peer's index into peers[]
	heartCh     chan bool
	grantVoteCh      chan bool
	beLeaderCh  chan bool
	status      int8 // 0:follower 1:candidate 2:leader
	term        int
	voteCount   int
	voteFor     int
	log         []int
	commitIndex int
	lastApplied int
	nextIndex   []int
	matchIndex  []int
	// Your data here (2A, 2B, 2C).
	// Look at the paper's Figure 2 for a description of what
	// state a Raft server must maintain.

}

// return currentTerm and whether this server
// believes it is the leader.
func (rf *Raft) GetState() (int, bool) {
	// Your code here (2A).
	return rf.term, rf.status == 2
}

//
// save Raft's persistent state to stable storage,
// where it can later be retrieved after a crash and restart.
// see paper's Figure 2 for a description of what should be persistent.
//
func (rf *Raft) persist() {
	// Your code here (2C).
	// Example:
	// w := new(bytes.Buffer)
	// e := gob.NewEncoder(w)
	// e.Encode(rf.xxx)
	// e.Encode(rf.yyy)
	// data := w.Bytes()
	// rf.persister.SaveRaftState(data)
}

//
// restore previously persisted state.
//
func (rf *Raft) readPersist(data []byte) {
	// Your code here (2C).
	// Example:
	// r := bytes.NewBuffer(data)
	// d := gob.NewDecoder(r)
	// d.Decode(&rf.xxx)
	// d.Decode(&rf.yyy)
	if data == nil || len(data) < 1 { // bootstrap without any state?
		return
	}
}

//
// example RequestVote RPC arguments structure.
// field names must start with capital letters!
//
type RequestVoteArgs struct {
	// Your data here (2A, 2B).
	CandidateID int
	Term        int
}

//
// example RequestVote RPC reply structure.
// field names must start with capital letters!
//
type RequestVoteReply struct {
	// Your data here (2A).
	Term      int
	VoteGrant bool
}

//
// example RequestVote RPC handler.
//
func (rf *Raft) RequestVote(args *RequestVoteArgs, reply *RequestVoteReply) {
	// Your code here (2A, 2B).
	rf.mu.Lock()
	defer rf.mu.Unlock()
	if args.Term < rf.term {
		reply.VoteGrant = false
		reply.Term = rf.term
	}
	
	if args.Term>rf.term{
		rf.status=0
		rf.term=args.Term
	}

	else {
		if rf.voteFor == -1 {
			tf.voteFor=args.CandidateID
			reply.VoteGrant = true
			rf.term = args.Term
			reply.Term = rf.term
			rf.grantVoteCh <- true
		}
	}
}

//
// example code to send a RequestVote RPC to a server.
// server is the index of the target server in rf.peers[].
// expects RPC arguments in args.
// fills in *reply with RPC reply, so caller should
// pass &reply.
// the types of the args and reply passed to Call() must be
// the same as the types of the arguments declared in the
// handler function (including whether they are pointers).
//
// The labrpc package simulates a lossy network, in which servers
// may be unreachable, and in which requests and replies may be lost.
// Call() sends a request and waits for a reply. If a reply arrives
// within a timeout interval, Call() returns true; otherwise
// Call() returns false. Thus Call() may not return for a while.
// A false return can be caused by a dead server, a live server that
// can't be reached, a lost request, or a lost reply.
//
// Call() is guaranteed to return (perhaps after a delay) *except* if the
// handler function on the server side does not return.  Thus there
// is no need to implement your own timeouts around Call().
//
// look at the comments in ../labrpc/labrpc.go for more details.
//
// if you're having trouble getting RPC to work, check that you've
// capitalized all field names in structs passed over RPC, and
// that the caller passes the address of the reply struct with &, not
// the struct itself.
//
func (rf *Raft) sendRequestVote(server int, args *RequestVoteArgs, reply *RequestVoteReply) bool {
	ok := rf.peers[server].Call("Raft.RequestVote", args, reply)
	return ok
}

func election(rf *Raft) {
	rf.mu.Lock()
	rf.voteCount = 1
	rf.term++
	rf.mu.Unlock()
	for i := range rf.peers {
		if i != rf.me {
			args := RequestVoteArgs{rf.me, rf.term}
			reply := RequestVoteReply{}
			go func(i int) {
				rf.sendRequestVote(i, &args, &reply)
				fmt.Printf("raft:%d term:%d send vote request to raft:%d\n", rf.me, rf.term, i)
				rf.mu.Lock()
				rf.grantVoteCh <- true
				if reply.VoteGrant == true && rf.status == 1 {
					rf.voteCount++
					fmt.Printf("raft:%d term:%d has %d votes\n", rf.me, rf.term, rf.voteCount)
					if rf.voteCount > (len(rf.peers) / 2) {
						rf.beLeaderCh <- true
					}
				}
				rf.mu.Unlock()
			}(i)
		}
	}
}

//RequestAppendArgs
type RequestAppendArgs struct {
	//s
	Term         int
	LeaderID     int
	PrevLogIndex int
	PrevLogTerm  int
	Entries      []int
	LeaderCommit int
}

//RequestAppendReply
type RequestAppendReply struct {
	Term    int
	Success bool
}

//RequestAppend
func (rf *Raft) RequestAppend(args *RequestAppendArgs, reply *RequestAppendReply) {
	rf.heartCh <- true
}

func (rf *Raft) sendRequestAppend(server int, args *RequestAppendArgs, reply *RequestAppendReply) bool {
	ok := rf.peers[server].Call("Raft.RequestAppend", args, reply)
	return ok
}

func broadcastAppendEntries(rf *Raft) {
	for i := range rf.peers {
		if i != rf.me {
			args := RequestAppendArgs{}
			reply := RequestAppendReply{}
			go func(i int) {
				rf.sendRequestAppend(i, &args, &reply)
				//fmt.Printf("raft:%d term:%d send appendEntires to raft:%d\n", rf.me, rf.term, i)
			}(i)
		}
	}
}

//
// the service using Raft (e.g. a k/v server) wants to start
// agreement on the next command to be appended to Raft's log. if this
// server isn't the leader, returns false. otherwise start the
// agreement and return immediately. there is no guarantee that this
// command will ever be committed to the Raft log, since the leader
// may fail or lose an election.
//
// the first return value is the index that the command will appear at
// if it's ever committed. the second return value is the current
// term. the third return value is true if this server believes it is
// the leader.
//
func (rf *Raft) Start(command interface{}) (int, int, bool) {
	index := -1
	term := -1
	isLeader := true

	// Your code here (2B).

	return index, term, isLeader
}

//
// the tester calls Kill() when a Raft instance won't
// be needed again. you are not required to do anything
// in Kill(), but it might be convenient to (for example)
// turn off debug output from this instance.
//
func (rf *Raft) Kill() {
	// Your code here, if desired.
}

func randTermTime() time.Duration {
	return time.Duration(400+rand.Intn(200)) * time.Millisecond
}

//
// the service or tester wants to create a Raft server. the ports
// of all the Raft servers (including this one) are in peers[]. this
// server's port is peers[me]. all the servers' peers[] arrays
// have the same order. persister is a place for this server to
// save its persistent state, and also initially holds the most
// recent saved state, if any. applyCh is a channel on which the
// tester or service expects Raft to send ApplyMsg messages.
// Make() must return quickly, so it should start goroutines
// for any long-running work.
//
func Make(peers []*labrpc.ClientEnd, me int,
	persister *Persister, applyCh chan ApplyMsg) *Raft {
	rf := &Raft{}
	rf.peers = peers
	rf.persister = persister
	rf.me = me
	rf.heartCh = make(chan bool, 100)
	rf.grantVoteCh = make(chan bool, 100)
	rf.beLeaderCh = make(chan bool, 100)
	rf.status = 0
	rf.term = 0
	// Your initialization code here (2A, 2B, 2C).
	go func(rf *Raft) {
		for true {
			switch rf.status {
			case 0: //FOLLOWER
				select {
				case <-time.After(randTermTime()):
					rf.mu.Lock()
					fmt.Printf("raft:%d term:%d become candidate\n", rf.me, rf.term)
					rf.status = 1
					rf.mu.Unlock()
				case <-rf.heartCh:
					//fmt.Printf("raft:%d recieve heart\n", rf.me)
				case <-rf.grantVoteCh:
					//fmt.Printf("raft:%d recieve vote request\n", rf.me)
				}
			case 1: //CANDIDATE
				election(rf)
				select {
				case <-time.After(randTermTime()):
				case <-rf.beLeaderCh:
					rf.mu.Lock()
					rf.status = 2
					fmt.Printf("raft:%d term:%d become leader\n", rf.me, rf.term)
					rf.mu.Unlock()
				case <-rf.heartCh:
					rf.mu.Lock()
					rf.status = 0
					rf.mu.Unlock()
					fmt.Printf("raft:%d recieve heart change candidate to follower\n", rf.me)
				}
			case 2: //LEADER
				broadcastAppendEntries(rf)
				time.Sleep(time.Duration(200) * time.Millisecond)
			}
		}
	}(rf)

	// initialize from state persisted before a crash
	rf.readPersist(persister.ReadRaftState())

	return rf
}
