Summary:	Studierstube - The Augmented Reality Environment
Name:		studierstube
Version:	4.0
Release:	1
Copyright:	GPL
Group:		Development/Libraries
Source:		%{name}-%{version}.tar.bz2
#Source1:	%{name}-scons.tar.gz
#Vendor:		...
#URL:		http://www.grinninglizard.com/tinyxml
Packager:	Institute for Computer Graphics and Vision, Graz University of Technology, Austria
Prefix:		/usr
BuildRoot: 	%{_tmppath}/buildroot-%{name}-%{version}
Requires:	tinyxml ACE openvideo opentracker
BuildRequires:	scons tinyxml tinyxml-devel ACE ACE-devel openvideo openvideo-devel opentracker opentracker-devel

%define _prefix %{prefix}

%description
...

%prep
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT
%setup

%build
scons PREFIX=%{_prefix} LIBDIR=%{_libdir}

%install
scons INSTALL_ROOT_DIR=$RPM_BUILD_ROOT PREFIX=%{_prefix} LIBDIR=%{_libdir} install
#rm $RPM_BUILD_ROOT/%{_libdir}/.sconsign

%clean
scons -c
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_libdir}/*.so*
%{_libdir}/pkgconfig/*
%{_bindir}/*
%{_prefix}/share/stb/*
%{_prefix}/include/*

# what are these files good for???
#%exclude %{_bindir}/stb.pc
#%exclude %{_prefix}/share/stb.pc

#%exclude %{_libdir}/pkgconfig/.sconsign
#%exclude %{_prefix}/include/openvideo/.sconsign
#%exclude %{_prefix}/include/openvideo/nodes/.sconsign

#%package devel
#Summary:	...
#Group:		Development/Libraries
#Requires:	%{name} = %{version}
#
#%description devel
#This package contains header files and include files that are needed for development using ...
#
#%files devel
#%defattr(-,root,root)
#%{_bindir}/*
#%{_prefix}/include/*
#%{_prefix}/share/aclocal/*
#%{_prefix}/share/Coin/conf/*
