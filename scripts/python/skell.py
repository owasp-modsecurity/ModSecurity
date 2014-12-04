#!/usr/bin/env python
# -*- coding: utf-8 -*-

# ModSecurity core binding.
from modsecurity import ModSecurity

class ModSecurityExtension(ModSecurity):
  """
  Class ModSecurityExtension should represents your custom module.
  Nocite that this class should derivate from ModSecurity and should
  implement the method process.
  """

  def __init__(self):
    ModSecurity.__init__(self)

  def process(self):
    """
    The method is called by ModSecurity core whenever a request is
    needed to be evaluated.
    """

    # self.log can be utilised to produce content inside the SecDebugLog
    # (https://github.com/SpiderLabs/ModSecurity/wiki/Reference-Manual#secdebuglog)
    self.log(8, "This is our custom Python script, it seems that I am working" 
      "like a charm.")


    self.log(8, "Hum... Do we have something at FILES_TMPNAMES? %s" %
      self["FILES_TMPNAMES"])

    # Returns True whenever you want to send a "match" to ModSecurity core.
    return True

# Should be used to test your custom extension, deattached from ModSecurity core.
if __name__ == "__main__":
  myExtension = ModSecurityExtension()

  # Setting FILES_TMPNAMES property.
  # https://github.com/SpiderLabs/ModSecurity/wiki/Reference-Manual#files_tmpnames
  myExtension.FILES_TMPNAMES = [ "/etc/issue", "/etc/resolv.conf" ]

  # Process the content.
  ret = myExtension.process()

  if ret == True:
    print("Matched!")
  else:
    print("_not_ matched")

