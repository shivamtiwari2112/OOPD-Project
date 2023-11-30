#!/bin/bash

# Create directory for 1GB files
mkdir -p dir_1gb
cd dir_1gb
for i in {1..10}; do
    dd if=/dev/zero of=file_${i}.txt bs=1M count=1000 status=none
done
cd ..

# Create directory for 10MB files
mkdir -p dir_10mb
cd dir_10mb
for i in {1..100}; do
    dd if=/dev/zero of=file_${i}.txt bs=1M count=10 status=none
done
cd ..

# Create directory for 10MB files with subdirectories recursively
mkdir -p dir_recursive
cd dir_recursive

# Function to create files recursively
create_files_recursive() {
    local depth=$1
    local count=$2

    if [ "$depth" -eq 0 ]; then
        return
    fi

    mkdir subdirectory_${depth}
    cd subdirectory_${depth}

    for i in $(seq 1 $count); do
        dd if=/dev/zero of=file_${i}.txt bs=1M count=10 status=none
    done

    create_files_recursive $((depth - 1)) $count

    cd ..
}

create_files_recursive 4 100

cd ..

# Measure time for each operation
echo "Time taken for creating 1GB files:"
time (mkdir -p dir_1gb; cd dir_1gb; for i in {1..10}; do dd if=/dev/zero of=file_${i}.txt bs=1M count=1000 status=none; done; cd ..)

echo "Time taken for creating 10MB files:"
time (mkdir -p dir_10mb; cd dir_10mb; for i in {1..100}; do dd if=/dev/zero of=file_${i}.txt bs=1M count=10 status=none; done; cd ..)

echo "Time taken for creating 10MB files with subdirectories recursively:"
time (mkdir -p dir_recursive; cd dir_recursive; create_files_recursive 4 100; cd ..)
