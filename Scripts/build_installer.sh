#!/bin/bash

version=$1

mkdir installer/packages/ca.cidco.opensidescan/data
cp build/bin/OpenSidescan installer/packages/ca.cidco.opensidescan/data
linuxdeployqt.AppImage installer/packages/ca.cidco.opensidescan/data/OpenSidescan

sed -i "s/1.0.0/$version/g" installer/config/config.xml
sed -i "s/1.0.0/$version/g" installer/packages/ca.cidco.opensidescan/meta/package.xml
sed -i "s/2000-00-00/$(date '+%Y-%m-%d')/g" installer/packages/ca.cidco.opensidescan/meta/package.xml

binarycreator -c installer/config/config.xml -p installer/packages OpenSidescan_installer_$version
