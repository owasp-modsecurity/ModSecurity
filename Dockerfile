FROM fedora:25
MAINTAINER Chaim Sanders chaim.sanders@gmail.com

# Install Prereqs
RUN dnf -y update && \
  dnf install -y httpd \
  httpd-devel \
  lua-devel \
  pcre-devel \
  libxml2-devel \
  libcurl-devel \
  libtool \
  yajl-devel \
  git \
  unzip \
  ssdeep \ 
  gcc \
  wget

# Download ModSecurity
RUN mkdir -p /usr/share/ModSecurity && \
  cd /usr/share/ModSecurity && \
  wget --quiet "https://github.com/SpiderLabs/ModSecurity/releases/download/v2.9.1/modsecurity-2.9.1.tar.gz" && \
  tar -xvzf modsecurity-2.9.1.tar.gz 

# Install ModSecurity
RUN cd /usr/share/ModSecurity/modsecurity-2.9.1/ && \
  sh autogen.sh && \ 
  ./configure && \
  make && \
  make install

# Move Files
RUN cd /usr/share/ModSecurity/modsecurity-2.9.1/ && \
  mkdir -p /etc/httpd/modsecurity.d && \
  mv modsecurity.conf-recommended  /etc/httpd/modsecurity.d/modsecurity.conf && \
  mv unicode.mapping /etc/httpd/modsecurity.d/

# Setup Config
Run printf "LoadModule security2_module modules/mod_security2.so\nInclude modsecurity.d/*.conf" > /etc/httpd/conf.modules.d/10-modsecurty.conf && \
  echo "ServerName localhost" > /etc/httpd/conf.d/ServerName.conf

EXPOSE 80

CMD ["httpd", "-k", "start"]
