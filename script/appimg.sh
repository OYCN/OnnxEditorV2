#!/bin/bash

set -e

if [[ -z $1 ]]
then
  echo "release dir with args[1] miss"
  exit 1
fi

CODE_DIR=$(cd "$(dirname "${0}")/../"; pwd)

cd ${CODE_DIR}
rm -rf OnnxEditor.AppDir
rm -rf OnnxEditor-*.AppImage
cp -r $1 OnnxEditor.AppDir

cd OnnxEditor.AppDir
mv run.sh AppRun
echo "[Desktop Entry]
Name=OnnxEditor
Exec=AppRun
Terminal=false
Type=Application
Icon=OnnxEditor
StartupWMClass=OnnxEditor
X-AppImage-Version=0.0.0
Comment=Onnx Editor
Categories=Development;" > OnnxEditor.desktop
cp ${CODE_DIR}/gui/res/img/appicon.ico OnnxEditor.png

cd ${CODE_DIR}
if [[ ! -f "appimagetool-x86_64.AppImage" ]]
then
  wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
fi
chmod +x appimagetool-x86_64.AppImage
./appimagetool-x86_64.AppImage OnnxEditor.AppDir
