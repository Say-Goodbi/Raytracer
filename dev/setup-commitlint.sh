# !/bin/bash

## Install commitlint and husky
cd ../
echo "Installing commitlint and husky..."
npm install --save-dev husky @commitlint/cli @commitlint/config-conventional
echo "----"

## Initialize husky and add commit-msg hook
echo "Initializing husky and adding commit-msg hook..."
npx husky init
echo "npx --no -- commitlint --edit "$1"" > .husky/commit-msg
rm .husky/pre-commit
echo "----"

## Create the config file for commitlint
echo "Creating commitlint config file..."
echo "module.exports = { extends: ['@commitlint/config-conventional'] };" > commitlint.config.js
echo "----"

echo "Commitlint and husky setup complete!"
