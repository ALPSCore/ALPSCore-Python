#!/usr/bin/python
# Python caller for the test for pyobj_interface (to be run via py.test)

import pyobjtest_c

def test_pyseq():
    result=pyobjtest_c.test_pyseq(10,20,30)
    assert result=="OK"

def test_bool():
    assert pyobjtest_c.test_bool(True)=="OK"

def test_int():
    assert pyobjtest_c.test_int(123)=="OK"

def test_long():
    assert pyobjtest_c.test_long(456L)=="OK"

def test_float():
    assert pyobjtest_c.test_float(43.25)=="OK"

def test_string():
    assert pyobjtest_c.test_string("hello, world!")=="OK"

