#!/bin/bash
set -e
export BASH_TAP_ROOT=./bash-tap/
source $(dirname $0)/bash-tap/bash-tap-bootstrap
testdir=$(mktemp -d -t test_れんぞく_XXXXX)

echo "# Changing to test directory: ${testdir}"
cd ${testdir}
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
		command = "bash -c \\"cat test2_{1,2} > test3_out\\""
	}
}
EOF
cat <<EOF >background.sh
#!/bin/bash
sleep 0.2;
echo "Test file 1" > test_1
echo "Test file 2" > test_2
echo "Dummy file 1" > test2_1
echo "Dummy file 2" > test2_2
sleep 0.2;
EOF
plan no_plan
../src/renzoku &
renzoku=$!
bash background.sh
like "${renzoku}" "[[:digit:]]" "Renzoku startup pid"
is "hi" "hi" "hi is hi"
isnt "hi" "bye" "hi is bye"

declare -a test_data
test_data=("Dummy file 1" "Dummy file 2")
test_file=test3_out
if [ ! -f "$test_file" ]; then
	ok 0 "Failed to find output file $test_file"
	for ((i = 0; i < ${#test_data[@]}; i++)); do
		is "" "${test_data[$i]}" "Validate output"
	done
else
	counter=0
	while read -r line; do
		skip "v" "v" "meep"
		is "$line" "${test_data[$counter]}" "Validate output"
		counter = $counter + 1
	done < $testfile
	is $counter 2
fi

kill ${renzoku} || BAIL_OUT "Failed to stop Renzoku!"
rm -rf ${testdir} || BAIL_OUT "# Failed to remove tmp test directory: ${testdir}"
done_testing
