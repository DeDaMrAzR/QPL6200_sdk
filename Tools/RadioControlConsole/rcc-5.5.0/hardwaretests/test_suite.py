# flake8: noqa
# pylint: skip-file
# type: ignore
import os
import sys

import pftests
import regCheckTestsPy
from regCheckTestsPy import *
from testsuitecommon import (
    PythonUnitTestRunnerTest,
    initCommonGroups,
    leaveclasses,
    tac_unit,
    tac_win,
)

testsuite_name = "RCC regression"
testsuite_mailto = ["LPW.producttestsupport@qorvo.com"]

pftests.projectdir = os.path.join(os.path.dirname(__file__), "..")
pftests.rootdir = os.path.abspath(os.path.join(pftests.projectdir, "..", "..", ".."))
pftests.testdir = os.path.join(pftests.rootdir, "Components", "Qorvo", "Test", "vlatest", "test")
sys.path.append(pftests.testdir)


# =============================================================================
# Unit Test Cases
# =============================================================================
class PythonUnitTests(PythonUnitTestRunnerTest, tac_unit, tac_win):
    discovery_root = pftests.projectdir
    timeout = 600

    def __init__(self):
        super(PythonUnitTests, self).__init__()


from sanity_tests import tc_sanity  # noqa F401

tg_sanity = set(leaveclasses(tc_sanity))

# ============================================================================
# Tests - Register Dump
# ============================================================================

moduleroot = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
rcc_executable = "python3 " + os.path.join(moduleroot, "rcc", "rcc.py")
regCheckTestsPy.update_RCC_exe(rcc_executable)

# ============================================================================
# Test Groups
# ============================================================================
# Populate groups with our new tests
allgroups = initCommonGroups()
regCheckTestsPy.add_test_groups(allgroups)
