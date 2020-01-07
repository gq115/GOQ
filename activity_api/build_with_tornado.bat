
rmdir /s dist
rmdir /s __pycache__
rmdir /s build

pyinstaller -F tornado_server.py
cp ./config.yaml ./dist



