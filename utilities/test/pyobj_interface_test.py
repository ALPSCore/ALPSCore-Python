#!/usr/bin/python
# Python caller for the test for pyobj_interface (to be run via py.test)

import pyobjtest_c

def test_pyseq():
    result=pyobjtest_c.test_pyseq(10,20,30)
    assert result=="OK"

# Testing scalars

def test_bool():
    assert pyobjtest_c.test_bool(True)=="OK"
    assert pyobjtest_c.test_bool(False)=="True expected, got False"
    assert pyobjtest_c.test_bool(123)=="Not boolean"


def test_int():
    assert pyobjtest_c.test_int(123)=="OK"
    assert pyobjtest_c.test_int(0)=="Expected 123, got 0"
    assert pyobjtest_c.test_int(12.5)=="Not integer"

def test_long():
    assert pyobjtest_c.test_long(456L)=="OK"

def test_float():
    assert pyobjtest_c.test_float(43.25)=="OK"

def test_string():
    assert pyobjtest_c.test_string("Hello, world!")=="OK"

# Testing vectors

def test_vbool():
    assert pyobjtest_c.test_vbool((True,False))=="OK"

def test_vint():
    assert pyobjtest_c.test_vint((123, 456))=="OK"

def test_vlong():
    assert pyobjtest_c.test_vlong((456L, 123L))=="OK"

def test_vfloat():
    assert pyobjtest_c.test_vfloat((43.25, 12.5))=="OK"

def test_vstring():
    assert pyobjtest_c.test_vstring(("Hello", "world!"))=="OK"
