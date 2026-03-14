#/bin/sh
./scripts/code.sh --enable-features=UseOzonePlatform --ozone-platform=wayland --ozone-platform-hint=auto --enable-features=WaylandWindowDecorations $@

# for package.json for electron forge
# electron-forge start -- --enable-features=UseOzonePlatform --ozone-platform=wayland --ozone-platform-hint=auto --enable-features=WaylandWindowDecorations
