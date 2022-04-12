#pragma once

#include <mutex>
#include <condition_variable>

// Implemetation based on here:
// https://sandbox.mc.edu/~bennet/cs422b/notes/semaphore_h.html

class semaphore {
    public:
        semaphore(int init): m_value(init) { }

        void aquire()
        {
            std::unique_lock<std::recursive_mutex> lck(m_mutex);

            if (--m_value < 0)
            {
                m_waitcond.wait(lck);
            }
        }

        void release()
        {
            std::unique_lock<std::recursive_mutex> lck(m_mutex);
            if (++m_value <= 0) m_waitcond.notify_one();
        }

        /* Returns true if acquired semaphore, and false if it
         * didn't.
         * It's a safe implementation, meaning that it may return
         * false even if it was possible to acquire, but it never
         * actually tries to acquire the semaphore if it's not
         * possible.
         */
        bool try_acquire()
        {
            // create unique_lock object, but don't aquire mutex
            std::unique_lock<std::recursive_mutex> lck(m_mutex, std::defer_lock);
            if (lck.try_lock()) 
            {
                if (m_value - 1 < 0)
                {
                    return false;
                } else {
                    aquire();
                    return true;
                }
            }
            return false;
        }

    private:
        int m_value;                            // Value of semaphore.
        std::recursive_mutex m_mutex;           // Controls access.
        std::condition_variable_any m_waitcond; // Controls waiting and restart
        // we must use the "any" version to work with recursive_mutex
};
