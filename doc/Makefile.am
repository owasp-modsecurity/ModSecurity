apache:
	$(DOXYGEN) doxygen-apache
	touch apache.stamp

iis:
	$(DOXYGEN) doxygen-iis
	touch iis.stamp


standalone:
	$(DOXYGEN) doxygen-standalone
	touch standalone.stamp


if BUILD_APACHE2_MODULE
all-local: apache
endif

if BUILD_STANDALONE_MODULE
all-local: iis standalone
endif

clean-local:
	rm -rf apache iis standalone
