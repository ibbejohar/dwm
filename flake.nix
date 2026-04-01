{
  description = "Custom dwm build";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

    # Your custom dwm repo
    dwm-custom.url = "github:ibbejohar/dwm";
  };

  outputs = { self, nixpkgs, dwm-custom, ... }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      packages.${system}.dwm = pkgs.stdenv.mkDerivation rec {
        pname = "dwm";
        version = "custom";

        src = dwm-custom;

        nativeBuildInputs = [
          pkgs.pkg-config
        ];

        buildInputs = [
          pkgs.xorg.libX11
          pkgs.xorg.libXft
          pkgs.xorg.libXinerama

          # XCB support (for your modified dwm)
          pkgs.xorg.libxcb
          pkgs.xorg.xcbutil
          pkgs.xorg.xcbutilwm
          pkgs.xorg.xcbutilkeysyms
          pkgs.xorg.xcbutilrenderutil

          pkgs.freetype
          pkgs.fontconfig
        ];

        # Fix hardcoded paths in config.mk if present
        postPatch = ''
          if [ -f config.mk ]; then
            sed -i "s|/usr/X11R6|${pkgs.xorg.libX11.dev}|g" config.mk
          fi
        '';

        buildPhase = ''
          runHook preBuild
          make
          runHook postBuild
        '';

        installPhase = ''
          runHook preInstall
          mkdir -p $out/bin
          cp dwm $out/bin/
          runHook postInstall
        '';

        meta = with pkgs.lib; {
          description = "Personal dwm build";
          license = licenses.mit;
          platforms = platforms.linux;
        };
      };

      # Optional: make `nix build` default to this
      defaultPackage.${system} = self.packages.${system}.dwm;
    };
}
