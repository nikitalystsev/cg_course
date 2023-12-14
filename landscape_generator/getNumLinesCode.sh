find . -type f \( -name "*.cpp" -o -name "*.h" \) -print0 | xargs -0 wc -l

