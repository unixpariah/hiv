{
  stdenv,
  zig_0_13,
}:
stdenv.mkDerivation (finalAttrs: {
  pname = "hiv";
  version = "0.1.0";

  src = ./.;

  dontConfigure = true;
  doCheck = false;

  nativeBuildInputs = [zig_0_13];

  buildPhase = ''
    mkdir -p .cache
    zig build install --cache-dir $(pwd)/.zig-cache --global-cache-dir $(pwd)/.cache -Dcpu=baseline -Doptimize=ReleaseSafe --prefix $out
  '';

  postFixup = ''
    mkdir $out/lib/pkgconfig
    echo "prefix=$out
    exec_prefix=$out
    libdir=$out/lib
    includedir=$out/include

    Name: hiv
    Description: My std lib
    Version: 0.1.0
    Libs: -L$out/lib --lhiv
    Cflags: -I$out/include" > $out/lib/pkgconfig/hiv.pc
  '';

  installPhase = ''
    mkdir -p $out/lib $out/include/hiv
    cp include/* $out/include/hiv
  '';
})
