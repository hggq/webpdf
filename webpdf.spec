# webpdf.spec

Name:           webpdf
Version:        1.9.0
Release:        1%{?dist}
Summary:        HTML/CSS/SVG to PDF renderer library

License:        MIT
URL:            https://github.com/hggq/webpdf
Source0:        %{url}/archive/v%{version}/%{name}-%{version}.tar.gz

BuildRequires:  cmake >= 3.16
BuildRequires:  gcc-c++
BuildRequires:  zlib-devel
# 如果项目使用了其他库，在这里添加 BuildRequires

# 运行时依赖
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
    -DWEBPDF_INSTALL_FONTS=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=%{_prefix}
%cmake_build

%install
%cmake_install

# 清理多余的文件（如果有）
rm -rf %{buildroot}%{_datadir}/webpdf/example/fonts

%files
# 文档
%doc README.md README_CN.md LICENSE

# 头文件
%{_includedir}/webpdf/*.h

# 库文件
%{_libdir}/libwebpdf.so.*
%{_libdir}/cmake/webpdf/
%{_libdir}/pkgconfig/webpdf.pc

# 可执行文件
%{_bindir}/webpdf
%{_bindir}/webpdf-hello
%{_bindir}/webpdf-render-html

# 字体数据（可选）
%{_datadir}/webpdf/example/fonts/

%changelog
* Sat Jul 19 2026 Your Name <your.email@example.com> - 1.9.0-1
- Initial RPM release for webpdf 1.9.0