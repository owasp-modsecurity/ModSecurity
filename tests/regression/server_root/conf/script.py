#!/usr/bin/env python
# -*- coding: utf-8 -*-

from modsecurity import ModSecurity

class ModSecurityExtension(ModSecurity):
  def process(self):
    self.log(8, "Python test message.")
    return False

if __name__ == "__main__":
  myExtension = ModSecurityExtension()

  # Process the content.
  ret = myExtension.process()

  if ret == True:
    print("Matched!")
  else:
    print("_not_ matched")

