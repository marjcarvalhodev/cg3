#!/bin/bash
set -euo pipefail

#cd public

echo "Open http://localhost:8080/index.html"

# Serve files using a local webserver
emrun --no_browser --port 8080 public/
