from qorvo import rcc


def test_exposes_version():
    assert rcc.__version__
