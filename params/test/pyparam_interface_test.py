#!/usr/bin/python

# Testing dict-to-alps::params interface

import pyparams_test_c as par

def test_params():
    d={}
    assert par.test_params(d)=="OK"
