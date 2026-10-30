{
  description = "LinnStrument updater";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-26.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };

        lib = pkgs.lib;

        runtimeLibs = with pkgs; [
          freetype
          curl
          (lib.getLib stdenv.cc.cc)
          pcre2
          ladspa-sdk
          libsysprof-capture
          libthai
          libdatrie
          lerc
          libepoxy
          sqlite
          alsa-lib
          libglvnd
          webkitgtk_4_1
          util-linuxMinimal
          libselinux
          libsepol
          libxdmcp
          libxkbcommon
          libxtst

          # Explicitly include this because it is preloaded.
          libX11
        ];

        runtimeLibraryPath = lib.makeLibraryPath runtimeLibs;

        bossac = pkgs.stdenv.mkDerivation rec {
          pname = "bossac";
          version = "1.6.1-arduino";

          src = ./.;

          nativeBuildInputs = with pkgs; [
            pkg-config
            gnumake
          ];

          buildInputs = with pkgs; [
            readline
          ];

          postPatch = ''
            substituteInPlace Binaries/BOSSA-${version}/src/Samba.cpp \
              --replace-fail \
                'catch (SambaError)' \
                'catch (const SambaError&)'

            substituteInPlace Binaries/BOSSA-${version}/src/Samba.cpp \
              --replace-fail \
                'uint8_t cmd[13]' \
                'uint8_t cmd[14]'

            substituteInPlace Binaries/BOSSA-${version}/Makefile \
              --replace-fail \
                '-Werror' \
                '-Werror -Wno-error=deprecated-declarations'
          '';

          buildPhase = ''
            runHook preBuild

            cd Binaries/BOSSA-${version}/
            make bossac

            runHook postBuild
          '';

          installPhase = ''
            runHook preInstall

            mkdir -p "$out/bin"

            if [ -x bin/bossac ]; then
              install -Dm755 bin/bossac "$out/bin/bossac"
            elif [ -x bossac ]; then
              install -Dm755 bossac "$out/bin/bossac"
            else
              echo "Could not find the bossac executable"
              find . -type f -executable -print
              exit 1
            fi

            runHook postInstall
          '';

          meta.mainProgram = "bossac";
        };

        linnstrument-updater = pkgs.stdenv.mkDerivation {
          pname = "linnstrument-updater";
          version = "git";

          # If this is in the same repository as the flake, this includes
          # the local project sources.
          src = ./.;

          NIX_CFLAGS_COMPILE = "-DJUCE_MODAL_LOOPS_PERMITTED=1";

          nativeBuildInputs = with pkgs; [
            pkg-config
            makeWrapper
            autoPatchelfHook
          ];

          buildInputs = runtimeLibs;

          # The generated JUCE Linux project is already present in the repo.
          buildPhase = ''
            runHook preBuild

            make -C Builds/LinuxMakefile \
              CONFIG=Release \
              -j$NIX_BUILD_CORES

            runHook postBuild
          '';

          installPhase = ''
            runHook preInstall

            mkdir -p "$out/libexec/linnstrument-updater"
            mkdir -p "$out/bin"

            # Adjust this name if your generated JUCE target has a different
            # executable name.
            install -Dm755 \
              Builds/LinuxMakefile/build/LinnStrument\ Updater \
              "$out/libexec/linnstrument-updater/LinnStrumentUpdater"

            # The application historically looks for firmware beside itself.
            # Copy any firmware files shipped by the repository.
            if [ -d Binaries ]; then
              cp -r Binaries/. "$out/libexec/linnstrument-updater/"
            fi

            # Preserve the Windows-style tools layout expected by the
            # existing updater code.
            mkdir -p "$out/libexec/linnstrument-updater/tools"
            install -Dm755 \
              ${bossac}/bin/bossac \
              "$out/libexec/linnstrument-updater/tools/bossac"

            # Wrap the real executable in place. The explicit X11 preload is
            # useful for the current JUCE/WebKit startup issue.
            wrapProgram \
              "$out/libexec/linnstrument-updater/LinnStrumentUpdater" \
              --prefix LD_LIBRARY_PATH : "${runtimeLibraryPath}" \
              --set LD_PRELOAD "${pkgs.libX11}/lib/libX11.so.6"

            # Public launcher used by `nix run`.
            makeWrapper \
              "$out/libexec/linnstrument-updater/LinnStrumentUpdater" \
              "$out/bin/linnstrument-updater"

            runHook postInstall
          '';

          meta = {
            description = "LinnStrument firmware updater";
            homepage = "https://github.com/rogerlinndesign/linnstrument-updater";
            mainProgram = "linnstrument-updater";
          };
        };
      in {
        packages.default = linnstrument-updater;
        packages.bossac = bossac;

        apps.default = {
          type = "app";
          program = "${linnstrument-updater}/bin/linnstrument-updater";
        };

        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            gdb
            pkg-config
            gnumake
            bossac
          ] ++ runtimeLibs;
        };
      });
}
