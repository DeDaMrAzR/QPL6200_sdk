import threading
import time
import traceback
from typing import Any, Callable


class ProcessThread:
    """code running in a python thread"""

    def __init__(self, function: Callable[[], None], name: str = ""):
        """Initializer for ProcessThread class

        :param function: Function to execute in this thread.
                         Function expected to have no required parameters.
                         func()
        :type function: function
        :param name: Name of the Thread
        :type name: str
        """
        self.verbose = False
        self.jointimeout = 5.0
        self.sleeptime = 0.005

        self.function = function
        self.name = name
        self.thr = None

        self.StopIndication = True
        self.StopRequest = True
        self.exception = None

    def Start(self):
        if self.thr is None:
            self.daemon = False
            self.StopIndication = False  # TestDevice -> #Test: test has stopped
            self.StopRequest = False  # Test->TestDevice
            self.thr = threading.Thread(target=self._Execute)
            # by setting this thread as a daemon,
            # a keyboard interrupt (that only kills the main thread)
            # will kill the complete application
            self.thr.setDaemon(True)
            self.exception = None

            self.thr.start()
            self.log("Thread started")
        else:
            self.log("Thread already running")

    def Execute(self):
        while self.StopRequest is False:
            time.sleep(self.sleeptime)  # Don't overload the processor by adding some delay
            self.function()

    def _Execute(self):
        # apparently, an exception will not stop the test.
        # we need to catch it and pass to the main thread.

        try:
            self.Execute()
            self.log("ProcessThread exited normally")
        except Exception as e:
            traceback.print_exc()
            self.exception = e
            self.log("ProcessThread exited with exception")

        self.StopIndication = True

    def Stop(self):
        self.log("Stopping")
        if (self.thr is not None) and (self.thr.is_alive()):
            self.StopRequest = True
            self.thr.join(self.jointimeout)
            if self.thr.is_alive():
                print("%s:could not stop ProcessThread retrying!" % (self.name))
                self.thr.join(5 * self.jointimeout)
                if self.thr.is_alive():
                    self.exception = Exception("%s:could not stop ProcessThread!" % (self.name))
                    print("%s:could not stop ProcessThread!" % (self.name))
        else:
            # in case a process was killed or crashed
            self.StopIndication = True
        self.thr = None

    def IsDone(self):
        if self.exception is not None:
            raise RuntimeError("%s:ProcessThread has thrown an exception: %s" % (self.name, self.exception))
        return self.StopIndication

    def log(self, s: Any) -> None:
        if self.verbose:
            print(self.name + ": " + str(s))
