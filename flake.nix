{
  description = "Flake to build dwl from a custom GitHub repo";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

    # Your custom dwl repo as a flake input
    dwl-custom.url = "github:ibbejohar/dwl";
  };

  outputs = { self, nixpkgs, dwl-custom, ... }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      packages.${system}.dwl = pkgs.stdenv.mkDerivation {
        pname = "dwl";
        version = "custom";

        # Use the source from the flake input
        src = dwl-custom;

        nativeBuildInputs = [
          pkgs.pkgconfig
          pkgs.meson
          pkgs.ninja
          pkgs.wayland
          pkgs.wlroots
          pkgs.glib
          pkgs.fontconfig
          pkgs.pkg-config
          pkgs.git
          pkgs.cairo
          pkgs.pango
          pkgs.libxkbcommon
          pkgs.pixman
          pkgs.libdrm
          pkgs.libinput
          pkgs.udev
        ];

        buildPhase = ''
          meson setup build
          meson compile -C build
        '';

        installPhase = ''
          meson install -C build
        '';

        meta = with pkgs.lib; {
          description = "Dynamic Wayland compositor based on dwm";
          license = licenses.mit;
          maintainers = with maintainers; [ ];
          platforms = platforms.linux;
        };
      };
    };
}
