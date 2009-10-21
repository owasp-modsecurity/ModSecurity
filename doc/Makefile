FOP = /opt/fop/fop.sh
XALAN = /opt/fop/xalan.sh
JAVA_HOME = /usr/lib/jvm/java-6-sun

ALLDOCS = modsecurity2-apache-reference.pdf modsecurity2-data-formats.pdf modsecurity2-apache-reference.html modsecurity2-data-formats.html html-multipage/index.html index.html

all: $(ALLDOCS)

modsecurity2-apache-reference.pdf: modsecurity2-apache-reference.xml
	JAVA_HOME=$(JAVA_HOME); export JAVA_HOME; \
	$(FOP) -q -xsl pdf.xsl -xml modsecurity2-apache-reference.xml -pdf modsecurity2-apache-reference.pdf;

modsecurity2-data-formats.pdf: modsecurity2-data-formats.xml
	JAVA_HOME=$(JAVA_HOME); export JAVA_HOME; \
	$(FOP) -q -xsl pdf.xsl -xml modsecurity2-data-formats.xml -pdf modsecurity2-data-formats.pdf;

modsecurity2-apache-reference.html: modsecurity2-apache-reference.xml
	JAVA_HOME=$(JAVA_HOME); export JAVA_HOME; \
	$(XALAN) -q -xsl html.xsl -in modsecurity2-apache-reference.xml -param base.dir ./; \
	mv index.html modsecurity2-apache-reference.html;

modsecurity2-data-formats.html: modsecurity2-data-formats.xml
	JAVA_HOME=$(JAVA_HOME); export JAVA_HOME; \
	$(XALAN) -q -xsl html.xsl -in modsecurity2-data-formats.xml -param base.dir ./; \
	mv index.html modsecurity2-data-formats.html;

html-multipage/index.html: modsecurity2-apache-reference.xml
	JAVA_HOME=$(JAVA_HOME); export JAVA_HOME; \
	mkdir html-multipage/; \
	$(XALAN) -q -xsl html-chunked.xsl -in modsecurity2-apache-reference.xml -param base.dir html-multipage/; \
	cp modsecurity-reference.css html-multipage/; \
	cp modsecurity.gif html-multipage/; \
	cp breach-logo-small.gif html-multipage/; \
	cp apache_request_cycle-modsecurity.jpg html-multipage/

index.html: main-index.html
	(cp main-index.html index.html && \
	VERSION=`grep "<releaseinfo>.*</releaseinfo>" modsecurity2-apache-reference.xml | sed 's/[ \t]*<\/\?releaseinfo>[ \t]*//g'` && \
	perl -pi -e "s/\\\$$version/$$VERSION/" index.html)

clean:
	rm -rf docindex.html $(ALLDOCS) html-multipage

.PHONY: setup cleanup

