Name:           webpdf
Version:        1.0.1
Release:        7%{?dist}
Summary:        HTML/CSS/SVG to PDF renderer library

License:        MIT
URL:            https://github.com/hggq/webpdf
Source0:        webpdf_1.0.1.tar.gz

BuildRequires:  cmake >= 3.16
BuildRequires:  gcc-c++
BuildRequires:  zlib-devel

Requires:       zlib

%description
webpdf is a C++ library for rendering HTML/CSS/SVG content to PDF files.
This package provides the shared library, development headers, and a
command-line interface for PDF generation.

%prep
%autosetup -n %{name}-%{version}

%build
%cmake \
    -DWEBPDF_BUILD_EXAMPLES=ON \
    -DWEBPDF_INSTALL=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_SKIP_RPATH=ON
%cmake_build

%install
%cmake_install

# 手动安装示例程序（它们未被 CMake 安装）
mkdir -p %{buildroot}%{_bindir}
install -m 755 %{_builddir}/%{name}-%{version}/example/webpdf-hello %{buildroot}%{_bindir}/
install -m 755 %{_builddir}/%{name}-%{version}/example/webpdf-render-html %{buildroot}%{_bindir}/

%files
%doc README.md LICENSE
%{_includedir}/webpdf/*.h
%{_libdir}/libwebpdf.so.*
%{_libdir}/libwebpdf.so
%{_libdir}/cmake/webpdf/
%{_libdir}/pkgconfig/webpdf.pc
%{_bindir}/webpdf
%{_bindir}/webpdf-hello
%{_bindir}/webpdf-render-html
%{_datadir}/webpdf/example/fonts/

%changelog
* Sat Jul 18 2026 Your Name <email@example.com> - 1.0.1-7
- Add missing unversioned libwebpdf.so symlink