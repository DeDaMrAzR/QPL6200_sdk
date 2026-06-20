# flake8: noqa
# pylint: skip-file
# type: ignore
import logging
import os
import sys
from typing import List

# from pftests import pfbuilder
from testEnv.scripts import generictest, runcmd
from testsuitecommon import tac_win

from qorvo.rcc.rcc import connect_rcc_dut

projectdir = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.append(projectdir)


class tc_sanity(object):
    pass


cmd_list = ["INFO", "AN 0", "PHY BLE", "CH 15", "TX 100 10 ON"]
func_list = ["set_antenna", "set_phymode", "set_channel", "set_mode_tr_packets"]
arg_list = ["0", "BLE", 15, {"exec_mode": "ON", "packet_count": 100, "packet_interval_ms": 10}]


# =============================================================================
# Setup Logging
# =============================================================================
def configure_logging(filename):  # type: (str) -> None
    file = logging.FileHandler(filename)
    file.setLevel(logging.DEBUG)
    file.setFormatter(logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s"))

    ch = logging.StreamHandler(sys.stdout)
    # set the logging level for the console output
    ch.setLevel(logging.INFO)
    ch.setFormatter(logging.Formatter("%(name)s - %(levelname)s - %(message)s"))

    root_logger = logging.getLogger()

    # set the minimum logging level for all registered handlers
    root_logger.setLevel(logging.NOTSET)

    root_logger.addHandler(file)
    root_logger.addHandler(ch)


class SanityTestThreadModule(generictest.PyTthread):
    def __init__(self, hostname, port, coex, funclist, arglist, version=1):
        self._logfilename = "RCC.log"
        generictest.PyTthread.__init__(self, "RCCSanityTestModule", False)
        self.hostname = hostname
        self.port = port
        self.coex = coex
        self.version = version
        self.funclist = funclist
        self.arglist = arglist
        self.logger = None
        self._to_remove = []  # type: List[str]

    def Connect(self):
        pass

    def Build(self):
        pass

    def AcquireHw(self, hw):
        pass

    def GatherInfo(self):
        pass

    def BringToCleanState(self):
        for filename in self._to_remove:
            try:
                os.remove(filename)
            except OSError:
                self.logger.warning(f"Failed to remove {filename}")

    def Execute(self):
        # Define local logger, configure logging
        configure_logging(self._logfilename)
        self.logger = logging.getLogger("SanitTestThreadModule")

        rcc_driver, dut_info = connect_rcc_dut(self.hostname + ":" + self.port, self.coex, self.version)
        self.logger.info(f"Test reading info from the device: {dut_info}")

        failed = []
        for func, arg in zip(func_list, arg_list):
            func = getattr(rcc_driver, func)
            typedict = rcc_driver.get_function_typedict(func)
            if typedict:
                arg = typedict[arg]

            try:
                if arg is not None:
                    if isinstance(arg, dict):
                        func(**arg)
                    elif isinstance(arg, list):
                        func(*arg)
                    else:
                        func(arg)
                else:
                    func()
            except Exception as e:
                self.logger.error("Failed RCC module test " + func.__name__)
                self.logger.debug("Traceback:")
                self.logger.debug(str(e))
                failed.append(func.__name__)

        if len(failed):
            raise RuntimeError("Failed " + ", ".join(failed))


class SanityTestThreadCli(generictest.PyTthread):
    def __init__(self, hostname, port, coex, cmdlist, version=1):
        self._logfilename = "RCC.log"
        generictest.PyTthread.__init__(self, "RCCSanityTestCli", False)
        self.hostname = hostname
        self.port = port
        self.coex = coex
        self.version = version
        self.cmdlist = cmdlist
        self.logger = None
        self._to_remove = []  # type: List[str]

    def Connect(self):
        pass

    def Build(self):
        pass

    def AcquireHw(self, hw):
        pass

    def GatherInfo(self):
        pass

    def BringToCleanState(self):
        for filename in self._to_remove:
            try:
                os.remove(filename)
            except OSError:
                self.logger.warn(f"Failed to remove {filename}")

    def Execute(self):
        # Define local logger, configure logging
        configure_logging(self._logfilename)
        self.logger = logging.getLogger("SanitTestThreadCli")

        test_script = "test_script_cli.txt"
        self._to_remove.append(test_script)

        command = "python3 {}".format(os.path.join(projectdir, "rcc", "rcc.py"))
        failed = []
        for cmd in self.cmdlist:
            with open(test_script, "w") as f:
                f.write(cmd + "\n")
                f.write("EXIT" + "\n")

            # Run RCC command
            logcmd = []
            args = _get_cli_args(self.hostname, self.port, self.coex, self.version, test_script)

            try:
                _invoke(logcmd.append, command, *args)
                if "error" in "\n".join(logcmd).lower():
                    raise
            except Exception as e:
                self.logger.error("Failed RCC cli test " + cmd)
                self.logger.debug("Traceback:")
                self.logger.debug(str(e))
                failed.append(cmd)

        if len(failed):
            raise RuntimeError("Failed " + ", ".join(failed))


class SanityTestThreadExe(generictest.PyTthread):
    def __init__(self, hostname, port, coex, cmdlist, version=1):
        self._logfilename = "RCC.log"
        generictest.PyTthread.__init__(self, "RCCSanityTestExe", False)
        self.hostname = hostname
        self.port = port
        self.coex = coex
        self.version = version
        self.cmdlist = cmdlist
        self.logger = None
        self._to_remove = []  # type: List[str]

    def Connect(self):
        pass

    def Build(self):
        pass

    def AcquireHw(self, hw):
        pass

    def GatherInfo(self):
        pass

    def BringToCleanState(self):
        for filename in self._to_remove:
            try:
                os.remove(filename)
            except OSError:
                self.logger.warn(f"Failed to remove {filename}")

    def Execute(self):
        # Define local logger, configure logging
        configure_logging(self._logfilename)
        self.logger = logging.getLogger("SanitTestThreadExe")

        test_script = "test_script_exe.txt"
        self._to_remove.append(test_script)

        command = os.path.join(projectdir, "bin", "rcc.exe")
        failed = []
        for cmd in self.cmdlist:
            with open(test_script, "w") as f:
                f.write(cmd + "\n")
                f.write("EXIT" + "\n")

            # Run RCC command
            logcmd = []
            args = _get_cli_args(self.hostname, self.port, self.coex, self.version, test_script)

            try:
                _invoke(logcmd.append, command, *args)
                if "error" in "\n".join(logcmd).lower():
                    raise
            except Exception as e:
                self.logger.error("Failed RCC exe test " + cmd)
                self.logger.debug("Traceback:")
                self.logger.debug(str(e))
                failed.append(cmd)

        if len(failed):
            raise RuntimeError("Failed " + ", ".join(failed))


# The actual test case
class SanityTestModule(generictest.GenericTest, tc_sanity, tac_win):
    def Init(self):
        self.add_tthread(
            SanityTestThreadModule(hostname="QTT1-0002", port="9190", coex=False, funclist=func_list, arglist=arg_list)
        )
        self.timeout = 60

    def AcquireHw(self):
        pass


class SanityTestCli(generictest.GenericTest, tc_sanity, tac_win):
    def Init(self):
        self.add_tthread(SanityTestThreadCli(hostname="QTT1-0002", port="9190", coex=False, cmdlist=cmd_list))
        self.timeout = 60

    def AcquireHw(self):
        pass


class SanityTestExe(generictest.GenericTest, tc_sanity, tac_win):
    def Init(self):
        self.add_tthread(SanityTestThreadExe(hostname="QTT1-0002", port="9190", coex=False, cmdlist=cmd_list))
        self.timeout = 60

    def AcquireHw(self):
        pass


# ==============================================================================
# Command invoker function
# ==============================================================================
def _invoke(logcmd, command, *args):
    """Base command execution
    Wrapper default configuration is to raise exceptions when the command
        has a return code != 0. This behavior can be changed to return the
        rc code instead.
    """
    if not isinstance(command, list):
        command = [command]
    command.extend(args)

    runcmd.RunCmd(
        cmd=" ".join(command),
        logcmd=logcmd,
        cwd=os.getcwd(),
        expectedrc=None,
        logger=None,
    )
    return 0


def _get_cli_args(hostname, port, coex, version, test_script):
    args = [f"-e {hostname}", f"-p {port}", f"-v {version}", f"-s {test_script}"]

    if coex:
        args.append("--coex")

    return args
