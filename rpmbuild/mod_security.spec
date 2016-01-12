Summary: ModSecurity module for Apache HTTP Server
Name: mod_security
Version: 2.9.0
Release: 1%{?dist}
License: ASL 2.0
URL: http://www.modsecurity.org/
Group: System Environment/Daemons
Source: http://www.modsecurity.org/download/modsecurity-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Requires: httpd httpd-mmn = %([ -a %{_includedir}/httpd/.mmn ] && cat %{_includedir}/httpd/.mmn || echo missing)
BuildRequires: httpd-devel libxml2-devel pcre-devel curl-devel lua-devel

%description
ModSecurity: Open Source Web Application Firewall

%prep
%setup -q -n modsecurity-%{version}

%build
find doc -type f -exec chmod -x {} \;
./autogen.sh
%configure
make %{_smp_mflags}

%install
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install
rm -rf $RPM_BUILD_ROOT%{_datadir}/doc/%{name}

%clean
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr (-,root,root)
%doc CHANGES LICENSE README.* modsecurity* doc
%{_libdir}/httpd/modules/mod_security2.so
%{_bindir}/mlogc
%{_bindir}/mlogc-batch-load.pl
%{_bindir}/rules-updater.pl
/usr/lib/mod_security2.so


%changelog
* Tue Jan 12 2016 Antony Hutchison <antony@hutchisontechnical.co.uk> - 2.9.0-1
- initial build of RPM
