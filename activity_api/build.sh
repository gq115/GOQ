#!/usr/bin/env bash


LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/

rm -rf dist/*
pyinstaller -F MainWebApi.py
cp ./ApiConfig.yaml ./dist



