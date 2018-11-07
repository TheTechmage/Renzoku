#!/bin/bash
rm -rf test_れんぞく || echo "Failed to remove tmp test directory"
mkdir test_れんぞく
cd test_れんぞく
cat <<EOF > config.conf
Watch Dummy {
	files = "test_*"
	step "Cat dummy files" {
		command = "bash -c \\"cat test_*\\""
	}
}
Watch "Second Dummy Listener" {
	files = "test2_*"
	step "Cat dummy files" {
		command = "bash -c \\"cat test2_*\\""
	}
}
EOF
echo "Using config:"
cat config.conf
../test_background.sh &
exec ../src/renzoku
