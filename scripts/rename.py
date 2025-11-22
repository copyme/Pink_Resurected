import os
import sys

def is_bash_script(filepath):
    """
    Checks the first line of the file (shebang) to see if it indicates a bash/sh script.
    """
    try:
        # Open file in text mode, ignoring encoding errors for binary safety
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            first_line = f.readline().strip()

            # Check if it starts with #! and contains 'bash' or 'sh'
            # Common examples: #!/bin/bash, #!/usr/bin/env bash, #!/bin/sh
            if first_line.startswith("#!"):
                if "bash" in first_line or "sh" in first_line:
                    return True
    except (IOError, UnicodeDecodeError):
        # If file cannot be read or is binary, assume it's not a script
        return False
    return False

def process_directory(directory, dry_run=False):
    """
    Walks through the directory and renames files.
    """
    count = 0

    for root, dirs, files in os.walk(directory):
        for filename in files:
            filepath = os.path.join(root, filename)

            # 1. Check if it is a bash script
            if is_bash_script(filepath):

                # 2. Check if it already has the .sh extension
                if not filename.endswith(".sh"):
                    new_filename = filename + ".sh"
                    new_filepath = os.path.join(root, new_filename)

                    # Check if the destination file already exists to prevent overwriting
                    if os.path.exists(new_filepath):
                        print(f"[SKIPPED] Target exists: {filename} -> {new_filename}")
                        continue

                    if dry_run:
                        print(f"[DRY RUN] Would rename: {filename} -> {new_filename}")
                    else:
                        try:
                            os.rename(filepath, new_filepath)
                            print(f"[RENAMED] {filename} -> {new_filename}")
                            count += 1
                        except OSError as e:
                            print(f"[ERROR] Could not rename {filename}: {e}")

    if not dry_run:
        print(f"\nTotal files renamed: {count}")

if __name__ == "__main__":
    # Set the directory you want to scan ('.' means current directory)
    TARGET_DIR = "."

    # Set this to True first to see what will happen without making changes
    DRY_RUN_MODE = False

    print(f"Scanning directory: {os.path.abspath(TARGET_DIR)}")
    if DRY_RUN_MODE:
        print("--- RUNNING IN DRY RUN MODE (No changes will be made) ---")

    process_directory(TARGET_DIR, dry_run=DRY_RUN_MODE)
