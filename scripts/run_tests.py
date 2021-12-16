import os
import shutil
import subprocess
import sys


def clean_up(folder):
    shutil.rmtree(folder)


def print_result(folder, file, output, pair_error_reason=None):
    result = folder + ": " + output + "\n"
    print(result)
    file.write(result)
    if pair_error_reason:
        file.write(pair_error_reason[1] + ": \n" + pair_error_reason[0] + "\n")
    file.write("\n")


root = sys.path[0] + "/"
tests_folder = root + "../mito/tests/"
if not os.path.isdir(tests_folder):
    raise NameError("Please check that tests folder exists")

# List of all folders
folders = [x for x in os.listdir(
    tests_folder) if os.path.isdir(tests_folder + x)]

output_file = root + 'out.log'

# Environment
pyre_dir = os.environ['PYRE_DIR']

with open(output_file, 'w') as f:
    for folder in folders:
        folder_path = tests_folder + folder + "/"
        os.chdir(folder_path)
        test_name = "main"
        test_path = folder_path + test_name
        test_ext = ".cc"
        if not os.path.isfile(test_path + test_ext):
            raise NameError("Please name the test as main.cc")

        # Temporary folder for compile and run the test
        tmp_folder_path = folder_path + "build_temp/"
        if os.path.isdir(tmp_folder_path):
            clean_up(tmp_folder_path)
        os.mkdir(tmp_folder_path)

        # Commands to execute
        compile_cmd = 'g++ -g -std=c++2a ' + \
            '-isystem ' + pyre_dir + '/include -lpyre -ljournal -L' + pyre_dir + '/lib ' + \
            '-Wno-unused-variable -Wno-unused-parameter ' + \
            test_path + test_ext + ' -o ' + tmp_folder_path + test_name
        run_cmd = tmp_folder_path + test_name

        compile_process = subprocess.run(
            compile_cmd, capture_output=True, text=True, shell=True)
        if compile_process.returncode == 1:
            print_result(folder, f, "FAIL",
                         (compile_process.stderr, "Compilation error"))
            clean_up(tmp_folder_path)
            continue

        run_process = subprocess.run(
            run_cmd, capture_output=True, text=True, shell=True)
        if run_process.returncode == 1:
            print_result(folder, f, "FAIL",
                         (run_process.stderr, "Runtime error"))
            clean_up(tmp_folder_path)
            continue

        print_result(folder, f, "SUCCESS")
        clean_up(tmp_folder_path)

print("All done, see file {} for output log.".format(output_file))
