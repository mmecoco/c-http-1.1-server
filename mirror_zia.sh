#!/bin/bash -e
# GitHubToBlih Copyright (C) 2020 Maxime Houis
# This program comes with ABSOLUTELY NO WARRANTY.
# This is free software, and you are welcome to redistribute it
# under certain conditions; see LICENSE for details.

if [[ $# != 1 ]]; then
    echo "Usage: $0 REPO_NAME"
    exit 1
fi

TMP_DIR="blih"

USER_GHUB="MaximeHouis"
USER_BLIH="maxime.houis@epitech.eu"

REPO_GHUB="$1"
REPO_BLIH="$1"

mkdir -p $TMP_DIR
rm -rf $TMP_DIR/*

git clone --bare git@github.com:$USER_GHUB/$REPO_GHUB.git $TMP_DIR

cd $TMP_DIR
git push --mirror git@git.epitech.eu:/$USER_BLIH/$REPO_BLIH

cd ..
rm -rf $TMP_DIR
