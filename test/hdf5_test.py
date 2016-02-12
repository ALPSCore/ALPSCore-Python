# Copyright (C) 1998-2014 ALPS Collaboration. See COPYRIGHT.TXT
# All rights reserved. Use is subject to license terms. See LICENSE.TXT
# For use in publications, see ACKNOWLEDGE.TXT

import alps.hdf5 as h5
import numpy as np
import sys
import os

import pytest

def check_scalar(ar,name,vtype,expected_val):
    ext=ar.extent(name)
    assert len(ext)==1 #, ("Invalid extent length for scalar \"%s\"" % name)
    assert ext[0]==1 #, ("Invalid extent value for scalar \"%s\"" % name)
    x = ar[name]
    xtype = type(x)
    assert xtype==vtype #, ("Invalid type for scalar \"%s\": %s" % (name,str(xt)))
    assert x==expected_val #, ("Invalid value for scalar \"%s\"" % name)


def check_1d_array(ar, name, sz, val):
    ext=ar.extent(name)
    assert len(ext)==1, ("Invalid extent length for 1D-array \"%s\"" % name)
    assert ext[0]==sz, ("Invalid extent value for 1D-array \"%s\"" % name)
    x=ar[name]
    assert np.all(x==val), ("Invalid value for 1D-array \"%s\"" % name)

def check_2d_array(ar, name, sz, val):
    ext=ar.extent(name)
    assert len(ext)==2, ("Invalid extent length for 2D-array \"%s\"" % name)
    assert ext[0]==sz[0] and ext[1]==sz[1], ("Invalid extent values for 2D-array \"%s\"" % name)
    x=ar[name]
    assert np.all(x==val), ("Invalid value for 2D-array \"%s\"" % name)


def check_read(ar):
    childs = ar.list_children('/')
    assert len(childs)==7, "Invalid number of children in /"

    assert not ar.is_complex("/int"), "Invalid complex detection"
    assert not ar.is_complex("/double"), "Invalid complex detection"
    assert ar.is_complex("/cplx"), "Invalid complex detection"

    check_scalar(ar,"/int",int,9)
    check_scalar(ar,"/double",float,9.125)
    check_scalar(ar,"/cplx",complex,complex(1,2))

    s = ar["/str"]
    assert type(s)==str, "Invalid type for string \"/str\""
    assert s=="test", "Invalid value for string \"/str\""

    check_1d_array(ar,"/np/int",3,np.array([1,2,3]))

    check_2d_array(ar,"/np/cplx", (2,2), np.array([[1 + 1j,2 +2j ],[3 + 3j,4 + 4j]]))
    check_2d_array(ar,"/np2/int", (3,3), np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]]))

# @pytest.fixture(scope="module")
# def ar_filled(fname="py.h5"):
#     oar = h5.archive(fname, 'w')
#     oar["/int"] =  9
#     oar["/double"] =  9.125
#     oar["/cplx"] =  complex(1, 2)
#     oar["/str"] =  "test"
#     oar["/np/int"] =  np.array([1, 2, 3])
#     oar["/np2/int"] =  np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
#     oar["/np/cplx"] =  np.array([[1 + 1j,2 +2j ],[3 + 3j,4 + 4j]])
    
#     oar.create_group("/my/group")
#     oar["/my/double"] = 8.5
    
#     oar.delete_group("/my/group")
#     oar.delete_data("/my/double")

#     return oar


# @pytest.fixture(scope="module")
# def iar(fname="py2.h5"):
#     oar=ar(fname)
#     del oar
#     myar=h5.archive(fname,"r")
#     return myar

@pytest.fixture
def oar(fname="py1.h5"):
    try:
        os.remove(fname)
    except:
        pass
    ar=h5.archive(fname,"w")
    return ar

def test_openwrite():
    ar=oar("py0.h5")
    del ar

def test_children(oar):
    oar["c1"]=1
    oar["c2/d1"]=2
    oar["c2/d2"]=3
    oar["c3"]=4
    childs = oar.list_children('/')
    assert len(childs)==3
    assert "c1" in childs
    assert "c2" in childs
    assert "c3" in childs

def test_int(oar):
    oar["/int"]=9
    check_scalar(oar, "/int", int, 9)




#def test_children(ar):
#    childs = ar.list_children('/')
#    assert len(childs)==7, "Invalid number of children in /"




# def test_h5_with_close():
#     try:
#         oar = h5.archive("py.h5", 'w')
#         check_write(oar)
#         del oar
#     except Exception, e:
#         assert 0, "Exception occured on writing: "+str(e)
#     try:
#         iar = h5.archive("py.h5", 'r')
#         check_read(iar)
#     except Exception, e:
#         assert 0, "Exception occured on reading: "+str(e)
        
# def test_h5_same_object():
#     try:
#         ar = h5.archive("py.h5", 'w')
#         check_write(ar)
#     except Exception, e:
#         assert 0, "Exception occured on writing: "+str(e)
#     try:
#         check_read(ar)
#     except Exception, e:
#         assert 0, "Exception occured on reading: "+str(e)
