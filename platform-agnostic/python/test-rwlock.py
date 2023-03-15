import threading

class ReadWriteLock:
    # A lock object that allows many simultaneous "read locks", but
    # only one "write lock.

    def __init__(self):
        self._read_ready = threading.Condition(threading._PyRLock())
        self._readers = 0
        
    def acquire_read(self):
    # Acquire a read lock. Blocks only if a thread has
    # acquired the write lock. 
        self._read_ready.acquire()
        try:
            self._readers += 1
        finally:
            self._read_ready.release()

    def release_read(self):
    # Release a read lock. """
        self._read_ready.acquire()
        try:
            self._readers -= 1
            if not self._readers:
                self._read_ready.notifyAll( )
        finally:
            self._read_ready.release()

    def acquire_write(self):
    # Acquire a write lock. Blocks until there are no
    # acquired read or write locks. """
        self._read_ready.acquire()
        
        while self._readers > 0 and threading.currentThread().ident != self._read_ready._lock._owner:
            self._read_ready.wait()

    def release_write(self):
    # Release a write lock. """
        self._read_ready.release()

if __name__ == '__main__':
    # create RW lock
    rw = ReadWriteLock()
    # Lock for reading 
    rw.acquire_read()
    print("Acquired reading")
    # Lock for writing 
    rw.acquire_write()
    print("Acquired writing")

    