#include <CORBA.h>
#include <deque>
#include <string>
#include <unistd.h>


using namespace std;

MICOMT::Mutex stream_mutex;

class Queue
    : public MICOMT::Mutex
{
    deque<long> queue_;
    MICOMT::CondVar condition_;
public:
    Queue()
	: Mutex(FALSE, MICOMT::Mutex::Recursive), condition_(this) 
    {}

    void 
    push(long val)
    {
	MICOMT::AutoLock t_lock(*this);
	queue_.push_front(val);
	condition_.signal();
    }

    long
    pop()
    {
	MICOMT::AutoLock t_lock(*this);
	while (queue_.size() == 0) {
	    condition_.wait();
	}
	long ret = queue_.back();
	queue_.pop_back();
	return ret;
    }
};

Queue gqueue;

class Producer
    : public MICOMT::Thread
{
    string name_;
    long from_;
    long howmuch_;
public:
    Producer(string name, long from, long how_much)
	: name_(name), from_(from), howmuch_(how_much)
    {}
    ~Producer()
    {}

    void _run(void*)
    {
	for (long i = from_; i < from_ + howmuch_; i++) {
	    gqueue.push(i);
	    stream_mutex.lock();
	    cout << "Producer(" << name_ << ") pushs <- " << i << endl;
	    stream_mutex.unlock();
#ifndef _WIN32
	    sleep(1);
#else // _WIN32
	    _sleep(1);
#endif // _WIN32
	}
    }
};

class Consumer
    : public MICOMT::Thread
{
    string name_;
public:
    Consumer(string name)
	: name_(name)
    {}

    void _run(void*)
    {
	for (;;) {
	    int val = gqueue.pop();
	    stream_mutex.lock();
	    cout << "Consumer(" << name_ << ") consumes: " << val << endl;
	    stream_mutex.unlock();
	}
    }
};

int
main(int argc, char* argv[])
{
  Producer* prod1 = new Producer("p1", 1, 100);
  Producer* prod2 = new Producer("p2", 200, 100);
  Consumer* cons1 = new Consumer("c1");
  Consumer* cons2 = new Consumer("c2");
  Consumer* cons3 = new Consumer("c3");

  cons1->start();
  cons2->start();
  cons3->start();
  prod1->start();
  prod2->start();

  for (;;) {
      // infinite loop 
#ifndef _WIN32
      sleep(1);
#else // _WIN32
      _sleep(1);
#endif // _WIN32
  }
  return 0;
}
