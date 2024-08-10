import glob
import os
import sys

input_dir = sys.argv[1] if len(sys.argv) >= 2 else ''
output_dir = sys.argv[2] if len(sys.argv) >= 3 else ''

# Get shader files
fnames = glob.glob(os.path.join(input_dir, '**', '*.vert'), recursive=True)
fnames += glob.glob(os.path.join(input_dir, '**', '*.geom'), recursive=True)
fnames += glob.glob(os.path.join(input_dir, '**', '*.frag'), recursive=True)


def read_file(fname, loaded_files):
    # Quit if the file has already been loaded
    if fname in loaded_files:
        return ''

    # Read file
    with open(fname, 'r') as f:
        code = f.read()
    loaded_files.add(fname)

    # Get directory
    dir_path = os.path.dirname(fname)
    
    include_idx = code.find('#include')
    while include_idx >= 0:
        # Get the include file name
        line_end = code.find('\n', include_idx)
        include_fname = code[include_idx : line_end].split('"')[1]

        # Read file
        include_code = read_file(os.path.join(dir_path, include_fname), loaded_files)

        # Replace the include with the contents
        code = code[: include_idx] + include_code + code[line_end :]

        # Search for more includes
        include_idx = code.find('#include', include_idx + len(include_code))

    # Remove version directives if not on first line
    code = code.strip()

    version_idx = code.find('#version', 1)
    while version_idx > 0:
        # Get the version line
        line_end = code.find('\n', version_idx)

        # Remove line
        code = code[: version_idx] + code[line_end :]
        
        # Find next one
        version_idx = code.find('#version', version_idx)

    # Replace newlines with actual characters
    code = code.replace('\n', '\\n')
    code = code.replace('\r', '')
    code = code.replace('"', '\\"')

    return code


# Read each file
for fname in fnames:
    # Get relative path
    relpath = os.path.relpath(fname, input_dir)
    print('Processing file', relpath)

    # Get code
    code = read_file(fname, set())

    # Create variable name
    header_guard_path = relpath.upper().replace('/', '_').replace('\\', '_').replace('.', '_')
    var_name = 'SHADER_' + header_guard_path
    
    # Create directory if needed
    output_fname = os.path.join(output_dir, relpath + '.h')
    dir_path = os.path.dirname(output_fname)
    if not os.path.exists(dir_path):
        os.makedirs(dir_path)
    
    # Write to output file
    with open(output_fname, 'w') as f:
        code = '#ifndef %s\n#define %s "%s"\n#endif' % (var_name, var_name, code)
        f.write(code)