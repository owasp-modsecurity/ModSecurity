#!/usr/bin/env python

from __future__ import print_function
import sys

# Needs to be singlenton ?
class Singleton(object):
  _instances = {}
"""
  def __new__(class_, *args, **kwargs):
    if class_ not in class_._instances:
        class_._instances[class_] = super(Singleton, class_).__new__(class_, *args, **kwargs)
    return class_._instances[class_]
"""

class ModSecurity():
  def __init__(self):
    self.default_attr = ["default_attr", "name", "modsecCore"]
    self.name = None
    self.modsecCore = None

  def setModSecurityCore(self, cb):
    self.modsecCore = cb
    self.log(8, "Log attached");
    return True

  def log(self, level, msg):
    if self.modsecCore == None:
      print("ModSecurity Python: ", str(level) + " " + str(msg), file=sys.stderr)  
    else:
      self.modsecCore.log(level, msg)
    return True

  def __getattribute__(self, key):
    v = None
    try:
      v = object.__getattribute__(self, key)
      if hasattr(v, '__get__'):
        return v.__get__(None, self)
    except:
      if self.modsecCore != None:
        v = self.modsecCore.getVariable(key)

    return v

  def __setattr__(self, name, value):
    self.__dict__[name] = value

    if name not in self.default_attr:
      if self.modsecCore != None:
        self.modsecCore.setVariable("tx." + name, value)


"""
TODO: transformation
"""
