class Webpdf < Formula
  desc "HTML/CSS/SVG to PDF renderer library"
  homepage "https://github.com/hggq/webpdf"
  # For a tagged release, point url/sha256 at the release tarball and update
  # the checksum with:  brew fetch webpdf  (or)  shasum -a 256 <tarball>
  url "https://github.com/hggq/webpdf/archive/refs/tags/v1.9.0.tar.gz"
  sha256 "0000000000000000000000000000000000000000000000000000000000000000"
  license "MIT"
  head "https://github.com/hggq/webpdf.git", branch: "main"

  depends_on "cmake" => :build
  depends_on "zlib"

  def install
    system "cmake", "-S", ".", "-B", "build", *std_cmake_args, "-DWEBPDF_BUILD_EXAMPLES=OFF"
    system "cmake", "--build", "build"
    system "cmake", "--install", "build"
  end

  test do
    # Verify the installed library, headers and CMake config work together.
    (testpath/"test.cpp").write <<~CPP
      #include <webpdf.h>
      #include <memory>
      int main() {
        auto pdf = std::make_unique<pz::webpdf>();
        pdf->AddPage();
        return 0;
      }
    CPP

    system ENV.cxx, "-std=c++20", "test.cpp",
           "-I#{include}/webpdf",
           "-L#{lib}", "-lwebpdf",
           "-L#{Formula["zlib"].opt_lib}", "-lz",
           "-o", "test"
    system "./test"
  end
end
