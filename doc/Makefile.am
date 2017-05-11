apache:
	$(DOXYGEN) doxygen-apache
	touch apache.stamp

iis:
	$(DOXYGEN) doxygen-iis
	touch iis.stamp

nginx:
	$(DOXYGEN) doxygen-nginx
	touch nginx.stamp

standalone:
	$(DOXYGEN) doxygen-standalone
	touch standalone.stamp


if BUILD_APACHE2_MODULE
all-local: apache
endif

if BUILD_STANDALONE_MODULE
all-local: iis nginx standalone
endif

clean-local:
	rm -rf apache iis nginx standalone
