Summary:	Studierstube - The Augmented Reality Environment
Name:		studierstube
Version:	4.0
Release:	1
Copyright:	GPL
Group:		Development/Libraries
Source:		%{name}-%{version}.tar.bz2
Vendor:		Institute for Computer Graphics and Vision, Graz University of Technology, Austria
URL:		http://www.studierstube.org
Packager:	Institute for Computer Graphics and Vision, Graz University of Technology, Austria
Prefix:		/usr
BuildRoot: 	%{_tmppath}/buildroot-%{name}-%{version}
Requires:	tinyxml ACE openvideo opentracker
BuildRequires:	scons tinyxml tinyxml-devel ACE ACE-devel openvideo openvideo-devel opentracker opentracker-devel

%define _prefix %{prefix}

%description
STUDIERSTUBE is a leading framework for the development of mobile, collaborative and ubiquitous AR applications. The development of the STUDIERSTUBE software platform started at Vienna University of Technology in 1996. Since 2004, the main research and development activities are carried out at Graz University of Technology.

%prep
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT
%setup

%build
scons PREFIX=%{_prefix} LIBDIR=%{_libdir}

%install
scons INSTALL_ROOT_DIR=$RPM_BUILD_ROOT PREFIX=%{_prefix} LIBDIR=%{_libdir} install

%clean
scons -c
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_libdir}/*.so*
%{_bindir}/*
%{_prefix}/share/stb/*

%package devel
Summary:	Studierstube header files
Group:		Development/Libraries
Requires:	%{name} = %{version}

%description devel
This package contains header files and include files that are needed for development using ...

%files devel
%defattr(-,root,root)
%{_prefix}/include/*
%{_libdir}/pkgconfig/*
