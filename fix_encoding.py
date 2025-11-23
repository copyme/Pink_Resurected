import os
import sys

# ==========================================
# CONFIGURATION
# ==========================================
SEARCH_DIR = 'src'

# Extensions to explicitly ignore (binaries, images, etc.)
IGNORE_EXTENSIONS = (
    '.o', '.obj', '.a', '.so', '.dll', '.exe', '.bin',
    '.png', '.jpg', '.jpeg', '.gif', '.bmp', '.pdf', '.zip', '.tar', '.gz'
)

# ANSI Colors
class Colors:
    OKGREEN = '\033[92m'
    OKBLUE = '\033[94m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

def is_binary(filepath):
    """
    Heuristic to detect binary files.
    Reads the first 8KB; if it contains a NULL byte, it's likely binary.
    """
    try:
        with open(filepath, 'rb') as f:
            chunk = f.read(8192)
            if b'\0' in chunk:
                return True
    except Exception:
        return True # If we can't read it, skip it
    return False

def convert_file(filepath):
    # 1. Check extension blacklist
    if filepath.lower().endswith(IGNORE_EXTENSIONS):
        return False

    # 2. Check binary heuristic
    if is_binary(filepath):
        # print(f"Skipping binary file: {filepath}")
        return False

    # 3. Try reading as UTF-8
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            f.read()
        # If this succeeds, the file is already valid UTF-8. Do nothing.
        return False
    except UnicodeDecodeError:
        pass # This is what we want to fix

    # 4. Try reading as Latin-1 (Common fallback for older C files)
    # Latin-1 maps 1:1 to bytes, so it practically never fails to "decode",
    # but we only use it if UTF-8 failed.
    try:
        with open(filepath, 'r', encoding='latin-1') as f:
            content = f.read()

        # 5. Write back as UTF-8
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)

        print(f"{Colors.OKGREEN}Converted:{Colors.ENDC} {filepath} (Latin-1 -> UTF-8)")
        return True

    except Exception as e:
        print(f"{Colors.FAIL}Failed to convert {filepath}: {e}{Colors.ENDC}")
        return False

def main():
    if not os.path.exists(SEARCH_DIR):
        print(f"{Colors.FAIL}Error: Directory '{SEARCH_DIR}' not found.{Colors.ENDC}")
        sys.exit(1)

    print(f"{Colors.OKBLUE}Scanning '{SEARCH_DIR}' for non-UTF-8 files...{Colors.ENDC}")

    count = 0
    checked = 0

    for root, dirs, files in os.walk(SEARCH_DIR):
        # Filter out hidden directories (like .git)
        dirs[:] = [d for d in dirs if not d.startswith('.')]

        for file in files:
            if file.startswith('.'): continue

            filepath = os.path.join(root, file)
            checked += 1
            if convert_file(filepath):
                count += 1

    print("-" * 40)
    print(f"Scan complete.")
    print(f"Files checked: {checked}")
    print(f"Files converted: {count}")
    if count > 0:
        print(f"{Colors.OKGREEN}Your source tree is now UTF-8.{Colors.ENDC}")
    else:
        print(f"{Colors.OKBLUE}No conversions were needed.{Colors.ENDC}")

if __name__ == "__main__":
    main()
