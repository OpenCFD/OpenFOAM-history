#ifndef __SIGHANDLER_H__
#define __SIGHANDLER_H__

namespace MICOMT {
  class SigHandler : public MICOMT::Thread {
  private:
     sigset_t old_mask, new_mask;
  public:
    typedef enum { Terminate, Continue } Action;

    SigHandler () {
      sigemptyset (&new_mask);
    }

    inline void AddSignal (MICO_Long sig) {
      sigaddset (&new_mask, sig);
    }

    inline void DelSignal (MICO_Long sig) {
      sigdelset (&new_mask, sig);
    }

    virtual void _run (void *arg) {
      int sig;

      do
      {
        pthread_sigmask (SIG_BLOCK, &new_mask, &old_mask);
        sigwait (&new_mask, &sig);

        pthread_sigmask (SIG_UNBLOCK, &old_mask, &new_mask);

      } while (sighandler (sig, arg) == Continue);
    }

    virtual SigHandler::Action sighandler (int sig, void *arg) = 0;
  };
}; // MICOMT

#endif // __SIGHANDLER_H__
