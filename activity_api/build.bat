
rm -rf dist/*

pyinstaller -F MainWebApi.py
cp ./config.yaml ./dist

pause

