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
    assert pyobjtest_c.test_long(456)=="Not long integer"
    assert pyobjtest_c.test_long(0L)=="Expected 456, got 0"

def test_float():
    assert pyobjtest_c.test_float(43.25)=="OK"
    assert pyobjtest_c.test_float(0)=="Not float"
    assert pyobjtest_c.test_float(0.0)=="Expected 43.25, got 0"

def test_string():
    assert pyobjtest_c.test_string("Hello, world!")=="OK"
    assert pyobjtest_c.test_string(0)=="Not string"
    assert pyobjtest_c.test_string("hi")=='Expected "Hello, world!", got "hi"'

# Testing vectors

def test_vbool():
    assert pyobjtest_c.test_vbool((True,False))=="OK"
    assert pyobjtest_c.test_vbool((False,True))=="Expected True as 1st element"
    assert pyobjtest_c.test_vbool((True,True))=="Expected False as 2nd element"
    assert pyobjtest_c.test_vbool((True,))=="Wrong size: 1"
    assert pyobjtest_c.test_vbool((True,0))=="Not boolean vector"
    assert pyobjtest_c.test_vbool(True)=="Not boolean vector"

def test_vint():
    assert pyobjtest_c.test_vint((123, 456))=="OK"
    assert pyobjtest_c.test_vint((123, 456, 789))=="Wrong size: expected 2, got 3"
    assert pyobjtest_c.test_vint((123, 123))=="Wrong vector"
    assert pyobjtest_c.test_vint((123, 12.25))=="Not integer vector"

def test_vlong():
    assert pyobjtest_c.test_vlong((456L, 123L))=="OK"
    assert pyobjtest_c.test_vlong((456L, 123L, 0L))=="Wrong size: expected 2, got 3"
    assert pyobjtest_c.test_vlong((1L, 2L))=="Wrong vector"
    assert pyobjtest_c.test_vlong((456L, 0))=="Not long vector"

def test_vfloat():
    assert pyobjtest_c.test_vfloat((43.25, 12.5))=="OK"
    assert pyobjtest_c.test_vfloat((43.25, 12.5, 0.0))=="Wrong size: expected 2, got 3"
    assert pyobjtest_c.test_vfloat((43.25, 0.0))=="Wrong vector"
    assert pyobjtest_c.test_vfloat((43.25, 0))=="Not float vector"

def test_vstring():
    assert pyobjtest_c.test_vstring(("Hello", "world!"))=="OK"
    assert pyobjtest_c.test_vstring(("Hello", "there..."))=="Wrong vector"
    assert pyobjtest_c.test_vstring(("Hello", "world!", "and all!"))=="Wrong size: expected 2, got 3"
    assert pyobjtest_c.test_vstring(("Hello", 123))=="Not string vector"
