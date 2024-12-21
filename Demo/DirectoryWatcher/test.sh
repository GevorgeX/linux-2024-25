#!/bin/bash

# Create a temporary directory for testing
test_dir="test_directory"
mkdir -p "$test_dir"
file2="$test_dir/file2.txt"
touch "$file2"

echo "Test directory created: $test_dir"
# Run the DirectoryWatcher program in the background (adjust path if necessary)
./DirectoryWatcherTest "$test_dir" &
dw_pid=$!

# Give the DirectoryWatcher some time to initialize
sleep 2

# Test file creation
echo "Testing file creation..."
file1="$test_dir/file1.txt"
touch "$file1"
sleep 1

# Test file modification
echo "Testing file modification..."
echo "Hello, World!" > "$file2"
sleep 1

# Test file deletion
echo "Testing file deletion..."
rm "$file1"
sleep 1

# Test directory creation and deletion
echo "Testing directory creation and deletion..."
sub_dir="$test_dir/subdir"
mkdir "$sub_dir"
sleep 1
rmdir "$sub_dir"
sleep 1

# Cleanup and stop the DirectoryWatcher process
kill $dw_pid
wait $dw_pid 2>/dev/null
echo "Test completed. Cleanup done."

# Remove the test directory
rm -r "$test_dir"
echo "Test directory removed: $test_dir"
