Summary: generate C function prototypes and convert function definitions
# $Id: cproto.spec,v 1.21 2025/09/01 10:51:16 tom Exp $
Name: cproto
Version: 4.7y
Release: 1
License: Public Domain
Group: Applications/Development
URL: https://invisible-island.net/%{name}
Source0: %{name}-%{version}.tgz
Packager: Thomas Dickey <dickey@invisible-island.net>

%description
Cproto generates function prototypes for functions defined in the specified C
source files to the standard output.  The function definitions may be in the
old style or ANSI C style.  Optionally, cproto also outputs declarations for
variables defined in the files.

%prep

%define debug_package %{nil}

%setup -q -n %{name}-%{version}

%build

INSTALL_PROGRAM='${INSTALL}' \
%configure \
  --target %{_target_platform} \
  --prefix=%{_prefix} \
  --bindir=%{_bindir} \
  --libdir=%{_libdir} \
  --mandir=%{_mandir} \
  --enable-llib           

make

%install
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

make install                    DESTDIR=$RPM_BUILD_ROOT

strip $RPM_BUILD_ROOT%{_bindir}/%{name}

%clean
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_prefix}/bin/%{name}
%{_mandir}/man1/%{name}.*

%changelog
# each patch should add its ChangeLog entries here

* Mon Sep 01 2025 Thomas E. Dickey
- testing cproto 4.7y-1

* Wed May 23 2018 Thomas Dickey
- turn off debug package; add llib feature

* Sun Jan 02 2011 Thomas Dickey
- cproto 4.7j

* Wed Jul 14 2010 Thomas Dickey
- initial version
