{
  stdenv,
  zig_0_13,
  shared ? false,
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
    zig build --cache-dir $(pwd)/.zig-cache --global-cache-dir $(pwd)/.cache -Dcpu=baseline -Doptimize=ReleaseSafe
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

  installPhase =
    if shared
    then ''
      mkdir -p $out/lib $out/include/hiv
      cp zig-out/lib/libhiv.so $out/lib
      cp include/* $out/include/hiv
    ''
    else ''
      mkdir -p $out/lib $out/include/hiv
      cp zig-out/lib/libhiv.a $out/lib
      cp include/* $out/include/hiv
    '';
})
