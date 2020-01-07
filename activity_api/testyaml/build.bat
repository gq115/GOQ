
rm -rf dist/*

pyinstaller -F test_python.py
cp ./test.yaml ./dist



